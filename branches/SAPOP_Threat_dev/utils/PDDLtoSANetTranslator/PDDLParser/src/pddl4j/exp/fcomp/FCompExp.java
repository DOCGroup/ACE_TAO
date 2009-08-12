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

package pddl4j.exp.fcomp;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import pddl4j.EvaluationException;
import pddl4j.exp.AbstractExp;
import pddl4j.exp.ExpID;
import pddl4j.exp.fexp.OpExp;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.TermID;
import pddl4j.exp.term.Variable;

/**
 * This class implements a compared function.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class FCompExp extends AbstractExp {

    /**
     * The binary operator compared of this compared function.
     */
    private Comp op;

    /**
     * The first argument of the compared function.
     */
    private Term arg1;

    /**
     * The second argument of the compared function.
     */
    private Term arg2;

    /**
     * Creates a new compared function.
     * 
     * @param op the compared operator of the function.
     * @param arg1 the first argument of the compared function.
     * @param arg2 the second argument of the compared function.
     * @throws NullPointerException if <code>op == null</code> or arg1 == null</code>
     *             or <code>arg2 == null</code>.
     */
    protected FCompExp(Comp op, Term arg1, Term arg2) {
        super(ExpID.F_COMP);
        if (op == null || arg1 == null || arg2 == null)
            throw new NullPointerException();
        this.op = op;
        this.arg1 = arg1;
        this.arg2 = arg2;
    }

    /**
     * Sets a new binary compared operator of this compared function.
     * 
     * @param op the new binary compared operator of this compared
     *            function.
     * @throws NullPointerException if <code>op == null</code>.          
     */
    public final void setOp(Comp op) {
        if (op == null)
            throw new NullPointerException();
        this.op = op;
    }

    /**
     * Returns the binary compared operator of this compared function.
     * 
     * @return the binary compared operator of this compared function.
     */
    public final Comp getOp() {
        return this.op;
    }

    /**
     * Sets the first argument of this compared function.
     * 
     * @param arg1 The new first argument of this compared function to set.
     * @throws NullPointerException if <code>arg1 == null</code>.           
     */
    public final void setArg1(Term arg1) {
        if (arg1 == null)
            throw new NullPointerException();
        this.arg1 = arg1;
    }

    /**
     * Returns the first argument of the compared function.
     * 
     * @return the first argument of the compared function.
     */
    public final Term getArg1() {
        return this.arg1;
    }

    /**
     * Sets the second argument of this compared function.
     * 
     * @param arg - the new second argument of this compared function to set.
     * @throws NullPointerException if <code>arg2 == null</code>.  
     */
    public final void setArg2(Term arg) {
        if (arg2 == null)
            throw new NullPointerException();
        this.arg2 = arg;
    }

    /**
     * Returns the second argument of the compared function.
     * 
     * @return the second argument of the compared function.
     */
    public final Term getArg2() {
        return this.arg2;
    }

    /**
     * Returns <code>true</code> if a term occurs in this compared
     * function.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this compared
     *         function; <code>false</code> otherwise.
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
    public FCompExp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        FCompExp other = this.clone();
        other.arg1 = this.arg1.apply(sigma);
        other.arg2 = this.arg2.apply(sigma);
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
     */
    public FCompExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        FCompExp other = this.clone();
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
     * Evaluates this compared expression.
     * 
     * @return the result of the evaluation.
     * @throws EvaluationException if the compared expression is not ground or
     *             if an numeric error occurs during the evaluation of an
     *             numeric error.
     * @see pddl4j.exp.fcomp.FCompExp#isEvaluable()            
     */
    public abstract boolean evaluate() throws EvaluationException;
    
    /**
     * Returns <code>true</code> if this compared expression is evaluable.
     * In the general case, an compared expression is evaluable if it is
     * ground and its arguments are recursively either a number either a
     * arithmetic function. In the case of the equality, a compared expression
     * is also evaluable if the arguments of the expression are constant
     * terms.
     * 
     * @return <code>true</code> if this compared expression is evaluable;
     *         <code>false</code> otherwise.
     */
    public boolean isEvaluable() {
        boolean evaluable = this.isGround();
        ArrayList<Term> arguments = new ArrayList<Term>();
        arguments.add(this.arg1);
        arguments.add(this.arg2);
        Iterator<Term> i = arguments.iterator();
        while (i.hasNext() && evaluable) {
            Term arg = i.next();
            if (arg.getTermID().equals(TermID.ARITHMETIC_FUNCTION)) {
                OpExp func = (OpExp) arg;
                evaluable = func.isEvaluable();
            } else if (arg.getTermID().equals(TermID.NUMBER)) {
                evaluable = true;
            } else {
                evaluable = false;
            }
        }
        return evaluable;
    }
    
    /**
     * Returns <code>true</code> if this compared function is
     * equals to an other object, i.e., if the object is a not null instance of
     * the class <code>FCompExp</code> and both function have the same
     * arguments.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this compared function is
     *         equals to an other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof FCompExp) {
            FCompExp other = (FCompExp) obj;
            return super.equals(other)  
                        && this.getOp().equals(other.getOp())
                        && this.arg1.equals(other.arg1)
                        && this.arg2.equals(other.arg2);
        }
        return false;
    }

    /**
     * Returns the hash code value of this compared function.
     * 
     * @return the hash code value of this compared function.
     */
    public int hashCode() {
        return super.hashCode() + this.getOp().hashCode()
                    + this.arg1.hashCode() + this.arg2.hashCode();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public FCompExp clone() {
        FCompExp other = (FCompExp) super.clone();
        other.arg1 = this.arg1.clone();
        other.arg2 = this.arg2.clone();
        return other;
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public FCompExp toNegativeNormalForm() {
        return this.clone();
    }
    
    /**
     * Returns a string representation of this compared function.
     * 
     * @return a string representation of compared function.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(");
        str.append(this.getOp().toString().toLowerCase());
        str.append(" ");
        str.append(this.getArg1().toString());
        str.append(" ");
        str.append(this.getArg2().toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of compared function.
     * 
     * @return a typed string representation of compared function.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(");
        str.append(this.getOp().toString().toLowerCase());
        str.append(" ");
        str.append(this.getArg1().toTypedString());
        str.append(" ");
        str.append(this.getArg2().toTypedString());
        str.append(")");
        return str.toString();
    }
}
