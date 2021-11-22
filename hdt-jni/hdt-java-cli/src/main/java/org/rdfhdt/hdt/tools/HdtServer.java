package org.rdfhdt.hdt.tools;

import com.beust.jcommander.JCommander;
import com.beust.jcommander.Parameter;
import org.rdfhdt.hdt.exceptions.NotFoundException;
import org.rdfhdt.hdt.exceptions.ParserException;
import org.rdfhdt.hdt.hdt.HDT;
import org.rdfhdt.hdt.hdt.HDTManager;
import org.rdfhdt.hdt.listener.ProgressListener;
import org.rdfhdt.hdt.triples.IteratorTripleString;
import org.rdfhdt.hdt.triples.TripleString;
import org.rdfhdt.hdt.util.UnicodeEscape;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;


/**
 * @author guillermo.debernardo
 *
 */
public class HdtServer implements ProgressListener {
	@Parameter(names = "-input", description = "<HDT File>")
	public String input;

	@Parameter(names = "-port", description = "<Port>")
	public int port = 17730;

	private HDT hdt;

	private PrintStream logger;

	protected int runQuery(TripleString triple) {
		try {
			int count = 0;
			IteratorTripleString it = hdt.search(triple.getSubject(), triple.getPredicate(), triple.getObject());

			count = 0;
			while (it.hasNext()) {
				count++;
			}

			if (logger != null) {
				logger.println((triple.getSubject().equals("") ? "?" : triple.getSubject())
						+ " " + (triple.getPredicate().equals("") ? "?" : triple.getPredicate())
						+ " " + (triple.getObject().equals("") ? "?" : triple.getObject()) + " " + count);
			}
			return count;
		} catch (Exception e) {
			System.out.println(triple);
			e.printStackTrace();
		}
		return 0;
	}

	private int runQueries(List<TripleString> queries) throws NotFoundException {
		System.out.println("Executing queries: " + queries.size());
		int nres = 0;
		for (int i = 0; i < queries.size(); i++) {
			nres += runQuery(queries.get(i));
		}
		return nres;
	}
	
	/**
	 * Read from a line, where each component is separated by space.
	 * @param line
	 */
	private TripleString parseQuery(String line) throws ParserException {
		TripleString dest = new TripleString();
		int split, posa, posb;
		dest.clear();
		
		// SET SUBJECT
		posa = 0;
		posb = split = line.indexOf(' ', posa);
		
		if(posb==-1) throw new ParserException("Make sure that you included three terms."); // Not found, error.

		String sub = line.substring(posa, posb);
		if (sub.equals("?")) sub = "";
		dest.setSubject(sub);
	
		// SET PREDICATE
		posa = split+1;
		posb = split = line.indexOf(' ', posa);
		
		if(posb==-1) throw new ParserException("Make sure that you included three terms.");

		String pred = line.substring(posa, posb);
		if (pred.equals("?")) pred = "";
		dest.setPredicate(pred);

		// SET OBJECT
		posa = split+1;
		posb = line.length();
		
		if(line.charAt(posb-1)=='.') posb--;	// Remove trailing <space> <dot> from NTRIPLES.
		if(line.charAt(posb-1)==' ') posb--;

		String obj = line.substring(posa, posb);
		if (obj.equals("?")) obj = "";
		dest.setObject(UnicodeEscape.unescapeString(obj));
		return dest;
	}
	
	public void execute() throws IOException {
		hdt = HDTManager.loadIndexedHDT(input, this);
		ServerSocket serverSocket = null;
		Socket socket = null;
		try {
			System.out.println("Server starting on port " + port);
			serverSocket = new ServerSocket(port, 1); //Minimalistic server
			boolean keepAlive = true;
			PrintStream out = null;
			while (keepAlive) {
				socket = serverSocket.accept();
				BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				out = new PrintStream(socket.getOutputStream());
				try {
					String line = reader.readLine();
					System.out.println("Got: " + line);
					if ("DIE".equals(line)) {
						out.println("BYE");
						keepAlive = false;
						System.exit(0);
					} else if ("CHECK".equals(line)) {
						out.println("OK");
					} else if (line.startsWith("LOG")) {
						logger = new PrintStream(line.substring(4));
					} else if (line.startsWith("RUN")){
						String filename = line.substring(4);
						BufferedReader in = new BufferedReader(new FileReader(filename));
						int nqueries = Integer.parseInt(in.readLine());
						List<TripleString> queries = new ArrayList<TripleString>(nqueries);
						for (int i = 0; i < nqueries; i++) {
							queries.add(parseQuery(in.readLine()));
						}
						in.close();
						Long startTime = System.currentTimeMillis();
						int nres = runQueries(queries);
						Long endTime = System.currentTimeMillis();

						System.out.println("Got " + nqueries + ". Results: " + nres + ", time: " + (endTime - startTime));
						out.println("Results: " + nres + ", time: " + (endTime - startTime));
						if (logger != null) {
							logger.close();
							logger = null;
						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					try {
						if (out != null) out.close();
						if (socket != null) socket.close();
					} catch (Exception e) {}

				}

			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (hdt != null) hdt.close();
				if (serverSocket != null) serverSocket.close();
			} catch (Exception e) {
			}
		}

	}
	
	/* (non-Javadoc)
	 * @see hdt.ProgressListener#notifyProgress(float, java.lang.String)
	 */
	@Override
	public void notifyProgress(float level, String message) {
		//System.out.println(message + "\t"+ Float.toString(level));
	}
	
	public static void main(String[] args) throws Throwable {
		HdtServer hdtServer = new HdtServer();
		JCommander com = new JCommander(hdtServer, args);
		com.setProgramName("hdtSearch");


		hdtServer.execute();
	}

	
}
