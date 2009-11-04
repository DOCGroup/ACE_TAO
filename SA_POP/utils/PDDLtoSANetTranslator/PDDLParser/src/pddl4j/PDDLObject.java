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
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.AndExp;
import pddl4j.exp.AtomicFormula;
import pddl4j.exp.DerivedPredicate;
import pddl4j.exp.Exp;
import pddl4j.exp.InitEl;
import pddl4j.exp.action.ActionDef;
import pddl4j.exp.fexp.FHead;
import pddl4j.exp.metric.MetricExp;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;
import pddl4j.exp.term.Constant;

/**
 * This class implements a PDDL object that represente a planning domain either
 * a planning problem.
 * 
 * @author Damien Pellier
 * @version 1.0
 */
public class PDDLObject implements Domain, Problem {

    /**
     * The serial version id of the class.
     */
    private static final long serialVersionUID = -8333968277204698622L;

    /**
     * This enumeration defines the content of the PDDL object.
     * 
     * @author Damien Pellier
     * @version 1.0
     */
    public enum Content {
        /**
         * The DOMAIN content.
         */
        DOMAIN,
        /**
         * The PARTIAL_PROBLEM content.
         */
        PARTIAL_PROBLEM,
        /**
         * The FULL_PROBLEM content.
         */
        FULL_PROBLEM;
    }

    /**
     * The source version of PDDL object.
     */
    protected Source source;

    /**
     * The domain name of this PDDL object.
     */
    protected String domainName;

    /**
     * The problem name of this PDDL object.
     */
    protected String problemName;

    /**
     * The require table of this PDDL object.
     */
    protected LinkedHashSet<RequireKey> requirements;

    /**
     * Returns an iterator over the requirements of this pddl object.
     * 
     * @return an iterator over the requirements of this pddl object.
     */
    public Iterator<RequireKey> requirementsIterator() {
        return this.requirements.iterator();
    }

    /**
     * The types table of this PDDL object.
     */
    protected Map<String, Set<String>> types;

    /**
     * The map used to store for each type its domain.
     */
    protected Map<Type, Set<Constant>> typeDomains;
    
    /**
     * Returns the set of constants associated to a specified type.
     * 
     * @param type the type.
     * @return the set of constants associated to a type or <code>null</code>
     *         if the type is not defined in the pddl object.
     */
    public Set<Constant> getTypedDomain(Type type) {
        Set<Constant> domain = this.typeDomains.get(type);
        if (domain == null) {
            domain = new LinkedHashSet<Constant>();
        }
        for (Type st : type.getSubTypes()) {
            Set<Constant> tmp = this.typeDomains.get(st);
            if (tmp != null) domain.addAll(tmp);
        }
        return domain;
    }
    
    /**
     * Returns the set of constants associated to a specified type.
     * 
     * @param typeSet the type.
     * @return the set of constants associated to a type or <code>null</code>
     *         if the type is not defined in the pddl object.
     */
    public Set<Constant> getTypedDomain(TypeSet typeSet) {
        Set<Constant> domain = new LinkedHashSet<Constant>();
        for (Type type : typeSet) {
            domain.addAll(this.getTypedDomain(type));
        }
        return domain;
    }
    
    

    /**
     * The constants table of this PDDL object.
     */
    protected Map<String, Constant> constants;

    /**
     * Returns an iterator over the constants defined in this pddl object.
     * 
     * @return an iterator over the constants defined in this pddl object.
     */
    public Iterator<Constant> constantsIterator() {
        return this.constants.values().iterator();
    }

    /**
     * The predicates table of this PDDL object.
     */
    protected Set<AtomicFormula> predicates;

    /**
     * Returns an iterator over the constants defined in this pddl object.
     * 
     * @return an iterator over the constants defined in this pddl object.
     */
    public Iterator<AtomicFormula> predicatesIterator() {
        return this.predicates.iterator();
    }

    /**
     * The functions table of this PDDL object.
     */
    protected Set<FHead> functions;
    
