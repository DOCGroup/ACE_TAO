/*
 * Copyright Dept. of Mathematics & Computer Science Univ. Paris-Descartes This
 * software is governed by the CeCILL license under French law and abiding by
 * the rules of distribution of free software. You can use, modify and/ or
 * redistribute the software under the terms of the CeCILL license as circulated
 * by CEA, CNRS and INRIA at the following URL "http://www.cecill.info". As a
 * counterpart to the access to the source code and rights to copy, modify and
 * redistribute granted by the license, users are provided only with a limited
 * warranty and the software's author, the holder of the economic rights, and
 * the successive licensors have only limited liability. In this respect, the
 * user's attention is drawn to the risks associated with loading, using,
 * modifying and/or developing or reproducing the software by the user in light
 * of its specific status of free software, that may mean that it is complicated
 * to manipulate, and that also therefore means that it is reserved for
 * developers and experienced professionals having in-depth computer knowledge.
 * Users are therefore encouraged to load and test the software's suitability as
 * regards their requirements in conditions enabling the security of their
 * systems and/or data to be ensured and, more generally, to use and operate it
 * in the same conditions as regards security. The fact that you are presently
 * reading this means that you have had knowledge of the CeCILL license and that
 * you accept its terms.
 */

package pddl4j.exp.cond;

import java.util.Map;
import java.util.LinkedHashMap;

import pddl4j.exp.Exp;
import pddl4j.exp.fexp.Number;
import pddl4j.exp.term.Substitution;

/**
 * This class implements an "always-within" conditional expression of the PDDL
 * language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class AlwaysWithinExp extends BinaryCondExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -8736698454531582219L;

    /**
     * The time stamp.
     */
    private Number ts;

    /**
     * Creates a new conditional "always-within" expression with two specific
     * expression arguments.
     * 
     * @param arg1 the first expression argument. The expression must be a not
     *            null reference.
     * @param arg2 the second expression argument. The expression must be a not
     *            null reference.
     * @param ts the new time stamp to this this "always-within" conditional
     *            expression. The time stamp must be a not null reference and
     *            its value must be greater than 0.
     */
    public AlwaysWithinExp(Exp arg1, Exp arg2, Number ts) {
        super(Condition.ALWAYS_WITHIN, arg1, arg2);
        this.ts = ts;
    }

    /**
     * Sets a new time stamp to this this "always-within" conditional
     * expression.
     * 
     * @param ts the new time stamp to this this "always-within" conditional
     *            expression. The time stamp must be a not null reference and
     *            its value must be greater than 0.
     */
    public final void setTimeStamp(Number ts) {
        this.ts = ts;
    }

    /**
     * Returns the time stamp to this this "always-within" conditional
     * expression.
     * 
     * @return the time stamp to this this "always-within" conditional
     *         expression.
     */
    public final Number getTimeStamp() {
        return this.ts;
    }

    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public AlwaysWithinExp apply(Substitution sigma) {
        return (AlwaysWithinExp) super.apply(sigma);
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public AlwaysWithinExp standardize() {
        return this.standardize(new LinkedHashMap<String, String>());
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. The map argument is used to store the variable already
     * standardized. The bound argument is used to store the bound variables. a
     * standardized copy of this expression. Remember that free variables are
     * existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this expression.
     * @throws NullPointerException if
     *             <code>images == null || bound == null</code>.
     */
    public AlwaysWithinExp standardize(Map<String, String> images) {
        return (AlwaysWithinExp) super.standardize(images);
    }

    /**
     * Returns <code>true</code> if this "always-within" conditional
     * expression is equal to an other object, i.e., if the object is a not null
     * reference to an instance of the class <code>AlwaysWithinExp</code> and
     * both "always-within" conditional expressions have the same two expression
     * arguments and the same time stamp.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this "always-within" conditional
     *         expression is equal to an other object; <code>false</code>
     *         otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof AlwaysWithinExp) {
            AlwaysWithinExp other = (AlwaysWithinExp) obj;
            return super.equals(obj) && this.ts.equals(other.ts);
        }
        return false;
    }

    /**
     * Returns the hash code value of this "always-within" conditional
     * expression.
     * 
     * @return the hash code value of this "always-within" conditional
     *         expression.
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.getArg1().hashCode()
                    + this.getArg2().hashCode() + this.ts.hashCode();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public AlwaysWithinExp clone() {
        AlwaysWithinExp other = (AlwaysWithinExp) super.clone();
        other.ts = this.ts.clone();
        return other;
    }

    /**
     * Returns a string representation of this "always-within" conditional
     * expression.
     * 
     * @return a string representation of this "always-within" conditional
     *         expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(always-within ");
        str.append(this.ts.toString());
        str.append(" ");
        str.append(this.getArg1().toString());
        str.append(" ");
        str.append(this.getArg2().toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of this "always-within" conditional
     * expression.
     * 
     * @return a typed string representation of this "always-within" conditional
     *         expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(always-within ");
        str.append(this.ts.toTypedString());
        str.append(" ");
        str.append(this.getArg1().toTypedString());
        str.append(" ");
        str.append(this.getArg2().toTypedString());
        str.append(")");
        return str.toString();
    }

}
