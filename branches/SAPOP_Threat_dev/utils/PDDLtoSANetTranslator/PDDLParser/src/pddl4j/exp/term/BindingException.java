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

package pddl4j.exp.term;

/**
 * BindingException is thrown if an attempt is made to bind a exp to a symbolic
 * or numeric constant.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class BindingException extends RuntimeException {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -4600701161512855951L;

    /**
     * Creates a new binding exception. with null as its detail message. The
     * cause is not initialized, and may subsequently be initialized by a call
     * to <code>Throwable.initCause(java.lang.Throwable)</code>.
     */
    public BindingException() {
        super();
    }

    /**
     * Creates a new binding exception with the specified detail message and
     * cause.
     * 
     * @param message the detail message (which is saved for later retrieval by
     *            the <code>Throwable.getMessage() method</code>).
     * @param cause the cause (which is saved for later retrieval by the
     *            <code>Throwable.getCause()</code> method). (A
     *            <code>null</code> value is permitted, and indicates that the
     *            cause is nonexistent or unknown.)
     */
    public BindingException(String message, Throwable cause) {
        super(message, cause);
    }

    /**
     * Creates a new binding exception with the specified detail message. The
     * cause is not initialized, and may subsequently be initialized by a call
     * to <code>Throwable.initCause(java.lang.Throwable)</code>.
     * 
     * @param message the detail message (which is saved for later retrieval by
     *            the Throwable.getMessage() method).
     */
    public BindingException(String message) {
        super(message);
    }

    /**
     * Creates a new binding exception with the specified cause and a detail
     * message of <code>(cause==null ? null : cause.toString())</code> (which
     * typically contains the class and detail message of cause). This
     * constructor is useful for exceptions that are little more than wrappers
     * for other throwables (for example, <code>PrivilegedActionException</code>).
     * 
     * @param cause the cause (which is saved for later retrieval by the
     *            <code>Throwable.getCause()</code> method). (A
     *            <code>null</code> value is permitted, and indicates that the
     *            cause is nonexistent or unknown.)
     */
    public BindingException(Throwable cause) {
        super(cause);
    }

}
