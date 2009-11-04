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

import pddl4j.exp.term.Variable;

/**
 * This class implements an existential expression of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public final class ExistsExp extends QuantifiedExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 509423923623946119L;

    /**
     * Creates a new existential expression with a specific list of bound
     * variables and quantified expression.
     * 
     * @param exp the quantified expression.
     */
    public ExistsExp(Exp exp) {
        super(ExpID.EXIST, exp);
    }
    
    /**
     * Creates a new existential expression with a specific list of bound
     * variables and quantified expression.
     */
    public ExistsExp() {
        super(ExpID.EXIST);
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression. 
     */
    public ExistsExp standardize() {
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
     * @return a standardized copy of this expression.     
     * @throws NullPointerException if <code>images == null || bound == null</code>.      
     * @see pddl4j.exp.Exp#standardize(Map)
     */
    public ExistsExp standardize(Map<String, String> images) {
        return (ExistsExp) super.standardize(images);
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public ExistsExp clone() {
        return (ExistsExp) super.clone();
    }
       
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public ExistsExp toDisjunctiveNormalForm() {
        ExistsExp dnf = this.clone();
        dnf.exp = this.exp.toDisjunctiveNormalForm();
        return dnf;
    }
    
    /**
     * Returns the conjunctive normal form of this expression. Note, the
     * expression returned is a copy.
     * 
     * @return the conjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toConjunctiveNormalForm()
     */
    public ExistsExp toConjunctiveNormalForm() {
        ExistsExp cnf = this.clone();
        cnf.exp = this.exp.toConjunctiveNormalForm();
        return cnf;
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public ExistsExp toNegativeNormalForm() {
        ExistsExp nnf = this.clone();
        nnf.exp = this.exp.toNegativeNormalForm();
        return nnf;
    }
    
    /**
     * Returns a copy of this expression such that the quantifiers appear first.
     * 
     * @return a copy of this expression such that the quantifiers appear first.
     * @see pddl4j.exp.Exp#moveQuantifierOutward()
     */
    public Exp moveQuantifierOutward() {
        ExistsExp exists = this.clone();
        exists.exp = this.exp.moveQuantifierOutward();
        if (exists.exp.getExpID().equals(ExpID.FORALL)) {
            ForallExp forall = (ForallExp) exists.exp;
            exists.setExp(forall.exp);
            forall.setExp(exists);
            return forall;
        } else if (exists.exp.getExpID().equals(ExpID.EXIST)) {
            ExistsExp other = (ExistsExp) exists.exp;
            for (Variable var : other) {
                exists.add(var);
            }
            exists.setExp(other.exp);
            return exists;
        } else if (exists.exp.getExpID().equals(ExpID.WHEN)) {
            WhenExp when = (WhenExp) exists.exp;
            exists.setExp(when.getCondition());
            when.setCondition(exists);
            return when;
        } 
        return exists;
    }
}
