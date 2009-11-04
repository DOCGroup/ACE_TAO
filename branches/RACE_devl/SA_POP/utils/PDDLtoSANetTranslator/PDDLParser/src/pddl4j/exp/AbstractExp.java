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

/**
 * This abstract class implements the common part of all PDDl expression.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public abstract class AbstractExp implements Exp {
    
    /**
     * The id of the expression.
     */
    private ExpID id;
    
    /**
     * Creates a new expression with a specific id.
     * 
     * @param id The id of the expression.
     */
    protected AbstractExp(ExpID id) {
        this.id = id;
    }
    
    /**
     * Returns the id of the expression.
     * 
     * @return the id of the expression.
     * @see pddl4j.exp.Exp#getExpID()
     */
    public final ExpID getExpID() {
       return this.id;
    }
    
    /**
     * Returns <code>true</code> if this expression is equal to an other
     * object. This method returns <code>true</code> if the object is a not
     * null reference to an instance of the class <code>AbstractExp</code> and
     * both expressions have the same expression id.
     * 
     * @param obj the reference object with which to compare.
     * @return <code>true</code> if the expression is equal to an other
     *         object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof AbstractExp) {
            AbstractExp other = (AbstractExp) obj;
            return this.id.equals(other.id);
        }
        return false;
    }

    /**
     * Returns a hash code value for this expression. This method
     * is supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for this expression.
     */
    public int hashCode() {
        return this.id.hashCode();
    }
    
    /**
     * Returns a deep copy of this expression.
     * 
     * @return a deep copy of this expression.
     */
    public Exp clone() {
        try {
            Exp other = (Exp) super.clone();
            return other;
        } catch (CloneNotSupportedException e) {
            throw new InternalError();
        }
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
        return this.clone();
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public Exp toDisjunctiveNormalForm() {
        return this.clone();
    }
    
    /**
     * Return the disjunctive normal form of this expression. Note, the
     * expression returned is a copy. 
     * 
     * @return the disjunctive normal form of this expression.
     * @see pddl4j.exp.Exp#toDisjunctiveNormalForm()
     */
    public Exp toConjunctiveNormalForm() {
        return this.clone();
    }
    
    /**
     * Returns the negative normal form of this expression. Note, the expression
     * returned is a copy. 
     * 
     * @return the negative normal form of this expression.
     * @see pddl4j.exp.Exp#toNegativeNormalForm()
     */
    public Exp toNegativeNormalForm() {
        return this.clone();
    }
    
}