    /**
     * Returns an iterator over the functions defined in this pddl object.
     * 
     * @return an iterator over the functions defined in this pddl object.
     */
    public Iterator<FHead> functionsIterator() {
        return this.functions.iterator();
    }
    
    /**
     * The constraints table of this PDDL object.
     */
    protected Set<Exp> constraints;

    /**
     * The actions table of this PDDL object.
     */
    protected Map<String, ActionDef> actions;

    /**
     * Returns an iterator over the actions defined in this pddl object.
     * 
     * @return an iterator over the actions defined in this pddl object.
     */
    public Iterator<ActionDef> actionsIterator() {
        return this.actions.values().iterator();
    }

    /**
     * The axioms predicates table of this PDDL object.
     */
    protected Set<DerivedPredicate> axioms;

    /**
     * The initial state.
     */
    protected ArrayList<InitEl> init;

    /**
     * The goal.
     */
    protected Exp goal;

    /**
     * The content of the PDDL object.
     */
    protected Content content;

    /**
     * The metric specification.
     */
    protected MetricExp metric;

    /**
     * The domain file where the PDDL object is defined.
     */
    protected File domainFile;

    /**
     * The domain file where the PDDL object is defined.
     */
    protected File problemFile;

    
    /**
     * Creates a new PDDL object.
     */
    public PDDLObject() {
        this.content = Content.DOMAIN;
        this.domainFile = null;
        this.problemFile = null;
        this.source = Source.V3_0;
        this.domainName = new String();
        this.problemName = new String();
        this.requirements = new LinkedHashSet<RequireKey>();
        this.requirements.add(RequireKey.STRIPS);
        this.types = new LinkedHashMap<String, Set<String>>();
        this.types.put(Type.OBJECT_SYMBOL, new LinkedHashSet<String>());
        this.typeDomains = new LinkedHashMap<Type, Set<Constant>>();
        this.constants = new LinkedHashMap<String, Constant>();
        this.predicates = new LinkedHashSet<AtomicFormula>();
        this.functions = new LinkedHashSet<FHead>();
        this.constraints = new LinkedHashSet<Exp>();
        this.actions = new LinkedHashMap<String, ActionDef>();
        this.axioms = new LinkedHashSet<DerivedPredicate>();
        this.init = new ArrayList<InitEl>();
        this.goal = new AndExp();
        this.metric = null;
    }

    /**
     * Returns the metric of the PDDL object.
     * 
     * @return the metric of the PDDL object.
     */
    public MetricExp getMetric() {
        return this.metric;
    }

    /**
     * Returns the domain file where the PDDL object is defined.
     * 
     * @return the domain file where the PDDL object is defined.
     */
    public File getDomainFile() {
        return this.domainFile;
    }
    
    /**
     * Returns the problem file where the PDDL object is defined.
     * 
     * @return the problem file where the PDDL object is defined.
     */
    public File getProblemFile() {
        return this.problemFile;
    }
    
    /**
     * Returns the content of the PDDL object.
     * 
     * @return the content of the PDDL object.
     */
    public Content getContent() {
        return this.content;
    }

    /**
     * Returns the goal of the PDDL object.
     * 
     * @return the goal of the PDDL object.
     */
    public Exp getGoal() {
        return this.goal;
    }

    /**
     * Returns the initial state of the PDDL object.
     * 
     * @return the initial state of the PDDL object.
     */
    public List<InitEl> getInit() {
        return this.init;
    }

    /**
     * Returns the name of the domain.
     * 
     * @return the name of the domain.
     */
    public String getDomainName() {
        return this.domainName;
    }

    /**
     * Returns the name of the problem.
     * 
     * @return the name of the problem.
     */
    public String getProblemName() {
        return this.problemName;
    }

