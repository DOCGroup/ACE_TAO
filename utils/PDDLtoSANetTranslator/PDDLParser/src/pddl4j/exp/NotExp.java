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
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

/**
 * This class is implemented by all negative expression in the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class NotExp extends AbstractExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 4325921465416962373L;

    /**
     * The negative goal descriptions of this not formula.
     */
    protected Exp exp;

    /**
     * Creates a new empty negative expression from a specific expression.
     * 
     * @param exp The negative expression.
     * @throws NullPointerException if <code>exp == null</code>.
     */
    public NotExp(Exp exp) {
        super(ExpID.NOT);
        if (exp == null)
            throw new NullPointerException();
        this.exp = exp;

    }

    /**
     * Sets a new negative goal description to this not formula.
     * 
     * @param exp the new negative expression to set.
     * @throws NullPointerException if <code>exp == null</code>.
     */
    public final void setExp(Exp exp) {
        if (exp == null)
            throw new NullPointerException();
        this.exp = exp;
    }

    /**
     * Returns the negative expression of expression.
     * 
     * @return the negative expression of expression.
     */
    public Exp getExp() {
        return this.exp;
    }

    /**
     * Returns <code>true</code> if a term occurs in this not expression.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this not expression;
     *         <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.
     */
    public final boolean occurs(Term term) {
        if (term == null)
            throw new NullPointerException();
        return this.exp.occurs(term);
    }

    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding expression.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public NotExp apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        return new NotExp(this.exp.apply(sigma));
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return  a standardized copy of this expression. 
     * @see pddl4j.exp.Exp#standardize()
     */
    public NotExp standardize() {
        return this.standardize(new LinkedHashMap<String, String>());
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
    public NotExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        return new NotExp(this.exp.standardize(images));
    }

    /**
     * Returns <code>true</code> if the expression is ground.
     * 
     * @return <code>true</code> if the expression is ground;
     *         <code>false</code>otherwise.
     * @see pddl4j.exp.Exp#isGround()
     */
    public final boolean isGround() {
        return this.exp.isGround();
    }

    /**
     * Returns <code>true</code> if this negative expression is equal to an
     * other object, i.e., if the object is a not null reference to an instance
     * of the class <code>NotExp</code> and both negative expressions have the
     * same expression.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if this negative expression is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof NotExp) {
            NotExp other = (NotExp) obj;
            return this.getExpID().equals(other.getExpID())
                        && this.exp.equals(other.exp);
        }
        return false;
    }

    /**
     * Returns a hash code value for the negative expression. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the negative expression.
     */
    public int hashCode() {
        return this.getExpID().hashCode() + this.exp.hashCode();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public NotExp clone() {
        NotExp other = (NotExp) super.clone();
        other.exp = this.exp.clone();
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
        ExistsExp exists = null;
        ForallExp forall = null;
        
        switch(this.exp.getExpID()) {
        case EXIST:
            exists = (ExistsExp) this.exp;
            forall = new ForallExp();
            for (Variable var : exists) {
                forall.add(var.clone());
            }
            forall.setExp(new NotExp(exists.getExp()).moveQuantifierOutward());
            exp = forall.moveQuantifierOutward();
            break;
        case FORALL:
            forall = (ForallExp) this.exp;
            exists = new ExistsExp();
            for (Variable var : forall) {
                exists.add(var.clone());
            }
            exists.setExp(new NotExp(forall.getExp()).moveQuantifierOutward());
            exp = exists.moveQuantifierOutward();
            break;
        default:
            exp = this.clone();
        }
        return exp; 
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public OrExp toDisjunctiveNormalForm() {
        Exp nnf = this.toNegativeNormalForm();
        if (nnf.getExpID().equals(ExpID.NOT)) {
            OrExp dnf = new OrExp();
            dnf.add(new NotExp(this.exp.toDisjunctiveNormalForm()).toNegativeNormalForm());
            return dnf;
        } 
        return (OrExp) nnf.toDisjunctiveNormalForm();
    }
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public AndExp toConjunctiveNormalForm() {
        Exp nnf = this.toNegativeNormalForm();
        if (nnf.getExpID().equals(ExpID.NOT)) {
            AndExp cnf = new AndExp();
            cnf.add(new NotExp(this.exp.toConjunctiveNormalForm()).toNegativeNormalForm());
            return cnf;
        } 
        return (AndExp) nnf.toConjunctiveNormalForm();
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public Exp toNegativeNormalForm() {
        AndExp and = null;
        OrExp or = null;
        NotExp not = null;
        ImplyExp imply = null;
        ExistsExp exists = null;
        ForallExp forall = null;
        
        switch(this.exp.getExpID()) {
        case AND:
            and = (AndExp) this.exp;
            or = new OrExp();
            for (Exp e : and) {
                not = new NotExp(e);
                or.add(not.toNegativeNormalForm());
            }
            return or.size() == 1 ? or.get(0) : or;
        case OR:
            or = (OrExp) this.exp;
            and = new AndExp();
            for (Exp e : or) {
                not = new NotExp(e);
                and.add(not.toNegativeNormalForm());
            }
            return and.size() == 1 ? and.get(0) : and;
        case NOT:
         //   return this.exp.toNegativeNormalForm();
        	return ((NotExp)this.exp).exp.toNegativeNormalForm();
        case IMPLY:
            imply = (ImplyExp) this.exp;
            and = new AndExp();
            and.add(imply.getHead());
            and.add(new NotExp(imply.getBody()));
            return and.toNegativeNormalForm();
        case EXIST:
            exists = (ExistsExp) this.exp;
            forall = new ForallExp();
            for (Variable var : exists) {
                forall.add(var.clone());
            }
            forall.setExp(new NotExp(exists.getExp()).toNegativeNormalForm());
            return forall;
        case FORALL:
            forall = (ForallExp) this.exp;
            exists = new ExistsExp();
            for (Variable var : forall) {
                exists.add(var.clone());
            }
            exists.setExp(new NotExp(forall.getExp()).toNegativeNormalForm());
            return exists;
        default:
            return this.clone();
        }
    }
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        return this.exp.getFreeVariables();
    }
    
    /**
     * Returns a string representation of the negative expression.
     * 
     * @return a string representation of the negative expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(not ");
        str.append(this.exp.toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of the negative expression.
     * 
     * @return a typed string representation of the negative expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(not ");
        str.append(this.exp.toTypedString());
        str.append(")");
        return str.toString();
    }

}