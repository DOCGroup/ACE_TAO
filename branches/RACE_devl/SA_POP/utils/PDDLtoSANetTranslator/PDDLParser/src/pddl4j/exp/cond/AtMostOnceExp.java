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
import pddl4j.exp.term.Substitution;

/**
 * This class implements an "at-most-once" conditional expression of the PDDL
 * language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public final class AtMostOnceExp extends UnaryCondExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -118526516823423737L;

    /**
     * Creates a new conditional "at-most-once" expression with a specific
     * expression.
     * 
     * @param arg the expression. The expression must be a not null reference.
     */
    public AtMostOnceExp(Exp arg) {
        super(Condition.AT_MOST_ONCE, arg);
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
    public AtMostOnceExp apply(Substitution sigma) {
        return (AtMostOnceExp) super.apply(sigma);
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public AtMostOnceExp standardize() {
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
    public AtMostOnceExp standardize(Map<String, String> images) {
        return (AtMostOnceExp) super.standardize(images);
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public AtMostOnceExp clone() {
        return (AtMostOnceExp) super.clone();
    }

    /**
     * Returns a string representation of this "at-most-once" conditional
     * expression.
     * 
     * @return a string representation of this "at-most-once" conditional
     *         expression. description.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(at-most-once ");
        str.append(this.getArg().toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of this "at-most-once" conditional
     * expression.
     * 
     * @return a typed string representation of this "at-most-once" conditional
     *         expression. description.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(at-most-once ");
        str.append(this.getArg().toTypedString());
        str.append(")");
        return str.toString();
    }

}
