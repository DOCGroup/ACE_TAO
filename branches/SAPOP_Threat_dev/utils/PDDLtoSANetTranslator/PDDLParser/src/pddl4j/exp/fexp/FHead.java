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
import pddl4j.exp.term.TermID;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;

/**
 * This class implements a function head of the PDDL langage.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public final class FHead extends FExp {

    /**
     * The serial version id of this class.
     */
    private static final long serialVersionUID = -2343417682285770327L;

    /**
     * Create an empty function head with a specific functor.
     * 
     * @param functor the function functor. 
     * @param type the type of this function.
     * @throws NullPointerException if <code>functor == null</code> or
     *             <code>type == null</code>.
     */
    public FHead(String functor, TypeSet type) {
        super(TermID.FUNCTION, functor, type);
    }
    
    /**
     * Create an empty function head with a specific functor.
     * 
     * @param functor the function functor.
     * @param type the type of this function head.
     * @throws NullPointerException if <code>functor == null</code>
     */
    public FHead(String functor, Type type) {
        super(TermID.FUNCTION, functor, new TypeSet(type));
    } 
    
    /**
     * Adds a new argument to this function.
     * 
     * @param arg The argument to add.
     * @return <code>true</code> if the arg was added; <code>false</code>
     *         otherwise.
     * @throws NullPointerException if <code>arg == null</code>.
     */
    public boolean add(Term arg) {
        return super.add(arg);
    }

    /**
     * Returns the argument at the specified position in this function.
     * 
     * @param index index of the argument to return.
     * @return the argument at the specified position in this function.
     * @throws IndexOutOfBoundsException if the index is out of range 
     *      (<code>index < 0  || index >= getArity()</code>).
     */
    public Term get(int index) {
        return super.get(index);
    }
    
    /**
     * Replaces the argument at the specified position in this function with the
     * specified term.
     * 
     * @param index index of the argument to replace.
     * @param arg argument to be stored at the specified position.
     * @return the argument previously at the specified position.
     * @throws IndexOutOfBoundsException if the index is out of range (<code>index < 0</code>
     *             or <code>index >= getArity()</code>).
     * @throws NullPointerException if <code>arg == null</code>.
     */
    public Term set(int index, Term arg) {
        return super.set(index, arg);
    }
    
    /**
     * Sets a new type of this function.
     * 
     * @param type the type to set.
     * @throws NullPointerException if <code>type == null</code>.
     */
    public void setTypeSet(TypeSet type) {
        super.setTypeSet(type);
    }
    

    /**
     * Standardizes all occurences of the variables that occur in this
     * term.
     * 
     * @return this term.
     */
    public FHead standardize() {
        return (FHead) super.standardize();
    }
    
    /**
     * Returns a deep copy of this arithmetic function.
     * 
     * @return a deep copy of this arithmetic function.
     * @see pddl4j.exp.AbstractExp#clone()
     */
    public FHead clone() {
        return (FHead) super.clone();
    }
    
}
