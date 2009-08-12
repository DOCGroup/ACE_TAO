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

package pddl4j.exp.type;

/**
 * A typing exception is thrown if an attempt is made to create a type with a
 * inconsistant type hierarchy. A type hierarchy is consistant if it does not
 * contain cycle and all types inherite from object except the primitive type
 * number.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class TypingException extends RuntimeException {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 6846672849363008483L;

    /**
     * The inconsistant type.
     */
    private Type type;
    
    /**
     * Creates a new typing exception. with null as its detail message. The
     * cause is not initialized, and may subsequently be initialized by a call
     * to <code>Throwable.initCause(java.lang.Throwable)</code>.
     * 
     * @param type the inconsitant type.
     */
    public TypingException(Type type) {
        super();
        this.type = type;
    }

    /**
     * Creates a new typing exception with the specified cause and a detail
     * message of <code>(cause==null ? null : cause.toString())</code> (which
     * typically contains the class and detail message of cause). This
     * constructor is useful for exceptions that are little more than wrappers
     * for other throwables (for example, <code>PrivilegedActionException</code>).
     * 
     * @param type the inconsistant type.
     * @param cause the cause (which is saved for later retrieval by the
     *            <code>Throwable.getCause()</code> method). (A
     *            <code>null</code> value is permitted, and indicates that the
     *            cause is nonexistent or unknown.)
     */
    public TypingException(Type type, Throwable cause) {
        super(cause);
    }
    
    /**
     * Returns the typing source of this exception.
     * 
     * @return the typing source of this exception.
     */
    public Type getType() {
        return this.type;
    }
    
    /**
     * Returns a string representation of this typing exception.
     * 
     * @return a string representation of this typing exception.
     */
    public String toString() {
        return this.getClass().getName() + ": type hierachy of "
                    + this.type.toString() + " is insconsistant";
    }
}
