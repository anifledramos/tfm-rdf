/**
 * File: $HeadURL: https://hdt-java.googlecode.com/svn/trunk/hdt-java/src/org/rdfhdt/hdt/hdt/impl/HDTImpl.java $
 * Revision: $Rev: 202 $
 * Last modified: $Date: 2013-05-10 18:04:41 +0100 (vie, 10 may 2013) $
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

package org.rdfhdt.hdt.hdt.impl;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Date;
import java.util.zip.GZIPInputStream;

import org.rdfhdt.hdt.dictionary.Dictionary;
import org.rdfhdt.hdt.dictionary.DictionaryFactory;
import org.rdfhdt.hdt.dictionary.DictionaryPrivate;
import org.rdfhdt.hdt.dictionary.TempDictionary;
import org.rdfhdt.hdt.enums.TripleComponentRole;
import org.rdfhdt.hdt.exceptions.IllegalFormatException;
import org.rdfhdt.hdt.exceptions.NotFoundException;
import org.rdfhdt.hdt.exceptions.NotImplementedException;
import org.rdfhdt.hdt.hdt.HDTPrivate;
import org.rdfhdt.hdt.hdt.HDTVocabulary;
import org.rdfhdt.hdt.hdt.TempHDT;
import org.rdfhdt.hdt.header.Header;
import org.rdfhdt.hdt.header.HeaderFactory;
import org.rdfhdt.hdt.header.HeaderPrivate;
import org.rdfhdt.hdt.iterator.DictionaryTranslateIterator;
import org.rdfhdt.hdt.listener.ProgressListener;
import org.rdfhdt.hdt.options.ControlInfo;
import org.rdfhdt.hdt.options.ControlInformation;
import org.rdfhdt.hdt.options.HDTOptions;
import org.rdfhdt.hdt.triples.IteratorTripleString;
import org.rdfhdt.hdt.triples.TempTriples;
import org.rdfhdt.hdt.triples.TripleID;
import org.rdfhdt.hdt.triples.Triples;
import org.rdfhdt.hdt.triples.TriplesFactory;
import org.rdfhdt.hdt.triples.TriplesPrivate;
import org.rdfhdt.hdt.util.StopWatch;
import org.rdfhdt.hdt.util.StringUtil;
import org.rdfhdt.hdt.util.io.CountInputStream;
import org.rdfhdt.hdt.util.io.IOUtil;
import org.rdfhdt.hdt.util.listener.IntermediateListener;

/**
 * Basic implementation of HDT interface
 *
 */
public class HDTImpl implements HDTPrivate {

	private HDTOptions spec;

	protected HeaderPrivate header;
	protected DictionaryPrivate dictionary;
	protected TriplesPrivate triples;

	private String hdtFileName;
	private String baseUri;
	private boolean isMapped;

	private void createComponents() {
		System.out.println("Free memory (bytes): " + 
				  Runtime.getRuntime().freeMemory());
		header = HeaderFactory.createHeader(spec);
		System.out.println("Free memory (bytes): " + 
				  Runtime.getRuntime().freeMemory());
        dictionary = DictionaryFactory.createDictionary(spec);
        System.out.println("Free memory (bytes): " + 
				  Runtime.getRuntime().freeMemory());
        triples = TriplesFactory.createTriples(spec);
	}

