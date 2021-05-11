

package org.rdfhdt.hdt.triples.impl;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;

import org.rdfhdt.hdt.dictionary.TempDictionary;
import org.rdfhdt.hdt.enums.TripleComponentOrder;
import org.rdfhdt.hdt.exceptions.IllegalFormatException;
import org.rdfhdt.hdt.hdt.HDTVocabulary;
import org.rdfhdt.hdt.header.Header;
import org.rdfhdt.hdt.listener.ProgressListener;
import org.rdfhdt.hdt.options.ControlInfo;
import org.rdfhdt.hdt.options.HDTOptions;
import org.rdfhdt.hdt.options.HDTSpecification;
import org.rdfhdt.hdt.triples.IteratorTripleID;
import org.rdfhdt.hdt.triples.TempTriples;
import org.rdfhdt.hdt.triples.TripleID;
import org.rdfhdt.hdt.triples.TriplesPrivate;
import org.rdfhdt.hdt.util.io.CountInputStream;
import org.rdfhdt.hdt.util.io.IOUtil;

/**
 * @author guillermo.debernardo
 *
 */
public class K2Triples implements TriplesPrivate {
	
	static {
		try {
			//Load shared library. Copy to  temp file
//			String filename = System.mapLibraryName("k2triples");			
//			File tmpFile = File.createTempFile("lib"+System.currentTimeMillis(), "");
//			InputStream is = K2Triples.class.getResourceAsStream("/"+filename);			
//			OutputStream os = new FileOutputStream(tmpFile);			
//			IOUtil.copyStream(is, os);
//			System.load(tmpFile.getPath());
//			tmpFile.deleteOnExit();
			System.loadLibrary("k2triples");
		} catch (Exception e) {
			throw new RuntimeException("Unable to load shared library for K2Triples", e);
		}
	}
	
	protected TripleComponentOrder order=TripleComponentOrder.Unknown;
	
	long numberOfElements;
	
	// JNI pointer
	protected long k2triples;
	
	

	public K2Triples() {
		this(new HDTSpecification());
	}
	
	public K2Triples(HDTOptions spec) {
		String orderStr = spec.get("triplesOrder");
		if(orderStr!=null) {
			order = TripleComponentOrder.valueOf(orderStr);
		}

	}		
		
	public void load(IteratorTripleID it, ProgressListener listener, int npreds, int nso) {
		try {
			
			assert(it.getOrder() == TripleComponentOrder.SPO);			
			String filename = "tmploadtriples" + System.currentTimeMillis();
			
			File f = new File(filename);
			f.createNewFile();			
			DataOutputStream os = new DataOutputStream(new FileOutputStream(filename));
			while(it.hasNext()) {				
				TripleID triple = it.next();
				os.writeInt(triple.getPredicate());
				os.writeInt(triple.getSubject());
				os.writeInt(triple.getObject());
				numberOfElements++;
			}
			IOUtil.closeQuietly(os);
			
			k2triples = _createK2Triples(filename, npreds, nso);
			
			f.deleteOnExit();
			if (f.exists()) {
				try {
					f.delete();
				} catch (Exception e) {
					
				}
			}
			
		} catch (Exception e) {
			throw new RuntimeException("loading triples", e);
		}
	}

	/* (non-Javadoc)
	 * @see hdt.triples.Triples#load(hdt.triples.TempTriples, hdt.ProgressListener)
	 */
	@Override
	public void load(TempTriples triples, ProgressListener listener) {
		throw new UnsupportedOperationException("K2Triples requires dictionary to load additional information");
	}
	
	/* (non-Javadoc)
	 * @see hdt.triples.Triples#load(hdt.triples.TempTriples, hdt.ProgressListener)
	 */
	@Override
	public void load(TempTriples triples, ProgressListener listener, TempDictionary dictionary) {
		triples.setOrder(order);
		triples.sort(listener);
		
		IteratorTripleID it = triples.searchAll();
		this.load(it, listener, dictionary.getPredicates().getNumberOfElements(), dictionary.getShared().getNumberOfElements());
	}


