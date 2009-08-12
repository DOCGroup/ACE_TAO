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

package pddl4j.exp.fexp;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import pddl4j.EvaluationException;
import pddl4j.exp.term.AbstractTerm;
import pddl4j.exp.term.BindingException;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.TermID;
import pddl4j.exp.term.Variable;
import pddl4j.exp.type.TypeSet;

/**
 * This class implements a function expression in the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public abstract class FExp extends AbstractTerm implements Iterable<Term> {
    
    /**
     * The serial version id of this class.
     */
    private static final long serialVersionUID = -2343417682285770327L;

    /**
     * The list of the arguments of this function head.
     */
    private ArrayList<Term> arguments;
    
    /**
     * Create an empty function with a specific functor.
     * 
     * @param id the term id of this function.
     * @param functor The function functor.
     * @throws NullPointerException if <code>id == null</code> or
     *             <code>functor == null</code> or <code>type == null</code>.
     */
    protected FExp(TermID id, String functor) {
        super(id, functor);
        this.arguments = new ArrayList<Term>();
    }
    
    /**
     * Create an empty function with a specific functor.
     * 
     * @param id the term id of this function.
     * @param functor The function functor.
     * @param type the type of this function.
     * @throws NullPointerException if <code>id == null</code> or
     *             <code>functor == null</code> or <code>type == null</code>.
     */
    protected FExp(TermID id, String functor, TypeSet type) {
        super(id, functor, type);
        this.arguments = new ArrayList<Term>();
    }
     
    /**
     * Adds a new argument to this function.
     * 
     * @param arg The argument to add.
     * @return <code>true</code> if the arg was added; <code>false</code>
     *         otherwise.
     * @throws NullPointerException if <code>arg == null</code>.
     */
    protected boolean add(Term arg) {
        return this.arguments.add(arg);
    }

    /**
     * Returns the argument at the specified position in this function.
     * 
     * @param index index of the argument to return.
     * @return the argument at the specified position in this function.
     * @throws IndexOutOfBoundsException if the index is out of range 
     *      (<code>index < 0  || index >= getArity()</code>).
     */
    protected Term get(int index) {
        return this.arguments.get(index);
    }
    
    /**
     * Replaces the argument at the specified position in this function with the
     * specified term.
     * 
     * @param index index of the argument to replace.
     * @param arg argument to be stored at the specified position.
     * @return the argument previously at the specified position.
     * @throws IndexOutOfBoundsException if the index is out of range (<code>index < 0  || index >= getArity()</code>).
     * @throws NullPointerException if <code>arg == null</code>.
     */
    protected Term set(int index, Term arg) {
        return this.arguments.set(index, arg);
    }
    
    /**
     * Returns the arity of this function.
     * 
     * @return the arity of this function.
     */
    public final int getArity() {
        return this.arguments.size();
    }
    
    /**
     * Returns <code>true</code> if a expression occurs in this function.
     * 
     * @param term the term to be tested.
     * @return <code>true</code> if a expression occurs in this function;
     *         <code>false</code> otherwise.
     * @throws NullPointerException if <code>term == null</code>.        
     */
    public final boolean occurs(Term term) {
        if (term == null) 
            throw new NullPointerException();
        boolean occurs = false;
        Iterator<Term> i = this.iterator();
        while (i.hasNext() && !occurs) {
            occurs = i.next().occurs(term);
        }
        return occurs;
    }
    
    /**
     * Returns <code>true</code> if this function expression is ground.
     * 
     * @return <code>true</code> if this function expression is ground;
     *         <code>false</code> otherwise.
     */
    public final boolean isGround() {
        boolean ground = true;
        Iterator<Term> i = this.iterator();
        while (i.hasNext() && ground) {
            ground = i.next().isGround();
        }
        return ground;
    }
    
    /**
     * Substitutes all occurrences of the variables that occur in this term and
     * that are bound in the substitution by its binding term. This method
     * modify the content of the term on which it is applied.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     * @throws EvaluationException if a ground arithmetic function occurs in
     *             this function expression and its evaluation fails.
     */
    public Term apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        FExp other = this.clone();
        other.arguments.clear();
        for (Term arg : this.arguments) {
            other.add(arg.apply(sigma));
        }
        return other;
    }

    /**
     * Standardizes this function expression. Remember that free variables are
     * existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public FExp standardize() {
        return this.standardize(new LinkedHashMap<String, String>());
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. The map argument is used to store the variable already
     * standardized.Remember that free variables are existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this expression.
     * @throws NullPointerException if <code>images == null</code>.
     */
    public FExp standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        FExp other = this.clone();
        other.arguments.clear();
        for (Term arg : this.arguments) {
            other.add(arg.standardize(images));
        }
        return other;
    }

    /**
     * Returns an iterator over a set of arguments contained in this function.
     * 
     * @return an iterator over a set of arguments contained in this function.
     */
    public final Iterator<Term> iterator() {
        return this.arguments.iterator();
    }

    /**
     * Unify this term with an other specified term. Note, call unify does not modify
     * the parameters of this method. 
     * 
     * @param term the term to unify with.
     * @return The substitution that defines the binding constraints needed to
     *         unify this term with the other term or <code>null</code> if no
     *         binding constraints allows to unify the two terms.
     * @see pddl4j.exp.term.Term#unify(Term, Substitution)  
     * @throws BindingException if the term to unify with this function is a
     *             function with the same symbol and the same arity and has an
     *             incompatible type, i.e.,
     *             <code>!this.getTypeSet().getSubTypes().containsAll(term.getTypeSet().getSubTypes())</code>.      
     */
    public final Substitution unify(Term term) { 
       return this.unify(term, new Substitution()); 
    }
    
    /**
     * Unify this term with an other specified term by taking into account a
     * specified set of binding constraints. Note, call unify does not modify
     * the parameters of this method.
     * 
     * @param term the term to unify with.
     * @param sigma the substitution containing the binding constraints.
     * @return The substitution that defines the binding constraints needed to
     *         unify this term with the other term or <code>null</code> if no
     *         binding constraints allows to unify the two terms.
     * @throws BindingException if the term to unify with this function is a
     *             function with the same symbol and the same arity and has an
     *             incompatible type, i.e.,
     *             <code>!this.getTypeSet().getSubTypes().containsAll(term.getTypeSet().getSubTypes())</code>.
     */
    public final Substitution unify(Term term, Substitution sigma) {
        if (term.getTermID().equals(TermID.VARIABLE)) {
            return term.unify(this, sigma);
        } else {
            FExp func = (FExp) term;
            if (func.getImage().equals(this.getImage())
                        && func.getArity() == this.getArity()) {
                if (this.getTypeSet().getSubTypes().containsAll(
                            func.getTypeSet().getSubTypes())) {
                    Substitution theta = sigma.clone();
                    int i = 0;
                    boolean failure = false;
                    while (i < this.getArity() && !failure) {
                        Term tthis = this.arguments.get(i);
                        Term tother = func.arguments.get(i);
                        Substitution gamma = tthis.unify(tother, theta);
                        if (gamma == null) {
                            failure = true;
                        } else {
                            theta.compose(gamma);
                        }
                        i++;
                    }
                    return failure ? null : theta;
                } else {
                    throw new BindingException("cannot bind "
                            + this.toTypedString() + " with "
                            + term.toTypedString() + ": imcompatible type");
                }
            }
        }
        return null;
    }
    
    /**
     * Returns the set of free variables of this expression.
     * 
     * @return the set of free variables of this expression.
     */
    public Set<Variable> getFreeVariables() {
        Set<Variable> vars = new LinkedHashSet<Variable>();
        for (Term arg : this) {
            vars.addAll(arg.getFreeVariables());
        }
        return vars;
    }
    
    /**
     * Returns <code>true</code> if this function is equals to an other
     * object, i.e., if the object is a not null instance of the class
     * <code>FExp</code> and both function have the same functor and the
     * same arguments.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this function head is equals to an other
     *         object; <code>false</code> otherwise.
     */
    public boolean equals(final Object obj) {
        if (obj != null && obj instanceof FExp) {
            final FExp other = (FExp) obj;
            return super.equals(obj) && this.arguments.equals(other.arguments);
        }
        return false;
    }

    /**
     * Returns the hash code value of this function head.
     * 
     * @return the hash code value of this function head.
     */
    public int hashCode() {
        return super.hashCode() + this.arguments.hashCode();
   }

    /**
     * Return a deep copy of this function.
     * 
     * @return a deep copy of this function.
     * @see pddl4j.exp.term.AbstractTerm#clone()
     */
    public FExp clone() {
        FExp other = (FExp) super.clone();
        other.arguments = new ArrayList<Term>();
        for (Term arg : this.arguments) {
            other.arguments.add(arg.clone());
        }
        return other;        
    }
    
    /**
     * Returns a string representation of this function.
     * 
     * @return a string representation of this function
     */
    public String toString() {
        final StringBuilder str = new StringBuilder();
        str.append("(");
        str.append(this.getImage().toString());
        for (Term t : this.arguments) {
            str.append(" " + t.toString());
        }
        str.append(")");
        return str.toString();
    }
    
    /**
     * Returns typed string representation of this function.
     * 
     * @return typed string representation of this function.
     */
    public String toTypedString() {
        final StringBuilder str = new StringBuilder();
        str.append("(");
        str.append(this.getImage());
        for (Term t : this.arguments) {
            str.append(" " + t.toTypedString());
        }
        str.append(")");
        str.append(" - ");
        str.append(this.getTypeSet().toString());
        return str.toString();
    }
    
 
}
