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
import java.io.PrintStream;
import java.util.Collection;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.Set;
import java.util.SortedMap;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;

/**
 * This class implements the error manager used by the parser and java pddl
 * compiler.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class ErrorManager {
    
    /**
     * This enumeration defines the type of message log in the error manager.
     */
    public enum Message {
        /**
         * The parser error message.
         */
        PARSER_ERROR, 
        
        /**
         * The parser warning message.
         */ 
        PARSER_WARNING, 
        /**
         * The lexical error message.
         */
        LEXICAL_ERROR, 
        /**
         * The linker error message.
         */
        LINKER_ERROR,
        /**
         * The linker warning message.
         */
        LINKER_WARNING,
        /**
         * The all type message.
         */
        ALL,
        /**
         * The parser message.
         */
        PARSER,
        /**
         * The parser message.
         */
        LINKER,
        /**
         * The error message.
         */
        ERROR,
        /**
         * The warning message.
         */
        WARNING,
        
    }
    /**
     * The tree set used to store the messages.
     */
    private SortedSet<String> msg;

    /**
     * The tree set used to store the messages by type.
     */
    private TreeMap<Message, Set<String>> tMsg;
    
    /**
     * The tree set used to store the messages by file.
     */
    private SortedMap<File, Set<String>> fMsg;
    
    /**
     * The print stream of the error manager.
     */
    private PrintStream out;
    
    /**
     * Creates a new msg manager.
     */
    public ErrorManager() {
        this.msg = new TreeSet<String>();
        this.tMsg = new TreeMap<Message, Set<String>>();
        this.fMsg = new TreeMap<File, Set<String>>();
        this.out = System.out;
    }
    
    /**
     * Sets a new output stream to the error manager.
     * 
     * @param out the new output stream of the error manager.
     */
    private void setOutputStream(PrintStream out) {
        this.out = out;
    }
    
    /**
     * Print the messages log in the error manager.
     * 
     * @param type the type of message to print.
     */
    public void print(Message type) {
        this.print(this.getMessages(type));
    }
     
    /**
     * Print the messages log in the error manager that concerns a specific file.
     * 
     * @param file The file.
     */
    public void print(File file) {
        this.print(this.fMsg.get(file));
    }
    
    /**
     * Print the messages log in the error manager of a specific and concerning a specific file.
     * 
     * @param type the type of message to print.
     * @param file the file.
     */
    public void print(Message type, File file) {
        this.print(this.getMessages(type, file));
    }
    
    /**
     * Print the message to the current output stream.
     * 
     * @param msg the list of messages to print.
     */
    private void print(Collection<String> msg) {
        if (msg != null) {  
            for (String error : msg) {
                this.out.println(error);
            }
        }
    }
    
    /**
     * Returns the list of messages of a specific type.
     * 
     * @param type The type of messages.
     * @return The list of messages of a specific type.
     */
    public Set<String> getMessages(Message type) {
        Set<String> messages = new HashSet<String>();
        switch(type) {
        case PARSER: 
            messages.addAll(this.getMessages(Message.PARSER_ERROR));
            messages.addAll(this.getMessages(Message.PARSER_WARNING));
            break;
        case LINKER: 
            messages.addAll(this.getMessages(Message.LINKER_ERROR));
            messages.addAll(this.getMessages(Message.LINKER_WARNING));
            break; 
        case ERROR: 
            messages.addAll(this.getMessages(Message.LEXICAL_ERROR));
            messages.addAll(this.getMessages(Message.PARSER_ERROR));
            messages.addAll(this.getMessages(Message.LINKER_ERROR));
            break;
        case WARNING: 
            messages.addAll(this.getMessages(Message.PARSER_WARNING));
            messages.addAll(this.getMessages(Message.LINKER_WARNING));
            break;
        case ALL:
            messages = this.msg;
            break;
        default:
            messages = this.tMsg.get(type);
            if (messages == null) {
                messages = new HashSet<String>();
            }
        }
        return messages;
    }
    
    /**
     * Returns the list of messages that concerns a specific file.
     * 
     * @param file The file.
     * @return The list of messages.
     */
    public Set<String> getMessages(File file) {
        Set<String> msg = this.fMsg.get(file);
        return msg == null ? new HashSet<String>() : msg;
    }
    
    /**
     * Returns the list of messages of a specific type concerning a specific file.
     * 
     * @param type The type of messages.
     * @param file The file.
     * @return The list of messages of a specific type concerning a specific file.
     */
    public Set<String> getMessages(Message type, File file) {
       Set<String> s1 = new HashSet<String>(this.getMessages(type));
       Set<String> s2 = this.getMessages(file);
       s1.retainAll(s2);
       return s1;
    }
    
    /**
     * Init the msg manager.
     */
    public void clear() {
        this.msg.clear();
    }

    /**
     * Returns if the msg manager is empty, i.e., contains no message of any
     * type.
     * 
     * @return <code>true</code> if the msg manager is empty;
     *         <code>false</code> otherwise.
     */
    public boolean isEmpty() {
        return this.msg.isEmpty();
    }
    
    /**
     * Returns <code>true</code> if the error manager contains message of a
     * specific type.
     * 
     * @param type th type tested.
     * @return <code>true</code> if the error manager contains message of a
     *         specific type; <code>false</code> otherwise.
     */
    public boolean contains(Message type) {
        return !this.getMessages(type).isEmpty();
    }
   
    /**
     * Log an error message to the current print stream.
     * 
     * @param msg the error message.
     * @param file the file where the warning was encoutered.
     * @param line the line of the error.
     * @param column the column of the error.
     */
    public void logParserError(String msg, File file, int line, int column) {
        String error = "Parser error at line " + line + ", column " + column 
        + " file (" + file.getName() + ") : " + msg;
        this.msg.add(error);
        Set<String> msgl = this.tMsg.get(Message.PARSER_ERROR);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.tMsg.put(Message.PARSER_ERROR, msgl);
        }
        msgl.add(error);
        msgl = this.fMsg.get(file);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.fMsg.put(file, msgl);
        }
        msgl.add(error);
    }

    /**
     * Log an lexical error message.
     * 
     * @param msg the error message.
     * @param file the file where the error was encoutered.
     */
    public void logLexicalError(String msg, File file) {
        String error = msg + " in file " + file.getName();
        this.msg.add(error);
        Set<String> msgl = this.tMsg.get(Message.LEXICAL_ERROR);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.tMsg.put(Message.LEXICAL_ERROR, msgl);
        }
        msgl.add(error);
        msgl = this.fMsg.get(file);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.fMsg.put(file, msgl);
        }
        msgl.add(error);
    }
    
    /**
     * Log an lexical error message.
     * 
     * @param msg the error message.
     * @param file the file where the error was encoutered.
     * @param line the line of the error.
     * @param column the column of the error.
     */
    public void logLexicalError(String msg, File file, int line, int column) {
        String error = "Lexical error at line " + line + ", column " + column + ", file (" + file.getName() + ") : " + msg;
        this.msg.add(error);
        Set<String> msgl = this.tMsg.get(Message.LEXICAL_ERROR);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.tMsg.put(Message.LEXICAL_ERROR, msgl);
        }
        msgl.add(error);
        msgl = this.fMsg.get(file);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.fMsg.put(file, msgl);
        }
        msgl.add(error);
    }
    
    /**
     * Log a warning message to the current print stream.
     * 
     * @param msg the warning message.
     * @param file the file where the warning was encoutered.
     * @param line the line of the warning.
     * @param column the column of the warning.
     */
    public void logParserWarning(String msg, File file, int line, int column) {
        String warning = "Parser warning at line " + line + ", column " + column 
            + " file (" + file.getName() + ") : " + msg;
        this.msg.add(warning);
        Set<String> msgl = this.tMsg.get(Message.PARSER_WARNING);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.tMsg.put(Message.PARSER_WARNING, msgl);
        }
        msgl.add(warning);
        msgl = this.fMsg.get(file);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.fMsg.put(file, msgl);
        }
        msgl.add(warning);
    }

    /**
     * Log a warning message to the current print stream.
     * 
     * @param msg the warning message.
     * @param file the file where the warning was encoutered.
     */
    public void logLinkerWarning(String msg, File file) {
        String warning = "Linker warning (" + file.getName() + "): " + msg;
        this.msg.add(warning);
        Set<String> msgl = this.tMsg.get(Message.LINKER_WARNING);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.tMsg.put(Message.LINKER_WARNING, msgl);
        }
        msgl.add(warning);
        msgl = this.fMsg.get(file);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.fMsg.put(file, msgl);
        }
        msgl.add(warning);
    }
    
    /**
     * Log an error message to the current print stream.
     * 
     * @param msg the warning message.
     * @param file the file where the warning was encoutered.
     */
    public void logLinkerError(String msg, File file) {
        String error = "Linker error (" + file.getName() + "): " + msg;
        this.msg.add(error);
        Set<String> msgl = this.tMsg.get(Message.LINKER_ERROR);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.tMsg.put(Message.LINKER_ERROR, msgl);
        }
        msgl.add(error);
        msgl = this.fMsg.get(file);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.fMsg.put(file, msgl);
        }
        msgl.add(error);
    }
    
    /**
     * Log a warning message to the current print stream.
     * 
     * @param msg the warning message.
     * @param file the file where the warning was encoutered.
     */
    public void logParserWarning(String msg, File file) {
        String warning = "Parser warning (" + file.getName() + "): " + msg;
        this.msg.add(warning);
        Set<String> msgl = this.tMsg.get(Message.PARSER_WARNING);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.tMsg.put(Message.PARSER_WARNING, msgl);
        }
        msgl.add(warning);
        msgl = this.fMsg.get(file);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.fMsg.put(file, msgl);
        }
        msgl.add(warning);
    }
    
    /**
     * Log an error message to the current print stream.
     * 
     * @param msg the warning message.
     * @param file the file where the warning was encoutered.
     */
    public void logParserError(String msg, File file) {
        String error = "Compiler error (" + file.getName() + "): " + msg;
        this.msg.add(error);
        Set<String> msgl = this.tMsg.get(Message.PARSER_ERROR);
        
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.tMsg.put(Message.PARSER_ERROR, msgl);
        }
        msgl.add(error);
        msgl = this.fMsg.get(file);
        if (msgl == null) {
            msgl = new LinkedHashSet<String>();
            this.fMsg.put(file, msgl);
        }
        msgl.add(error);
    }
    
}
