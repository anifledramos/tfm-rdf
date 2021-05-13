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

import java.util.Arrays;

import org.rdfhdt.hdt.enums.ResultEstimationType;
import org.rdfhdt.hdt.enums.TripleComponentOrder;
import org.rdfhdt.hdt.triples.IteratorTripleID;
import org.rdfhdt.hdt.triples.TripleID;

/**
 * @author guillermo.debernardo
 *
 */
public class JNITriplesIterator implements IteratorTripleID {

	protected TripleID query;
	protected boolean fixedS = false, fixedP = false, fixedO = false;
	protected int [] s;
	protected int [] p;
	protected int [] o;
	protected int current;
	protected int size;
	protected TripleID returnTriple;
	protected TripleComponentOrder order;
	
	JNITriplesIterator(TripleID query, int nres, int [] s, int [] p, int [] o) {
		this(query, nres, s, p, o, TripleComponentOrder.Unknown);
	}
	
	JNITriplesIterator(TripleID query, int nres, int [] s, int [] p, int [] o, TripleComponentOrder order) {
		this.query = query;
		this.current = 0;
		this.order = order;
		this.size = nres;
		returnTriple = new TripleID();
		computeFixed(s, p, o);		
//		System.out.println("QUERY: " + query.getSubject()+"--"+query.getPredicate() + "--" + query.getObject());
//		System.out.println("S= " + Arrays.toString(s));
//		System.out.println("P= " + Arrays.toString(p));
//		System.out.println("O= " + Arrays.toString(o));
	}
	
	private void computeFixed(int [] s, int [] p, int [] o) {
		if (query.getSubject() > 0) {
			fixedS = true;
			returnTriple.setSubject(query.getSubject());
		} else {
			this.s = s;
		}
		if (query.getPredicate() > 0) {
			fixedP = true;
			returnTriple.setPredicate(query.getPredicate());
		} else {
			this.p = p;
		}
		if (query.getObject() > 0) {
			fixedO = true;
			returnTriple.setObject(query.getObject());
		} else {
			this.o = o;
		}
	}
	
	
	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#hasNext()
	 */
	@Override
	public boolean hasNext() {
		return current<size;
	}

	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#next()
	 */
	@Override
	public TripleID next() {
		updateTriple();
		
		current ++;
		
		return returnTriple;
	}

	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#hasPrevious()
	 */
	@Override
	public boolean hasPrevious() {
		return current > 0;
	}

	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#previous()
	 */
	@Override
	public TripleID previous() {
		current --;
		updateTriple();
				
		return returnTriple;
	}

	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#goToStart()
	 */
	@Override
	public void goToStart() {
		current = 0;
	}

	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#estimatedNumResults()
	 */
	@Override
	public long estimatedNumResults() {
		return size;
	}

	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#numResultEstimation()
	 */
	@Override
	public ResultEstimationType numResultEstimation() {
	    return ResultEstimationType.EXACT;
	}

	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#canGoTo()
	 */
	@Override
	public boolean canGoTo() {
		return true;
	}

	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#goTo(int)
	 */
	@Override
	public void goTo(long pos) {
		current = (int) pos;
	}
	
	/* (non-Javadoc)
	 * @see hdt.iterator.IteratorTripleID#getOrder()
	 */
	@Override
	public TripleComponentOrder getOrder() {
		return TripleComponentOrder.Unknown;
	}
	
	/* (non-Javadoc)
	 * @see java.util.Iterator#remove()
	 */
	@Override
	public void remove() {
		throw new UnsupportedOperationException();
	}
	
	private void updateTriple() {
		if (!fixedS) returnTriple.setSubject 	(s[current+1]+1);
		if (!fixedP) returnTriple.setPredicate	(p[current+1]+1);
		if (!fixedO) returnTriple.setObject		(o[current+1]+1);
	}
}
