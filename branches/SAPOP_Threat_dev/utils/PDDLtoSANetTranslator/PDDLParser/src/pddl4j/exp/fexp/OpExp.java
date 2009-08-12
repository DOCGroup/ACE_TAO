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

package pddl4j.exp.fexp;

import java.util.Iterator;

import pddl4j.EvaluationException;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.TermID;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;

/**
 * This class implements an arithmetic function.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public abstract class OpExp extends FExp {
    
    /**
     * The operator of this arithmetic function.
     */
    private Op op;
        
    /**
     * Creates a new arithmetic function with a specified operator.
     * 
     * @param op The arithmetic operator of the function.
     * @throws NullPointerException if <code>op == null</code>.
     */
    protected OpExp(Op op) {
        super(TermID.ARITHMETIC_FUNCTION, op.toString(), new TypeSet(Type.NUMBER));
        this.setOp(op);
    }

    /**
     * Returns the operator of this arithmetic function.
     * 
     * @return the operator of this arithmetic function.
     */
    public final Op getOp() {
        return this.op;
    }
    
    /**
     * Sets a new operator to this arithmetic function.
     * 
     * @param op the new op to set.
     * @throws NullPointerException if <code>op == null</code.
     */
    protected void setOp(Op op) {
        if (op == null) 
            throw new NullPointerException();
        this.op = op;
    }
    
    /**
     * Returns a deep copy of this arithmetic function.
     * 
     * @return a deep copy of this arithmetic function.
     * @see pddl4j.exp.AbstractExp#clone()
     */
    public OpExp clone() {
        return (OpExp) super.clone();
    }
    
    /**
     * Substitutes all occurrences of the variables that occur in this term and
     * that are bound in the substitution by its binding term. This method
     * modify the content of the term on which it is applied.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     * @throws EvaluationException if this arithmetic function is ground after
     *             applying the substitution and its evaluation fails.
     */
    public Term apply(Substitution sigma) {
        OpExp other = (OpExp) super.apply(sigma);
        return other.isEvaluable() ? other.evaluate() : other;
    }
    
    /**
     * Returns <code>true</code> if this arithmetic function is evaluable.
     * Specially, an arithmetic function is evaluable if it is gound and its
     * arguments are recursively either a number either a aritmetic function.
     * 
     * @return <code>true</code> if this arithmetic function is evaluable;
     *         <code>false</code> otherwise.
     */
    public final boolean isEvaluable() {
        boolean evaluable = this.isGround();
        Iterator<Term> i = this.iterator();
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
     * Evaluates this arithmetic expression.
     * 
     * @return the result of the evaluation.
     * @throws EvaluationException if the arithemtic expression is not ground or
     *             if an numeric error occurs during the evaluation of an
     *             numeric error.
     */
    public abstract Number evaluate() throws EvaluationException;
    
}
