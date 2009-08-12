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
import java.util.Iterator;
import java.util.Map;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Variable;

/**
 * This generic class is implemented by all disjunctive PDDL expression.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public final class OrExp extends ListExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 7735506016529447105L;

    /**
     * Creates a new empty disjunctive expression.
     */
    public OrExp() {
        super(ExpID.OR);
    }
    
    /**
     * Creates a new list expression with a specific id and expressions.
     * 
     * @param exps an expressions list.
     */
    public OrExp(Exp... exps) {
        super(ExpID.OR, exps);
    }
    
    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding
     * expression.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public OrExp apply(Substitution sigma) {
        return (OrExp) super.apply(sigma);
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return this expression.
     */
    public OrExp standardize() {
        return this.standardize(new LinkedHashMap<String,String>());
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. The map argument is used to store the variable already
     * standardized. The bound argument is used to store the bound variables.
     * Remember that free variables are existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return this expression.     
     * @throws IllegalArgumentException if <code>images == null || bound == null</code>.      
     */
    public OrExp standardize(Map<String, String> images) {
        return (OrExp) super.standardize(images);
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public OrExp clone() {
        return (OrExp) super.clone();
    }
    
    /**
     * Returns the prenex normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the prenex normal form of this expression.
     */
    public Exp toPrenexNormaForm() {
        return this.standardize().moveQuantifierOutward();
    }
    
    /**
     * Returns a copy of this expression such that the quantifiers appear first.
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     * @see pddl4j.exp.Exp#moveQuantifierOutward()
     */
    public Exp moveQuantifierOutward() {
        Exp qexp = null;
        OrExp or = new OrExp();
        Iterator<Exp> i = this.iterator();
        while (i.hasNext()) {
            Exp exp = i.next();
            if (exp.getExpID().equals(ExpID.FORALL)) {
                ForallExp forall = (ForallExp) exp;
                ForallExp tmp = new ForallExp();
                for (Variable var : forall) {
                    tmp.add(var);
                }
                or.add(forall.getExp());
                while (i.hasNext()) {
                    or.add(i.next());
                }
                tmp.setExp(or.moveQuantifierOutward());
                qexp = tmp;
            }
            else if (exp.getExpID().equals(ExpID.EXIST)) {
                ExistsExp exists = (ExistsExp) exp;
                ExistsExp tmp = new ExistsExp();
                for (Variable var : exists) {
                    tmp.add(var);
                }
                or.add(exists.getExp());
                while (i.hasNext()) {
                    or.add(i.next());
                }
                tmp.setExp(or.moveQuantifierOutward());
                qexp = tmp;
            } else {
                or.add(exp);
            }
        }
        return qexp == null ? or : qexp;
        
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public OrExp toNegativeNormalForm() {
        OrExp other = new OrExp();
        for (Exp exp : this) {
            other.add(exp.toNegativeNormalForm());
        }
        return other;
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public OrExp toDisjunctiveNormalForm() {
        OrExp nnf = this.toNegativeNormalForm();
        OrExp dnf = new OrExp();
        for (Exp i : nnf) {
            Exp exp = i.toDisjunctiveNormalForm();
            dnf.add(exp);
        }
        return dnf;
    }
      
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public AndExp toConjunctiveNormalForm() {
        OrExp nnf = this.toNegativeNormalForm();
        AndExp cnf = new AndExp();
        cnf.add(new OrExp());
        for (Exp e : nnf) {
            Exp ecnf = e.toConjunctiveNormalForm();
            if (ecnf.getExpID().equals(ExpID.AND)) {
                AndExp and = (AndExp) ecnf;
                AndExp newCnf = new AndExp();
                for (Exp ae : and) {
                    for (Exp or : cnf) {
                        OrExp other = (OrExp) or.clone();
                        other.add(ae);
                        newCnf.add(other);
                    }
                }
                cnf = newCnf;
            } else {
                for (Exp ex : cnf) {
                    ((OrExp) ex).add(ecnf);
                }
            }
        }
        return cnf;
    }
        
    /**
     * Returns a string representation of the disjunctive expression.
     * 
     * @return a string representation of the disjunctive expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(or");
        for (Exp elt : this.elements) {
            str.append(" ");
            str.append(elt.toString());
        }
        str.append(")");
        return str.toString();
    }
    
    /**
     * Returns a typed string representation of the disjunctive expression.
     * 
     * @return a typed string representation of the disjunctive expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(or");
        for (Exp elt : this.elements) {
            str.append(" ");
            str.append(elt.toTypedString());
        }
        str.append(")");
        return str.toString();
    }
    
}
