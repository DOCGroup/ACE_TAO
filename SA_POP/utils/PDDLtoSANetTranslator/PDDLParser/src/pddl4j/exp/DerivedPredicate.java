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

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This class implements a derived predicate of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public final class DerivedPredicate extends AbstractExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 523481664576398238L;

    /**
     * The expression that defines the derived predicate.
     */
    private ImplyExp exp;

    /**
     * Creates a new derived predicate with a specific head and body.
     * 
     * @param head the head of this derived predicate.
     * @param body the body of this derived predicate
     * @throws NullPointerException if <code>head == null || body == null</code>.
     */
    public DerivedPredicate(AtomicFormula head, Exp body) {
        super(ExpID.DERIVED_PREDICATE);
        if (head == null || body == null) 
            throw new NullPointerException();
        this.exp = new ImplyExp(head, body);
    }
    
    /**
     * Sets a new head to this derived predicate.
     * 
     * @param head the new head of this implication expression. The new head of
     *            the derived predicate must be a not null reference to an
     *            instance of <code>AtomicFormula</code>.
     * @throws NullPointerException if <code>head == null</code>.
     */
    public final void setHead(AtomicFormula head) {
        if (head == null) 
            throw new NullPointerException();
        this.exp.setHead(head);
    }

    /**
     * Returns the head of the derived predicate.
     * 
     * @return the head of the derived predicate.
     */
    public final AtomicFormula getHead() {
        return (AtomicFormula) this.exp.getHead();
    }

    /**
     * Sets a new body to this derived predicate.
     * 
     * @param body the new body of this derived predicate. The new body of
     *            the derived predicate must be a not null reference to an
     *            instance of <code>Exp</code>.
     * @throws NullPointerException if <code>body == null</code>.           
     */
    public final void setBody(Exp body) {
        if (body == null)
            throw new NullPointerException();
        this.setBody(body);
    }

    /**
     * Returns the body of the derived predicate.
     * 
     * @return the body of the derived predicate.
     */
    public final Exp getBody() {
        return this.exp.getBody();
    }
    
    /**
     * Returns <code>true</code> if a expression occurs in this derived predicate.
     * 
     * @param term the expression to be tested.
     * @return <code>true</code> if a expression occurs in this derived predicate;
     *         <code>false</code> otherwise.
     */
    public final boolean occurs(Term term) {
        return this.exp.occurs(term);
    }

    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding expression.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public final Exp apply(Substitution sigma) {
        if (sigma == null) 
            throw new NullPointerException();
        DerivedPredicate other = this.clone();
        other.exp = this.exp.apply(sigma);
        return other;
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.  
     * @see pddl4j.exp.Exp#standardize()
     */
    public DerivedPredicate standardize() {
        return this.standardize(new LinkedHashMap<String,String>());
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. The map argument is used to store the variable already
     * standardized. Remember that free variables are existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this expression.
     * @throws NullPointerException if <code>images == null</code>.
     * @see pddl4j.exp.Exp#standardize(Map)
     */
    public DerivedPredicate standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        DerivedPredicate other = this.clone();
        other.exp = this.exp.standardize(images);
        return other;
    }

    /**
     * Returns <code>true</code> if the expression is ground.
     * 
     * @return <code>true</code> if the expression is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public final boolean isGround() {
        return this.exp.isGround();
    }


    /**
     * Returns <code>true</code> if this derived predicate is equal to an
     * other object, i.e., if the object is a not null reference to an instance
     * of the class <code>DerivedPredicate</code> and both derived predicates
     * have the same parameters and expression.
     * 
     * @param obj - the reference object with which to compare.
     * @return <code>true</code> if this derived predicate is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof DerivedPredicate) {
            DerivedPredicate other = (DerivedPredicate) obj;
            return this.exp.equals(other.exp);
        }
        return false;
    }

    /**
     * Returns a hash code value for the derived predicate. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the derived predicate.
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.exp.hashCode();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public DerivedPredicate clone() {
        DerivedPredicate other = (DerivedPredicate) super.clone();
        other.exp = exp.clone();
        return other;
    }
    
    /**
     * Returns a copy of this expression such that the quantifiers appear first.
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     * @see pddl4j.exp.Exp#moveQuantifierOutward()
     */
    public Exp moveQuantifierOutward() {
        DerivedPredicate other = this.clone();
        other.exp.body = this.exp.body.moveQuantifierOutward();
        return other;
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public DerivedPredicate toDisjunctiveNormalForm() {
        DerivedPredicate dnf = this.clone();
        dnf.exp.body = this.exp.body.toDisjunctiveNormalForm();
        return dnf;
    }
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public DerivedPredicate toConjunctiveNormalForm() {
        DerivedPredicate cnf = this.clone();
        cnf.exp.body = this.exp.body.toConjunctiveNormalForm();
        return cnf;
    }
   
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public DerivedPredicate toNegativeNormalForm() {
        DerivedPredicate nnf = this.clone();
        nnf.exp.body = this.exp.body.toNegativeNormalForm();
        return nnf;
    }
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
       return this.exp.getFreeVariables();
    }
    
    /**
     * Returns a string representation of the derived predicate.
     * 
     * @return a string representation of the derived predicate.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(:derived ");
        str.append("(");
        str.append(this.exp.toString());
        str.append(")");
        str.append(" ");
        str.append(this.exp.toString());
        str.append(")");
        return str.toString();
    }
    
    /**
     * Returns a string representation of the derived predicate.
     * 
     * @return a string representation of the derived predicate.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(:derived ");
        str.append("(");
        str.append(this.exp.toTypedString());
        str.append(")");
        str.append(" ");
        str.append(this.exp.toTypedString());
        str.append(")");
        return str.toString();
    }

}
