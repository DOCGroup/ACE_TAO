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

import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.Set;

/**
 * This class implements a type set.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public final class TypeSet implements Iterable<Type>, Cloneable {
    
    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -3938534221059491043L;
        
    /**
     * The list of types.
     */
    private Set<Type> types;

    /**
     * Creates a new type set containing a specified type.
     * 
     * @param type the type.
     * @throws NullPointerException if <code>type == null</code>.
     */
    public TypeSet(Type type) {
        if (type == null)
            throw new NullPointerException();
        this.types = new LinkedHashSet<Type>();
        this.types.add(type);
    }

    /**
     * Creates a new empty type set.
     */
    public TypeSet() {
        this.types = new LinkedHashSet<Type>();
    }

    /**
     * Adds a type to this type set.
     * 
     * @param type the type to add.
     * @return <code>true</code> if the was added; <code>false</code> otherwise.
     * @throws NullPointerException if <code>type == null</code>.
     */
    public boolean add(Type type) {
        if (type == null)
            throw new NullPointerException();
        return this.types.add(type);
    }

    /**
     * Removes a type to this type set.
     * 
     * @param type the type to remove.
     * @return <code>true</code> if the type was removed; <code>false</code>
     *         otherwise.
     * @throws NullPointerException if <code>type == null</code>.
     */
    public boolean remove(Type type) {
        if (type == null)
            throw new NullPointerException();
        return this.types.remove(type);
    }

    /**
     * Returns an iterator over the primitive type contained in this type.
     * 
     * @return an iterator over the primitive type contained in this type.
     */
    public Iterator<Type> iterator() {
        return this.types.iterator();
    }

    /**
     * Returns <code>true</code> if this type set is empty.
     * 
     * @return <code>true</code> if this type set is empty; <code>false</code>
     *         otherwise.
     */
    public boolean isEmpty() {
        return this.types.isEmpty();
    }

    /**
     * Return the sub-types if this type set.
     * 
     * @return the sub-types if this type set.
     */
    public Set<Type> getSubTypes() {
        Set<Type> subTypes = new LinkedHashSet<Type>(this.types);
        for (Type t : this.types) {
            subTypes.addAll(t.getAllSubTypes());
        }
        return subTypes;
    }

    /**
     * Return the super-types if this type set.
     * 
     * @return the super-types if this type set.
     */
    public Set<Type> getSuperTypes() {
        Set<Type> superTypes = new LinkedHashSet<Type>(this.types);
        for (Type t : this.types) {
            superTypes.addAll(t.getAllSuperTypes());
        }
        return superTypes;
    }
    
    
    /**
     * Returns <code>true</code> if this type set is an instance of an other
     * type set.
     * 
     * @param typeSet the other type set.
     * @return <code>true</code> if this type set is an instance of an other
     *         type set; <code>false</code> otherwise.
     */
    public boolean isInstanceOf(TypeSet typeSet) {
        Set<Type> thisSet = new LinkedHashSet<Type>(this.types);
        thisSet.addAll(this.getSubTypes());
        Set<Type> otherSet = new LinkedHashSet<Type>(typeSet.types);
        otherSet.addAll(typeSet.getSubTypes());
        return thisSet.containsAll(otherSet);
    }
    
    /**
     * Returns <code>true</code> if this type set contains only the object
     * type.
     * 
     * @return <code>true</code> if this type set contains only the object
     *         type; <code>false</code> otherwise.
     */
    public boolean isObject() {
        return this.types.size() == 1
                    && this.types.iterator().next().getImage().equals(Type.OBJECT_SYMBOL);
    }

    /**
     * Returns <code>true</code> if this type set contains only the number
     * type.
     * 
     * @return <code>true</code> if this type set contains only the number
     *         type; <code>false</code> otherwise.
     */
    public boolean isNumber() {
        return this.types.size() == 1
                    && this.types.iterator().next().getImage().equals(Type.NUMBER_SYMBOL);
    }
    
    /**
     * Returns the infinum type set between this type set and a specified type
     * set.
     * 
     * @param ts the type set.
     * @return the infinum type set between this type set and a specified type
     *         set.
     * @throws NullPointerException if <code>ts == null</code>.
     */
    public TypeSet infimum(TypeSet ts) {
        if (ts == null)
            throw new NullPointerException();
        TypeSet ts1 = new TypeSet();
        ts1.types = new LinkedHashSet<Type>(this.types);
        ts1.types.removeAll(ts.types);
        
        TypeSet ts2 = new TypeSet();
        ts2.types = new LinkedHashSet<Type>(ts.types);
        ts2.types.removeAll(this.types);
        
        TypeSet infinum = new TypeSet();
        infinum.types = new LinkedHashSet<Type>(this.types);
        infinum.types.retainAll(ts.types);
            
        Set<Type> st = ts1.getSubTypes();
        st.retainAll(ts2.getSubTypes());
        infinum.types.addAll(st);
        return infinum;
    }

    /**
     * Returns <code>true</code> if this type set intersection a specified
     * type set.
     * 
     * @param typeSet the type set to be tested.
     * @return <code>true</code> if this type set intersection a specified
     *         type set; <code>false</code> otherwise.
     * @throws NullPointerException if <code>typeSet == null</code>.
     */
    public boolean intersect(TypeSet typeSet) {
        if (typeSet == null)
            throw new NullPointerException();
        Set<Type> types = new LinkedHashSet<Type>(this.types);
        types.addAll(this.getSubTypes());
        return types.retainAll(typeSet.types);
    }
    
    
    /**
     * Returns <code>true</code> if this type is equals to an other object.
     * This method returns <code>true</code> if the object is a not null
     * instance of the class <code>TypeSet</code> and contains the same types.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this type is equals to an other object;
     *         <code>false</code> otherwise.
     * @see pddl4j.exp.type.Type
     */
    public boolean equals(Object obj) {
        if (obj != null && obj.getClass().equals(this.getClass())) {
            TypeSet other = (TypeSet) obj;
            return this.types.equals(other.types);
        }
        return false;
    }

    /**
     * Returns the hash code value of this type set. This method is supported
     * for the benefit of hashtables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return the hash code value of this type set.
     * @see java.lang.Object#hashCode()
     */
    public int hashCode() {
        return this.types.hashCode();
    }

    /**
     * Return a shallow copy of this type set.
     * 
     * @return a shallow copy of this type set.
     */
    public TypeSet clone() {
        try {
            TypeSet other = (TypeSet) super.clone();
            other.types = new LinkedHashSet<Type>();
            for (Type type : this.types) {
                other.types.add(type);
            }
            return other;
        } catch (CloneNotSupportedException e) {
            throw new Error();
        }
    }

    /**
     * Returns a string representation of this type.
     * 
     * @return a string representation of this type.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        if (this.types.size() == 0) {
            str.append("empty-type");
        } else if (this.types.size() == 1) {
            str.append(this.types.iterator().next().toString());
        } else {
            str.append("(either");
            for (Type t : this.types) {
                str.append(" " + t.toString());
            }
            str.append(")");
        }
        return str.toString();
    }

}