	public void populateHeaderStructure(String baseUri) {
		header.insert(baseUri, HDTVocabulary.RDF_TYPE, HDTVocabulary.HDT_DATASET);
		header.insert(baseUri, HDTVocabulary.RDF_TYPE, HDTVocabulary.VOID_DATASET);

		// VOID
		header.insert(baseUri, HDTVocabulary.VOID_TRIPLES, triples.getNumberOfElements());
		header.insert(baseUri, HDTVocabulary.VOID_PROPERTIES, dictionary.getNpredicates());
		header.insert(baseUri, HDTVocabulary.VOID_DISTINCT_SUBJECTS, dictionary.getNsubjects());
		header.insert(baseUri, HDTVocabulary.VOID_DISTINCT_OBJECTS, dictionary.getNobjects());

		// Structure
		String formatNode = "_:format";
		String dictNode = "_:dictionary";
		String triplesNode = "_:triples";
		String statisticsNode = "_:statistics";
		String publicationInfoNode = "_:publicationInformation";

		header.insert(baseUri, HDTVocabulary.HDT_FORMAT_INFORMATION, formatNode);
		header.insert(formatNode, HDTVocabulary.HDT_DICTIONARY, dictNode);
		header.insert(formatNode, HDTVocabulary.HDT_TRIPLES, triplesNode);
		header.insert(baseUri, HDTVocabulary.HDT_STATISTICAL_INFORMATION, statisticsNode);
		header.insert(baseUri, HDTVocabulary.HDT_PUBLICATION_INFORMATION, publicationInfoNode);

		dictionary.populateHeader(header, dictNode);
		triples.populateHeader(header, triplesNode);

		header.insert(statisticsNode, HDTVocabulary.HDT_SIZE, getDictionary().size()+getTriples().size());

		// Current time
		header.insert(publicationInfoNode, HDTVocabulary.DUBLIN_CORE_ISSUED, StringUtil.formatDate(new Date()));
	}

	/**
	 * @param spec2
	 */
	public HDTImpl(HDTOptions spec) {
		this.spec = spec;

		createComponents();
	}

	@Override
	public void loadFromHDT(InputStream input, ProgressListener listener) throws IOException {
		ControlInfo ci = new ControlInformation();
		IntermediateListener iListener = new IntermediateListener(listener);

		// Load Global ControlInformation
		ci.clear();
		ci.load(input);
		String hdtFormat = ci.getFormat();
		if(!hdtFormat.equals(HDTVocabulary.HDT_CONTAINER)) {
			throw new IllegalFormatException("This software cannot open this version of HDT File");
		}

		// Load header
		ci.clear();
		ci.load(input);
		iListener.setRange(0, 5);
		header = HeaderFactory.createHeader(ci);
		header.load(input, ci, iListener);

		// Set base URI.
		try {
			IteratorTripleString it = header.search("", HDTVocabulary.RDF_TYPE, HDTVocabulary.HDT_DATASET);
			if(it.hasNext()) {
				this.baseUri = it.next().getSubject().toString();
			}
		} catch (NotFoundException e) {
			e.printStackTrace();
		}

		// Load dictionary
		ci.clear();
		ci.load(input);
		iListener.setRange(5, 60);
		//recibe tipo de diccionario
		dictionary = DictionaryFactory.createDictionary(ci);
		// nuestro diccionario debería tener hdtFileName de input y no un inputStream
		if (dictionary.singleFileStorage(dictionary.getType())==false) {
			dictionary.load(hdtFileName, ci, iListener);
		} else {
			dictionary.load(input, ci, iListener);
		}

		// Load Triples
		ci.clear();
		ci.load(input);
		iListener.setRange(60, 100);
		triples = TriplesFactory.createTriples(ci);	
		if (triples.singleFileStorage(triples.getType())==false) {
			triples.load(hdtFileName, ci, iListener);
		} else {
			triples.load(input, ci, iListener);
		}
	}

	@Override
	public void loadFromHDT(String hdtFileName, ProgressListener listener)	throws IOException {
		InputStream in;
		if(hdtFileName.endsWith(".gz")) {
			in = new BufferedInputStream(new GZIPInputStream(new FileInputStream(hdtFileName)));
		} else {
			in = new CountInputStream(new BufferedInputStream(new FileInputStream(hdtFileName)));
		}
		this.hdtFileName = hdtFileName;
		loadFromHDT(in, listener);
		in.close();

	}

