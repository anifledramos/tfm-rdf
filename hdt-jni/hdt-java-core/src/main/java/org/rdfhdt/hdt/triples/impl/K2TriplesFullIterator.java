/**
 * File: $HeadURL: https://hdt-java.googlecode.com/svn/trunk/hdt-java/src/org/rdfhdt/hdt/triples/impl/BitmapTriplesIterator.java $
 * Revision: $Rev: 191 $
 * Last modified: $Date: 2013-03-03 11:41:43 +0000 (dom, 03 mar 2013) $
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

package org.rdfhdt.hdt.triples.impl;

import org.rdfhdt.hdt.enums.ResultEstimationType;
import org.rdfhdt.hdt.enums.TripleComponentOrder;
import org.rdfhdt.hdt.triples.IteratorTripleID;
import org.rdfhdt.hdt.triples.TripleID;

/**
 * @author guillermo.debernardo
 *
 */
public class K2TriplesFullIterator implements IteratorTripleID {

	private int currentPredicate = -1;
	private int nPredicates;
	private K2Triples k2triples;
	private K2TriplesIterator currentIterator;
	
	K2TriplesFullIterator(K2Triples triples, int npreds) {
		this.k2triples = triples;
		this.nPredicates = npreds;
		initializePredicate();
		initializeNextPredicate();
	}

	@Override
	public boolean hasNext() {
		do {
			if (currentIterator.hasNext()) {
				return true;
			} else {
				initializeNextPredicate();
			}	
		} while (!finished());
		return false;
	}

	@Override
	public TripleID next() {
		return currentIterator.next();
	}

	@Override
	public boolean hasPrevious() {
		return false;
	}

	@Override
	public TripleID previous() {
		throw new UnsupportedOperationException("Not yet implemented for this pattern");
	}

	@Override
	public void goToStart() {
		initializePredicate();
		initializeNextPredicate();
	}

	@Override
	public boolean canGoTo() {
		return false;
	}

	@Override
	public void goTo(long pos) {
		throw new IllegalAccessError("Cannot goto on this pattern");
	}

	@Override
	public long estimatedNumResults() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public ResultEstimationType numResultEstimation() {
		return ResultEstimationType.UNKNOWN;
	}

	@Override
	public TripleComponentOrder getOrder() {
		return TripleComponentOrder.Unknown;
	}
	
	private void initializePredicate() {
		currentPredicate = -1;
	}
	
	private void initializeNextPredicate() {
		
		currentPredicate ++;
		if (currentPredicate < nPredicates) {
			this.currentIterator = (K2TriplesIterator) k2triples.search(new TripleID(0, currentPredicate+1, 0));
		}
	}
	
	private boolean finished() {
		return currentPredicate >= nPredicates;
	}
	
}
