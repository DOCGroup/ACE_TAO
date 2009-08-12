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

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Iterator;

import pddl4j.exp.AndExp;
import pddl4j.exp.Exp;
import pddl4j.exp.term.Term;

/**
 * This class implements a strips action of the pddl language.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public class Action extends AbstractActionDef {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -991066278473010044L;

    /**
     * The preconditions of the action.
     */
    private Exp precondition;

    /**
     * The effect of the action.
     */
    private Exp effect;
    
    /**
     * Creates a new strips action with a specific image. By default the
     * precondition is set to an empty conjunctive precondition goal description
     * and the effect to an empty proposition effect.
     * 
     * @param name - the name of the action. The image musts be a not reference
     *            to a string.
     * @throws NullPointerException if <code>name == null<code>.           
     */
    public Action(String name) {
        super(ActionID.ACTION, name);
        this.precondition = new AndExp();
        this.effect = new AndExp();
    }
    
    /**
     * Returns the precondition of the action. 
     * 
     * @return the precondition of the action.
     */
    public final Exp getPrecondition() {
        return this.precondition;
    }
    
    /**
     * Sets the new precondition of the action.
     * 
     * @param pre the new precondition of the action.
     * @throws NullPointerException if <code>pre == null</code>.
     */
    public final void setPrecondition(Exp pre) {
        if (pre == null) 
            throw new NullPointerException();
        this.precondition = pre;
    }
    
    /**
     * Returns the effect of the action. 
     * 
     * @return the effect of the action.
     * @throws NullPointerException if <code>effect == null</code>.
     */
    public final Exp getEffect() {
        return this.effect;
    }
    
    /**
     * Sets the new effect of the action.
     * 
     * @param effect the new effect of the action.
     */
    public final void setEffect(Exp effect) {
        if (effect == null) 
            throw new NullPointerException();
        this.effect = effect;
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public Action standardize() {
        Map<String,String> images = new LinkedHashMap<String, String>();
        Action other = this.clone();
        other.parameters.clear();
        for (Term param : this.parameters) {
            other.add(param.standardize(images));
        }
        other.precondition = this.precondition.standardize(images);
        other.effect = this.effect.standardize(images);
        return other;
    }
    
    /**
     * Normalize this action. This method returns a copy of this action where:
     * <ul>
     * <li> each variable is standardized,</li>
     * <li> precondition expression is convert into disjunctive prenex normal
     * form and</li>
     * <li> effect expression is is convert into conjunctive prenex normal form.</li>
     * <ul>
     * 
     * @return a normalize copy of this action.
     * @see pddl4j.exp.Exp#toPrenexNormaForm()
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public Action normalize() {
        Action other = (Action) this.standardize();
        other.precondition = other.precondition.moveQuantifierOutward()
                    .toDisjunctiveNormalForm();
        other.effect = other.effect.moveQuantifierOutward()
                    .toConjunctiveNormalForm();
        return other;
    }

    /**
     * Returns <code>true</code> if the action is ground.
     * 
     * @return <code>true</code> if the action is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public boolean isGround() {
        return super.isGround() && this.precondition.isGround()
                    && this.effect.isGround();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public Action clone() {
        Action other = (Action) super.clone();
        other.precondition = this.precondition.clone();
        other.effect = this.effect.clone();
        return other;
    }
    
    /**
     * Returns a string representation of the action.
     * 
     * @return a string representation of the action.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(:action ");
        str.append(this.getName().toString());
        str.append("\n");
        str.append("   :parameters ");
        str.append("(");
        if (!this.parameters.isEmpty()) {
            Iterator<Term> i = this.parameters.iterator();
            Term param = i.next();
            str.append(param.toTypedString());
            while (i.hasNext()) {
                str.append(" " + i.next().toTypedString());
            }
        }
        str.append(")");
        str.append("\n   :precondition ");
        str.append(this.precondition.toString());
        str.append("\n   :effect ");
        str.append(this.effect.toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of the action.
     * 
     * @return a typed string representation of the action.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(:action ");
        str.append(this.getName().toString());
        str.append("\n");
        str.append("   :parameters ");
        str.append("(");
        if (!this.parameters.isEmpty()) {
            Iterator<Term> i = this.parameters.iterator();
            str.append(i.next().toTypedString());
            while (i.hasNext()) {
                str.append(" " + i.next().toTypedString());
            }
        }
        str.append(")");
        str.append("\n   :precondition ");
        str.append(this.precondition.toTypedString());
        str.append("\n   :effect ");
        str.append(this.effect.toTypedString());
        str.append(")");
        return str.toString();
    }
    
}
