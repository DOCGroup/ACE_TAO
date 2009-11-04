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

import java.util.Map;
import java.util.LinkedHashMap;
import java.util.Iterator;

import pddl4j.exp.Exp;
import pddl4j.exp.term.Term;

/**
 * This class implements a durative action of the PDDl language.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public class DurativeAction extends AbstractActionDef {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 7123555664571927698L;
    
    /**
     * The duration constraint of the action.
     */
    private Exp constraint;
    
    /**
     * The condition of the durative action.
     */
    private Exp condition;
    
    /**
     * The effect of the durative action.
     */
    private Exp effect;
    
    /**
     * Creates a new durative action with a specific name.
     * 
     * @param name the name of the durative action. The name must a not null
     *            reference.
     * @throws NullPointerException if <code>name == null</code>.           
     */
    public DurativeAction(String name) {
        super(ActionID.DURATIVE_ACTION, name);   
    }
    
    /**
     * Returns the duration constraint of the durative action. 
     * 
     * @return the duration constraint of the durative action.
     */
    public final Exp getConstraint() {
        return this.constraint;
    }
    
    /**
     * Sets the new duration constraint of the durative action.
     * 
     * @param cons the new duration constraint of the durative action.
     * @throws NullPointerException if <code>cons == null</code>.
     */
    public final void setConstraint(Exp cons) {
        if (cons == null)
            throw new NullPointerException();
        this.constraint = cons;
    }
    
    /**
     * Returns the condition of the durative action. 
     * 
     * @return the condition of the durative action.
     */
    public final Exp getCondition() {
        return this.condition;
    }
    
    /**
     * Sets the new condition of the durative action.
     * 
     * @param cond the new condition of the durative action.
     * @throws NullPointerException if <code>cond == null</code>.
     */
    public final void setCondition(Exp cond) {
        if (cond == null)
            throw new NullPointerException();
        this.condition = cond;
    }
    
    /**
     * Returns the effect of the durative action. 
     * 
     * @return the effect of the durative action.
     */
    public final Exp getEffect() {
        return this.effect;
    }
    
    /**
     * Sets the new effect of the durative action.
     * 
     * @param effect the new effect of the durative action.
     * @throws NullPointerException if <code>effect == null</code>.
     */
    public final void setEffect(Exp effect) {
        if (effect == null) 
            throw new NullPointerException();
        this.effect = effect;
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. a standardized copy of this expression.
     * 
     * @return a standardized copy of this expression.
     */
    public DurativeAction standardize() {
        Map<String,String> images = new LinkedHashMap<String, String>();
        DurativeAction other = this.clone();
        other.parameters.clear();
        for (Term param : this.parameters) {
            other.add(param.standardize(images));
        }
        other.constraint = this.constraint.standardize(images);
        other.condition = this.condition.standardize(images);
        other.effect = this.effect.standardize(images);
        return other;
    }
    
    /**
     * Normalize this action. This method returns a copy of this action where:
     * <ul>
     * <li> each variable is standardized,</li>
     * <li> duration constraints expression is convert into conjunctive prenex
     * normal form and</li>
     * <li> condition expression is convert into disjunctive prenex normal form
     * and</li>
     * <li> effect expression is is convert into conjunctive prenex normal form.</li>
     * <ul>
     * 
     * @return a normalize copy of this action.
     * @see pddl4j.exp.Exp#toPrenexNormaForm()
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public DurativeAction normalize() {
        DurativeAction other = (DurativeAction) this.standardize();
        other.constraint = other.constraint.moveQuantifierOutward()
                    .toConjunctiveNormalForm();
        other.condition = other.condition.moveQuantifierOutward()
                    .toDisjunctiveNormalForm();
        other.effect = other.effect.moveQuantifierOutward()
                    .toConjunctiveNormalForm();
        return other;
    }
    
    /**
     * Returns <code>true</code> if the expression is ground.
     * 
     * @return <code>true</code> if the expression is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public boolean isGround() {
        return super.isGround() && this.constraint.isGround()
                    && this.condition.isGround()
                    && this.effect.isGround();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public DurativeAction clone() {
        DurativeAction other = (DurativeAction) super.clone();
        other.constraint = this.constraint.clone();
        other.condition = this.condition.clone();
        other.effect = this.effect.clone();
        return other;
    }
    
    /**
     * Returns a string representation of the durative action.
     * 
     * @return a string representation of the durative action.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(:durative-action ");
        str.append(this.getName());
        str.append(":parameters ");
        str.append("(");
        if (!this.parameters.isEmpty()) {
            Iterator<Term> i = this.parameters.iterator();
            str.append(i.next().toString());
            while (i.hasNext()) {
                str.append(" " + i.next().toString());
            }
        }
        str.append(")");
        str.append("\n:duration ");
        str.append(this.constraint.toString());
        str.append("\n:condition ");
        str.append(this.condition.toString());
        str.append("\n:effect ");
        str.append(this.effect.toString());
        str.append(")");
        return str.toString();
    }
    
    /**
     * Returns a typed string representation of the durative action.
     * 
     * @return a typed string representation of the durative action.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(:durative-action ");
        str.append(this.getName());
        str.append(":parameters ");
        str.append("(");
        if (!this.parameters.isEmpty()) {
            Iterator<Term> i = this.parameters.iterator();
            str.append(i.next().toTypedString());
            while (i.hasNext()) {
                str.append(" " + i.next().toTypedString());
            }
        }
        str.append(")");
        str.append("\n:duration ");
        str.append(this.constraint.toTypedString());
        str.append("\n:condition ");
        str.append(this.condition.toTypedString());
        str.append("\n:effect ");
        str.append(this.effect.toTypedString());
        str.append(")");
        return str.toString();
    }
}
