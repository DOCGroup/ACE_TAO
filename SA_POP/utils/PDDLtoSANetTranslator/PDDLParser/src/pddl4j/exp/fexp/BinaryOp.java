/*
 * Copyright Dept. of Mathematics & Computer Science Univ. Paris-Descartes
 *
 * This software is governed by the CeCILL  license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */

package pddl4j.exp.fexp;

import pddl4j.exp.term.Term;

/**
 * This class implements an binary arithmetic function.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class BinaryOp extends OpExp {

    /**
     * Creates a new binary arithemtic function with a specified operator and two arguments.
     * 
     * @param op The arthemitic operator of the function.
     * @param arg1 The first argument of the function.
     * @param arg2 The second argument of the function.
     * @throws NullPointerException if <code>arg1 == null</code> or
     *             <code>arg2 == null</code>.            
     */
    protected BinaryOp(Op op, Term arg1, Term arg2) {
        super(op);
        super.add(arg1);
        super.add(arg2);
    }

    /**
     * Sets the fisrt argument of this arithmetic operation.
     * 
     * @param arg1 the new fisrt argument of this arithmetic function.
     * @throws NullPointerException if <code>arg1 == null</code>.
     */
    public final void setArg1(Term arg1) {
        super.set(0, arg1);
    }

    /**
     * Returns the fisrt argument of the arithmetic operation.
     * 
     * @return the fisrt argument of the arithmetic operation.
     */
    public final Term getArg1() {
        return super.get(0);
    }

    /**
     * Sets the second argument of this arithmetic operation.
     * 
     * @param arg2 the new second argument of this arithmetic function.
     * @throws NullPointerException if <code>arg2 == null</code>.
     */
    public final void setArg2(Term arg2) {
        super.set(1, arg2);
    }

    /**
     * Returns the second argument of the arithmetic function.
     * 
     * @return the second argument of the arithmetic function.
     */
    public final Term getArg2() {
        return super.get(1);
    }
    
}
