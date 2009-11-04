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

package pddl4j.exp.metric;

import java.util.LinkedHashMap;
import java.util.Map;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;

/**
 * This class defines the minimize expression used in metric
 * specification of the PDDL language.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class MinimizeExp extends MetricExp {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = 3422169319365697377L;

    /**
     * Creates a new minimize expression with a specified function.
     * 
     * @param exp the term expression to minimize.
     * @throws NullPointerException if <code>exp == null</code>.
     */
    public MinimizeExp(Term exp) {
        super(Optimization.MINIMIZE, exp);
    }
       
    /**
     * Substitutes all occurrences of the variables that occur in this
     * expression and that are mapped in the substitution by its binding term.
     * 
     * @param sigma the substitution.
     * @return a substituted copy of this expression.
     * @throws NullPointerException if <code>sigma == null</code>.
     */
    public MinimizeExp apply(Substitution sigma) {
        return (MinimizeExp) super.apply(sigma);
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. Remember that free variables are existentially quantified.
     * 
     * @return a standardized copy of this expression.
     */
    public MinimizeExp standardize() {
        return this.standardize(new LinkedHashMap<String, String>());
    }
    
    /**
     * Standardizes all occurrences of the variables that occur in this
     * expression. The map argument is used to store the variable already
     * standardized. The bound argument is used to store the bound variables. a
     * standardized copy of this expression. Remember that free variables are
     * existentially quantified.
     * 
     * @param images the object that maps old variable images to the standardize
     *            image.
     * @return a standardized copy of this expression.
     * @throws NullPointerException if
     *             <code>images == null || bound == null</code>.
     */
    public MinimizeExp standardize(Map<String, String> images) {
        return (MinimizeExp) super.standardize(images);
    }
    
    /**
     * Creates and returns a copy of this expression.
     * 
     * @return a clone of this expression instance.
     * @see pddl4j.exp.Exp#clone()
     */
    public MinimizeExp clone() {
        return (MinimizeExp) super.clone();
    }
      
}
