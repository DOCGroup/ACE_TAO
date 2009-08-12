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

import java.util.Map;

import pddl4j.exp.Exp;
import pddl4j.exp.type.TypeSet;

/**
 * This interface define all the method applicable to a PDDL term.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public interface Term extends Exp, Cloneable {
   
    /**
     * Return the image of this term.
     * 
     * @return the image of this term.
     */
    String getImage();
    
    /**
     * Returns the type of this term.
     * 
     * @return the type of this term.
     */
    TypeSet  getTypeSet();
    
    /**
     * Returns a typed representation of this term.
     * 
     * @return a typed representation of this term.
     */
    String toTypedString();
    
    /**
     * Unify this term with an other specified term. Note, call unify does not
     * modify the parameters of this method.
     * 
     * @param term the term to unify with.
     * @return The substitution that defines the binding constraints needed to
     *         unify this term with the other term or <code>null</code> if no
     *         binding constraints allows to unify the two terms.
     */
    Substitution unify(final Term term);
    
    /**
     * Unify this term with an other specified term by taking into account a
     * specified set of binding constraints. Note, call unify does not modify
     * the parameters of this method. 
     * 
     * @param term the term to unify with.
     * @param sigma the substitution containing the binding constraints.
     * @return The substitution that defines the binding constraints needed to
     *         unify this term with the other term or <code>null</code> if no
     *         binding constraints allows to unify the two terms.
     */
    Substitution unify(final Term term, final Substitution sigma);
    
    /**
     * Creates and returns a copy of this term expression.
     * 
     * @return a clone of this term expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    Term clone();
    
    /**
     * Returns the term id of this term.
     * 
     * @return the term id of this term.
     * @see pddl4j.exp.term.TermID
     */
    TermID getTermID();
    
    /**
     * Returns <code>true</code> if this term is numeric.
     * 
     * @return <code>true</code> if this term is numeric; <code>false</code>
     *         otherwise.
     */
    //boolean isNumeric();
    
    /**
     * Substitutes all occurrences of the variables that occur in this
     * term and that are bound in the substitution by its binding term.
     * This method modify the content of the term on which it is applied. 
     *  
     * @param sigma the substitution.
     * @return the substituted term.
     */
    public Term apply(Substitution sigma);
    
    /**
    * Returns <code>true</code> if a term occurs in this term.
    * 
    * @param term the term to be tested.
    * @return <code>true</code> if a term occurs in this expression;
    *         <code>false</code> otherwise.
    */
   boolean occurs(Term term);
   
   /**
    * Returns <code>true</code> if the term is ground.
    * 
    * @return <code>true</code> if the term is ground;
    *         <code>false</code>otherwise.
    */
   boolean isGround();
   
   /**
    * Standardizes all occurrences of the variables that occur in this
    * term.
    * 
    * @return this term.
    */
   Term standardize();
   
   /**
    * Standardizes all occurrences of the variables that occur in this
    * term. The map argument is used to store the variable already
    * standardized. The bound argument is used to store the bound variables. a
    * standardized copy of this expression. Remember that free variables are
    * existentially quantified.
    * 
    * @param images the object that maps old variable images to the standardize
    *            image.
    * @return a standardized copy of this term.
    */
   Term standardize(Map<String, String> images);
   
}
