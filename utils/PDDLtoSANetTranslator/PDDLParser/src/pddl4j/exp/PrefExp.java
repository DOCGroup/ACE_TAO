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
 * This class implements the common methods of all preference expressions of the
 * PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class PrefExp extends AbstractExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 2518753354183976408L;

    /**
     * The name of this preference.
     */
    private String name;

    /**
     * The goal description of the preference.
     */
    private Exp exp;

    /**
     * Creates a new preference expression with a specific expression name and
     * expression preference.
     * 
     * @param name The name of the preference.
     * @param exp The preference expression.
     * @throws NullPointerException if <code>name == null</code> or
     *             <code>exp == null</code>.
     */
    public PrefExp(String name, Exp exp) {
        super(ExpID.PREFERENCE);
        if (name == null || exp == null)
            throw new NullPointerException();
        this.name = name;
        this.exp = exp;
    }

    /**
     * Creates a new preference expression with a specific expression
     * preference.
     * 
     * @param exp The preference expression.
     * @throws NullPointerException if <code>exp == null</code>.
     */
    public PrefExp(Exp exp) {
        this(new String(), exp);
    }

    /**
     * Returns the name of the preference expression.
     * 
     * @return the name of the preference expression.
     */
    public final String getName() {
        return this.name;
    }

    /**
     * Sets a new name to the preference expression.
     * 
     * @param name the new name to set.
     * @throws NullPointerException if <code>name == null</code>.
     */
    public final void setName(String name) {
        if (name == null)
            throw new NullPointerException();
        this.name = name;
    }

    /**
     * Returns the expression describing the preference.
     * 
     * @return the expression describing the preference.
     */
    public final Exp getExp() {
        return this.exp;
    }

    /**
     * Sets a new expression describing the preference.
     * 
     * @param exp the new expression describing the preference.
     * @throws NullPointerException if <code>exp == null</code>.
     */
    public final void setExp(Exp exp) {
        if (exp == null)
            throw new NullPointerException();
        this.exp = exp;
    }

    /**
     * Returns <code>true</code> if a term occurs in this preference
     * expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this preference
     *         expression; <code>false</code> otherwise.
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
     * expression.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public PrefExp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        return new PrefExp(this.name, this.exp.apply(sigma));
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     * @see pddl4j.exp.Exp#standardize()
     */
    public PrefExp standardize() {
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
    public PrefExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        return new PrefExp(this.exp.standardize(images));
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
     * Returns <code>true</code> if this preference expression is equal to an
     * other object, i.e., if the object is a not null reference to an instance
     * of the class <code>PrefExp</code> and both preference expressions have
     * the name and expression.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if this preference expression is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof PrefExp) {
            PrefExp other = (PrefExp) obj;
            return this.getExpID().equals(other.getExpID())
                        && this.name.equals(other.name)
                        && this.exp.equals(other.exp);
        }
        return false;
    }

    /**
     * Returns a hash code value for the preference expression. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the preference expression.
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.name.hashCode()
                    + this.exp.hashCode();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public PrefExp clone() {
        PrefExp other = (PrefExp) super.clone();
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
        return new PrefExp(this.name, this.exp.moveQuantifierOutward());
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public PrefExp toDisjunctiveNormalForm() {
        return new PrefExp(this.name, this.exp.toDisjunctiveNormalForm());
    }
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public PrefExp toConjunctiveNormalForm() {
        return new PrefExp(this.name, this.exp.toConjunctiveNormalForm());
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public PrefExp toNegativeNormalForm() {
        return new PrefExp(this.name, this.exp.toNegativeNormalForm());
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
     * Returns a string representation of the preference expression.
     * 
     * @return a string representation of the preference expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(preference ");
        str.append(this.name.toString());
        str.append(" ");
        str.append(this.exp.toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of the preference expression.
     * 
     * @return a typed string representation of the preference expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(preference ");
        str.append(this.name.toString());
        str.append(" ");
        str.append(this.exp.toTypedString());
        str.append(")");
        return str.toString();
    }
}
