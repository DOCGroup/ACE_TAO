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
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.TermID;
import pddl4j.exp.term.Variable;

/**
 * This class implements a atomic formula of the PDDL langage.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class AtomicFormula extends AbstractExp implements Literal {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -9015916938283328690L;

    /**
     * The predicate of the atomic formula.
     */
    private String predicate;

    /**
     * The list of arguments contained in the atomic formula.
     */
    private ArrayList<Term> arguments;

    /**
     * Creates a new atomic formula with a specific predicate.
     * 
     * @param predicate the predicate of the atomic formula. The predicate must
     *            be a not null reference.
     */
    public AtomicFormula(String predicate) {
        super(ExpID.ATOMIC_FORMULA);
        this.predicate = predicate;
        this.arguments = new ArrayList<Term>();
    }

    /**
     * Sets a new predicate to this atomic formula.
     * 
     * @param predicate the new predicate to set. The predicate must be a not
     *            null reference.
     */
    public final void setPredicate(String predicate) {
        this.predicate = predicate;
    }

    /**
     * Returns the predicate of this atomic formula.
     * 
     * @return the predicate of this atomic formula.
     */
    public final String getPredicate() {
        return this.predicate;
    }

    /**
     * Adds a new term to this atomic formula.
     * 
     * @param term the term to add. The term must be not null.
     * @return <code>true</code> if the term was added; <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.
     */
    public final boolean add(Term term) {
        if (term == null)
            throw new NullPointerException();
        return this.arguments.add(term);
    }

    /**
     * Returns the arity of this atomic formula.
     * 
     * @return the arity of this atomic formula.
     */
    public final int getArity() {
        return this.arguments.size();
    }

    /**
     * Returns an iterator over the terms of the atomic formula.
     * 
     * @return an iterator over the terms of the atomic formula.
     * @see java.lang.Iterable#iterator()
     */
    public final Iterator<Term> iterator() {
        return this.arguments.iterator();
    }

    /**
     * Returns <code>true</code> if a term occurs in this atomic formula.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a term occurs in this atomic formula;
     *         <code>false</code> otherwise.
     */
    public boolean occurs(Term term) {
        return this.arguments.contains(term);
    }

    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public final AtomicFormula apply(Substitution sigma) {
        if (sigma == null) 
            throw new NullPointerException();
        AtomicFormula other = new AtomicFormula(this.predicate);
        for (Term arg : this) {
            other.add(arg.apply(sigma));
        }
        return other;
    }

    /**
     * Matches this atomic formula with an other specified atomic formula. Two
     * atomic formulas match if they have the same predicate symbol and arity
     * and each arguments are pairwise unifiable. Note, called match does not
     * modify the parameters of this method.
     * 
     * @param atom the atomic formula to match with.
     * @return The substitution that defines the binding constraints needed to
     *         match this atomic formula with the other atomic formula or
     *         <code>null</code> if no binding constraints allows to match the
     *         two atomic formulas.
     * @throws NullPointerException if <code>atom == null</code> or
     *             <code>sigma == null</code>.
     * @see pddl4j.exp.term.Term#unify(Term, Substitution)
     */
    public final Substitution match(AtomicFormula atom) {
        return this.match(atom, new Substitution());
    }
    
    /**
     * Matches this atomic formula with an other specified atomic formula by
     * taking into account a specified set of binding constraints. Two atomic
     * formulas match if they have the same predicate symbol and arity and each
     * arguments are pairwise unifiable. Note, called match does not modify the
     * parameters of this method.
     * 
     * @param atom the atomic formula to match with.
     * @param sigma the substitution containing the binding constraints.
     * @return The substitution that defines the binding constraints needed to
     *         match this atomic formula with the other atomic formula or
     *         <code>null</code> if no binding constraints allows to match the
     *         two atomic formulas.
     * @throws NullPointerException if <code>atom == null</code> or
     *             <code>sigma == null</code>.
     * @see pddl4j.exp.term.Term#unify(Term, Substitution)
     */
    public final Substitution match(AtomicFormula atom, Substitution sigma) {
        if (this.predicate.equals(atom.predicate)
                    && this.getArity() == atom.getArity()) {
            Substitution theta = sigma.clone();
            int i = 0;
            boolean failure = false;
            while (i < this.getArity() && !failure) {
                Term tthis = this.arguments.get(i);
                Term tother = atom.arguments.get(i);
                Substitution gamma = tthis.unify(tother, theta);
                if (gamma == null) {
                    failure = true;
                } else {
                    theta.compose(gamma);
                }
                i++;
            }
            return failure ? null : theta;
        }
        return null;
        
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.   
     */
    public AtomicFormula standardize() {
        return this.standardize(new HashMap<String,String>());
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
    public AtomicFormula standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        AtomicFormula other = new AtomicFormula(this.predicate);
        for (Term arg : this.arguments) {
            other.add(arg.standardize(images));
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
        Iterator<Term> i = this.iterator();
        while (i.hasNext() && isGround) {
            isGround = i.next().isGround();
        }
        return isGround;
    } 
    
    /**
     * Returns <code>true</code> if this atomic formula is equals to an other
     * object. This method return <code>true</code> if the object is a not
     * null instance of the class <code>AtomicFormula</code> and both atomic
     * formulas have the same predicate and arguments.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this atomic formula is equals to an other
     *         object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof AtomicFormula) {
            AtomicFormula other = (AtomicFormula) obj;
            return this.predicate.equals(other.predicate)
                        && this.arguments.equals(other.arguments);
        }
        return false;
    }

    /**
     * Returns the hash code value of this atomic formula. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return the hash code value of this atomic formula.
     * @see java.lang.Object#hashCode()
     */
    public int hashCode() {
        return this.predicate.hashCode() + this.arguments.hashCode();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public AtomicFormula clone() {
        AtomicFormula other = (AtomicFormula) super.clone();
        other.arguments = new ArrayList<Term>();
        for (Term arg : this.arguments) {
            other.arguments.add(arg.clone());
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
        OrExp dnf = new OrExp();
        dnf.add(this.clone());
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
        AndExp cnf = new AndExp();
        cnf.add(this.clone());
        return cnf;
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public AtomicFormula toNegativeNormalForm() {
        return this.clone();
    }
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new HashSet<Variable>();
        for (Term arg : this) {
            if (arg.getTermID().equals(TermID.VARIABLE)) {
                vars.add((Variable) arg);  
            }
        }
        return vars;
    }
    
    /**
     * Returns a string representation of this atomic formula.
     * 
     * @return a string representation of this atomic formula.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(");
        str.append(this.predicate);
        for (Term t : this.arguments) {
            str.append(" ");
            str.append(t.toString());
        }
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of this atomic formula.
     * 
     * @return a typed string representation of this atomic formula.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(");
        str.append(this.predicate);
        for (Term t : this.arguments) {
            str.append(" ");
            str.append(t.toTypedString());
        }
        str.append(")");
        return str.toString();
    }
        
}
