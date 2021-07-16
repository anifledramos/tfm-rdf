package org.rdfhdt.hdt.hdt;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import org.rdfhdt.hdt.enums.RDFNotation;
import org.rdfhdt.hdt.exceptions.NotFoundException;
import org.rdfhdt.hdt.exceptions.ParserException;
import org.rdfhdt.hdt.hdt.impl.HDTImpl;
import org.rdfhdt.hdt.hdt.impl.JNIHDTImpl;
import org.rdfhdt.hdt.hdt.impl.TempHDTImporterOnePass;
import org.rdfhdt.hdt.hdt.impl.TempHDTImporterTwoPass;
import org.rdfhdt.hdt.header.HeaderUtil;
import org.rdfhdt.hdt.listener.ProgressListener;
import org.rdfhdt.hdt.options.HDTOptions;
import org.rdfhdt.hdt.options.HDTSpecification;
import org.rdfhdt.hdt.triples.IteratorTripleString;
import org.rdfhdt.hdt.util.StopWatch;

public class JNIHDTManagerImpl extends HDTManager {

	@Override
	public HDTOptions doReadOptions(String file) throws IOException {
		return new HDTSpecification(file);
	}

	@Override
	public HDT doLoadHDT(String hdtFileName, ProgressListener listener) throws IOException {
		HDTPrivate hdt = new JNIHDTImpl(new HDTSpecification());
		hdt.loadFromHDT(hdtFileName, listener);
		return hdt;
	}
	
	@Override
	protected HDT doMapHDT(String hdtFileName, ProgressListener listener) throws IOException {
		HDTPrivate hdt = new JNIHDTImpl(new HDTSpecification());
		hdt.mapFromHDT(new File(hdtFileName), 0, listener);
		return hdt;
	}


	@Override
	public HDT doLoadHDT(InputStream hdtFile, ProgressListener listener) throws IOException {
		HDTPrivate hdt = new JNIHDTImpl(new HDTSpecification());
		hdt.loadFromHDT(hdtFile, listener);
		return hdt;
	}

	@Override
	public HDT doLoadIndexedHDT(String hdtFileName, ProgressListener listener) throws IOException {
		HDTPrivate hdt = new JNIHDTImpl(new HDTSpecification());
		hdt.loadFromHDT(hdtFileName, listener);
		hdt.loadOrCreateIndex(listener);
		return hdt;
	}
	


	@Override
	protected HDT doMapIndexedHDT(String hdtFileName, ProgressListener listener) throws IOException {
		HDTPrivate hdt = new JNIHDTImpl(new HDTSpecification());
		hdt.mapFromHDT(new File(hdtFileName), 0, listener);
		hdt.loadOrCreateIndex(listener);
		return hdt;
	}

	@Override
	public HDT doLoadIndexedHDT(InputStream hdtFile, ProgressListener listener) throws IOException {
		HDTPrivate hdt = new JNIHDTImpl(new HDTSpecification());
		hdt.loadFromHDT(hdtFile, listener);
		hdt.loadOrCreateIndex(listener);
		return hdt;
	}

	@Override
	public HDT doIndexedHDT(HDT hdt, ProgressListener listener) {
		((HDTPrivate)hdt).loadOrCreateIndex(listener);
		return hdt;
	}

	@Override
	public HDT doGenerateHDT(String rdfFileName, String baseURI, RDFNotation rdfNotation, HDTOptions spec, ProgressListener listener) throws IOException, ParserException {
		
		return nativeGenerateHDT(rdfFileName, baseURI, rdfNotation, spec);
	}

	@Override
	public HDT doGenerateHDT(IteratorTripleString triples, String baseURI, HDTOptions spec, ProgressListener listener) throws IOException {
		
		return null;
	}

	
	protected native HDT nativeGenerateHDT(String rdfFileName, String baseURI, RDFNotation rdfNotation, HDTOptions spec) throws IOException, ParserException;
	
}
