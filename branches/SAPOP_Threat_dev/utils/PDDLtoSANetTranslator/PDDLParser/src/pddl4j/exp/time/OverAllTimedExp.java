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

import java.util.LinkedHashMap;
import java.util.Map;

import pddl4j.exp.Exp;

/**
 * This class implements an "over all" timed expression of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public final class OverAllTimedExp extends OverTimedExp {

    /**
     * The serial id version of the class.
     */
    private static final long serialVersionUID = 5674054849424501507L;

    /**
     * Creates a new "over all" timed expression with a specific timed
     * description.
     * 
     * @param exp the timed expression.
     * @throws NullPointerException if <code>exp == null</code>.
     */
    public OverAllTimedExp(Exp exp) {
        super(Interval.ALL, exp);
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public OverAllTimedExp standardize() {
        return this.standardize(new LinkedHashMap<String, String>());
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. The map argument is used to store the variable already
     * standardized. The bound argument is used to store the bound variables.
     * Remember that free variables are existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this expression.
     * @throws NullPointerException if
     *             <code>images == null || bound == null</code>.
     * @see pddl4j.exp.Exp#standardize(Map)
     */
    public OverAllTimedExp standardize(Map<String, String> images) {
        return (OverAllTimedExp) super.standardize(images);
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public OverAllTimedExp clone() {
        return (OverAllTimedExp) super.clone();
    }

    /**
     * Returns a string representation of this "over all" timed expression.
     * 
     * @return a string representation of this "over all" timed expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(over all ");
        str.append(this.exp.toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of this "over all" timed expression.
     * 
     * @return a type string representation of this "over all" timed expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(over all ");
        str.append(this.exp.toTypedString());
        str.append(")");
        return str.toString();
    }
}