	/* (non-Javadoc)
	 * @see hdt.triples.Triples#search(hdt.triples.TripleID)
	 */
	@Override
	public IteratorTripleID search(TripleID pattern) {
		int []s = null;
		int []p = null;
		int []o = null;
		int nres = 0;
		
		if (pattern.getSubject() > 0) {
			if (pattern.getPredicate() > 0) {
				if (pattern.getObject() > 0) {
					// <S,P,O>
					nres = searchSPO(k2triples, pattern.getSubject()-1, pattern.getPredicate()-1, pattern.getObject()-1);	
				} else {
					// <S,P,?O>
					o = searchSP(k2triples, pattern.getSubject()-1, pattern.getPredicate()-1);
					nres = o[0];
				}
			} else {
				if (pattern.getObject() > 0) {
					// <S,?P,O>
					p = searchSO(k2triples, pattern.getSubject()-1, pattern.getObject()-1);	
					nres = p[0];
				} else {
					// <S,?P,?O>
					int [][]res = searchS(k2triples, pattern.getSubject()-1);
					p = res[0];
					o = res[1];
					nres = p[0];
				}
			}
		} else {
			//Sujeto libre
			if (pattern.getPredicate() > 0) {
				if (pattern.getObject() > 0) {
					// <?S,P,O>
					s = searchPO(k2triples, pattern.getPredicate()-1, pattern.getObject()-1);
					nres = s[0];
				} else {
					// <?S,P,?O>
					int [][]res = searchP(k2triples, pattern.getPredicate()-1);
					s = res[0];
					o = res[1];
					nres = s[0];
				}
			} else {
				if (pattern.getObject() > 0) {
					// <?S,?P,O>
					int [][]res = searchO(k2triples, pattern.getObject()-1);
					s = res[0];
					p = res[1];
					nres = s[0];
				} else {
					return new K2TriplesFullIterator(this, getNPreds(k2triples));
				}
			}
		}
		
		
		return new K2TriplesIterator(pattern, nres, s, p, o);
	}

	/* (non-Javadoc)
	 * @see hdt.triples.Triples#searchAll()
	 */
	@Override
	public IteratorTripleID searchAll() {
		return this.search(new TripleID());
	}

	/* (non-Javadoc)
	 * @see hdt.triples.Triples#getNumberOfElements()
	 */
	@Override
	public long getNumberOfElements() {
		return numberOfElements;
	}

	/* (non-Javadoc)
	 * @see hdt.triples.Triples#size()
	 */
	@Override
	public long size() {
		return _sizeK2Triples(k2triples);
	}

	/* (non-Javadoc)
	 * @see hdt.triples.Triples#save(java.io.OutputStream, hdt.ControlInfo, hdt.ProgressListener)
	 */
	@Override
	public void save(OutputStream output, ControlInfo ci, ProgressListener listener) throws IOException {
				
		ci.clear();
		ci.setFormat(getType());		
		ci.setType(ControlInfo.Type.TRIPLES);
		ci.save(output);
		
		String filename = "tmpsave" + System.currentTimeMillis();		
		
		_storeK2Triples(k2triples, filename);
		
		File f = new File(filename);
		Long size = f.length();
					
	    output.write(longToBytes(size));	
	    
	    System.out.println("Size saved .... " + size);
		
	    InputStream is = new FileInputStream(filename);
	    
		IOUtil.copyStream(is, output);
		
		IOUtil.closeQuietly(is);
		
		f.deleteOnExit();
		if (f.exists()) {
			try {
				f.delete();
			} catch (Exception e) {
				
			}
		}
		
	}

