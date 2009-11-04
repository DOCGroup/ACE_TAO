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

import pddl4j.exp.Exp;

/**
 * This class implements an "at" timed expression of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class AtTimedExp extends TimedExp {

    /**
     * The time specifier of the "at" timed expression.
     */
    protected TimeSpecifier specifier;

    /**
     * Creates a new "at" timed expression with a specific time specifier and
     * expression.
     * 
     * @param specifier The time specifier of the timed expression.
     * @param exp The timed expression..
     * @throws NullPointerException if <code>specifier == null</code> of
     *             <code> exp == null</code>.
     */
    protected AtTimedExp(TimeSpecifier specifier, Exp exp) {
        super(TimedExpID.AT, exp);
        this.specifier = specifier;
    }

    /**
     * Returns the time specifier of the expression.
     * 
     * @return the time specifier of the expression.
     */
    public final TimeSpecifier getTimeSpecifier() {
        return this.specifier;
    }

    /**
     * Sets a new time specifier to the expression.
     * 
     * @param specifier The new specifier to set.
     * @throws NullPointerException if <code>specifier == null</code>.
     */
    public final void setTimeSpecifier(TimeSpecifier specifier) {
        this.specifier = specifier;
    }
   
    /**
     * Returns <code>true</code> if the "at" timed expression is equal to
     * an other object, i.e., if the object is a not null reference to an
     * instance of the class <code>AtEndTimedExp</code> and both "at start"
     * timed expressions contain the same timed expression.
     * 
     * @param obj - the reference object with which to compare.
     * @return <code>true</code> if the "at" timed expression equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof AtTimedExp) {
            AtTimedExp other = (AtTimedExp) obj;
            return super.equals(obj) && this.specifier.equals(other.specifier);
        }
        return false;
    }

    /**
     * Returns a hash code value for the "at" timed expression. This method
     * is supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the at timed expression.
     */
    public int hashCode() {
        return super.hashCode() + this.specifier.hashCode();
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public AtTimedExp clone() {
        return (AtTimedExp) super.clone();
    }
    
}
