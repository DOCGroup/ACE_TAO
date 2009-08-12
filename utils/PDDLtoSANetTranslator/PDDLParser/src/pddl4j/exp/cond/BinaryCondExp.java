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
 * This class implements a binary conditional expression.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class BinaryCondExp extends CondExp {

    /**
     * The first expression argument of this conditional binary expression.
     */
    private Exp arg1;

    /**
     * The second expression argument of this conditional binary expression.
     */
    private Exp arg2;

    /**
     * Creates a new binary conditional expression with a specific expression.
     * 
     * @param cond The condition of the binary conditional expression.
     * @param arg1 the first expression argument.
     * @param arg2 the second expression argument.
     * @throws NullPointerException if <code>id == null</code> or
     *             <code>arg1 == null</code> or <code>arg2 == null</code>.
     */
    public BinaryCondExp(Condition cond, Exp arg1, Exp arg2) {
        super(cond);
        if (arg1 == null || arg2 == null)
            throw new NullPointerException();
        this.arg1 = arg1;
        this.arg2 = arg2;
    }

    /**
     * Sets the new first expression argument of this binary conditional
     * expression.
     * 
     * @param arg1 the first expression argument.
     * @throws NullPointerException if <code>arg1 == null</code>.
     */
    public final void setArg1(Exp arg1) {
        if (arg1 == null)
            throw new NullPointerException();
        this.arg1 = arg1;
    }

    /**
     * Returns the first expression argument of this binary conditional
     * expression.
     * 
     * @return the first expression argument of this binary conditional
     *         expression.
     */
    public final Exp getArg1() {
        return this.arg1;
    }

    /**
     * Sets the new second expression argument of this binary conditional
     * expression.
     * 
     * @param arg2 the second expression argument. 
     * @throws NullPointerException if <code>arg2 == null</code>.
     */
    public final void setArg2(Exp arg2) {
        this.arg2 = arg2;
    }

    /**
     * Returns the second expression argument of this binary conditional
     * expression.
     * 
     * @return the second expression argument of this binary conditional
     *         expression.
     */
    public final Exp getArg2() {
        return this.arg2;
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
        return this.arg1.occurs(term) || this.arg2.occurs(term);
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
    public BinaryCondExp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        BinaryCondExp other = this.clone();
        other.arg1 = this.arg1.apply(sigma);
        other.arg2 = this.arg2.apply(sigma);
        return other;
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are
     * existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public BinaryCondExp standardize() {
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
    public BinaryCondExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        BinaryCondExp other = this.clone();
        other.arg1 = this.arg1.standardize(images);
        other.arg2 = this.arg2.standardize(images);
        return other;
    }

    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new LinkedHashSet<Variable>();
        vars.addAll(this.arg1.getFreeVariables());
        vars.addAll(this.arg2.getFreeVariables());
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
        return this.arg1.isGround() && this.arg2.isGround();
    }

    /**
     * Returns <code>true</code> if this binary conditional expression is
     * equal to an other object, i.e., if the object is a not null reference to
     * an instance of the class <code>BinaryCondExp</code> and both binary
     * conditional expressions have the same two expression arguments and the
     * same time stamp.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this conditional expression is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof BinaryCondExp) {
            BinaryCondExp other = (BinaryCondExp) obj;
            return super.equals(obj) && this.arg1.equals(other.arg1)
                        && this.arg2.equals(other.arg2);
        }
        return false;
    }

    /**
     * Returns the hash code value of this binary conditional expression.
     * 
     * @return the hash code value of this binary conditional expression.
     */
    public int hashCode() {
        return super.hashCode() + this.arg1.hashCode() + this.arg2.hashCode();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public BinaryCondExp clone() {
        BinaryCondExp other = (BinaryCondExp) super.clone();
        other.arg1 = this.arg1.clone();
        other.arg2 = this.arg2.clone();
        return other;
    }

    /**
     * Returns a copy of this expression such that the quantifiers appear first.
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     * @see pddl4j.exp.Exp#moveQuantifierOutward()
     */
    public BinaryCondExp moveQuantifierOutward() {
        BinaryCondExp other = (BinaryCondExp) super.clone();
        other.arg1 = this.arg1.moveQuantifierOutward();
        other.arg2 = this.arg2.moveQuantifierOutward();
        return other;
    }

    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public BinaryCondExp toDisjunctiveNormalForm() {
        BinaryCondExp other = this.clone();
        other.arg1 = this.arg1.toDisjunctiveNormalForm();
        other.arg2 = this.arg2.toDisjunctiveNormalForm();
        return other;
    }

    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public BinaryCondExp toConjunctiveNormalForm() {
        BinaryCondExp other = this.clone();
        other.arg1 = this.arg1.toConjunctiveNormalForm();
        other.arg2 = this.arg2.toConjunctiveNormalForm();
        return other;
    }

    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public BinaryCondExp toNegativeNormalForm() {
        BinaryCondExp other = this.clone();
        other.arg1 = this.arg1.toNegativeNormalForm();
        other.arg2 = this.arg2.toNegativeNormalForm();
        return other;
    }
}