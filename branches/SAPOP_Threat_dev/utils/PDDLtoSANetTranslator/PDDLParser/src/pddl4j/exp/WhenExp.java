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
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This class implements a conditional expression of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class WhenExp extends AbstractExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -1373199261710247868L;

    /**
     * The condition of the expression.
     */
    private Exp condition;

    /**
     * The effect of the conditional effect expression.
     */
    private Exp effect;

    /**
     * Creates a new conditional expression.
     * 
     * @param condition The condition expression.
     * @param effect The effect expression.
     * @throws NullPointerException if <code>condition == null</code> or
     *             <code>effect == null</code>.
     */
    public WhenExp(Exp condition, Exp effect) {
        super(ExpID.WHEN);
        if (condition == null || effect == null)
            throw new NullPointerException();
        this.condition = condition;
        this.effect = effect;
    }
    
    /**
     * Creates a new conditional expression.
     */
    private WhenExp() {
        super(ExpID.WHEN);
        this.condition = null;
        this.effect = null;
    }
    
    /**
     * Returns the condition of this conditional expression.
     * 
     * @return the condition of this conditional expression.
     */
    public final Exp getCondition() {
        return this.condition;
    }

    /**
     * Sets a new condition to this this conditional expression.
     * 
     * @param condition the new condition to set.
     * @throws NullPointerException if <code>condition == null</code>.
     */
    public final void setCondition(Exp condition) {
        if (condition == null)
            throw new NullPointerException();
        this.condition = condition;
    }

    /**
     * Returns the effect of this conditional expression.
     * 
     * @return the effect of this conditional expression.
     */
    public final Exp getEffect() {
        return this.effect;
    }

    /**
     * Sets a new effect to this conditional expression.
     * 
     * @param effect The effect to set.
     * @throws NullPointerException if <code>condition == null</code>.
     */
    public final void setEffect(Exp effect) {
        if (effect == null)
            throw new NullPointerException();
        this.effect = effect;
    }

    /**
     * Returns <code>true</code> if a term occurs in this when expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this when expression;
     *         <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.
     */
    public final boolean occurs(Term term) {
        if (term == null)
            throw new NullPointerException();
        return this.condition.occurs(term) || this.effect.occurs(term);
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
    public WhenExp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        return new WhenExp(this.condition.apply(sigma),
                    this.effect.apply(sigma));
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     * @see pddl4j.exp.Exp#standardize()
     */
    public WhenExp standardize() {
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
     * @see pddl4j.exp.Exp#standardize(Map)
     */
    public WhenExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        return new WhenExp(this.condition.standardize(images),
                    this.effect.standardize(images));
    }

    /**
     * Returns <code>true</code> if the expression is ground.
     * 
     * @return <code>true</code> if the expression is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public final boolean isGround() {
        return this.condition.isGround() && this.effect.isGround();
    }

    /**
     * Returns <code>true</code> if this conditional expression is equal to an
     * other object, i.e., if the object is a not null reference to an instance
     * of the class <code>WhenExp</code> and both conditional expressions have
     * the same condition and effect.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if this conditional expression is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof WhenExp) {
            WhenExp other = (WhenExp) obj;
            return this.getExpID().equals(other.getExpID())
                        && this.condition.equals(other.condition)
                        && this.effect.equals(other.effect);
        }
        return false;
    }

    /**
     * Returns a hash code value for the conditional expression. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the conditional expression.
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.condition.hashCode()
                    + this.effect.hashCode();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public WhenExp clone() {
        WhenExp other = (WhenExp) super.clone();
        other.condition = this.condition.clone();
        other.effect = this.effect.clone();
        return other;
    }
    
    /**
     * Normalize this when expression. The condition is converted into
     * disjunctive normal form and effect in conjunctive normal form.
     * 
     * @return a normalize copy of this when expression.
     */
    public WhenExp normalize() {
        return new WhenExp(this.condition.moveQuantifierOutward().toDisjunctiveNormalForm(), 
                    this.effect.toConjunctiveNormalForm());
    }
    
    /**
     * Returns a copy of this expression such that the quantifiers appear first.
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     * @see pddl4j.exp.Exp#moveQuantifierOutward()
     */
    public Exp moveQuantifierOutward() {
        return new WhenExp(this.condition.moveQuantifierOutward(), 
                    this.effect.moveQuantifierOutward());
    }

    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public WhenExp toDisjunctiveNormalForm() {
        return new WhenExp(this.condition.toDisjunctiveNormalForm(),
                    this.effect.toDisjunctiveNormalForm());
    }
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public WhenExp toConjunctiveNormalForm() {
        return new WhenExp(this.condition.toConjunctiveNormalForm(),
                    this.effect.toConjunctiveNormalForm());
    }

    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy.
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public WhenExp toNegativeNormalForm() {
        return new WhenExp(this.condition.toNegativeNormalForm(), this.effect
                    .toNegativeNormalForm());
    }
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new LinkedHashSet<Variable>();
        vars.addAll(this.condition.getFreeVariables());
        vars.addAll(this.effect.getFreeVariables());
        return vars;
    }
    
    /**
     * Returns a string representation of the conditional expression.
     * 
     * @return a string representation of the conditional expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(when ");
        str.append(this.condition.toString());
        str.append(" ");
        str.append(this.effect.toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of the conditional expression.
     * 
     * @return a typed string representation of the conditional expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(when ");
        str.append(this.condition.toTypedString());
        str.append(" ");
        str.append(this.effect.toTypedString());
        str.append(")");
        return str.toString();
    }
    
}
