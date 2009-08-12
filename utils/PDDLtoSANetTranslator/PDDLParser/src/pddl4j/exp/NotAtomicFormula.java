/*
 * Copyright Dept. of Mathematics & Computer Science Univ. Paris-Descartes This
 * software is governed by the CeCILL license under French law and abiding by
 * the rules of distribution of free software. You can use, modify and/ or
 * redistribute the software under the terms of the CeCILL license as circulated
 * by CEA, CNRS and INRIA at the following URL "http://www.cecill.info". As a
 * counterpart to the access to the source code and rights to copy, modify and
 * redistribute granted by the license, users are provided only with a limited
 * warranty and the software's author, the holder of the economic rights, and
 * the successive licensors have only limited liability. In this respect, the
 * user's attention is drawn to the risks associated with loading, using,
 * modifying and/or developing or reproducing the software by the user in light
 * of its specific status of free software, that may mean that it is complicated
 * to manipulate, and that also therefore means that it is reserved for
 * developers and experienced professionals having in-depth computer knowledge.
 * Users are therefore encouraged to load and test the software's suitability as
 * regards their requirements in conditions enabling the security of their
 * systems and/or data to be ensured and, more generally, to use and operate it
 * in the same conditions as regards security. The fact that you are presently
 * reading this means that you have had knowledge of the CeCILL license and that
 * you accept its terms.
 */

package pddl4j.exp;

import java.util.Iterator;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;

/**
 * This class implements negative atomic formula of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class NotAtomicFormula extends NotExp implements Literal {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -1751696323200745754L;

    /**
     * Creates a new negative atomic formula with a specific atomic formula.
     * 
     * @param atom the atomic formula contained in this negative atomic formula.
     * @throws NullPointerException if <code>atom == null</code>.
     */
    public NotAtomicFormula(AtomicFormula atom) {
        super(atom);
    }

    /**
     * Sets a new predicate to this negative atomic formula.
     * 
     * @param predicate the new predicate to set.
     * @throws NullPointerException if <code>predicate == null</code>.
     */
    public final void setPredicate(String predicate) {
        this.getExp().setPredicate(predicate);
    }

    /**
     * Returns the predicate of this negative atomic formula.
     * 
     * @return the predicate of this negative atomic formula.
     */
    public final String getPredicate() {
        return this.getExp().getPredicate();
    }

    /**
     * Adds a new term to this negative atomic formula.
     * 
     * @param term the term to add.
     * @return <code>true</tt> if the expression was added; <code>false</code> 
     *      otherwise.
     * @throws NullPointerException if <code>term == null</code>.
     */
    public final boolean add(Term term) {
        return this.getExp().add(term);
    }

    /**
     * Returns the arity of this negative atomic formula.
     * 
     * @return the arity of this negative atomic formula.
     */
    public final int getArity() {
        return this.getExp().getArity();
    }

    /**
     * Returns an iterator over the expression of this negative atomic formula.
     * 
     * @return an iterator over the expression of this negative atomic formula.
     * @see java.lang.Iterable#iterator()
     */
    public final Iterator<Term> iterator() {
        return this.getExp().iterator();
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
    public final NotAtomicFormula apply(Substitution sigma) {
        return new NotAtomicFormula((AtomicFormula) this.exp.apply(sigma));
    }

    /**
     * Returns the negated expression of this expression.
     * 
     * @return the negated expression of this expression.
     */
    public AtomicFormula getExp() {
        return (AtomicFormula) super.getExp();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public NotAtomicFormula clone() {
        return (NotAtomicFormula) super.clone();
    }

}
