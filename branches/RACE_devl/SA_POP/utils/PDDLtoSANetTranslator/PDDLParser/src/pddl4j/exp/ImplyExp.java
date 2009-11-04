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

import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This class is implemented by all implication expression of the PDDL langage.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class ImplyExp extends AbstractExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 1L;

    /**
     * The head of the implication expression.
     */
    protected Exp head;

    /**
     * The body of the implication expression.
     */
    protected Exp body;

    /**
     * Creates a new empty implication expression with a specific head and body
     * expressions.
     * 
     * @param head the head the implication expression. The head of the imply
     *            expression must be a not null reference to an instance of
     *            <code>Exp</code>.
     * @param body the body the implication expression. The body of the imply
     *            formula must be a not null reference to an instance of
     *            <code>Exp</code>.
     */
    public ImplyExp(Exp head, Exp body) {
        super(ExpID.IMPLY);
        this.head = head;
        this.body = body;
    }
    
    /**
     * Sets a new head to this implication expression.
     * 
     * @param head the new head of this implication expression. The new head of
     *            the implication expression must be a not null reference to an
     *            instance of <code>Exp</code>.
     * @throws NullPointerException if <code>head == null</code>.            
     */
    public final void setHead(Exp head) {
        if (head == null)
            throw new NullPointerException();
        this.head = head;
    }

    /**
     * Returns the head of the implication expression.
     * 
     * @return the head of the implication expression.
     */
    public final Exp getHead() {
        return this.head;
    }

    /**
     * Sets a new body to this implication expression.
     * 
     * @param body the new body of this implication expression. The new body of
     *            the implication expression must be a not null reference to an
     *            instance of <code>Exp</code>.
     * @throws NullPointerException if <code>body == null</code>.           
     */
    public final void setBody(Exp body) {
        if (body == null)
            throw new NullPointerException();
        this.body = body;
    }

    /**
     * Returns the body of the implication expression.
     * 
     * @return the body of the implication expression.
     */
    public final Exp getBody() {
        return this.body;
    }
    
    /**
     * Returns <code>true</code> if a exp occurs in this not
     * expression.
     * 
     * @param term the exp to be tested.
     * @return <code>true</code> if a exp occurs in this not
     *         expression; <code>false</code> otherwise.
     */
    public final boolean occurs(Term term) {
        return this.head.occurs(term) || this.body.occurs(term);
    }
    
    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public final ImplyExp apply(Substitution sigma) {
        if (sigma == null) 
            throw new NullPointerException();
        return new ImplyExp(this.head.apply(sigma), this.body.apply(sigma));
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return  a standardized copy of this expression.   
     * @see pddl4j.exp.Exp#standardize()
     */
    public ImplyExp standardize() {
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
     * @throws NullPointerException if
     *             <code>images == null || bound == null</code>.
     * @see pddl4j.exp.Exp#standardize(Map)
     */
    public ImplyExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        return new ImplyExp(this.head.standardize(images), 
                    this.body.standardize(images));
    }
    
    /**
     * Returns <code>true</code> if the expression is ground.
     * 
     * @return <code>true</code> if the expression is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public final boolean isGround() {
        return this.head.isGround() && this.body.isGround();
    }
    
    /**
     * Returns <code>true</code> if this implication expression is equal to an
     * other object, i.e., if the object is a not null reference to an instance
     * of the class <code>ImplyExp</code> and both implication expressions
     * have the same head and body expressions.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if this implication expression is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof ImplyExp) {
            ImplyExp other = (ImplyExp) obj;
            return this.getExpID().equals(other.getExpID())
                        && this.head.equals(other.head)
                        && this.body.equals(other.body);
        }
        return false;
    }

    /**
     * Returns a hash code value for the implication expression. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the implication expression.
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.head.hashCode()
                    + this.body.hashCode();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public ImplyExp clone() {
        ImplyExp other = (ImplyExp) super.clone();
        other.head = this.head.clone();
        other.body = this.body.clone();
        return other;
    }
    
    /**
     * Returns a copy of this expression such that the quantifiers appear first.
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     * @see pddl4j.exp.Exp#moveQuantifierOutward()
     */
    public Exp moveQuantifierOutward() {
        Exp exp = null;
        if (this.head.getExpID().equals(ExpID.EXIST)) {
            ExistsExp exists = (ExistsExp) this.head;
            ForallExp forall = new ForallExp();
            for (Variable var : exists) {
                forall.add(var.clone());
            }
            ImplyExp imply = new ImplyExp(exists.getExp(), this.body);
            forall.setExp(imply.moveQuantifierOutward());
            exp = forall.moveQuantifierOutward();
        } else if (this.head.getExpID().equals(ExpID.FORALL)) {
            ForallExp forall = (ForallExp) this.head;
            ExistsExp exists = new ExistsExp();
            for (Variable var : forall) {
                exists.add(var.clone());
            }
            ImplyExp imply = new ImplyExp(forall.getExp(), this.body);
            exists.setExp(imply.moveQuantifierOutward());
            exp = exists.moveQuantifierOutward();
        } else if (this.body.getExpID().equals(ExpID.EXIST)) {
            ExistsExp exists = (ExistsExp) this.body;
            ExistsExp tmp = new ExistsExp();
            for (Variable var : exists) {
                tmp.add(var.clone());
            }
            ImplyExp imply = new ImplyExp(this.head, exists.getExp());
            tmp.setExp(imply.moveQuantifierOutward());
            exp = tmp.moveQuantifierOutward();
        } else if (this.body.getExpID().equals(ExpID.FORALL)) {
            ForallExp forall = (ForallExp) this.body;
            ForallExp tmp = new ForallExp();
            for (Variable var : forall) {
                tmp.add(var.clone());
            }
            ImplyExp imply = new ImplyExp(this.head, forall.getExp());
            tmp.setExp(imply.moveQuantifierOutward());
            exp = tmp.moveQuantifierOutward();
        } else {
            exp = this.clone();
        }
        return exp;
    }
    
    /**
     * Return the disjunctive normal form of this expression.
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public OrExp toDisjunctiveNormalForm() {
        return this.toNegativeNormalForm().toDisjunctiveNormalForm();
    }
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public AndExp toConjunctiveNormalForm() {
        return this.toNegativeNormalForm().toConjunctiveNormalForm();
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public OrExp toNegativeNormalForm() {
        OrExp nnf = new OrExp();
        nnf.add(new NotExp(this.head));
        nnf.add(this.body);
        return nnf.toNegativeNormalForm();
    }
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new LinkedHashSet<Variable>();
        vars.addAll(this.head.getFreeVariables());
        vars.addAll(this.body.getFreeVariables());
        return vars;
    }
    
    /**
     * Returns a string representation of the implication expression.
     * 
     * @return a string representation of the implication expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(imply ");
        str.append(this.head.toString());
        str.append(" ");
        str.append(this.body.toString());
        str.append(")");
        return str.toString();
    }
    
    /**
     * Returns a typed string representation of the implication expression.
     * 
     * @return a typed string representation of the implication expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(imply ");
        str.append(this.head.toTypedString());
        str.append(" ");
        str.append(this.body.toTypedString());
        str.append(")");
        return str.toString();
    }

}
