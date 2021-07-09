/**
 * File: $HeadURL: https://hdt-java.googlecode.com/svn/trunk/hdt-java/src/org/rdfhdt/hdt/dictionary/impl/section/PFCDictionarySection.java $
 * Revision: $Rev: 201 $
 * Last modified: $Date: 2013-04-17 23:40:20 +0100 (mié, 17 abr 2013) $
 * Last modified by: $Author: mario.arias $
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Contacting the authors:
 *   Mario Arias:               mario.arias@deri.org
 *   Javier D. Fernandez:       jfergar@infor.uva.es
 *   Miguel A. Martinez-Prieto: migumar2@infor.uva.es
 *   Alejandro Andres:          fuzzy.alej@gmail.com
 */

package org.rdfhdt.hdt.dictionary.impl.section;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Iterator;

import org.rdfhdt.hdt.compact.integer.VByte;
import org.rdfhdt.hdt.compact.sequence.SequenceLog64;
import org.rdfhdt.hdt.dictionary.DictionarySectionPrivate;
import org.rdfhdt.hdt.dictionary.TempDictionarySection;
import org.rdfhdt.hdt.exceptions.CRCException;
import org.rdfhdt.hdt.exceptions.IllegalFormatException;
import org.rdfhdt.hdt.listener.ProgressListener;
import org.rdfhdt.hdt.options.HDTOptions;
import org.rdfhdt.hdt.util.BitUtil;
import org.rdfhdt.hdt.util.Mutable;
import org.rdfhdt.hdt.util.crc.CRC32;
import org.rdfhdt.hdt.util.crc.CRC8;
import org.rdfhdt.hdt.util.crc.CRCInputStream;
import org.rdfhdt.hdt.util.io.IOUtil;
import org.rdfhdt.hdt.util.string.ByteStringUtil;
import org.rdfhdt.hdt.util.string.CompactString;
import org.rdfhdt.hdt.util.string.ReplazableString;

/**
 * @author delfina.ramos
 *
 */
public class JNIDictionarySection implements DictionarySectionPrivate {
	
	static {
		try {
			System.loadLibrary("jnidictionary");
		} catch (Exception e) {
			throw new RuntimeException("Unable to load shared library for JNIDictionary", e);
		}
	}

	public static String JNIDic;
	
	public static final int TYPE_INDEX = 2;
	public static final int DEFAULT_BLOCK_SIZE = 16;
	
	// FIXME: Due to java array indexes being int, only 2GB can be addressed per dictionary section.
	protected byte [] text; // Encoded sequence
	
	protected int blocksize;
	protected int numstrings;
	protected SequenceLog64 blocks;
	protected String fname;
	protected long jnidictionary;
	
	public static byte[] toByteArray(CharSequence charSequence) {
		if (charSequence == null) {
	    	return null;
	    }
	    byte[] barr = new byte[charSequence.length()];
	    for (int i = 0; i < barr.length; i++) {
	    	barr[i] = (byte) charSequence.charAt(i);
	    }
	    return barr;
	}
	
	public JNIDictionarySection(HDTOptions spec) {
		this.blocksize = (int) spec.getInt("jni.blocksize");
		if(blocksize==0) {
			blocksize = DEFAULT_BLOCK_SIZE;
		}
	}
	
	/* (non-Javadoc)
	 * @see hdt.dictionary.DictionarySection#load(hdt.dictionary.DictionarySection)
	 */
	@Override
	public void load(TempDictionarySection other, ProgressListener listener, String dict) throws IOException {
		this.blocks = new SequenceLog64(BitUtil.log2(other.size()), other.getNumberOfElements()/blocksize);
		Iterator<? extends CharSequence> it = other.getSortedEntries();
		this.load((Iterator<CharSequence>)it, other.getNumberOfElements(), listener, dict);
	}

	public void load(Iterator<CharSequence> it, long numentries, ProgressListener listener, String dict) throws IOException {
		this.blocks = new SequenceLog64(32, numentries/blocksize);
		this.numstrings = 0;
		
		ByteArrayOutputStream outputStream = new ByteArrayOutputStream();  

		while(it.hasNext()) {
			CharSequence str = it.next();
			byte[] barr = toByteArray(str);
						
			outputStream.write(barr);	
			outputStream.write(0);
			
			numstrings++;
		}

		text = outputStream.toByteArray();

		System.out.println("length of byte array : "+text.length);
		
		jnidictionary = _createJNIDictionary(text, blocksize, dict);

	}
	
