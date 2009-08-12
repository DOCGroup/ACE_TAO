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
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This abstract class is extended by all quantified expression.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class QuantifiedExp extends AbstractExp implements
            Iterable<Variable> {

    /**
     * The typed list of the quantified variables.
     */
    protected Set<Variable> vars;

    /**
     * The quantified expression.
     */
    protected Exp exp;

    /**
     * Creates a new quantified expression with a specific id, typed list of
     * quantified variables and quantified expression.
     * 
     * @param id The id of the list expression.
     * @param vars The typed list of quantified variables.
     * @param exp The quantified expression.
     */
    protected QuantifiedExp(ExpID id, Set<Variable> vars, Exp exp) {
        super(id);
        this.vars = vars;
        this.exp = exp;
    }

    /**
     * Creates a new quantified expression with a specific id, typed list of
     * quantified variables and quantified expression.
     * 
     * @param id The id of the list expression.
     * @param exp The quantified expression.
     */
    protected QuantifiedExp(ExpID id, Exp exp) {
        this(id, new LinkedHashSet<Variable>(), exp);
    }

    /**
     * Creates a new quantified expression with a specific id, typed list of
     * quantified variables and quantified expression.
     * 
     * @param id The id of the list expression.
     */
    protected QuantifiedExp(ExpID id) {
        this(id, new LinkedHashSet<Variable>(), new AndExp());
    }

    /**
     * Adds a new quantified variable to this quantified expression.
     * 
     * @param var the quantified variable to add.
     * @return <code>true</code> if the quantified variable was added;
     *         <code>false</code> otherwise.
     */
    public boolean add(Variable var) {
        return this.vars.add(var);
    }

    /**
     * Sets the quantified goal description of this universal expression.
     * 
     * @param exp the quantified expression. The goal description must be a not
     *            null reference.
     */
    public final void setExp(Exp exp) {
        this.exp = exp;
    }

    /**
     * Returns the quantified goal description of this universal expression.
     * 
     * @return the quantified goal description of this universal expression.
     */
    public final Exp getExp() {
        return this.exp;
    }

    /**
     * Returns <code>true</code> if this quantified expression is equal to an
     * other object, i.e., if the object is a not null reference to an instance
     * of the class <code>QuantifiedExpression</code> and both quantified
     * expressions have the list of bound variable and quantified expression.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if this quantified expression is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof QuantifiedExp) {
            QuantifiedExp other = (QuantifiedExp) obj;
            return this.getExpID().equals(other.getExpID())
                        && this.vars.equals(other.vars)
                        && this.exp.equals(other.exp);
        }
        return false;
    }

    /**
     * Returns the hash code value of this quantified expression. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return the hash code value of this quantified expression.
     * @see java.lang.Object#hashCode()
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.vars.hashCode()
                    + this.exp.hashCode();
    }

    /**
     * Returns an iterator over the bounded variables of the quantified
     * expression.
     * 
     * @return an iterator over the bounded variables of the quantified
     *         expression.
     * @see java.lang.Iterable#iterator()
     */
    public final Iterator<Variable> iterator() {
        return this.vars.iterator();
    }

    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding
     * expression.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression if all bound variables are not instantiated; the
     *         quantified copy expression otherwise.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public final Exp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        QuantifiedExp other = this.clone();
        other.exp = this.exp.apply(sigma);
        Iterator<Variable> i = other.vars.iterator();
        while (i.hasNext()) {
            Variable var = i.next();
            if (!other.exp.occurs(var)) {
                i.remove();
            }
        }
        return other.vars.isEmpty() ? other.exp : this;
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     * @see pddl4j.exp.Exp#standardize()
     */
    public QuantifiedExp standardize() {
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
    public QuantifiedExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        Map<String, String> newImages = new LinkedHashMap<String,String>(images);
        QuantifiedExp other = this.clone();
        other.vars.clear();
        for (Variable var : this.vars) {
            other.add(var.standardizeQuantifiedVariable(newImages));
        }
        other.exp = other.exp.standardize(newImages);
        for (Map.Entry<String, String> e : newImages.entrySet()) {
            if (!images.containsKey(e.getKey())) {
                images.put(e.getKey(), Variable.getStandardizedImage(e.getValue()));
            }
        }
        return other;
    }
    
    /**
     * Returns <code>true</code> if a term occurs in this quantified
     * expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this quantified
     *         expression; <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.
     */
    public final boolean occurs(Term term) {
        if (term == null)
            throw new NullPointerException();
        return this.exp.occurs(term);
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
     * Returns the prenex normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the prenex normal form of this expression.
     */
    public Exp toPrenexNormaForm() {
        QuantifiedExp pnf = this.standardize();
        pnf.exp = pnf.exp.moveQuantifierOutward();
        return pnf;
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public QuantifiedExp clone() {
        QuantifiedExp other = (QuantifiedExp) super.clone();
        other.vars = new LinkedHashSet<Variable>();
        for (Variable var : this.vars) {
            other.vars.add(var.clone());
        }
        other.exp = this.exp.clone();
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
        vars.removeAll(this.vars);
        return vars;
    }
    
    /**
     * Returns a string representation of the quantified expression.
     * 
     * @return a string representation of the quantified expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(" + this.getExpID().toString().toLowerCase() + " ");
        str.append("(");
        if (!this.vars.isEmpty()) {
            Iterator<Variable> i = this.iterator();
            str.append(i.next().toString());
            while (i.hasNext()) {
                str.append(" " + i.next().toString());
            }
        }
        str.append(")");
        str.append(" ");
        str.append(this.exp.toString());
        str.append(")");
        return str.toString();
    }
    
    /**
     * Returns a typed string representation of the quantified expression.
     * 
     * @return a typed string representation of the quantified expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(" + this.getExpID().toString().toLowerCase() + " ");
        str.append("(");
        if (!this.vars.isEmpty()) {
            Iterator<Variable> i = this.iterator();
            str.append(i.next().toTypedString());
            while (i.hasNext()) {
                str.append(" " + i.next().toTypedString());
            }
        }
        str.append(")");
        str.append(" ");
        str.append(this.exp.toTypedString());
        str.append(")");
        return str.toString();
    }
    
}
