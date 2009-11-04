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

package pddl4j.exp.type;

import java.io.Serializable;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;
import java.util.Stack;
import java.util.Map.Entry;

/**
 * This class implements a type.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public final class Type implements Serializable {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -3938534221059491043L;

    /**
     * The object type symbol.
     */
    public static final String OBJECT_SYMBOL = "object";

    /**
     * The number type symbol.
     */
    public static final String NUMBER_SYMBOL = "number";

    /**
     * The number primitive type.
     */
    public static final Type NUMBER = new Type(Type.NUMBER_SYMBOL);

    /**
     * The image of this type.
     */
    private String image;

    /**
     * The list of subtypes of this types.
     */
    private Map<String, Set<String>> subTypes;

    /**
     * The list of super types of this types.
     */
    private Map<String, Set<String>> superTypes;

    /**
     * Creates a new type with a specific image and type hierachy. 
     * 
     * @param image the image of the type.
     * @param hierarchy the type hierachy.
     * @throws NullPointerException if <code>image == null</code> or
     *             <code>hierarchy == null</code>.
     * @throws TypingException if the type hierarchy specified is not
     *             consistant. A type hierarchy is consistant if it does not
     *             contain cycle and all types inherite from object except the
     *             primitive type number.
     */
    public Type(String image, Map<String, Set<String>> hierarchy) {
        if (image == null || hierarchy == null) 
            throw new NullPointerException();
        this.image = image;
        this.superTypes = new LinkedHashMap<String, Set<String>>();
        this.subTypes = new LinkedHashMap<String, Set<String>>();        
        if (!this.checkTypeHierarchy(hierarchy)) {
            throw new TypingException(this);
        }
        for (Entry<String, Set<String>> e : hierarchy.entrySet()) {
            String typeImg = e.getKey();
            this.subTypes.put(e.getKey(), e.getValue());
            for (String st : e.getValue()) {
                Set<String> stSet = this.superTypes.get(st);
                if (stSet == null) {
                    stSet = new LinkedHashSet<String>();
                    this.superTypes.put(st, stSet);
                }
                stSet.add(e.getKey());
            }
        }
        this.superTypes.put(Type.OBJECT_SYMBOL, new LinkedHashSet<String>());
    }
    
    /**
     * Creates a new type with a specific image. This type is considered as a
     * primitive type because no type hierarchy is specified. Consequencely, 
     * this type has no sub type and no super type and is final. 
     * 
     * @param image the image of the type.
     * @throws NullPointerException if <code>image == null</code>.
     */
    private Type(String image) {
        this.image = image;
        this.superTypes = new LinkedHashMap<String, Set<String>>();
        this.subTypes = new LinkedHashMap<String, Set<String>>();
        this.subTypes.put(image, new LinkedHashSet<String>());
        this.superTypes.put(image, new LinkedHashSet<String>());
    }
    
    /**
     * Returns the image of this type.
     * 
     * @return the image of this type.
     */
    public String getImage() {
        return this.image;
    }

    /**
     * Returns <code>true</code> if this type is final, i.e., has no subtype.
     * 
     * @return <code>true</code> if this type is final, i.e., has no subtype;
     *         <code>false</code> otherwise.
     */
    public boolean isFinal() {
        return this.subTypes.get(this.image) == null
                    || this.subTypes.get(this.image).isEmpty();
    }
    
    /**
     * Returns the set of sub-types of this type.
     * 
     * @return the set of sub-types of this type.
     */
    public Set<Type> getSuperTypes() {
        Set<Type> types = new LinkedHashSet<Type>();
        Set<String> images = this.superTypes.get(this.image);
        for (String img : images) {
            Type type = new Type(img);
            type.subTypes = this.subTypes;
            type.superTypes = this.superTypes;
            types.add(type);
        }
        return types;
    }

    /**
     * Returns the set of super-types of this type.
     * 
     * @return the set of super-types of this type.
     */
    public Set<Type> getSubTypes() {
        Set<Type> types = new LinkedHashSet<Type>();
        Set<String> images = this.subTypes.get(this.image);
        for (String img : images) {
            Type type = new Type(img);
            type.subTypes = this.subTypes;
            type.superTypes = this.superTypes;
            types.add(type);
        }
        return types;
    }
    
    /**
     * Returns the set of all sub-types of this type.
     * 
     * @return the set of all sub-types of this type.
     */
    public Set<Type> getAllSuperTypes() {
        Set<Type> superTypes = new LinkedHashSet<Type>();
        Set<Type> types = this.getSuperTypes();
        superTypes.addAll(types);
        for (Type st : types) {
            superTypes.addAll(st.getAllSuperTypes());
        }
        return superTypes;
    }

    /**
     * Returns the set of all super-types of this type.
     * 
     * @return the set of all super-types of this type.
     */
    public Set<Type> getAllSubTypes() {
        Set<Type> subTypes = new LinkedHashSet<Type>();
        Set<Type> types = this.getSubTypes();
        subTypes.addAll(types);
        for (Type st : types) {
            subTypes.addAll(st.getAllSubTypes());
        }
        return subTypes;
    }

    /**
     * Returns <code>true</code> if this type is equals to an other object.
     * This method return <code>true</code> if the object is a not null
     * instance of the class <code>Type</code> and both type have the same
     * image.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this type is equals to an other object;
     *         <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj.getClass().equals(this.getClass())) {
            Type other = (Type) obj;
            return this.image.equals(other.image);
        }
        return false;
    }

    /**
     * Returns the hash code value of this type. This method is supported
     * for the benefit of hashtables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return the hash code value of this type.
     * @see java.lang.Object#hashCode()
     */
    public int hashCode() {
        return this.image.hashCode();
    }
    
    /**
     * Returns a string representation of this type.
     * 
     * @return a string representation of this type.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append(this.image);
        /*str.append("[");
        if (!this.subTypes.isEmpty()) {
            Iterator<Type> i = this.subTypes.iterator();
            str.append(i.next().image);
            while (i.hasNext()) {
                str.append(" " + i.next().image);
            }
        }
        str.append("][");
        if (!this.superTypes.isEmpty()) {
            Iterator<Type> i = this.superTypes.iterator();
            str.append(i.next().image);
            while (i.hasNext()) {
                str.append(" " + i.next().image);
            }
        }
        str.append("]");*/
        return str.toString();
    }
    
    
    /**
     * Checks the consistance of the type hierarchy. A type hierarchy is
     * consistance if the hierarchy does not contain cycle and all types
     * inherite from object except the primitive type number.
     * 
     * @param hierarchy the type hierarchy.
     * @return <code>true</code> if the type hierarchy is consistant;
     *         <code>false</code> otherwise.
     */
    private boolean checkTypeHierarchy(Map<String, Set<String>> hierarchy) {
        Set<String> explored = new LinkedHashSet<String>();
        if (hierarchy.containsKey(Type.OBJECT_SYMBOL)) {
            Stack<String> stack = new Stack<String>();
            stack.push(Type.OBJECT_SYMBOL);
            Set<String> prefix = new LinkedHashSet<String>();
            boolean consistant = true;
            while (!stack.isEmpty() && consistant) {
                String type = stack.pop();
                explored.add(type);
                Set<String> children = hierarchy.get(type);
                if (children.isEmpty()) {
                    prefix.remove(type);
                } else {
                    consistant = prefix.add(type);
                    if (!consistant) {
                    }
                    for (String child : children) {
                        stack.push(child);

                    }
                }
            }
            Set<String> types = new LinkedHashSet<String>(hierarchy.keySet());
            types.removeAll(explored);
            return consistant
                        && (types.isEmpty() || (types.size() == 1 && 
                                    types.contains(Type.NUMBER_SYMBOL)));
        }
        return false;
    }
        
    
}
