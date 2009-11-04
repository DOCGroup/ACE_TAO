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
 * This enumeration list all expression id used in the pddl language.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public enum ExpID {
     /**
     * The compared function expression id.
     */
    F_COMP,    
    /**
     * The assign operator expression id.
     */
    ASSIGN_OP,
    /**
     * The when expression id.
     */
    WHEN,
    /**
     * The and expression id.
     */
    AND,
    /**
     * The or expression id.
     */
    OR,
    /**
     * The imply expression id.
     */
    IMPLY,
    /**
     * The node expression id.
     */
    NOT,
    /**
     * The preference expression id.
     */
    PREFERENCE,
    /**
     * The existential expression id.
     */
    EXIST,
    /**
     * The universal expression id.
     */
    FORALL,
    /**
     * The atomic formula expression id.
     */
    ATOMIC_FORMULA,
    /**
     * The derived predicate expression id. 
     */
    DERIVED_PREDICATE, 
    /**
     * The term expression id.
     */
    TERM, 
    /**
     * The optimization expression id.
     */
    METRIC_EXP, 
    /**
     * The conditional expression id.
     */
    COND_EXP, 
    /**
     * The timed expression id.
     */
    TIMED_EXP, 
    
    /**
     *  New--probabilistic
     */
    PROB_EXP,
    
}
