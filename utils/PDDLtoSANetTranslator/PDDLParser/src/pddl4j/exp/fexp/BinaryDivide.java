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

import pddl4j.EvaluationException;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.TermID;

/**
 * This class implements the divide arithmetic function.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public final class BinaryDivide extends BinaryOp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 8265356137201795002L;

    /**
     * Creates a new divide arithmetic function with two arguments.
     * 
     * @param arg1 the first argument of the divide function.
     * @param arg2 the second argument of the divide function.
     * @throws NullPointerException if <code>arg1 == null</code> or
     *             <code>arg2 == null</code>.
     */
    public BinaryDivide(Term arg1, Term arg2) {
        super(Op.DIVIDE, arg1, arg2);
    }
     
    /**
     * Return a deep copy of this binary divide arithmetic function.
     * 
     * @return a deep copy of this binary divide arithmetic function.
     * @see pddl4j.exp.term.Term#clone()
     */
    public BinaryDivide clone() {
        return (BinaryDivide) super.clone();       
    }
    
    /**
     * Evaluates this binary divide arithmetic function.
     * 
     * @return the result of the evaluation.
     * @throws EvaluationException if the function is not evaluable or if an
     *             arithmetic error occurs during the evaluation.
     * @see pddl4j.exp.fexp.OpExp#isEvaluable()
     */
    public Number evaluate() throws EvaluationException {
        if (!this.isGround())
            throw new EvaluationException("arithmetic function "
                        + this.toString() + " not ground");
        try {
            Number arg1 = null;
            if (this.getArg1().getTermID().equals(TermID.ARITHMETIC_FUNCTION)) {
                OpExp func = (OpExp) this.getArg1();
                arg1 = func.evaluate();
            } else if (this.getArg1().getTermID().equals(TermID.NUMBER)) {
                arg1 = (Number) this.getArg1();
            } else {
                throw new EvaluationException("arithmetic function "
                            + this.toString() + ": argument " + this.getArg1()
                            + " is not evaluable");
            }
            Number arg2 = null;
            if (this.getArg2().getTermID().equals(TermID.ARITHMETIC_FUNCTION)) {
                OpExp func = (OpExp) this.getArg2();
                arg2 = func.evaluate();
            } else if (this.getArg2().getTermID().equals(TermID.NUMBER)) {
                arg2 = (Number) this.getArg2();
            } else {
                throw new EvaluationException("arithmetic function "
                            + this.toString() + ": argument " + this.getArg2()
                            + " is not evaluable");
            }
            return new Number(arg1.getValue() / arg2.getValue());
        } catch (ArithmeticException e) {
            throw new EvaluationException("arithmetic function "
                                + this.toString() + ": " + e.getMessage(), e);
        }
    }

}