	/* (non-Javadoc)
	 * @see hdt.triples.Triples#load(java.io.InputStream, hdt.ControlInfo, hdt.ProgressListener)
	 */
	@Override
	public void load(InputStream input, ControlInfo ci, ProgressListener listener) throws IOException {
		
		if(ci.getType()!=ControlInfo.Type.TRIPLES) {
			throw new IllegalFormatException("Trying to read a triples section, but was not triples.");
		}
		
		if(!ci.getFormat().equals(getType())) {
			throw new IllegalFormatException("Trying to read BitmapTriples, but the data does not seem to be BitmapTriples");
		}
		
		long size = 0;		
		
		byte [] barray = new byte[Long.BYTES];
		input.read(barray, 0, Long.BYTES);
		size = bytesToLong(barray);
		
		System.out.println("SIZE TO LOAD: " + size);
		
		barray = new byte[2048];
		
		String filename = "tmploadinput" + System.currentTimeMillis();
		File f = new File(filename);
		f.createNewFile();
		FileOutputStream os = new FileOutputStream(f);
		
		IOUtil.copyStream(input, os, size);
				
		IOUtil.closeQuietly(os);
		
		k2triples = _loadK2Triples(filename);
				
		f.deleteOnExit();
		if (f.exists()) {
			try {
				f.delete();
			} catch (Exception e) {
				
			}
		}
		
	}
	

	@Override
	public void mapFromFile(CountInputStream input, File f,	ProgressListener listener) throws IOException {
		
		throw new UnsupportedOperationException("mapFromFile not implemented.");
		
	}
	


	private void createIndexObjectMemoryEfficient() {
		return;
	}
	
	private void createIndexObjects() {
		return;
	}
	
	
	
	public void generateIndex(ProgressListener listener) {		
		return;
	}

	/* (non-Javadoc)
	 * @see hdt.triples.Triples#populateHeader(hdt.header.Header, java.lang.String)
	 */
	@Override
	public void populateHeader(Header header, String rootNode) {
		header.insert(rootNode, HDTVocabulary.TRIPLES_TYPE, getType());
		header.insert(rootNode, HDTVocabulary.TRIPLES_NUM_TRIPLES, getNumberOfElements() );
		header.insert(rootNode, HDTVocabulary.TRIPLES_ORDER, order.toString() );
	}

	/* (non-Javadoc)
	 * @see hdt.triples.Triples#getType()
	 */
	@Override
	public String getType() {
		return HDTVocabulary.TRIPLES_TYPE_K2;
	}

	@Override
	public void close() throws IOException {
		_destroyK2Triples(k2triples);
	}

	public TripleComponentOrder getOrder() {
		return this.order;
	}
	
	protected native long _createK2Triples(String filename, int npreds, int nso);
	protected native long _loadK2Triples(String filename);
	protected native void _storeK2Triples(long k2triples, String filename);
	protected native void _destroyK2Triples(long k2triples);
	protected native long _sizeK2Triples(long k2triples);
//	protected native long _nElementsK2Triples(long k2triples);
	
	protected native int[] doSearchK2Triples(long k2triples, int s, int p, int o);
	
	protected native int searchSPO(long k2triples, int s, int p, int o);
	protected native int[] searchSP(long k2triples, int s, int p);
	protected native int[] searchPO(long k2triples, int p, int o);
	protected native int[] searchSO(long k2triples, int s, int o);
	protected native int[][] searchS(long k2triples, int s);
	protected native int[][] searchP(long k2triples, int p);
	protected native int[][] searchO(long k2triples, int o);
	
	protected native int getNPreds(long k2triples);
	
	@Override
	public void loadIndex(InputStream input, ControlInfo ci, ProgressListener listener) throws IOException {
		return;
	}

	@Override
	public void mapIndex(CountInputStream input, File f, ControlInfo ci, ProgressListener listener) throws IOException {
		return;
	}

	@Override
	public void saveIndex(OutputStream output, ControlInfo ci, ProgressListener listener) throws IOException {
		return;
	}
	
	private byte[] longToBytes(long x) {
	    ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
	    buffer.putLong(x);
	    return buffer.array();
	}

	private long bytesToLong(byte[] bytes) {
	    ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
	    buffer.put(bytes);
	    buffer.flip();//need flip 
	    return buffer.getLong();
	}
}

