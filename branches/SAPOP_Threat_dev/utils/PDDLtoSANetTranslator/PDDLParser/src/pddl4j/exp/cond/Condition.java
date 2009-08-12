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

/**
 * This enumeration defines all condition that can be express with pddl language.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public enum Condition {
    
    /**
     * The always condition.
     */
    ALWAYS,
    /**
     * The sometime condition.
     */
    SOMETIME,
    /**
     * The whithin condition.
     */
    WITHIN,
    /**
     * The at most once condition.
     */
    AT_MOST_ONCE,
    /**
     * The some time after condition.
     */
    SOMETIME_AFTER,
    /**
     * The sometime before condition.
     */
    SOMETIME_BEFORE,
    /**
     * The always within condition.
     */
    ALWAYS_WITHIN,
    /**
     * The hold during condition.
     */
    HOLD_DURING,
    /**
     * The hold after econdition.
     */
    HOLD_AFTER,
}
