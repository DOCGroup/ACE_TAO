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

package pddl4j.exp.time;

import java.util.LinkedHashMap;
import java.util.Iterator;
import java.util.Map;

import pddl4j.exp.Literal;
import pddl4j.exp.fexp.Number;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;

/**
 * This class implements a timed literal used in initial state definition of the
 * PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class TimedLiteral extends TimedExp implements Literal {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -1735624931301766305L;

    /**
     * The time stamp of the timed literal.
     */
    private Number ts;

    /**
     * Creates a new timed literal with a literal.
     * 
     * @param time the time stamp of the timed literal.
     * @param literal the literal of the timed literal.
     * @throws NullPointerException if <code>time == null</code> or
     *             <code>literal == null</code>.
     * @throws IllegalArgumentException if <code>time.getValue() < 0</code>.
     */
    public TimedLiteral(Number time, Literal literal) {
        super(TimedExpID.LITERAL, literal);
        if (time == null || literal == null)
            throw new NullPointerException();
        if (time.getValue() < 0)
            throw new IllegalArgumentException();
        this.ts = time;
    }
    
    /**
     * Returns the literal contained in the timed expression.
     * 
     * @return the literal contained in the timed expression.
     */
    public Literal getExp() {
        return (Literal) this.exp;
    }
    
    /**
     * Returns the time stamp of the timed literal.
     * 
     * @return the time stamp of the timed literal.
     */
    public Number getTimeStamp() {
        return this.ts;
    }

    /**
     * Sets a new time stamp to the timed literal.
     * 
     * @param time the new time stamp to set..
     * @throws NullPointerException if <code>time == null</code> or
     *             <code>literal == null</code>.
     * @throws IllegalArgumentException if <code>time.getValue() < 0</code>.
     */
    public void setTimeStamp(Number time) {
        if (time == null)
            throw new NullPointerException();
        if (time.getValue() < 0)
            throw new IllegalArgumentException();
        this.ts = time;
    }

    /**
     * Adds a new term to the timed literal.
     * 
     * @param term the term to add.
     * @return <code>true</code> if the term was added;
     *         <code>false</code otherwise.
     * @throws NullPointerException if <code>term == null</code>.       
     * @see pddl4j.exp.Literal#add(pddl4j.exp.term.Term)
     */
    public boolean add(Term term) {
        return this.getExp().add(term);
    }

    /**
     * Returns the arity of the timed literal.
     * 
     * @return the arity of the timed literal.
     * @see pddl4j.exp.Literal#getArity()
     */
    public int getArity() {
        return this.getExp().getArity();
    }

    /**
     * Returns the predicate of the timed literal.
     * 
     * @return the predicate of the timed literal.
     * @see pddl4j.exp.Literal#getPredicate()
     */
    public String getPredicate() {
        return this.getExp().getPredicate();
    }

    /**
     * Returns a iterator over the expression of the timed literal.
     * 
     * @return a iterator over the expression of the timed literal.
     * @see java.lang.Iterable#iterator()
     */
    public Iterator<Term> iterator() {
        return this.getExp().iterator();
    }

    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding
     * term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public TimedLiteral apply(Substitution sigma) {
        if (sigma == null)
            throw new NullPointerException();
        return new TimedLiteral(this.getTimeStamp().clone(), 
                    (Literal) this.getExp().apply(sigma));
    }

    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public TimedLiteral standardize() {
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
    public TimedLiteral standardize(Map<String, String> images) {
        if (images == null)
            throw new NullPointerException();
        return new TimedLiteral(this.getTimeStamp().clone(), (Literal) this
                    .getExp().standardize(images));
    }

    /**
     * Returns <code>true</code> if the timed literal is equal to an other
     * object, i.e., if the object is a not null reference to an instance of the
     * class <code>TimedLiteral</code> and both timed literals have the same
     * time stamp and literal expression.
     * 
     * @param obj The reference object with which to compare.
     * @return <code>true</code> if the timed literal equal to an other
     *         object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && this.getClass().equals(obj.getClass())) {
            TimedLiteral other = (TimedLiteral) obj;
            return super.equals(obj) && this.ts.equals(other.ts);
        }
        return false;
    }

    /**
     * Returns a hash code value for the timed literal. This method is supported
     * for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the effect goal description.
     */
    public int hashCode() {
        return super.hashCode() + this.ts.hashCode();
    }

    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public TimedLiteral clone() {
        TimedLiteral other = (TimedLiteral) super.clone();
        other.ts = this.ts.clone();
        return other;
    }
    
    /**
     * Returns a string representation of this "at start" timed expression.
     * 
     * @return a string representation of this "at start" timed expression.
     */
    public String toString() {
        StringBuffer str = new StringBuffer();
        str.append("(at ");
        str.append(this.ts.toString());
        str.append(" ");
        str.append(this.getExp().toString());
        str.append(")");
        return str.toString();
    }

    /**
     * Returns a typed string representation of this "at start" timed
     * expression.
     * 
     * @return a typed string representation of this "at start" timed
     *         expression.
     */
    public String toTypedString() {
        StringBuffer str = new StringBuffer();
        str.append("(at ");
        str.append(this.ts.toTypedString());
        str.append(" ");
        str.append(this.getExp().toTypedString());
        str.append(")");
        return str.toString();
    }
}
