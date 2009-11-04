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

import java.io.File;

/**
 * This class implements a parser exception. The parser exception of exception
 * is thrown if an internal error occurs in parser.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class ParserException extends Exception {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 1695639410883338894L;

    /**
     * The line of the error.
     */
    private int line;

    /**
     * The column of the error.
     */
    private int column;

    /**
     * The file where the error was detected.
     */
    private File file;

    /**
     * Creates a new parser exception with a specific message and cause.
     * 
     * @param message the message.
     * @param cause the cause.
     */
    public ParserException(String message, Throwable cause) {
        super(message, cause);
        this.line = -1;
        this.column = -1;
        this.file = null;
    }

    /**
     * Creates a new parser exception with a specific message.
     * 
     * @param message the message.
     */
    public ParserException(String message) {
        super(message);
        this.line = -1;
        this.column = -1;
        this.file = null;
    }

    /**
     * Creates a new parser exception with a specific message.
     * 
     * @param message the message.
     * @param file the file where the error was detected.
     * @param line the line of the file where the error was detected.
     * @param column the column of the file where the error was detected. 
     */
    public ParserException(String message, File file, int line, int column) {
        super(message);
        this.file = file;
        this.line = line;
        this.column = column;
    }

    /**
     * Returns the file where the error was detected.
     * 
     * @return the file where the error was detected or <code>null</code> if no file was specifies.
     */
    public File getFile() {
        return this.file;
    }

    /**
     * Returns the column of parser exception.
     * 
     * @return the column of the parser exception or -1 if no column was specified.
     */
    public int getColumn() {
        return this.column;
    }

    /**
     * Returns the line of parser exception.
     * 
     * @return the line of the parser exception or -1 if no line was specified.
     */
    public int getLine() {
        return this.line;
    }

}
