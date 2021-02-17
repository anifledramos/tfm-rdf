package org.rdfhdt.hdt.dictionary.impl;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.rdfhdt.hdt.dictionary.DictionaryPrivate;
import org.rdfhdt.hdt.dictionary.DictionarySection;
import org.rdfhdt.hdt.dictionary.TempDictionary;
import org.rdfhdt.hdt.enums.TripleComponentRole;
import org.rdfhdt.hdt.header.Header;
import org.rdfhdt.hdt.listener.ProgressListener;
import org.rdfhdt.hdt.options.ControlInfo;
import org.rdfhdt.hdt.util.io.CountInputStream;

public class JNIDictionary implements DictionaryPrivate { 

	@Override
	public CharSequence idToString(int id, TripleComponentRole position) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int stringToId(CharSequence str, TripleComponentRole position) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public long getNumberOfElements() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public long size() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public long getNsubjects() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public long getNpredicates() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public long getNobjects() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public long getNshared() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public DictionarySection getSubjects() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public DictionarySection getPredicates() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public DictionarySection getObjects() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public DictionarySection getShared() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void populateHeader(Header header, String rootNode) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public String getType() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void close() throws IOException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void load(InputStream input, ControlInfo ci, ProgressListener listener) throws IOException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mapFromFile(CountInputStream in, File f, ProgressListener listener) throws IOException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void load(TempDictionary other, ProgressListener listener) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void save(OutputStream output, ControlInfo ci, ProgressListener listener) throws IOException {
		// TODO Auto-generated method stub
		
	}

}
