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

package pddl4j.exp.action;

import java.io.Serializable;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import pddl4j.exp.term.Term;

/**
 * This interface is implemented by all type of action defined in the PDDL
 * language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public interface ActionDef extends Iterable<Term>, Serializable, Cloneable {
    
    /**
     * Returns the action id of this action.
     * 
     * @return the action id of this action.
     * @see pddl4j.exp.action.ActionID
     */
    ActionID getActionID();
    
    /**
     * Returns the image of the action.
     * 
     * @return the image of this action.
     */
    String getName();

    /**
     * Returns the parameters of the action.
     * 
     * @return the parameters of the action.
     */
    List<Term> getParameters();
    
    /**
     * Adds a parameter to this action.
     * 
     * @param param the parameters to add.
     * @return <code>true</code> if the parameter was added;
     *         <code>false</code> otherwise.
     */
    boolean add(Term param);
    
    /**
     * Returns an iterator over the parameters of the action.
     * 
     * @return an iterator over the parameters of the action.
     */
    Iterator<Term> iterator();
    /**
     * Returns <code>true</code> if the expression is ground.
     * 
     * @return <code>true</code> if the expression is ground;
     *         <code>false</code>otherwise.
     */
    boolean isGround();
    
    /**
     * Returns <code>true</code> if this expression is equal to an other
     * object, i.e., if the object is a not null instance of the class of the
     * expression and both expressions have the same content.
     * 
     * @param obj the reference object with which to compare.
     * @return <code>true</code> if this expression is equal to an other
     *         object; <code>false</code> otherwise.
     */
    boolean equals(Object obj);

    /**
     * Returns a hash code value for the expression. This method is supported
     * for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the expression.
     */
    int hashCode();

    /**
     * Returns a string representation of the expression.
     * 
     * @return a string representation of the expression.
     */
    String toString();
    
    /**
     * Returns <code>true</code> if a term occurs in the parameters of this action.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this expression;
     *         <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.        
     */
    boolean occurs(Term term);
       
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    ActionDef standardize();

    /**
     * Normalize this action. This method returns a copy of this action where:
     * <ul>
     * <li> each variable is standardized,</li>
     * <li> precondition expression is convert into disjunctive prenex normal
     * form and</li>
     * <li> effect expression is is convert into conjunctive prenex normal form.</li>
     * <ul>
     * 
     * @return a normalize copy of this action.
     * @see pddl4j.exp.Exp#toPrenexNormaForm()
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    ActionDef normalize();
    
    /**
     * Returns a deep copy of this action. Creates and returns a copy of this
     * action. The precise meaning of "copy" is as follow:
     * 
     * <pre>
     * x.clone() != x
     * </pre>
     * 
     * will be true, and that the expression:
     * 
     * <pre>
     * x.clone().getClass() == x.getClass()
     * </pre>
     * 
     * will be true. While it is the case that:
     * 
     * <pre>
     * x.clone().equals(x)
     * </pre>
     * 
     * will be true. Finally, note that except type of term expression are
     * deeply copy.
     * 
     * @return a deep copy of this expression.
     * @see java.lang.Cloneable
     */
    ActionDef clone();
    
    /**
     * Returns a typed string of this action.
     * 
     * @return a typed string of this action.
     */
    String toTypedString();
    
}