	@Override
	public void mapFromHDT(File f, long offset, ProgressListener listener) throws IOException {
		this.hdtFileName = f.toString();
		this.isMapped = true;

		CountInputStream input;
		InputStream stream;
		if(hdtFileName.endsWith(".gz")) {
			File old = f;
			hdtFileName = hdtFileName.substring(0, hdtFileName.length()-3);
			f = new File(hdtFileName);

			if(!f.exists()) {
				System.err.println("We cannot map a gzipped HDT, decompressing into "+hdtFileName+" first.");
				IOUtil.decompressGzip(old, f);
				System.err.println("Gzipped HDT successfully decompressed. You might want to delete "+old.getAbsolutePath()+" to save disk space.");
			} else {
				System.err.println("We cannot map a gzipped HDT, using "+hdtFileName+" instead.");
			}
		}

		input = new CountInputStream(new BufferedInputStream(new FileInputStream(hdtFileName)));
		stream = new BufferedInputStream(new FileInputStream(hdtFileName));

		ControlInfo ci = new ControlInformation();
		IntermediateListener iListener = new IntermediateListener(listener);

		// Load Global ControlInformation
		ci.clear();
		ci.load(stream);
		String hdtFormat = ci.getFormat();
		if(!hdtFormat.equals(HDTVocabulary.HDT_CONTAINER)) {
			throw new IllegalFormatException("This software cannot open this version of HDT File");
		}

		// Load header
		ci.clear();
		ci.load(stream);
		iListener.setRange(0, 5);
		header = HeaderFactory.createHeader(ci);
		header.load(stream, ci, iListener);

		// Set base URI.
		try {
			IteratorTripleString it = header.search("", HDTVocabulary.RDF_TYPE, HDTVocabulary.HDT_DATASET);
			if(it.hasNext()) {
				this.baseUri = it.next().getSubject().toString();
			}
		} catch (NotFoundException e) {
			e.printStackTrace();
		}

		// Load dictionary
		ci.clear();
		stream.mark(1024);
		ci.load(stream);
		stream.reset();
		iListener.setRange(5, 60);
		dictionary = DictionaryFactory.createDictionary(ci);
		System.out.println("tipo del diccionario "+dictionary.getType());
		if (dictionary.singleFileStorage(dictionary.getType())==false) {
			dictionary.load(hdtFileName, ci, iListener);
		} else {
			dictionary.load(stream, ci, iListener);
		}

		// Load Triples
		ci.clear();
		stream.mark(1024);
		ci.load(stream);
		stream.reset();
		iListener.setRange(60, 100);
		System.out.println("tipo de las tripletas "+ci.getType());
		triples = TriplesFactory.createTriples(ci);
		if (triples.singleFileStorage(triples.getType())==false) {
			triples.load(hdtFileName, ci, iListener);
		} else {
			triples.load(stream, ci, iListener);
		}

		input.close();
		stream.close();
	}

	/*
	 * (non-Javadoc)
	 *
	 * @see hdt.HDT#saveToHDT(java.io.OutputStream)
	 */
	@Override
	public void saveToHDT(OutputStream output, String fileName, ProgressListener listener) throws IOException {
		ControlInfo ci = new ControlInformation();
		IntermediateListener iListener = new IntermediateListener(listener);

		ci.clear();
		ci.setType(ControlInfo.Type.GLOBAL);
		ci.setFormat(HDTVocabulary.HDT_CONTAINER);
		ci.save(output);

		ci.clear();
		ci.setType(ControlInfo.Type.HEADER);
		header.save(output, ci, iListener);

		ci.clear();
		ci.setType(ControlInfo.Type.DICTIONARY);
		if (dictionary.singleFileStorage(dictionary.getType())==false) {
			dictionary.save(output, fileName, ci, iListener);
		} else {
			dictionary.save(output, ci, iListener);
		}

		ci.clear();
		ci.setType(ControlInfo.Type.TRIPLES);
		if (triples.singleFileStorage(triples.getType())==false) {
			triples.save(output, fileName, ci, iListener);
		} else {
			triples.save(output, ci, iListener);
		}
	}

	/*
	 * (non-Javadoc)
	 *
	 * @see hdt.HDT#saveToHDT(java.io.OutputStream)
	 */
	@Override
	public void saveToHDT(String fileName, ProgressListener listener) throws IOException {
		OutputStream out = new BufferedOutputStream(new FileOutputStream(fileName));
		//OutputStream out = new GZIPOutputStream(new BufferedOutputStream(new FileOutputStream(fileName)));
		saveToHDT(out, fileName, listener);
		out.close();

		this.hdtFileName = fileName;
	}