	@Override
	public void load(String filename, ProgressListener listener) throws IOException {
			
		jnidictionary = _loadJNIDictionary(filename);
		numstrings=getNElements(jnidictionary);
		
	}
		
	protected int locateBlock(CharSequence str) {
		if(blocks.getNumberOfElements()==0) {
			return -1;
		}
		
		int low = 0;
		int high = (int)blocks.getNumberOfElements()-1;
		int max = high;
		
		while (low <= high) {
			int mid = (low + high) >>> 1;
			
			int cmp;
			if(mid==max) {
				cmp = -1;
			} else {
				int pos = (int)blocks.get(mid);
				cmp = ByteStringUtil.strcmp(str, text, pos);
//				System.out.println("Comparing against block: "+ mid + " which is "+ ByteStringUtil.asString(text, pos)+ " Result: "+cmp);
			}
			
			if (cmp<0) {
				high = mid - 1;
			} else if (cmp > 0) {
				low = mid + 1;
			} else {
				return mid; // key found
			}
		}
		return -(low + 1);  // key not found.
	}
	
	
	/* (non-Javadoc)
	 * @see hdt.dictionary.DictionarySection#locate(java.lang.CharSequence)
	 */
	@Override
	public int locate(CharSequence str) {
		if (numstrings==0) {
			return 0;
		}
		
		int location = locate(str.toString(), str.length(), jnidictionary);
		
		return location;
	}
		
	/* (non-Javadoc)
	 * @see hdt.dictionary.DictionarySection#extract(int)
	 */
	@Override
	public CharSequence extract(int id) {
		if (numstrings==0 || id<1 || id>numstrings) {
			return null;
		}

		String res = extract(id, jnidictionary);
		
		return res;
	}
	
	/* (non-Javadoc)
	 * @see hdt.dictionary.DictionarySection#load(java.io.InputStream, hdt.ProgressListener)
	 */
	@Override
	public void load(InputStream input, ProgressListener listener) throws IOException {
		
		//todo esto no hace falta porque lo hago en c, llama a método nativo
		CRCInputStream in = new CRCInputStream(input, new CRC8());
		
		// Read type
		int type = in.read();
		if(type!=TYPE_INDEX) {
			throw new IllegalFormatException("Trying to read a DictionarySectionPFC from data that is not of the suitable type");
		}
		
		// Read vars
		numstrings = (int) VByte.decode(in);
		long bytes = VByte.decode(in);
		blocksize = (int) VByte.decode(in);		
	
		if(!in.readCRCAndCheck()) {
			throw new CRCException("CRC Error while reading Dictionary Section Plain Front Coding Header.");
		}
		
		if(bytes>Integer.MAX_VALUE) {
			input.reset();
			throw new IllegalArgumentException("This class cannot process files with a packed buffer bigger than 2GB"); 
		}
		
		// Read blocks
		blocks = new SequenceLog64();
		blocks.load(input, listener);	// Read blocks from input, they have their own CRC check.
		
		// Read packed data
		in.setCRC(new CRC32());
		text = IOUtil.readBuffer(in, (int) bytes, listener);
		if(!in.readCRCAndCheck()) {
			throw new CRCException("CRC Error while reading Dictionary Section Plain Front Coding Data.");
		}
	}

	@Override
	public void close() throws IOException {
		text=null;
		if (blocks!=null) blocks.close();
		blocks=null;
	}

	@Override
	public long size() {
		return text.length+blocksize;
	}

	@Override
	public int getNumberOfElements() {
		int numero = getNElements(jnidictionary);
		return numero;
	}

	@Override
	public Iterator<? extends CharSequence> getSortedEntries() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void save(OutputStream output, String filename, ProgressListener listener) throws IOException {
		
		_saveJNIDictionary(jnidictionary, filename);
		System.out.println(filename+" saved successfully");
		
	}
	
	protected native String _writeJNIDictionary(String filename);
	protected native long _createJNIDictionary(byte [] it, int bucketsize, String dict);
	protected native int locate(String str, int strLen, long jnidictionary);
	protected native String extract(int id, long jnidictionary);
	protected native long _loadJNIDictionary(InputStream in);
	protected native long _loadJNIDictionary(String filename);
	protected native void _saveJNIDictionary(long jnidictionary, String filename);
	protected native int getNElements(long jnidictionary);

	@Override
	public void load(TempDictionarySection other, ProgressListener listener) throws IOException {
				
	}

	@Override
	public void save(OutputStream output, ProgressListener listener) throws IOException {
		
	}
	

}
