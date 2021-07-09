package org.rdfhdt.hdt.dictionary.impl;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.rdfhdt.hdt.dictionary.DictionarySectionPrivate;
import org.rdfhdt.hdt.dictionary.TempDictionary;
import org.rdfhdt.hdt.dictionary.impl.section.DictionarySectionFactory;
import org.rdfhdt.hdt.dictionary.impl.section.JNIDictionarySection;
import org.rdfhdt.hdt.exceptions.IllegalFormatException;
import org.rdfhdt.hdt.hdt.HDTVocabulary;
import org.rdfhdt.hdt.header.Header;
import org.rdfhdt.hdt.listener.ProgressListener;
import org.rdfhdt.hdt.options.ControlInfo;
import org.rdfhdt.hdt.options.HDTOptions;
import org.rdfhdt.hdt.options.ControlInfo.Type;
import org.rdfhdt.hdt.util.io.CountInputStream;
import org.rdfhdt.hdt.util.listener.IntermediateListener;

public class JNIDictionaryBig extends BaseDictionary { 
	
	public JNIDictionaryBig(HDTOptions spec, DictionarySectionPrivate s, DictionarySectionPrivate p, DictionarySectionPrivate o, DictionarySectionPrivate sh) {
	super(spec);
	this.subjects = s;
	this.predicates = p;
	this.objects = o;
	this.shared = sh;
	}
	
	public JNIDictionaryBig(HDTOptions spec) {
		super(spec);
		subjects = new JNIDictionarySection(spec);
		predicates = new JNIDictionarySection(spec);
		objects = new JNIDictionarySection(spec);
		shared = new JNIDictionarySection(spec);
	}
	
	@Override
	public void load(TempDictionary other, ProgressListener listener) throws IOException {
		IntermediateListener iListener = new IntermediateListener(listener);
		subjects.load(other.getSubjects(), iListener, "subjects");
		predicates.load(other.getPredicates(), iListener, "predicates");
		objects.load(other.getObjects(), iListener, "objects");
		shared.load(other.getShared(), iListener, "shared");
	}
	
	@Override
	public void load(String filename, ControlInfo ci, ProgressListener listener) throws IOException {
		if(ci.getType()!=ControlInfo.Type.DICTIONARY) {
			throw new IllegalFormatException("Trying to read a dictionary section, but was not dictionary.");
		}
		
		IntermediateListener iListener = new IntermediateListener(listener);
		subjects.load(filename+".dic.s", iListener);
		predicates.load(filename+".dic.p", iListener);
		objects.load(filename+".dic.ob", iListener);
		shared.load(filename+".dic.shr", iListener);
	}
	
	@Override
	public void load(InputStream input, String filename, ControlInfo ci, ProgressListener listener) throws IOException {
		if(ci.getType()!=ControlInfo.Type.DICTIONARY) {
			throw new IllegalFormatException("Trying to read a dictionary section, but was not dictionary.");
		}
		
		IntermediateListener iListener = new IntermediateListener(listener);
		subjects.load(input, iListener);
		predicates.load(input, iListener);
		objects.load(input, iListener);
		shared.load(input, iListener);		
	}
	
	@Override
	public void mapFromFile(CountInputStream in, File f, ProgressListener listener) throws IOException {
	}
	
	@Override
	public void save(OutputStream output, ControlInfo ci, ProgressListener listener) throws IOException {
		ci.setType(Type.DICTIONARY);
		ci.setFormat(HDTVocabulary.DICTIONARY_TYPE_JNI);
		ci.setInt("elements", this.getNumberOfElements());
		ci.save(output);

		IntermediateListener iListener = new IntermediateListener(listener);
		shared.save(output, iListener);
		subjects.save(output, iListener);
		predicates.save(output, iListener);
		objects.save(output, iListener);

	}
	
	@Override
	public void save(OutputStream output, String filename, ControlInfo ci, ProgressListener listener)
			throws IOException {
		ci.setType(Type.DICTIONARY);
		ci.setFormat(HDTVocabulary.DICTIONARY_TYPE_JNI);
		ci.setInt("elements", this.getNumberOfElements());
		ci.save(output);

		IntermediateListener iListener = new IntermediateListener(listener);
		shared.save(output, filename+".dic.shr", iListener);
		subjects.save(output, filename+".dic.s", iListener);
		predicates.save(output, filename+".dic.p", iListener);
		objects.save(output, filename+".dic.ob", iListener);
		
	}
	
	@Override
	public void load(InputStream input, ControlInfo ci, ProgressListener listener) throws IOException {
		if(ci.getType()!=ControlInfo.Type.DICTIONARY) {
			throw new IllegalFormatException("Trying to read a dictionary section, but was not dictionary.");
		}

		IntermediateListener iListener = new IntermediateListener(listener);
		shared = DictionarySectionFactory.loadFrom(input, iListener);		
		subjects = DictionarySectionFactory.loadFrom(input, iListener);
		predicates = DictionarySectionFactory.loadFrom(input, iListener);
		objects = DictionarySectionFactory.loadFrom(input, iListener);
	}
	
	@Override
	public void populateHeader(Header header, String rootNode) {
		header.insert(rootNode, HDTVocabulary.DICTIONARY_TYPE, HDTVocabulary.DICTIONARY_TYPE_JNI);
		header.insert(rootNode, HDTVocabulary.DICTIONARY_NUMSHARED, getNshared());
		header.insert(rootNode, HDTVocabulary.DICTIONARY_SIZE_STRINGS, size());
	}
	
	@Override
	public String getType() {
		return HDTVocabulary.DICTIONARY_TYPE_JNI;
	}
	@Override
	public void close() throws IOException {
		if (shared!=null) shared.close();
		if (subjects!=null) subjects.close();
		if (predicates!=null) predicates.close();
		if (objects!=null) objects.close();
	}

	@Override
	public boolean singleFileStorage(String type) {
		if (type==HDTVocabulary.DICTIONARY_TYPE_JNI) {
			return false;
		} else {
			return true;
		}
	}

	
		
}

