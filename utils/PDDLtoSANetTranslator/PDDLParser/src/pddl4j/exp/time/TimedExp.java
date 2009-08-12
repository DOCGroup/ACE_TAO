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

package pddl4j.exp.time;

import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.AbstractExp;
import pddl4j.exp.Exp;
import pddl4j.exp.ExpID;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * The abstract generic class implements the common methods of all timed
 * expression.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class TimedExp extends AbstractExp {
    
    /**
     * The timed expression id of this times expression.
     */
    private TimedExpID id;
    
    /**
     * The expression contained in this timed expression.
     */
    protected Exp exp;

    /**
     * Creates a new timed goal description expression with a specific id and an
     * expression.
     * 
     * @param id The timed expression id of the expression.
     * @param exp The timed expression.
     * @throws NullPointerException if <code>id == null</code> or
     *             <code> exp == null</code>.
     */
    protected TimedExp(TimedExpID id, Exp exp) {
        super(ExpID.TIMED_EXP);
        if (exp == null)
            throw new NullPointerException();
        this.id = id;
        this.exp = exp;
    }

    /**
     * Returns the timed expression id of this times expression.
     * 
     * @return the timed expression id of this times expression.
     * @see pddl4j.exp.time.TimedExpID
     */
    public final TimedExpID getTimeExpID() {
        return this.id;
    }
    
    /**
     * Returns the expression contained in the timed expression.
     * 
     * @return the expression contained in the timed expression.
     */
    public Exp getExp() {
        return this.exp;
    }

    /**
     * Sets a new expression contained in the timed expression.
     * 
     * @param exp the new expression to set.
     * @throws NullPointerException if <code>exp == null</code>.
     */
    public final void setExp(Exp exp) {
        if (exp == null)
            throw new NullPointerException();
        this.exp = exp;
    }
    
    /**
     * Returns <code>true</code> if a term occurs in this timed expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this timed expression;
     *         <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.
     */
    public final boolean occurs(Term term) {
        if (term == null)
            throw new NullPointerException();
        return this.exp.occurs(term);
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
    public TimedExp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        TimedExp other = this.clone();
        other.exp = this.exp.apply(sigma);
        return other;
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
    public TimedExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        TimedExp other = this.clone();
        other.exp = this.exp.standardize(images);
        return other;
    }

    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new LinkedHashSet<Variable>();
        vars.addAll(this.exp.getFreeVariables());
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
        return this.exp.isGround();
    }
    
    /**
     * Returns <code>true</code> if the timed expression is equal to an other
     * object. This method returns <code>true</code> if the object is a not
     * null reference to an instance of the class <code>TimedExp</code> and
     * both timed expression have the same timed expression id and the same
     * expression.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if the timed expression equal to an other
     *         object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && this.getClass().equals(obj.getClass())) {
            TimedExp other = (TimedExp) obj;
            return super.equals(obj) && this.exp.equals(other.exp);
        }
        return false;
    }

    /**
     * Returns a hash code value for the timed expression. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the timed expression.
     */
    public int hashCode() {
        return super.hashCode() + this.exp.hashCode();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public TimedExp clone() {
        TimedExp other = (TimedExp) super.clone();
        other.exp = this.exp.clone();
        return other;
    }
    
    /**
     * Returns a copy of this expression such that the quantifiers appear first.
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     * @see pddl4j.exp.Exp#moveQuantifierOutward()
     */
    public Exp moveQuantifierOutward() {
        TimedExp other = (TimedExp) super.clone();
        other.exp = this.exp.moveQuantifierOutward();
        return other;
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public TimedExp toDisjunctiveNormalForm() {
        TimedExp other = this.clone();
        other.exp = this.exp.toDisjunctiveNormalForm();
        return other;
    }
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public TimedExp toConjunctiveNormalForm() {
        TimedExp other = this.clone();
        other.exp = this.exp.toConjunctiveNormalForm();
        return other;
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public TimedExp toNegativeNormalForm() {
        TimedExp other = this.clone();
        other.exp = this.exp.toNegativeNormalForm();
        return other;
    }
}
