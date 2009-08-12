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

package pddl4j;

import pddl4j.exp.ExpID;


/**
 * This expression is throw when an unexpected pddl expression is detected.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public class InvalidExpException extends Exception {
    
    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 5912801078385971869L;
    
    /**
     * The id of the invalid pddl expression.
     */
    private ExpID id;
    
    /**
     * Creates a new invalid PDDL expression.
     */
    private InvalidExpException() {}
    
    /**
     * Creates a new invalid PDDL expression with a specified expression id.
     * 
     * @param id the id of the pddl expression that produce the exception.
     */
    public InvalidExpException(ExpID id) {
        super();
        this.id = id;
    }

    /**
     * Creates a new invalid PDDL expression with a specified expression id.
     * 
     * @param id the id of the pddl expression that produce the exception.
     * @param message The message of the exception.
     * @param cause The cause of the exception.
     */
    public InvalidExpException(ExpID id, String message, Throwable cause) {
        super(message, cause);
        this.id = id;
    }

    /**
     * Creates a new invalid PDDL expression with a specified expression id.
     * 
     * @param id the id of the pddl expression that produce the exception.
     * @param message The message of the exception.
     */
    public InvalidExpException(ExpID id, String message) {
        super(message);
        this.id = id;
    }

    /**
     * Creates a new invalid PDDL expression with a specified expression id.
     * 
     * @param id the id of the pddl expression that produce the exception.
     * @param cause The cause of the exception.
     */
    public InvalidExpException(ExpID id, Throwable cause) {
        super(cause);
        this.id = id;
    }
    
    /**
     * Returns a string representation of this exception.
     * 
     * @return a string representation of this exception.
     */
    public String toString() {
        return super.getMessage() != null ?
            "invalid PDDL expression " + this.id + " : " + super.getMessage() :
            "invalid PDDL expression " + this.id;
    }

}
