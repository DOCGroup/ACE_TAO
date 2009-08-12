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

package pddl4j.exp.cond;

import pddl4j.exp.AbstractExp;
import pddl4j.exp.ExpID;

/**
 * This abstract class defines a conditional expression of the pddl language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public abstract class CondExp extends AbstractExp {

    /**
     * The condition of this conditional expression.
     */
    private Condition condition;

    /**
     * Creates a new conditional expression with a specified condition.
     * 
     * @param cond the condition of this expression.
     * @throws NullPointerException if <code>condition  == null</code>.
     */
    protected CondExp(Condition cond) {
        super(ExpID.COND_EXP);
        this.condition = cond;
    }

    /**
     * Returns the condition of this conditional expression.
     * 
     * @return the condition of this conditional expression.
     */
    public final Condition getCondition() {
        return this.condition;
    }

    /**
     * Returns <code>true</code> if this conditional expression is equal to an
     * other object. This method returns <code>true</code> if the object is a
     * not null instance of the class <code>CondExp</code> and both
     * conditional expression have the same condition.
     * 
     * @param obj the object to be compared.
     * @return <code>true</code> if this conditional expression is equal to an
     *         other object; <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj instanceof CondExp) {
            CondExp other = (CondExp) obj;
            return this.condition.equals(other.condition);
        }
        return false;
    }

    /**
     * Returns a hash code value for the conditional expression. This method is
     * supported for the benefit of hash tables such as those provided by
     * <code>java.util.Hashtable</code>.
     * 
     * @return a hash code value for the conditional expression.
     */
    public int hashCode() {
        return super.hashCode() + this.condition.hashCode();
    }

}
