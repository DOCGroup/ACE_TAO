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

package pddl4j.exp.cond;

import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.Exp;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This class implements an unary conditional expression of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class UnaryCondExp extends CondExp {

    /**
     * The goal description of the unary conditional expression.
     */
    private Exp arg;

    /**
     * Creates a new unary conditional expression with a specific expression.
     * 
     * @param cond The condition of the unary conditional expression.
     * @param arg the expression.
     * @throws NullPointerException if <code>cond == null</code> or <code>arg == null</code>.
     */
    public UnaryCondExp(Condition cond, Exp arg) {
        super(cond);
        if (arg == null)
            throw new NullPointerException();
        this.arg = arg;
    }

    /**
     * Sets a new goal description to the unary conditional expression
     * 
     * @param arg the new expression to set.
     * @throws NullPointerException if <code>arg == null</code>.
     */
    public final void setArg(Exp arg) {
        if (arg == null)
            throw new NullPointerException();
        this.arg = arg;
    }

    /**
     * Returns the expression of the unary conditional expression.
     * 
     * @return the expression of the unary conditional expression.
     */
    public final Exp getArg() {
        return this.arg;
    }

    /**
     * Returns <code>true</code> if a term occurs in this binary conditional
     * expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this binary conditional
     *         expression; <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.        
     */
    public final boolean occurs(Term term) {
        if (term == null)
            throw new NullPointerException();
        return this.arg.occurs(term);
    }
    
    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding
     * term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public UnaryCondExp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        UnaryCondExp other = this.clone();
        other.arg = this.arg.apply(sigma);
        return other;
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public UnaryCondExp standardize() {
        return this.standardize(new LinkedHashMap<String, String>());
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
     */
    public UnaryCondExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        UnaryCondExp other = this.clone();
        other.arg = this.arg.standardize(images);
        return other;
    }

    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new LinkedHashSet<Variable>();
        vars.addAll(this.arg.getFreeVariables());
        return vars;
    }
    
    /**
     * Returns <code>true</code> if the expression is ground.
     * 
     * @return <code>true</code> if the expression is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public final boolean isGround() {
        return this.arg.isGround();
    }

    /**
     * Returns <code>true</code> if this unary conditional expression is
     * equal to an other object, i.e., if the object is a not null reference to
     * an instance of the class <code>UnaryConExp</code> and both 
     * conditional expressions have the same expression.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this unary conditional expression is
     *         equal to an other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof UnaryCondExp) {
            UnaryCondExp other = (UnaryCondExp) obj;
            return super.equals(obj) && this.arg.equals(other.arg);
        }
        return false;
    }

    /**
     * Returns the hash code value of this unary conditional expression.
     * 
     * @return the hash code value of this unary conditional expression.
     */
    public int hashCode() {
        return super.hashCode() + this.arg.hashCode();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public UnaryCondExp clone() {
        UnaryCondExp other = (UnaryCondExp) super.clone();
        other.arg = this.arg.clone();
        return other;
    }
    
    /**
     * Returns a copy of this expression such that the quantifiers appear first.
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     * @see pddl4j.exp.Exp#moveQuantifierOutward()
     */
    public UnaryCondExp moveQuantifierOutward() {
        UnaryCondExp other = (UnaryCondExp) super.clone();
        other.arg = this.arg.moveQuantifierOutward();
        return other;
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public UnaryCondExp toDisjunctiveNormalForm() {
        UnaryCondExp other = this.clone();
        other.arg = this.arg.toDisjunctiveNormalForm();
        return other;
    }
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public UnaryCondExp toConjunctiveNormalForm() {
        UnaryCondExp other = this.clone();
        other.arg = this.arg.toConjunctiveNormalForm();
        return other;
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public UnaryCondExp toNegativeNormalForm() {
        UnaryCondExp other = this.clone();
        other.arg = this.arg.toNegativeNormalForm();
        return other;
    }
}