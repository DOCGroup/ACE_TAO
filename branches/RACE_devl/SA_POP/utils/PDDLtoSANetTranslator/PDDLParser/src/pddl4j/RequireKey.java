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

/**
 * This enumeration defines le set of PDDL require key.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public enum RequireKey {
    /**
     * The PDDL :strips requirement.
     */
    STRIPS(":strips"),
    /**
     * The PDDL :typing requirement.
     */
    TYPING(":typing"),
    /**
     * The PDDL :negative-preconditions requirement.
     */
    NEGATIVE_PRECONDITIONS(":negative-preconditions"),
    /**
     * The PDDL :disjunctive-preconditions requirement.
     */
    DISJUNCTIVE_PRECONDITIONS(":disjunctive-preconditions"),
    /**
     * The PDDL :equality requirement.
     */
    EQUALITY(":equality"),
    /**
     * The PDDL :existential-preconditions requirement.
     */
    EXISTENTIAL_PRECONDITIONS(":existential-preconditions"),
    /**
     * The PDDL :universal-preconditions requirement.
     */
    UNIVERSAL_PRECONDITIONS(":universal-preconditions"),
    /**
     * The PDDL :quantified-preconditions requirement.
     */
    QUANTIFIED_PRECONDITIONS(":quantified-preconditions"),
    /**
     * The PDDL :conditional-effects requirement.
     */
    CONDITIONAL_EFFECTS(":conditional-effects"),
    /**
     * The PDDL :fluents requirement.
     */
    FLUENTS(":fluents"),
    /**
     * The PDDL :adl requirement.
     */
    ADL(":adl"),
    /**
     * The PDDL :durative-actions requirement.
     */
    DURATIVE_ACTIONS(":durative-actions"),
    /**
     * The PDDL :derived-predicates requirement.
     */
    DERIVED_PREDICATES(":derived-predicates"),
    /**
     * The PDDL :times-initial-literals requirement.
     */
    TIMED_INITIAL_LITERALS(":timed-initial-literals"),
    /**
     * The PDDL :preferences requirement.
     */
    PREFERENCES(":preferences"),
    /**
     * The PDDL :constraints requirement.
     */
    CONSTRAINTS(":constraints"),
    /**
     * The PDDL :continus-effects requirement.
     */
    CONTINUS_EFFECTS(":continous-effects"),
    /**
     * The PDDL :duration-inegalities requirement.
     */
    DURATION_INEQUALITIES(":duration-inequalities");

    /**
     * The image associate to the require key in the PDDL langage.
     */
    private String image;

    /**
     * Create a new require key with a specific image.
     * 
     * @param image the image of the require key. The image of the require key must be not null.
     */
    private RequireKey(String image) {
        this.image = image;
    }

    /**
     * Returns the require key with a specific image or <code>null</code> if there is no require key with the
     * specific image.
     * 
     * @param image the image of the require key searched.
     * @return the require key searched or <code>null</code> if it was not found in the enumeration.
     */
    public static RequireKey getRequireKey(String image) {
        for (RequireKey rk : RequireKey.values()) {
            if (rk.image.equalsIgnoreCase(image)) {
                return rk;
            }
        }
        return null;
     }
    
    /**
     * Returns the image of this require key.
     * 
     * @return the image of this require key.
     */
    public String getImage() {
        return this.image;
     }
    
    /**
     * Returns a string representation of this require key.
     * 
     * @return a string representation of this require key.
     */
    public String toString() {
        return this.image;
    }
    
}