    /**
     * Returns <code>true</code> if this domain is equal to an other object,
     * i.e., if the other object is an instance of the class <code>Domain</code>
     * not null and has the image.
     * 
     * @param obj the object to compare.
     * @return <code>true</code> if this domain is equals to an other object;
     *         <code>false</code> otherwise.
     */
    public boolean equals(Object obj) {
        if (obj != null && obj.getClass().equals(this.getClass())) {
            PDDLObject other = (PDDLObject) obj;
            return this.domainName.equals(other.domainName)
                        && this.problemName.equals(other.problemName)
                        && this.content.equals(other.content);
        }
        return false;
    }

    /**
     * Returns the hash code value of this domain.
     * 
     * @return the hash code value of this domain.
     */
    public int hashCode() {
        return this.domainName.hashCode() + this.problemName.hashCode()
                    + this.content.hashCode();
    }

    /**
     * Returns a string representation of the PDDL object.
     * 
     * @return the string representation of the PDDL object.
     */
    public String toString() {
        String str = new String();
        switch (this.content) {
        case DOMAIN:
            str = toDomainString();
            break;
        case PARTIAL_PROBLEM:
            str = toPartialProblemString();
            break;
        case FULL_PROBLEM:
            str = toFullProblemString();
            break;
        }
        return str;
    }

    /**
     * Returns a string representation of a domain.
     * 
     * @return the string representation of a domain.
     */
    private String toDomainString() {
        StringBuffer str = new StringBuffer();
        str.append("(define (domain " + this.domainName + ")\n");
        str.append(this.requirementstoString());
        str.append(this.typestoString());
        str.append(this.constantsToString("constants"));
        str.append(this.predicatesToString());
        str.append(this.functionsToString());
        str.append(this.constraintsToString());
        str.append(this.axiomsToString());
        str.append(this.actionsToString());
        return str.toString();

    }

    /**
     * Returns a string representation of a partial problem.
     * 
     * @return the string representation of a partial problem.
     */
    private String toPartialProblemString() {
        StringBuffer str = new StringBuffer();
        str.append("(define (problem " + this.problemName + ")\n");
        str.append("(:domain " + this.domainName + ")\n");
        str.append(this.requirementstoString());
        str.append(this.constantsToString("objects"));
        str.append(this.constraintsToString());
        str.append(this.initToString());
        str.append(this.goalToString());
        str.append(this.metricToString());
        return str.toString();

    }

    /**
     * Returns a string representation of a full problem.
     * 
     * @return the string representation of a full problem.
     */
    private String toFullProblemString() {
        StringBuffer str = new StringBuffer();
        str.append("(:problem " + this.problemName + ")\n");
        str.append("(:domain " + this.domainName + ")\n");
        str.append(this.requirementstoString());
        str.append(this.typestoString());
        str.append(this.constantsToString("constants"));
        str.append(this.predicatesToString());
        str.append(this.functionsToString());
        str.append(this.constraintsToString());
        str.append(this.axiomsToString());
        str.append(this.actionsToString());
        str.append(this.initToString());
        str.append(this.goalToString() + "\n");
        str.append(this.metricToString());
        return str.toString();

    }

