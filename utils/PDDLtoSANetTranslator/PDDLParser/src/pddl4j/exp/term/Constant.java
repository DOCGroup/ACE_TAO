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

package pddl4j.exp.term;

import java.util.LinkedHashMap;
import java.util.Map;

import pddl4j.exp.fexp.FExp;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;

/**
 * This class implements a constant.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public final class Constant extends FExp implements Comparable<Constant> {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -8580834206648477520L;
    
    /**
     * Creates a new constant of type object with a specified image.
     * 
     * @param image the image of the constant.
     * @throws NullPointerException if <code>image == null</code>.
     */
    public Constant(String image) {
        super(TermID.CONSTANT, image);
    }
    
    /**
     * Creates a new constant of type object with a specified image.
     * 
     * @param image the image of the constant.
     * @param type the type of this constant.
     * @throws NullPointerException if <code>image == null</code>.
     */
    public Constant(String image, Type type) {
        super(TermID.CONSTANT, image, new TypeSet(type));
    }
    
    /**
     * Creates a new constant with a specified image and type.
     * 
     * @param image the image of the constant.
     * @param type the type of this constant.
     * @throws NullPointerException if <code>image == null</code> or 
     *          <code>type == null</code>.
     */
    public Constant(String image, TypeSet type) {
        super(TermID.CONSTANT, image, type);
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * term. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public Constant standardize() {
        return this.standardize(new LinkedHashMap<String, String>());
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * term. The map argument is used to store the variable already
     * standardized. The bound argument is used to store the bound variables. a
     * standardized copy of this expression. Remember that free variables are
     * existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this term.
     * @throws NullPointerException if
     *             <code>images == null || bound == null</code>.
     */
    public Constant standardize(Map<String, String> images) {
        return (Constant) super.standardize(images);
    }
    
    /**
     * Returns a string representation of this constant.
     * 
     * @return a string representation of this constant.
     */
    public String toString() {
        return this.getImage();
    }
    
    /**
     * Returns a typed string representation of this constant.
     * 
     * @return a typed string representation of this constant.
     */
    public String toTypedString() {
        return this.getImage() + " - " + this.getTypeSet();
    }
    
    /**
     * Compares this constant with the specified constant for order. Returns a
     * negative integer, zero, or a positive integer as this constant is less
     * than, equal to, or greater than the specified constant according to the 
     * lexicographic order.
     * 
     * @param cst the constant to be compared.
     * @return a negative integer, zero, or a positive integer as this number is
     *         less than, equal to, or greater than the specified object.
     * @throws NullPointerException if <code>cst == null</code>.
     */
    public int compareTo(Constant cst) {
        return this.getImage().compareTo(cst.getImage());
    }
    
    /**
     * Sets a new type of this constant.
     * 
     * @param type the type to set.
     * @throws NullPointerException if <code>type == null</code>.
     */
    public void setTypeSet(TypeSet type) {
        super.setTypeSet(type);
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public Constant clone() {
        return (Constant) super.clone();
    }
    
    
    
}
