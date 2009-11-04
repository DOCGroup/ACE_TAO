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

import java.util.LinkedHashMap;
import java.util.Map;

import pddl4j.exp.term.TermID;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;

/**
 * This class implements a number in the PDDL langage.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public final class Number extends FExp implements Comparable<Number> {

    /**
     * The serial version id of this class.
     */
    private static final long serialVersionUID = 6081884046144807572L;

    /**
     * Create a new number with a specific value.
     * 
     * @param value the value set to this number.
     */
    public Number(double value) {
        super(TermID.NUMBER, String.valueOf(value), new TypeSet(Type.NUMBER));
    }

    /**
     * Create a new number with a default value set to 0.0.
     */
    public Number() {
        this(0.0);
    }

    /**
     * Returns the double value of this number.
     * 
     * @return the double value of this number.
     * @throws NumberFormatException if the constant is not numeric.
     */
    public double getValue() {
        return Double.parseDouble(this.getImage());
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public Number standardize() {
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
    public Number standardize(Map<String, String> images) {
        return (Number) super.standardize(images);
    }
    
    /**
     * Compares this number with the specified number for order. Returns a
     * negative integer, zero, or a positive integer as this number is less
     * than, equal to, or greater than the specified number.
     * 
     * @param num the number to be compared.
     * @return a negative integer, zero, or a positive integer as this number is
     *         less than, equal to, or greater than the specified object.
     */
    public int compareTo(Number num) {
        return new Double(this.getValue()).compareTo(new Double(num.getValue()));
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public Number clone() {
        return (Number) super.clone();
    }
    
    /**
     * Returns a string representation of this number.
     * 
     * @return a string representation of this number.
     */
    public String toString() {
        return this.getImage();
    }

    /**
     * Returns a types string representation of this number.
     * 
     * @return a typed string representation of this number.
     */
    public String toTypedString() {
        return this.getImage() + " - " + this.getTypeSet();
    }
}
