package org.rdfhdt.hdt.dictionary;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.rdfhdt.hdt.listener.ProgressListener;

public interface DictionarySectionPrivate extends DictionarySection {
	/**
	 * Load entries from another dictionary section.
	 * @param other
	 * @param listener
	 * @param string 
	 * @throws IOException 
	 */
	void load(TempDictionarySection other, ProgressListener listener) throws IOException;
	/**
	 * Load entries from another dictionary section.
	 * @param other
	 * @param listener
	 * @param string 
	 * @throws IOException 
	 */
	void load(TempDictionarySection other, ProgressListener listener, String string) throws IOException;
	
	/**
	 * Serialize dictionary section to a stream.
	 * @param output
	 * @param listener
	 * @throws IOException
	 */
	void save(OutputStream output, ProgressListener listener) throws IOException;
	void save(OutputStream output, String filename, ProgressListener listener) throws IOException;
	
	/**
	 * Load dictionary section from a stream.
	 * @param input
	 * @param listener
	 * @throws IOException
	 */
	void load(InputStream input, ProgressListener listener) throws IOException;
	void load(String filename, ProgressListener listener) throws IOException;
}
