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

package pddl4j.exp.metric;

import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.AbstractExp;
import pddl4j.exp.ExpID;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This abstract class defines the optimization expression used in metric
 * specification of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class MetricExp extends AbstractExp {
    
    /**
     * The optimization.
     */
    private Optimization optimization;
    
    /**
     * The term expression to optimize.
     */
    private Term exp;

    /**
     * Creates a new optimization expression.
     * 
     * @param optimization the optimization of the expression.
     * @param exp the function to optimize.
     * @throws NullPointerException if <code>optimization == null</code> or
     *             <code>exp == null</code>.
     */
    public MetricExp(Optimization optimization, Term exp) {
        super(ExpID.METRIC_EXP);
        if (optimization == null || exp == null) 
            throw new NullPointerException();
        this.optimization = optimization;
        this.exp = exp;
    }
    
    /**
     * Returns the term expression to optimize.
     * 
     * @return the term expression to optimize.
     */
    public final Optimization getOptimization() {
        return this.optimization;
    }
    
    /**
     * Returns the term expression to optimize.
     * 
     * @return the term expression to optimize.
     */
    public final Term getExp() {
        return this.exp;
    }

    /**
     * Sets a new term expression to optimize.
     * 
     * @param exp the new term expression to optimize.
     * @throws NullPointerException if <code>exp == null</code>.  
     */
    public final void setExp(Term exp) {
        if (exp == null)
            throw new NullPointerException();
        this.exp = exp;
    }
    
    /**
     * Returns <code>true</code> if a term occurs in this metric expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this metric expression;
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
     * expression and that are mapped in the substitution by its binding term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public MetricExp apply(Substitution sigma) {
        if (sigma == null) 
            throw new NullPointerException();
        MetricExp other = this.clone();
        other.exp = this.exp.apply(sigma);
        return other;
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are
     * existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this expression.
     * @throws NullPointerException if
     *             <code>images == null</code>.
     */
    public MetricExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        MetricExp other = this.clone();
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
     * Returns <code>true</code> if the maximize expression is equal to
     * an other object, i.e., if the object is a not null reference to an
     * instance of the class <code>MaximizeExp</code> and both maximize
     * expressions contain the same expression to maximize.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if the maximize expression equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof MetricExp) {
            MetricExp other = (MetricExp) obj;
            return this.getExpID().equals(other.getExpID())
                        && this.exp.equals(other.exp);
        }
        return false;
    }

    /**
     * Returns a hash code value for the maximize expression. This method
     * is supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the effect goal description.
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
    public MetricExp clone() {
        MetricExp other = (MetricExp) super.clone();
        other.exp.clone();
        return other;
    }
    
    /**
     * Returns a string representation of this metric expression.
     * 
     * @return a string representation of this metric expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(:metric " + this.optimization.toString().toLowerCase() + " ");
        str.append(this.getExp().toString());
        str.append(")");
        return str.toString();
    }
    
    /**
     * Returns a typed string representation of this metric expression.
     * 
     * @return a typed string representation of this metric expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(:metric " + this.optimization.toString().toLowerCase() + " ");
        str.append(this.getExp().toTypedString());
        str.append(")");
        return str.toString();
    }
    
}
