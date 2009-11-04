/*
 * Copyright Dept. of Mathematics & Computer Science Univ. Paris-Descartes
 *
 * This software is a computer program whose purpose is to propose an
 * environment for multi-agent planning.
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

package pddl4j.tests;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.Properties;

import pddl4j.ErrorManager;
import pddl4j.PDDLObject;
import pddl4j.Parser;
import pddl4j.ParserException;
import pddl4j.RequireKey;
import pddl4j.Source;
import pddl4j.ErrorManager.Message;
import pddl4j.exp.action.ActionDef;

public class Test {

    public static void main(String[] args) {
        try {    
            // Creates an instance of the java pddl parser
            Parser parser = new Parser(getParserOptions());
            //  Gets the error manager of the pddl parser
            ErrorManager mgr = parser.getErrorManager();
            
            boolean success;
            PDDLObject domain = parser.parse(new File(args[0]));

            if (mgr.contains(Message.ERROR)) {
                mgr.print(Message.ALL);
            } // else we print the warning and start the planning process
            else {
                mgr.print(Message.WARNING);
                System.out.println("\nParsing domain \"" + domain.getDomainName()
                            + "\" done successfully ...");
                //System.out.println(domain);
                
                
                PDDLObject problem = parser.parse(new File(args[1]));
                if (mgr.contains(Message.ERROR)) {
                    mgr.print(Message.ALL);
                } // else we print the warning and start the planning process
                else {
                    mgr.print(Message.WARNING);
                    System.out.println("\nParsing problem \"" + problem.getProblemName()
                            + "\" done successfully ...");
                    //System.out.println(problem);
                
            
                    PDDLObject obj = parser.link(domain, problem);
            
                    if (mgr.contains(Message.LINKER_ERROR)) {
                        mgr.print(Message.LINKER_ERROR);
                        } // else we print the warning and start the planning process
                    else {
                        mgr.print(Message.LINKER_WARNING);
                        System.out.println("Linking \"" + domain.getDomainName() + "\" with \"" + problem.getProblemName()
                            + "\" done successfully ...");
                        System.out.println(obj);
                        
                        /*Iterator<ActionDef> i = obj.actionsIterator();
                        while (i.hasNext()) {
                            ActionDef action = i.next();
                            System.out.println(action.toTypedString());
                            System.out.println(action.normalize().toTypedString());
                            
                        }*/
                        
                        
                    }
                }
            }
            
            // If the parser produces errors we print it and stop
            
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } 
    }
    /**
     * Returns the default options of the compiler.
     * 
     * @return the default options.
     */
    private static Properties getParserOptions() {
        Properties options = new Properties();
        options.put("source", Source.V3_0);
        options.put(RequireKey.STRIPS, true);
        options.put(RequireKey.TYPING, true);
        options.put(RequireKey.EQUALITY, true);
        options.put(RequireKey.FLUENTS, false);
        options.put(RequireKey.NEGATIVE_PRECONDITIONS, true);
        options.put(RequireKey.DISJUNCTIVE_PRECONDITIONS, true);
        options.put(RequireKey.EXISTENTIAL_PRECONDITIONS, true);
        options.put(RequireKey.UNIVERSAL_PRECONDITIONS, true);
        options.put(RequireKey.QUANTIFIED_PRECONDITIONS, true);
        options.put(RequireKey.CONDITIONAL_EFFECTS, true);
        options.put(RequireKey.DURATIVE_ACTIONS, true);
        options.put(RequireKey.ADL, true);
        return options;
    }
}
