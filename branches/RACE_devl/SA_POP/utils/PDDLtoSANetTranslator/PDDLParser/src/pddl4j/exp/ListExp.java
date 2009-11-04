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

package pddl4j.exp;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This abstract class implements the common part of the conjunctive and
 * disjunctive expression of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class ListExp extends AbstractExp implements Iterable<Exp> {

    /**
     * The list of the elements contained in this list.
     */
    protected ArrayList<Exp> elements;

    /**
     * Creates a new empty list expression with a specific id.
     * 
     * @param id The id of the list expression.
     */
    protected ListExp(ExpID id) {
        super(id);
        this.elements = new ArrayList<Exp>();
    }

    /**
     * Creates a new list expression with a specific id and expressions.
     * 
     * @param id The id of the list expression.
     * @param exps an expressions list.
     */
    protected ListExp(ExpID id, Exp... exps) {
        super(id);
        for (Exp exp : exps) {
            this.elements.add(exp);
        }
    }
    
    /**
     * Adds a new element to this to this list expression.
     * 
     * @param elt the element to add. The element must be a not null reference.
     * @return <code>true</code> if the element was added; <code>false</code>
     *         otherwise.
     */
    public final boolean add(Exp elt) {
        boolean added = true;
        if (elt.getExpID().equals(this.getExpID())) {
            ListExp listExp = (ListExp) elt;
            for (Exp exp : listExp) {
                added &= this.elements.add(exp);
            }
        } else {
            added = this.elements.add(elt); 
        }
        return added; 
    }
    
    /**
     * Adds a new element to this to this list expression at a specified position.
     * 
     * @param i the position where the element must be added.
     * @param elt the element to add. The element must be a not null reference.
     */
    public final void add(int i, Exp elt) {
        if (elt.getExpID().equals(this.getExpID())) {
            ListExp listExp = (ListExp) elt;
            int index = i; 
            for (Exp exp : listExp) {
                this.elements.add(index, exp);
                index++;
            }
        } else {
            this.elements.add(i, elt);
        }
    }
    
    /**
     * Set a new element at a specified position.
     * 
     * @param i the position where the element must be added.
     * @param elt the element to add. The element must be a not null reference.
     */
    public final void set(int i, Exp elt) {
        this.elements.set(i, elt);
    }
    
    /**
     * Returns the element at a specified position.
     * 
     * @param i the position of the element to be returned.
     * @return the element at the specified position.
     */
    public final Exp get(int i) {
        return this.elements.get(i);
    }
    
    /**
     * Removes a specified elements of the list expression.
     * 
     * @param elt the element to be removed.
     * @return <code>true</code> if the element of the list expression was
     *         removed; <code>false</code> otherwise.
     */
    public final boolean remove(Exp elt) {
        return this.elements.remove(elt);
    }

    /**
     * Removes an elements of the list expression at a specified position.
     * 
     * @param i the position of the element to be removed.
     * @return the element removed.
     */
    public final Exp remove(int i) {
        return this.elements.remove(i);
    }
    
    /**
     * Returns an iterator over the elements contained in this list expression.
     * 
     * @return an iterator over the elements contained in this list expression.
     * @see java.lang.Iterable#iterator()
     */
    public final Iterator<Exp> iterator() {
        return this.elements.iterator();
    }
    
    /**
     * Returns <code>true</code> if an expression occurs in this list
     * expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this list
     *         expression; <code>false</code> otherwise.
     */
    public final boolean occurs(Term term) {
        boolean occurs = false;
        Iterator<Exp> i = this.iterator();
        while (i.hasNext() && !occurs) {
            occurs = i.next().occurs(term);
        }
        return occurs;
    }
    
    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding exp.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public ListExp apply(Substitution sigma) {
        if (sigma == null) 
            throw new NullPointerException();
        ListExp other = this.clone();
        other.elements.clear();
        for (Exp exp : this) {
            other.add(exp.apply(sigma));
        }
        return other;
    }
    
    /**
     * Returns <code>true</code> if the expression is ground.
     * 
     * @return <code>true</code> if the expression is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public final boolean isGround() {
        boolean isGround = true;
        Iterator<Exp> i = this.iterator();
        while (i.hasNext() && isGround) {
            isGround = i.next().isGround();
        }
        return isGround;
    } 
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression.
     * 
     * @return a standardized copy of this expression. 
     * @see pddl4j.exp.Exp#standardize() 
     */
    public ListExp standardize() {
        return this.standardize(new LinkedHashMap<String,String>());
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
     * @see pddl4j.exp.Exp#standardize(Map)
     */
    public ListExp standardize(Map<String, String> images) {
        if (images == null) 
            throw new NullPointerException();
        ListExp other = this.clone();
        other.elements.clear();
        for (Exp exp : this.elements) {
            other.add(exp.standardize(images));
        }
        return other;
    }
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new LinkedHashSet<Variable>();
        for (Exp exp : this) {
            vars.addAll(exp.getFreeVariables());  
        }
        return vars;
    }
    
    /**
     * Returns <code>true</code> if this list expression is equal to an
     * other object, i.e., if the object is a not null reference to an instance
     * of the class <code>ListExp</code> and both conjunctive expressions have
     * the same elements.
     * 
     * @param obj - the reference object with which to compare.
     * @return <code>true</code> if this list expression is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof ListExp) {
            ListExp other = (ListExp) obj;
            return this.getExpID().equals(other.getExpID())
                        && this.elements.equals(other.elements);
        }
        return false;
    }

    /**
     * Returns a hash code value for the list expression. This method is
     * supported for the benefit of hashtables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the list expression.
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.elements.hashCode();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public ListExp clone() {
        ListExp other = (ListExp) super.clone();
        other.elements = new ArrayList<Exp>();
        for (Exp exp : this.elements) {
            other.elements.add(exp.clone());
        }
        return other;
    }
    
    /**
     * Returns the size of the list expression.
     * 
     * @return the size of the list expression.
     */
    public final int size() {
        return this.elements.size();
    }
    
}
