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

package pddl4j.exp.action;


import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import pddl4j.exp.term.Term;

/**
 * This class implements the common structures and method shares by all PDDL
 * action.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class AbstractActionDef implements ActionDef {
    
    /**
     * The action id.
     */
    private ActionID id;
    
    /**
     * The image of this action.
     */
    protected String name;

    /**
     * The parameters of the action.
     */
    protected List<Term> parameters;

    /**
     * Creates a new action with a specific image.
     * 
     * @param id the kind of the action.
     * @param name The name of the action. The image of an action must be a
     *            not null reference.
     * @throws NullPointerException if <code>id == null || name == null<code>.   
     */
    protected AbstractActionDef(ActionID id, String name) {
        this.id = id;
        this.setName(name);
        this.parameters = new ArrayList<Term>();
    }
    
    /**
     * Returns the action id of this action.
     * 
     * @return the action id of this action.
     * @see pddl4j.exp.action.ActionID
     */
    public final ActionID getActionID() {
        return this.id;
    }
    
    /**
     * Returns the image of the action.
     * 
     * @return the image of this action.
     */
    public final String getName() {
        return this.name;
    }

    /**
     * Adds a parameter to this action.
     * 
     * @param param the parameters to add.
     * @return <code>true</code> if the parameter was added;
     *         <code>false</code> otherwise.
     * @throws NullPointerException if <code>param == null</code>.
     */
    public final boolean add(Term param) {
        if (param == null)
            throw new NullPointerException();
        return this.parameters.add(param);
    }

    /**
     * Returns the parameters of the action.
     * 
     * @return the parameters of this action.
     */
    public final List<Term> getParameters() {
        return this.parameters;
    }
    
    /**
     * Sets a new image to this action.
     * 
     * @param name The new name to set. The image of an action must be a not
     *            null reference.
     * @throws NullPointerException if <code>name == null</code>.           
     */
    public final void setName(String name) {
        if (name == null)
            throw new NullPointerException();
        this.name = name;
    }
    
    /**
     * Returns <code>true</code> if a specified expression occurs in the
     * parameters of this action.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a specified expression occurs in the
     *         parameters of this action; <code>false</code> otherwise.
     * @see pddl4j.exp.Exp#occurs(pddl4j.exp.term.Term)
     */
    public final boolean occurs(Term term) {
        return this.parameters.contains(term);
    }
    
    /**
     * Returns an iterator over the parameters of this action.
     * 
     * @return an iterator over the parameters of this action.
     */
    public final Iterator<Term> iterator() {
        return this.parameters.iterator();
    }
    
    /**
     * Returns <code>true</code> if the abstract action is ground.
     * 
     * @return <code>true</code> if the abstract action is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public boolean isGround() {
        boolean gound = true;
        Iterator<Term> i = this.parameters.iterator();
        while (i.hasNext() && gound) {
            gound = i.next().isGround();
        }
        return gound;
    }
        
    /**
     * Returns <code>true</code> if this action is equal to an other
     * object, i.e., if the object is a not null instance of the class
     * <code>Action</code> and both action have the same name.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if this action is equal to an other
     *         object; <code>false</code> otherwise.
     */
    public final boolean equals(Object obj) {
        if (obj != null && obj instanceof ActionDef) {
            Action other = (Action) obj;
            return this.getName().equals(other.getName());
        }
        return false;
    }

    /**
     * Returns a hash code value for the action. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the strips action.
     */
    public final int hashCode() {
        return this.getName().hashCode();
    }

    /**
     * Returns a deep copy of this abstract action.
     * 
     * @return a deep copy of this abstract action.
     * @see pddl4j.exp.AbstractExp#clone()
     */
    public AbstractActionDef clone() {
        AbstractActionDef other = null;
        try {
            other = (AbstractActionDef) super.clone();
        } catch (CloneNotSupportedException e) {
            e.printStackTrace();
        }
        other.parameters = new ArrayList<Term>();
        for (Term param : this.parameters) {
            other.parameters.add(param.clone());
        }
        return other;        
    }
    
}