	@Override
	public IteratorTripleString search(CharSequence subject, CharSequence predicate, CharSequence object) throws NotFoundException {
		
		// Conversion from TripleString to TripleID
		TripleID triple = new TripleID(
				dictionary.stringToId(subject, TripleComponentRole.SUBJECT),
				dictionary.stringToId(predicate, TripleComponentRole.PREDICATE),
				dictionary.stringToId(object, TripleComponentRole.OBJECT)
			);
		
		if(triple.getSubject()==-1 || triple.getPredicate()==-1 || triple.getObject()==-1) {
			throw new NotFoundException("String not found in dictionary");
		}
		return new DictionaryTranslateIterator(triples.search(triple), dictionary, subject, predicate, object);
	}

	/*
	 * (non-Javadoc)
	 *
	 * @see hdt.HDT#getHeader()
	 */
	@Override
	public Header getHeader() {
		return header;
	}

	/*
	 * (non-Javadoc)
	 *
	 * @see hdt.HDT#getDictionary()
	 */
	@Override
	public Dictionary getDictionary() {
		return dictionary;
	}

	/*
	 * (non-Javadoc)
	 *
	 * @see hdt.HDT#getTriples()
	 */
	@Override
	public Triples getTriples() {
		return triples;
	}

	/* (non-Javadoc)
	 * @see hdt.hdt.HDT#getSize()
	 */
	@Override
	public long size() {
		return dictionary.size()+triples.size();
	}

	public void loadFromModifiableHDT(TempHDT modHdt, ProgressListener listener) throws IOException {

		modHdt.reorganizeDictionary(listener);
		modHdt.reorganizeTriples(listener);

        // Get parts
        TempTriples modifiableTriples = (TempTriples) modHdt.getTriples();
        TempDictionary modifiableDictionary = (TempDictionary) modHdt.getDictionary();

        // Convert triples to final format
        if(triples.getClass().equals(modifiableTriples.getClass())) {
                triples = modifiableTriples;
        } else {
        		//StopWatch tripleConvTime = new StopWatch();
        	
                triples.load(modifiableTriples, listener, modifiableDictionary);
                //System.out.println("Triples conversion time: "+tripleConvTime.stopAndShow());
        }

        // Convert dictionary to final format
        if(dictionary.getClass().equals(modifiableDictionary.getClass())) {
                dictionary = (DictionaryPrivate)modifiableDictionary;
        } else {
                //StopWatch dictConvTime = new StopWatch();
                dictionary.load(modifiableDictionary, listener);
                //System.out.println("Dictionary conversion time: "+dictConvTime.stopAndShow());
        }

        this.baseUri = modHdt.getBaseURI();
	}

	/* (non-Javadoc)
	 * @see hdt.hdt.HDT#generateIndex(hdt.listener.ProgressListener)
	 */
	@Override
	public void loadOrCreateIndex(ProgressListener listener) {
		ControlInfo ci = new ControlInformation();
		String indexName = hdtFileName+".index";
		indexName = indexName.replaceAll("\\.hdt\\.gz", "hdt");

		try {
			CountInputStream in = new CountInputStream(new BufferedInputStream(new FileInputStream(indexName)));
			ci.load(in);
			if(isMapped) {
				triples.mapIndex(in, new File(indexName), ci, listener);
			} else {
				triples.loadIndex(in, ci, listener);
			}
			in.close();
		} catch (Exception e) {
			System.out.println("Could not read .hdt.index, Generating a new one.");

			// GENERATE
			triples.generateIndex(listener);

			// SAVE
			try {
				FileOutputStream out = new FileOutputStream(indexName);
				ci.clear();
				triples.saveIndex(out, ci, listener);
				out.close();
			} catch (IOException e2) {

			}
		}
	}

	@Override
	public String getBaseURI() {
		return baseUri;
	}

	protected void setTriples(TriplesPrivate triples) {
		this.triples = triples;
	}

	@Override
	public void close() throws IOException {
		if (dictionary!=null) dictionary.close();
		if (triples!=null) triples.close();
	}

}
