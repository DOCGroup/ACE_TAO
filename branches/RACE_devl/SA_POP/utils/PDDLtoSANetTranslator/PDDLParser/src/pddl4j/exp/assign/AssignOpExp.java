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

package pddl4j.exp.assign;

import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.AbstractExp;
import pddl4j.exp.ExpID;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.TermID;
import pddl4j.exp.term.Variable;

/**
 * This class implements an assign operation of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class AssignOpExp extends AbstractExp {

    /**
     * The assign operator of this operation.
     */
    private AssignOp op;

    /**
     * The first argument of the operation.
     */
    private Term arg1;

    /**
     * The second argument of the operation.
     */
    private Term arg2;

    /**
     * Creates a new assign operation.
     * 
     * @param op the assign operator of the operation.
     * @param arg1 the first argument of the assign operation.
     * @param arg2 the second argument of the assign operation.
     * @throws NullPointerException if <code>op == null</code> or
     *             <code>arg1 == null</code> or <code>arg2 == null</code>.
     */
    public AssignOpExp(AssignOp op, Term arg1, Term arg2) {
        super(ExpID.ASSIGN_OP);
        if (op == null || arg1 == null || arg2 == null)
            throw new NullPointerException();
        this.op = op;
        this.arg1 = arg1;
        this.arg2 = arg2;
    }

    /**
     * Sets a new operator of this assign operation.
     * 
     * @param op the new operator of this assign operation.
     * @throws NullPointerException if <code>op == null</code>.
     */
    public final void setOp(AssignOp op) {
        if (op == null)
            throw new NullPointerException();
        this.op = op;
    }

    /**
     * Returns the operator of this assign operation.
     * 
     * @return the operator of this assign operation.
     */
    public final AssignOp getOp() {
        return this.op;
    }

    /**
     * Sets the first argument of this assign operation.
     * 
     * @param arg1 the new first argument of this assign operation to set.
     * @throws NullPointerException if <code>arg1 == null</code>.
     */
    public final void setArg1(Term arg1) {
        if (arg1 == null)
            throw new NullPointerException();
        this.arg1 = arg1;
    }

    /**
     * Returns the first argument of the assign operation.
     * 
     * @return the first argument of the assign operation.
     */
    public final Term getArg1() {
        return this.arg1;
    }

    /**
     * Sets the second argument of this assign operation.
     * 
     * @param arg2 the new second argument of this assign operation to set.
     * @throws NullPointerException if <code>arg2 == null</code>.
     */
    public final void setArg2(Term arg2) {
        if (arg2 == null)
            throw new NullPointerException();
        this.arg2 = arg2;
    }

    /**
     * Returns the second argument of the assign operation.
     * 
     * @return the second argument of the assign operation.
     */
    public final Term getArg2() {
        return this.arg2;
    }

    /**
     * Returns <code>true</code> if a term occurs in this assign op
     * expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this assign op
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
     * expression and that are mapped in the substitution by its binding term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public AssignOpExp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        AssignOpExp other = this.clone();
        other.arg1 = this.arg1.apply(sigma);
        other.arg2 = this.arg2.apply(sigma);
        return other;
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public AssignOpExp standardize() {
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
    public AssignOpExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        AssignOpExp other = this.clone();
        other.arg1 = this.arg1.standardize(images);
        other.arg2 = this.arg2.standardize(images);
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
        return this.arg1.isGround() && this.arg2.isGround();
    }

    /**
     * Returns <code>true</code> if this "assign" operation is equals to an
     * other object, i.e., if the object is a not null instance of the class
     * <code>AssignOpExp</code> and both operation have the same arguments.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this "assign" operation is equals to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof AssignOpExp) {
            AssignOpExp other = (AssignOpExp) obj;
            return this.getExpID().equals(other.getExpID())
                        && this.getOp().equals(other.op)
                        && this.arg1.equals(other.arg1)
                        && this.arg2.equals(other.arg2);
        }
        return false;
    }

    /**
     * Returns the hash code value of this "assign" operation.
     * 
     * @return the hash code value of this "assign" operation.
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.op.hashCode()
                    + this.arg1.hashCode() + this.arg2.hashCode();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public AssignOpExp clone() {
        AssignOpExp other = (AssignOpExp) super.clone();
        other.arg1 = this.arg1.clone();
        other.arg2 = this.arg2.clone();
        return other;
    }
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new LinkedHashSet<Variable>();
        if (this.arg1.getTermID().equals(TermID.VARIABLE)) {
            vars.add((Variable) this.arg1);
        }
        if (this.arg2.getTermID().equals(TermID.VARIABLE)) {
            vars.add((Variable) this.arg2);
        }
        return vars;
    }
    
    /**
     * Returns a string representation of this assign operation.
     * 
     * @return a string representation of this assign operation.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(" + this.op.toString().toLowerCase() + " ");
        str.append(this.getArg1().toString());
        str.append(" ");
        str.append(this.getArg2().toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of this assign operation.
     * 
     * @return a typed string representation of this assign operation.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(" + this.op.toString().toLowerCase() + " ");
        str.append(this.getArg1().toTypedString());
        str.append(" ");
        str.append(this.getArg2().toTypedString());
        str.append(")");
        return str.toString();
    }
}
