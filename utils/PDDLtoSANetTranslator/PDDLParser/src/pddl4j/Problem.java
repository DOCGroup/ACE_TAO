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
import java.io.Serializable;
import java.util.Iterator;
import java.util.List;

import pddl4j.exp.Exp;
import pddl4j.exp.InitEl;
import pddl4j.exp.metric.MetricExp;
import pddl4j.exp.term.Constant;

/**
 * This interface defines the accessible methods from a problem.
 * 
 * @author Damien Pellier
 * @version 1.0 
 */
public interface Problem extends Serializable {

    /**
     * Returns an iterator over the requirements of this pddl object.
     * 
     * @return an iterator over the requirements of this pddl object.
     */
    Iterator<RequireKey> requirementsIterator();

    /**
     * Returns an iterator over the constants defined in this pddl object.
     * 
     * @return an iterator over the constants defined in this pddl object.
     */
    Iterator<Constant> constantsIterator();

    /**
     * Returns the metric of the PDDL object.
     * 
     * @return the metric of the PDDL object.
     */
    MetricExp getMetric();

    /**
     * Returns the problem file where the PDDL object is defined.
     * 
     * @return the problem file where the PDDL object is defined.
     */
    File getProblemFile();

    /**
     * Returns the goal of the PDDL object.
     * 
     * @return the goal of the PDDL object.
     */
    Exp getGoal();

    /**
     * Returns the initial state of the PDDL object.
     * 
     * @return the initial state of the PDDL object.
     */
    List<InitEl> getInit();

    /**
     * Returns the name of the domain.
     * 
     * @return the name of the domain.
     */
    String getDomainName();

    /**
     * Returns the name of the problem.
     * 
     * @return the name of the problem.
     */
    String getProblemName();

}