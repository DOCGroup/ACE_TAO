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

package pddl4j.exp.cond;

import java.util.Map;
import java.util.LinkedHashMap;

import pddl4j.exp.Exp;
import pddl4j.exp.fexp.Number;
import pddl4j.exp.term.Substitution;

/**
 * This class implements an "hold-during" conditional expression of the PDDL
 * language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class HoldDuringExp extends UnaryCondExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -8736698454531582219L;

    /**
     * The lower bound of the time interval.
     */
    private Number lts;

    /**
     * The upper bound of the time interval.
     */
    private Number uts;

    /**
     * Creates a new conditional "hold-during" expression holding in a specific
     * time interval.
     * 
     * @param lts the lower bound of the time interval. The time stamp must be a
     *            not null reference and its value must be greater than 0.
     * @param uts the upper bound of the time interval. The time stamp must be a
     *            not null reference and its value must be greater than 0.
     * @param arg the expression. The expression must be a not null reference.
     */
    public HoldDuringExp(Number lts, Number uts, Exp arg) {
        super(Condition.HOLD_DURING, arg);
        this.lts = lts;
        this.uts = uts;
    }

    /**
     * Sets a new lower time stamp bound of this this "hold-during" conditional
     * expression.
     * 
     * @param lts the lower new time stamp bound of this this "hold-during"
     *            conditional expression to set. The time stamp must be a not
     *            null reference and its value must be greater than 0.
     */
    public final void setLowerTimeStamp(Number lts) {
        this.lts = lts;
    }

    /**
     * Returns the lower time stamp bound of this this "hold-during" conditional
     * expression.
     * 
     * @return the lower time stamp bound of this this "hold-during" conditional
     *         expression.
     */
    public final Number getLowerTimeStamp() {
        return this.lts;
    }

    /**
     * Sets a new upper time stamp bound of this this "hold-during" conditional
     * expression.
     * 
     * @param uts the upper new time stamp bound of this this "hold-during"
     *            conditional expression to set. The time stamp must be a not
     *            null reference and its value must be greater than 0.
     */
    public final void setUpperTimeStamp(Number uts) {
        this.uts = uts;
    }

    /**
     * Returns the upper time stamp bound of this this "hold-during" conditional
     * expression.
     * 
     * @return the upper time stamp bound of this this "hold-during" conditional
     *         expression.
     */
    public final Number getUpperTimeStamp() {
        return this.uts;
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
    public HoldDuringExp apply(Substitution sigma) {
        return (HoldDuringExp) super.apply(sigma);
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public HoldDuringExp standardize() {
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
     */
    public HoldDuringExp standardize(Map<String, String> images) {
        return (HoldDuringExp) super.standardize(images);
    }
    
    /**
     * Returns <code>true</code> if this "hold-during" conditional expression
     * is equal to an other object, i.e., if the object is a not null reference
     * to an instance of the class <code>HoldDuringExp</code> and both
     * "hold-during" conditional expressions have the same expression and the
     * same time stamp interval.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this "hold-during" conditional expression
     *         is equal to an other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof HoldDuringExp) {
            HoldDuringExp other = (HoldDuringExp) obj;
            return super.equals(obj)
                        && this.lts.equals(other.lts)
                        && this.uts.equals(other.uts);
        }
        return false;
    }

    /**
     * Returns the hash code value of this "hold-during" conditional expression.
     * 
     * @return the hash code value of this "hold-during" conditional expression.
     */
    public int hashCode() {
        return super.hashCode() + this.lts.hashCode() + this.uts.hashCode();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public HoldDuringExp clone() {
        HoldDuringExp other = (HoldDuringExp) super.clone();
        other.lts = this.lts.clone();
        other.uts = this.uts.clone();
        return other;
    }
    
    /**
     * Returns a string representation of this "hold-during" conditional
     * expression.
     * 
     * @return a string representation of this "hold-during" conditional
     *         expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(hold-during ");
        str.append(this.lts.toString());
        str.append(" ");
        str.append(this.uts.toString());
        str.append(" ");
        str.append(this.getArg().toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of this "hold-during" conditional
     * expression.
     * 
     * @return a typed string representation of this "hold-during" conditional
     *         expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(hold-during ");
        str.append(this.lts.toTypedString());
        str.append(" ");
        str.append(this.uts.toTypedString());
        str.append(" ");
        str.append(this.getArg().toTypedString());
        str.append(")");
        return str.toString();
    }
    
}