    /**
     * Returns a string representation of the requirements.
     * 
     * @return a string representation of the requirements.
     */
    private String requirementstoString() {
        StringBuffer str = new StringBuffer();
        if (!this.requirements.isEmpty()) {
            str.append("(:requirements");
            for (RequireKey rk : this.requirements) {
                str.append(" " + rk.toString());
            }
            str.append(")\n");
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the types.
     * 
     * @return a string representation of the types.
     */
    private String typestoString() {
        StringBuffer str = new StringBuffer();
        if (!this.types.isEmpty()) {
            str.append("(:types");
            for (String img : this.types.keySet()) {
                Type type = new Type(img, this.types);
                str.append("\n   " + type.toString());
                Set<Type> superTypes = type.getSuperTypes();
                if (!superTypes.isEmpty()) {
                    str.append(" -");
                    Iterator<Type> i = superTypes.iterator();
                    if (superTypes.size() > 1) {
                        str.append(" (either");
                        while (i.hasNext()) {
                            str.append(" " + i.next().toString());
                        }
                        str.append(")");
                    } else {
                        str.append(" " + i.next().toString());
                    }
                }
            }
            str.append(")\n");
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the constants.
     * 
     * @param label the label "constants" for domain and "objects for problem.
     * @return a string representation of the constants.
     */
    private String constantsToString(String label) {
        StringBuffer str = new StringBuffer();
        if (!this.constants.isEmpty()) {
            str.append("(:" + label);
            for (Constant cst : this.constants.values()) {
                if (this.requirements.contains(RequireKey.TYPING)) {
                    str.append("\n   " + cst.toTypedString());
                } else {
                    str.append(" " + cst.toString());
                }
            }
            str.append(")\n");
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the predicates.
     * 
     * @return a string representation of the predicates.
     */
    private String predicatesToString() {
        StringBuffer str = new StringBuffer();
        if (!this.predicates.isEmpty()) {
            str.append("(:predicates");
            for (AtomicFormula atom : this.predicates) {
                if (this.requirements.contains(RequireKey.TYPING)) {
                    str.append("\n   " + atom.toTypedString());
                } else {
                    str.append(" " + atom.toString());
                }
            }
            str.append(")\n");
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the functions.
     * 
     * @return a string representation of the functions.
     */
    private String functionsToString() {
        StringBuffer str = new StringBuffer();
        if (!this.functions.isEmpty()) {
            str.append("(:functions");
            for (FHead func : this.functions) {
                if (this.requirements.contains(RequireKey.TYPING)) {
                    str.append("\n   " + func.toTypedString());
                } else {
                    str.append("\n   " + func.toString());
                }
            }
            str.append(")\n");
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the constraints.
     * 
     * @return a string representation of the constraints.
     */
    private String constraintsToString() {
        StringBuffer str = new StringBuffer();
        if (!this.constraints.isEmpty()) {
            str.append("(:constraints");
            for (Exp exp : this.constraints) {
                if (this.requirements.contains(RequireKey.TYPING)) {
                    str.append("\n   " + exp.toTypedString());
                } else {
                    str.append("\n   " + exp.toString());
                }
            }
            str.append(")\n");
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the axioms.
     * 
     * @return a string representation of the axioms.
     */
    private String axiomsToString() {
        StringBuffer str = new StringBuffer();
        if (!this.axioms.isEmpty()) {
            for (DerivedPredicate axiom : this.axioms) {
                if (this.requirements.contains(RequireKey.TYPING)) {
                    str.append(axiom.toTypedString());
                } else {
                    str.append(axiom.toString());
                }
                str.append("\n");
            }
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the actions.
     * 
     * @return a string representation of the actions.
     */
    private String actionsToString() {
        StringBuffer str = new StringBuffer();
        if (!this.actions.isEmpty()) {
            for (ActionDef action : this.actions.values()) {
                str.append(action.toTypedString());
                str.append("\n");
            }
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the metric.
     * 
     * @return a string representation of the metric.
     */
    private String metricToString() {
        StringBuffer str = new StringBuffer();
        if (this.metric != null) {
            str.append(this.metric.toString());
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the init.
     * 
     * @return a string representation of the init.
     */
    private String initToString() {
        StringBuffer str = new StringBuffer();
        if (!this.init.isEmpty()) {
            str.append("(:init");
            for (InitEl el : this.init) {
                str.append("\n   ");
                if (this.requirements.contains(RequireKey.TYPING)) {
                    str.append(el.toTypedString());
                } else {
                    str.append(el.toString());
                }
            }
            str.append(")\n");
        }
        return str.toString();
    }

    /**
     * Returns a string representation of the goal.
     * 
     * @return a string representation of the goal.
     */
    private String goalToString() {
        StringBuffer str = new StringBuffer();
        if (this.goal != null) {
            str.append("(:goal ");
            str.append("\n   ");
            if (this.requirements.contains(RequireKey.TYPING)) {
                str.append(goal.toTypedString());
            } else {
                str.append(goal.toString());
            }
            str.append(")");
        }
        return str.toString();
    }

}
