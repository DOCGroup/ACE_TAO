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
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.AbstractExp;
import pddl4j.exp.ExpID;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;

/** 
 * This abstract class implements the common part of all term used in PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public abstract class AbstractTerm extends AbstractExp implements Term {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 4052803386962040209L;
    
    /**
     * The image of the term.
     */
    private String image;
    
    /**
     * The type of the term.
     */
    private TypeSet type;
    
    /**
     * The term id of this term.
     */
    private TermID id;
   
    /**
     * Creates a new term with a specified term id, image type and gound flag
     * property.
     * 
     * @param id The id expression of this term.
     * @param image the image of the term.
     * @throws NullPointerException if <code>id == null</code> or
     *        <code>image == null</code> or <code>type == null</code>.
     */
    protected AbstractTerm(TermID id, String image) {
        super(ExpID.TERM);
        this.id = id;
        this.setImage(image);
        Map<String, Set<String>> type = new LinkedHashMap<String,Set<String>>();
        type.put(Type.OBJECT_SYMBOL, new LinkedHashSet<String>());
        this.setTypeSet(new TypeSet(new Type(Type.OBJECT_SYMBOL, type)));
    }
    
    /**
     * Creates a new term with a specified term id, image type and ground flag
     * property.
     * 
     * @param id The id expression of this term.
     * @param image the image of the term.
     * @param type the type of this term.
     * @throws NullPointerException if <code>id == null</code> or
     *        <code>image == null</code> or <code>type == null</code>.
     */
    protected AbstractTerm(TermID id, String image, TypeSet type) {
        super(ExpID.TERM);
        this.id = id;
        this.setImage(image);
        this.setTypeSet(type);
    }

    /**
     * Returns the term id of this term.
     * 
     * @return the expression id of this term.
     */
    public final TermID getTermID() {
        return this.id;
    }
    
    /**
     * Returns the image of this term.
     * 
     * @return the image of this term.
     */
    public final String getImage() {
        return this.image;
    }
    
    /**
     * Sets a new image to this term.
     * 
     * @param image the new image of the term.
     * @throws NullPointerException if <code>image == null</code>.
     */
    protected void setImage(String image) {
        if (image == null) 
            throw new NullPointerException();
        this.image = image;
    }
    
    /**
     * Returns the type of the term.
     * 
     * @return the type of the term.
     */
    public final TypeSet getTypeSet() {
        return this.type;
    }
    
    /**
     * Sets a new type to this term.
     * 
     * @param type the new type to set to this term.
     * @throws NullPointerException if <code>type == null</code>.
     */
    protected void setTypeSet(TypeSet type) {
        if (type == null) 
            throw new NullPointerException();
        this.type = type;
    }
    
   /* /**
     * Returns <code>true</code> if this term is an instance of a specified
     * type. Specially, this method returns <code>true</code> if the type
     * of this term is the equal to the specified type or if the type of this
     * term is a subtype of the specified type.
     * 
     * @param type the type.
     * @return <code>true</code> if this type is an instance of a specified
     *         type; <code>false</code> otherwise.
     * @throws NullPointerException if <code>type == null</code>.
     */
   /* public boolean isInstanceOf(Type type) {
        if (type == null)
            throw new NullPointerException();
        return this.gequals(type) || type.getAllSubTypes().contains(this);
    }*/
    
    /**
     * Returns a deep copy of this term.
     * 
     * @return a deep copy of this term.
     * @see java.lang.Cloneable
     */
    public AbstractTerm clone() {
        AbstractTerm other = (AbstractTerm) super.clone();
        other.type = this.type.clone();
        return other;
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public Term toNegativeNormalForm() {
        return this.clone();
    }
    
    /**
     * Returns <code>true</code> if this term is equals to an other object,
     * i.e., if the object is a not null instance of the class
     * <code>AbstractTerm</code> and both term have the same image.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this function head is equals to an other
     *         object; <code>false</code> otherwise.
     */
    public boolean equals(final Object obj) {
        if (obj != null && obj instanceof AbstractTerm) {
            final AbstractTerm other = (AbstractTerm) obj;
            return super.equals(other) && this.id.equals(other.id)
                        && this.getImage().equals(other.getImage());
        }
        return false;
    }

    /**
     * Returns the hash code value of this term. This method is supported
     * for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return the hash code value of this term.
     * @see java.lang.Object#hashCode()
     */
    public int hashCode() {
        return super.hashCode() + this.id.hashCode() + this.getImage().hashCode();
   }
    
    /**
     * Returns <code>true</code> if this term is numeric.
     * 
     * @return <code>true</code> if this term is numeric; <code>false</code>
     *         otherwise.
     */
    /*public final boolean isNumeric() {
        return this.type.isNumber();
    }*/
}
 