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

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Variable;

/**
 * This abstract generic class is implemented by all conjunctive PDDL expression.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public class AndExp extends ListExp {
    
    /**
     * The serial version if of the class.
     */
    private static final long serialVersionUID = -61659117952471061L;
    
    /**
     * Creates a new empty conjunctive expression.
     */
    public AndExp() {
        super(ExpID.AND);
    }
    
    /**
     * Creates a new list expression with a specific id and expressions.
     * 
     * @param exps an expressions list.
     */
    public AndExp(Exp... exps) {
        super(ExpID.AND, exps);
    }
    
    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding
     * expression.
     * 
     * @param sigma the substitution.
     * @return @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public AndExp apply(Substitution sigma) {
        return (AndExp) super.apply(sigma);
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.   
     * @see pddl4j.exp.Exp#standardize()  
     */
    public AndExp standardize() {
        return this.standardize(new HashMap<String,String>());
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. The map argument is used to store the variable already
     * standardized. The bound argument is used to store the bound variables.
     * Remember that free variables are existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this expression.       
     * @throws NullPointerException if <code>images == null || bound == null</code>.    
     * @see pddl4j.exp.Exp#standardize(Map)  
     */
    public AndExp standardize(Map<String, String> images) {
        return (AndExp) super.standardize(images);
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public AndExp clone() {
        return (AndExp) super.clone();
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
        AndExp and = new AndExp();
        Iterator<Exp> i = this.iterator();
        while (i.hasNext()) {
            Exp exp = i.next();
            if (exp.getExpID().equals(ExpID.FORALL)) {
                ForallExp forall = (ForallExp) exp;
                if (!forall.exp.getExpID().equals(ExpID.WHEN)) {
                    ForallExp tmp = new ForallExp();
                    for (Variable var : forall) {
                        tmp.add(var.clone());
                    }
                    and.add(forall.getExp());
                    while (i.hasNext()) {
                        and.add(i.next());
                    }
                    tmp.setExp(and.moveQuantifierOutward());
                    qexp = tmp;
                } else {
                    and.add(exp.moveQuantifierOutward());
                }
            }
            else if (exp.getExpID().equals(ExpID.EXIST)) {
                ExistsExp exists = (ExistsExp) exp;
                if (!exists.exp.getExpID().equals(ExpID.WHEN)) {
                    ExistsExp tmp = new ExistsExp();
                    for (Variable var : exists) {
                        tmp.add(var.clone());
                    }
                    and.add(exists.getExp());
                    while (i.hasNext()) {
                        and.add(i.next());
                    }
                    tmp.setExp(and.moveQuantifierOutward());
                    qexp = tmp;
                } else {
                    and.add(exp.moveQuantifierOutward());
                }
            } else {
                and.add(exp);
            }
        }
        return qexp == null ? and : qexp;
    }
    
    /**
     * Returns <code>true</code> if this and expression is conditional, i.e.,
     * if this expression contains a when expression.
     * 
     * @return <code>true</code> if this and expression is conditional;
     *         <code>false</code> otherwise.
     */
    private boolean isConditional() {
        boolean isConditional = false;
        Iterator<Exp> i = this.iterator();
        while (i.hasNext() && !isConditional) {
            isConditional = i.next().getExpID().equals(ExpID.WHEN);
        }
        return isConditional;
    }

    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public AndExp toConjunctiveNormalForm() {
        AndExp nnf = this.toNegativeNormalForm();
        AndExp cnf = new AndExp();
        if (nnf.isConditional()) {
            for (Exp elt : nnf) {
                if (!elt.getExpID().equals(ExpID.WHEN)) {
                    for (Exp cnfi : cnf) {
                        if (cnfi.getExpID().equals(ExpID.WHEN)) {
                            WhenExp when = (WhenExp) cnfi;
                            AndExp effect = new AndExp();
                            effect.add(when.getEffect());
                            effect.add(elt);
                            when.setEffect(effect);
                            when = when.normalize();
                        }
                    }
                } else {
                    WhenExp when = (WhenExp) elt;
                    cnf.add(when.normalize());
                }
            }
        } else {
            for (Exp elt : nnf) {
                elt = elt.toConjunctiveNormalForm();
                cnf.add(elt);
            }
        }
        return cnf;
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public OrExp toDisjunctiveNormalForm() {
        AndExp nnf = this.toNegativeNormalForm();
        OrExp dnf = new OrExp();
        dnf.add(new AndExp());
        for (Exp e : nnf) {
            Exp ednf = e.toDisjunctiveNormalForm();
            if (ednf.getExpID().equals(ExpID.OR)) {
                OrExp or = (OrExp) ednf;
                OrExp newDnf = new OrExp();
                for (Exp oe : or) {
                    for (Exp and : dnf) {
                        AndExp other = (AndExp) and.clone();
                        other.add(oe);
                        newDnf.add(other);
                    }
                }
                dnf = newDnf;
            } else {
                for (Exp ex : dnf) {
                    ((AndExp) ex).add(ednf);
                }
            }
        }
        return dnf;
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public AndExp toNegativeNormalForm() {
        AndExp other = new AndExp();
        for (Exp exp : this) {
            other.add(exp.toNegativeNormalForm());
        }
        return other;
    }
    
    /**
     * Returns a string representation of the conjunctive expression.
     * 
     * @return a string representation of the conjunctive expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(and");
        for (Exp elt : this.elements) {
            str.append(" ");
            str.append(elt.toString());
        }
        str.append(")");
        return str.toString();
    }
    
    /**
     * Returns a typed string representation of the conjunctive expression.
     * 
     * @return a typed string representation of the conjunctive expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(and");
        for (Exp elt : this.elements) {
            str.append(" ");
            str.append(elt.toTypedString());
        }
        str.append(")");
        return str.toString();
    }
    
}
