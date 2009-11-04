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

package pddl4j.exp;

import java.io.Serializable;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This interface is implemented by all expression of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public interface Exp extends Serializable, Cloneable {
    
    /**
     * Returns the id of the expression.
     * 
     * @return the id of the expression.
     */
    ExpID getExpID();
    
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
     * Returns <code>true</code> if a term occurs in this expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this expression;
     *         <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.        
     */
    boolean occurs(Term term);
    
    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are bound in the substitution by its binding term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    Exp apply(Substitution sigma);
       
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression. 
     */
    Exp standardize();
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. The map argument is used to store the variable already
     * standardized. Remember that free variables are existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this expression.
     */
    Exp standardize(Map<String, String> images);

    /**
     * Returns a typed string of this expression.
     * 
     * @return a typed string of this expression.
     */
    String toTypedString();
    
    /**
     * Returns a deep copy of this expression. Creates and returns a copy of
     * this expression. The precise meaning of "copy" is as follow:
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
    Exp clone();
    
    /**
     * Returns the prenex normal form of this expression. Note, the expression
     * returned is a copy. The expression is first standardized and it is placed
     * in the prenex normal form by recursively moving quantifier inward with
     * the existential quantifier first using the following rewriting rules
     * (which are in fact equivalences):
     * <ul>
     * <li><code>(and (forall (x) A) B) => (forall (x) (and A B))</code></li>
     * <li><code>(or (forall (x) A) B) => (forall (x) (or A B))</code></li>
     * <li><code>(and (exists (x) A) B) => (exists (x) (and A B))</code></li>
     * <li><code>(or (exists (x) A) B) => (exists (x) (or A B))</code></li>
     * <li> <code>(not (exists (x) (A))) => (forall (x) (not A)) </code></li>
     * <li> <code>(not (forall (x) (A))) => (exists (x) (not A)) </code></li>
     * <li> <code>(imply A (forall (x) (B))) => (forall (x) (imply A B)) </code></li>
     * <li> <code>(imply A (exists (x) (B))) => (exists (x) (imply A B)) </code></li>
     * <li> <code>(imply (forall (x) (A)) B) => (exists (x) (imply A B)) </code></li>
     * <li> <code>(imply (exists (x) (A)) B) => (forall (x) (imply A B)) </code></li>
     * </ul>
     * 
     * @return the prenex normal form of this expression.
     */
    Exp toPrenexNormaForm();
    
    /**
     * Returns a copy of this expression such that the quantifiers appear first
     * and in which all existential quantifiers precede all universal
     * quantifiers
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     */
    Exp moveQuantifierOutward();
    
    /**
     * Returns the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. The expression is first converted into
     * negative normal form and it is placed in the disjunctive normal form by
     * recursively moving conjunctions inward and disjunctions outward using the
     * following rewriting rules (which are in fact equivalences):
     * <ul>
     * <li><code>(and A (or B C)) => (or (and A B) (and A C))</code></li>
     * <li><code>(and (or A B) C)) => (or (and A C) (and B C))</code></li>
     * </ul>
     * 
     * @return the disjunctive normal form of this expression.
     */
    Exp toDisjunctiveNormalForm();
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy. The expression is first converted into
     * negative normal form and it is placed in the conjunctive normal form by
     * recursively moving disjunction inward and conjunction outward using the
     * following rewriting rules (which are in fact equivalences):
     * <ul>
     * <li><code>(or A (and B C)) => (and (or A B) (or A C))</code></li>
     * <li><code>(or (and A B) C)) => (and (or A C) (or B C))</code></li>
     * </ul>
     * 
     * @return the disjunctive normal form of this expression.
     */
    Exp toConjunctiveNormalForm();
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. The negative normal form is obtained by recursively
     * replace formulas appearing on the left with formulas appearing on the
     * right:
     * <ul>
     * <li> <code>(imply A B) => (or (not A) B) </code></li>
     * <li> <code>(not (not F)) => F </code></li>
     * <li> <code>(not (or A B)) => (and (not A) (not B)) </code></li>
     * <li> <code>(not (and A B)) => (or (not A) (not B)) </code></li>
     * <li> <code>(not (imply A B)) => (and A (not B)) </code></li>
     * <li> <code>(not (exists (x) (F))) => (forall (x) (not F)) </code></li>
     * <li> <code>(not (forall (x) (F))) => (exists (x) (not F)) </code></li>
     * <li> default a copy of the expression where inner expressions are in
     * negative normal form</li>
     * 
     * @return the negative normal form of this expression.
     */
    Exp toNegativeNormalForm();
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    Set<Variable> getFreeVariables();
    
}
