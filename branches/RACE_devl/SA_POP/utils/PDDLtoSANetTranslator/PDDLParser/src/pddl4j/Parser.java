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
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;

import pddl4j.PDDLObject.Content;
import pddl4j.exp.AndExp;
import pddl4j.exp.AtomicFormula;
import pddl4j.exp.DerivedPredicate;
import pddl4j.exp.ExistsExp;
import pddl4j.exp.Exp;
import pddl4j.exp.ExpID;
import pddl4j.exp.ForallExp;
import pddl4j.exp.ImplyExp;
import pddl4j.exp.InitEl;
import pddl4j.exp.Literal;
import pddl4j.exp.NotAtomicFormula;
import pddl4j.exp.NotExp;
import pddl4j.exp.OrExp;
import pddl4j.exp.PrefExp;
import pddl4j.exp.ProbExp;
import pddl4j.exp.QuantifiedExp;
import pddl4j.exp.WhenExp;
import pddl4j.exp.action.Action;
import pddl4j.exp.action.ActionDef;
import pddl4j.exp.action.DurativeAction;
import pddl4j.exp.assign.Assign;
import pddl4j.exp.assign.AssignOpExp;
import pddl4j.exp.assign.Decrease;
import pddl4j.exp.assign.Increase;
import pddl4j.exp.assign.ScaleDown;
import pddl4j.exp.assign.ScaleUp;
import pddl4j.exp.cond.AlwaysExp;
import pddl4j.exp.cond.AlwaysWithinExp;
import pddl4j.exp.cond.AtMostOnceExp;
import pddl4j.exp.cond.HoldAfterExp;
import pddl4j.exp.cond.HoldDuringExp;
import pddl4j.exp.cond.SometimeAfterExp;
import pddl4j.exp.cond.SometimeBeforeExp;
import pddl4j.exp.cond.SometimeExp;
import pddl4j.exp.cond.WithinExp;
import pddl4j.exp.fcomp.Comp;
import pddl4j.exp.fcomp.EqualComp;
import pddl4j.exp.fcomp.FCompExp;
import pddl4j.exp.fcomp.GEqualComp;
import pddl4j.exp.fcomp.GreaterComp;
import pddl4j.exp.fcomp.LEqualComp;
import pddl4j.exp.fcomp.LessComp;
import pddl4j.exp.fexp.BinaryAdd;
import pddl4j.exp.fexp.BinaryDivide;
import pddl4j.exp.fexp.BinaryMultiply;
import pddl4j.exp.fexp.BinaryOp;
import pddl4j.exp.fexp.BinarySubstract;
import pddl4j.exp.fexp.FExp;
import pddl4j.exp.fexp.FHead;
import pddl4j.exp.fexp.NArityAdd;
import pddl4j.exp.fexp.NArityMultiply;
import pddl4j.exp.fexp.NArityOp;
import pddl4j.exp.fexp.Number;
import pddl4j.exp.metric.MaximizeExp;
import pddl4j.exp.metric.MetricExp;
import pddl4j.exp.metric.MinimizeExp;
import pddl4j.exp.term.Constant;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;
import pddl4j.exp.time.AtEndTimedExp;
import pddl4j.exp.time.AtStartTimedExp;
import pddl4j.exp.time.AtTimedExp;
import pddl4j.exp.time.OverAllTimedExp;
import pddl4j.exp.time.OverTimedExp;
import pddl4j.exp.time.TimedLiteral;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;
import pddl4j.lexer.Lexer;
import pddl4j.lexer.LexerTreeConstants;
import pddl4j.lexer.ParseException;
import pddl4j.lexer.SimpleNode;

/**
 * Implements the <tt>Parser</tt> of PDD4L library. The parser can be
 * configured to accept only specified requirements of PDDL langage. The list of
 * requirements accepted are as follow:
 * <ul>
 * <li><tt>:strips</tt> - Basic STRIPS-style.</li>
 * <li><tt>:typing</tt> - Allows type names in declaration of variables</li>
 * <li><tt>:negative-preconditions</tt> - Allows <tt>not</tt> in goal and
 * preconditions descriptions.</li>
 * <li><tt>:disjunctive-preconditions</tt> - Allows </tt>or</tt> in goal
 * and preconditions descriptions.</li>
 * <li><tt>:equality</tt> - Supports <tt>=</tt> as built-in predicate.</li>
 * <li><tt>:existential-preconditions</tt> - Allows <tt>exists</tt> in goal
 * and preconditions descriptions.</li>
 * <li><tt>:universal-preconditions</tt> - Allows <tt>forall</tt> in goal
 * and preconditions descriptions.</li>
 * <li><tt>:quantified-preconditions</tt> - Is equivalent to <tt>:existential-preconditions</tt> +
 * <tt>:universal-preconditions</tt>.</li>
 * <li><tt>:conditional-effects</tt> - Allows <tt>when</tt> clause in
 * actionCtx effects.</li>
 * <li><tt>:fluents</tt> - Allows function definitions and use of effects
 * using assignement operators and numeric preconditions.</li>
 * <li><tt>:adl</tt> - Is equivalent to <tt>:strips</tt> + <tt>:typing</tt> +
 * <tt>:negative-preconditions</tt> + <tt>:disjunctive-preconditions</tt> +
 * <tt>:equality</tt> + <tt>:quantified-preconditions</tt> + <tt>:conditional-effects</tt>.</li>
 * <li><tt>:durative-actions</tt> - Allows durative actions. Note that this
 * does note imply <tt>:fluents</tt>.</li>
 * <li><tt>:derived-predicate</tt> - Allows predicates whose truth value is
 * defined by a formula.</li>
 * <li><tt>:time-initial-literals</tt> - Allows the initial state to specify
 * literals that will become true at a specified time point implies <tt>durative-actions</tt>.</li>
 * <li><tt>:preferences</tt> - Allows use of preferences in actionCtx
 * preconditions and goals.</li>
 * <li><tt>:constraints</tt> - Allows use of constraints fields in domain and
 * problem description. These may contain modal operator supporting trajectory
 * constraints.</li>
 * </ul>
 * <p>
 * All the properties can be modified using the following methods (by default
 * the parser is ADL):
 * 
 * <pre>
 *  Properties options = new Properties();
 *  options.pu(RequireKey.TYPING, true);
 *  options.put(RequireKey.NEGATIVE_PRECONDITIONS, true);
 *  ...
 *  parser.setOptions(options);
 * </pre>
 * 
 * </p>
 * <p>
 * A simple example of how to use the ADL parser:
 * 
 * <pre>
 * public static void main(String[] args) {
 *     // Checks the command line
 *     if (args.length != 2) {
 *         System.err.println(&quot;Invalid command line&quot;);
 *     } else {
 *         // Creates an instance of the java pddl parser
 *         Parser parser = new Parser(options);
 *         Domain domain = parser.parse(new File(args[0]));
 *         Problem problem = parser.parse(new File(args[1]));
 *         PDDLObject obj = parser.link(domain, problem);
 *         // Gets the error manager of the pddl parser
 *         ErrorManager mgr = parser.getErrorManager();
 *         // If the parser produces errors we print it and stop
 *         if (mgr.contains(Message.ERROR)) {
 *             mgr.print(Message.ALL);
 *         } // else we print the warnings
 *         else {
 *             mgr.print(Message.WARNING);
 *             System.out.println(&quot;\nParsing domain \&quot;&quot; + domain.getDomainName()
 *                         + &quot;\&quot; done successfully ...&quot;);
 *             System.out.println(&quot;Parsing problem \&quot;&quot; + problem.getProblemName()
 *                         + &quot;\&quot; done successfully ...\n&quot;);
 *         }
 *     }
 * }
 * </pre>
 * 
 * @author Damien Pellier
 * @version 1.0, 14/01/03
 */
public final class Parser {
    
    /**
     * The file extention pddl.
     */
    public static final String PDDL_EXTENTION = ".pddl";
    
    /**
     * The error manager of the parser.
     */
    private ErrorManager mgr;

    /**
     * The parser input file.
     */
    private File file;
    
    /**
     * The PDDL object returns by the parser. 
     */
    private PDDLObject obj;

    /**
     * The requirement accepted by the parser.
     */
    private Properties options;
    
    /**
     * The actionCtx used to store the context.
     */
    private ActionDef actionCtx;
    
    /**
     * The quantified expression used to store the context.
     */
    private QuantifiedExp quantifiedExpCtx;
    
    /**
     * The flag use to specified the context.
     */
    private boolean constantDef = false;
    
    /**
     * Create a new <tt>Parser</tt>.
     */
    public Parser() {
        super();
        this.options = Parser.getDefaultOptions();
        this.mgr = new ErrorManager();   
    }

    /**
     * Create a new <tt>Parser</tt> with a specific errors manager.
     * 
     * @param options the options of the parser.
     */
    public Parser(Properties options) {
        super();
        this.options = options;
        this.mgr =  new ErrorManager();
    }
    
    /**
     * Create a new <tt>Parser</tt> with a specific errors manager.
     * 
     * @param mgr the error manager of the parser.
     */
    public Parser(ErrorManager mgr) {
        super();
        this.options = Parser.getDefaultOptions();
        this.mgr = mgr;
    }
    
    /**
     * Create a new <tt>Parser</tt> with a specific errors manager.
     * 
     * @param options the options of the parser.
     * @param mgr the error manager of the parser.
     */
    public Parser(Properties options, ErrorManager mgr) {
        super();
        this.options = options;
        this.mgr = mgr;
    }
    
    /**
     * Parses a PDDL file.
     * 
     * @param file the file to parse.
     * @return The PDDL object representing the PDDL file or null if the parsing fails.
     * @throws FileNotFoundException if the file to parse does not exist.
     * @throws NullPointerException if <code>file == null</code>.
     */
    public PDDLObject parse(File file) throws FileNotFoundException {
        if (file == null) 
            throw new NullPointerException("\"file\" parameter is null");
        PDDLObject obj = null;
        
        try {
            String ext = file.getName().substring(file.getName().lastIndexOf(".")); 
            if (!ext.equals(Parser.PDDL_EXTENTION)) {
                this.mgr.logParserWarning("File  \"" 
                        + file.getName() + "\": " + " File should have \".pddl\" extension.", file);
            }
        } catch (StringIndexOutOfBoundsException e) {
            this.mgr.logParserWarning("File  \"" 
                        + file.getName() + "\": " + " File should have \".pddl\" extension.", file);
        }
        SimpleNode root = null;
        this.file = file;
        Lexer lexer = new Lexer(new FileInputStream(file));
        lexer.setErrorManager(this.mgr);
        lexer.setFile(this.file);   
        try {
            root = lexer.parse();
            if (root != null) {
                obj = this.root(root);
            }
        } catch (Throwable e) {
            boolean print = false;
            Object debug = this.options.get("debug");
            if (debug instanceof String) {
                if (debug.equals("true")) {
                    print = true;
                } 
            } else if (debug instanceof Boolean) {
                if (debug.equals(true)) {
                    print = true;
                }
            }
            if (print) {
                e.printStackTrace(System.err);
            }
        }
        
        return obj;
    }
    
    /**
     * Prints the syntaxic tree of a specific pddl file.
     * 
     * @param file the pddl file.
     * @throws FileNotFoundException if the file to parse does not exist.
     * @throws NullPointerException if <code>file == null</code>.
     */
    public void printSyntaxicTree(File file) throws FileNotFoundException {
        if (file == null) 
            throw new NullPointerException("\"file\" parameter is null");
        SimpleNode n;
        try {
            this.file = file;
            Lexer lexer = new Lexer(new FileInputStream(file));
            lexer.setFile(file);
            lexer.setErrorManager(this.mgr);
            SimpleNode root = lexer.parse();
            root.dump("");
        } catch (ParseException pe) {
            boolean print = false;
            Object debug = this.options.get("debug");
            if (debug instanceof String) {
                if (debug.equals("true")) {
                    print = true;
                } 
            } else if (debug instanceof Boolean) {
                if (debug.equals(true)) {
                    print = true;
                }
            }
            if (print) {
                pe.printStackTrace(System.err);
            }
        }
    }
    
    /**
     * Returns the options of the parser.
     * 
     * @return the options of the parser.
     */
    public Properties getOptions() {
        return this.options;
    }
    
    /**
     * Sets the new options of the parser.
     * 
     * @param options the new options of the parser.
     */
    public void setOptions(Properties options) {
        this.options = options;
    }
    
    /**
     * Returns the error manager of the compiler.
     * 
     * @return the error manager of the compiler.
     */
    public ErrorManager getErrorManager() {
        return this.mgr;
    }
    
    /**
     * Sets a new error manager to the compiler.
     * 
     * @param mgr the new error manager of the compiler.
     */
    public void setErrorManager(ErrorManager mgr) {
        this.mgr = mgr;
    }
    
    /**
     * Returns the default options of the parser.
     * 
     * @return the default options.
     */
    public static Properties getDefaultOptions() {
        Properties options = new Properties();
        options.put("source", Source.V3_0);
        options.put("debug", true);
        options.put(RequireKey.STRIPS, true);
        options.put(RequireKey.TYPING, true);
        options.put(RequireKey.NEGATIVE_PRECONDITIONS, true);
        options.put(RequireKey.EQUALITY, true);
        options.put(RequireKey.QUANTIFIED_PRECONDITIONS, true);
        options.put(RequireKey.EXISTENTIAL_PRECONDITIONS, true);
        options.put(RequireKey.UNIVERSAL_PRECONDITIONS, true);
        options.put(RequireKey.CONDITIONAL_EFFECTS, true);
        options.put(RequireKey.ADL, true);
        return options;
    }
       
    /**
     * Creates a new PDDL
     * object containing also the domain informations. This method is usefull to
     * check the symbol shared between domain and problem input files. 
     * 
     * @param domain the domain. 
     * @param problem the problem.
     * @return a PDDL object containing all the information needed for planning.
     * @throws NullPointerException if <code>domain == null || problem == null</code>.
     */
    public PDDLObject link(Domain domain, Problem problem) {
        if (domain == null || problem == null) {
            throw new NullPointerException();
        }
        PDDLObject dom = (PDDLObject) domain;
        PDDLObject pb = (PDDLObject) problem;
        boolean failure = false;
        PDDLObject object = new PDDLObject();
        object.content = PDDLObject.Content.FULL_PROBLEM;
        object.domainFile = domain.getDomainFile();
        object.problemFile = problem.getProblemFile();
        object.actions.putAll(dom.actions);
        object.axioms.addAll(dom.axioms);
        object.constants.putAll(dom.constants);
        for (Constant cts : pb.constants.values()) {
            TypeSet newType = new TypeSet();
            for (Type ts : cts.getTypeSet()) {
                Set<String> pt = dom.types.get(ts.getImage());
                if (pt == null) {
                   this.mgr.logLinkerError("type \"" + ts.toString() + "\" of object \"" 
                               + cts + "\" is undefined.", problem.getProblemFile());
                   failure = true;
                } else {
                    newType.add(new Type(ts.getImage(), dom.types));
                }
            }
            cts.setTypeSet(newType);
            object.constants.put(cts.getImage(), cts);
        }
        
        for (Constant cst : object.constants.values()) {
            for (Type type : cst.getTypeSet()) {
                Set<Constant> setCst = object.typeDomains.get(type);
                if (setCst == null) {
                    setCst = new LinkedHashSet<Constant>();
                    object.typeDomains.put(type, setCst);
                }
                setCst.add(cst);
            }
        }
        object.constraints.addAll(dom.constraints);
        object.constraints.addAll(pb.constraints);
        object.domainName = domain.getDomainName();
        object.goal = pb.goal;
        object.init = pb.init;
        for (InitEl el : object.init) {    
            if (el.getExpID().equals(ExpID.ATOMIC_FORMULA)) {
                AtomicFormula atom = (AtomicFormula) el;
                boolean match = false;
                Iterator<AtomicFormula> i = dom.predicates.iterator();
                while (i.hasNext() && !match) {
                    AtomicFormula copy = i.next().clone().standardize();
                    Substitution sigma = copy.match(atom);
                    match = sigma != null;
                }
                if (!match) {
                    this.mgr.logLinkerError("predicate \""
                                + atom.toString()
                                + "\" undefined.", this.file);
                }
            } else if (el.getExpID().equals(ExpID.F_COMP)) {
                FCompExp fComp = (FCompExp) el;
                boolean match = false;
                Iterator<FHead> i = dom.functions.iterator();
                while (i.hasNext() && !match) {
                    FHead copy = i.next().clone().standardize();
                    Substitution sigma = copy.unify(fComp.getArg2());
                    match = sigma != null;
                }
                if (!match) {
                    this.mgr.logLinkerError("function \""
                                + fComp.getArg2().toString()
                                + "\" undefined.", this.file);
                }
            } else {
                this.mgr.logLinkerError("invalide expression in init block", 
                            problem.getProblemFile()); 
            }
            
        }
        
        object.metric = pb.metric;
        object.predicates.addAll(dom.predicates);
        object.problemName = problem.getProblemName();
        if (!pb.requirements.equals(dom.requirements)) {
            this.mgr.logLinkerWarning("domain and problem should have the same requirements.",
                        problem.getProblemFile());
        } 
        object.types.putAll(dom.types);
        object.requirements = dom.requirements;
        if (!dom.source.equals(pb.source)) {
            this.mgr.logLinkerError("Imcompatible source version\"" , problem.getProblemFile());
        } else {
            object.source = dom.source;
        }
        return failure ? null : object;
    }
    
    /**
     * Extracts the object structures from the <code>ROOT</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ROOT</code> node.
     * @return the domain extract.
     * @throws ParserException if an error occurs while parsing.
     */
    private PDDLObject root(SimpleNode node) throws ParserException {
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTDOMAIN:
                return this.domain(cn);
            case LexerTreeConstants.JJTPROBLEM:
                return this.problem(cn);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException(
                    "An internal parser error occurs: node "
                                + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from a <code>PROBLEM</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PROBLEM</code> node.
     * @return the problem structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private PDDLObject problem(SimpleNode node) throws ParserException {
        this.obj = new PDDLObject();
        this.obj.content = PDDLObject.Content.PARTIAL_PROBLEM;
        this.obj.problemFile = this.file;
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTPROBLEM_NAME:
                String name = this.file.getName();
                if (!cn.getImage().equals(name.substring(0, name.lastIndexOf(".")))) {
                    this.mgr.logParserWarning("problem \"" + cn.getImage() + "\" must be defined in a file \""
                                + cn.getImage() + ".pddl\".", this.file, cn.getLine(),
                                cn.getColumn());
                }
                this.obj.problemName = cn.getImage();
                break;
            case LexerTreeConstants.JJTDOMAIN_NAME:
                this.obj.domainName = cn.getImage();
                break;
            case LexerTreeConstants.JJTREQUIRE_DEF:
                this.requireDef(cn);
                break;
            case LexerTreeConstants.JJTOBJECT_DECLARATION:
                this.object_declaration(cn);
                break;
            case LexerTreeConstants.JJTINIT:
                this.init(cn);
                break;
            case LexerTreeConstants.JJTGOAL:
                this.goal(cn);
                break;
            case LexerTreeConstants.JJTCONSTRAINTS:
                this.constraints(cn);
                break;
            case LexerTreeConstants.JJTMETRIC_SPEC:
                this.metric_spec(cn);
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return obj;
    }
    
    /**
     * Extracts the object structures from the <code>METRIC_SPEC</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>METRIC_SPEC</code> node.
     * @return the metric expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private MetricExp metric_spec(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch(cn.getId()) {
             case LexerTreeConstants.JJTMAXIMIZE:
                 obj.metric = this.maximize(cn);
                 return obj.metric;
             case LexerTreeConstants.JJTMINIMIZE:
                 obj.metric = this.minimize(cn);
                 return obj.metric;
             default:
                 throw new ParserException("An internal parser error occurs: node "
                                 + cn.getLabel() + " unexpected."); 
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>MAXIMIZE</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>MAXIMIZE</code> node.
     * @return the maximize expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private MetricExp maximize(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTMETRIC_F_EXP) {
                return new MaximizeExp(this.metric_f_exp(cn));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>MINIMIZE</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>MINIMIZE</code> node.
     * @return the minimize expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private MetricExp minimize(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTMETRIC_F_EXP) {
                return new MinimizeExp(this.metric_f_exp(cn));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>METRIC_F_EXP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>METRIC_F_EXP</code> node.
     * @return the metric function expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Term metric_f_exp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch(cn.getId()) {
             case LexerTreeConstants.JJTUNARY_OP_METRIC_F_EXP:
                 return this.unary_op_metric_f_exp(cn);
             case LexerTreeConstants.JJTBINARY_OP_METRIC_F_EXP:
                 return this.binary_op_metric_f_exp(cn);
             case LexerTreeConstants.JJTMULTI_OP_METRIC_F_EXP:
                 return this.multi_op_metric_f_exp(cn);
             case LexerTreeConstants.JJTNUMBER:
                 return this.number(cn);
             case LexerTreeConstants.JJTCONSTANT_F_HEAD:
                 return this.constant_f_head(cn);
             case LexerTreeConstants.JJTTOTAL_TIME:
                 return new Variable(Variable.TOTAL_TIME, Type.NUMBER);
             case LexerTreeConstants.JJTVIOLATED_PREF_EXP:
                 return this.violated_pref_exp(cn);
                 
             default:
                 throw new ParserException("An internal parser error occurs: node "
                                 + cn.getLabel() + " unexpected."); 
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>VIOLATED_PREF_EXP</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>VIOLATED_PREF_EXP</code> node.
     * @return the violated preference expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Variable violated_pref_exp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTPREF_NAME) {
                return new Variable(Variable.VIOLATED_PREF_VARIABLE + this.pref_name(cn),
                            Type.NUMBER);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>CONSTANT_F_HEAD</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>CONSTANT_F_HEADF_HEAD</code> node.
     * @return the function head structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private FHead constant_f_head(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() > 0) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            if (cn1.getId() == LexerTreeConstants.JJTFUNCTOR) {
                FHead f_head = new FHead(this.functor(cn1), Type.NUMBER);
                for (int i = 1; i < node.jjtGetNumChildren(); i++) {
                    SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
                    switch (cn.getId()) {
                    case LexerTreeConstants.JJTCONSTANT:
                        f_head.add(this.constant(cn));
                        break;
                    default:
                        throw new ParserException("An internal parser error occurs: node "
                                    + cn.getLabel()
                                    + " unexpected.");
                    }
                }
                return f_head;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>MULTI_OP_METRIC_F_EXP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>MULTI_OP_METRIC_F_EXP</code> node.
     * @return the binray numeric operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private NArityOp multi_op_metric_f_exp(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTADD_NARITY_OP_METRIC_F_EXP:
                return this.add_narity_op_metric_f_exp(cn);
            case LexerTreeConstants.JJTMULTIPLY_NARITY_OP_METRIC_F_EXP:
                return this.multiply_narity_op_metric_f_exp(cn);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>ADD_NARITY_OP_METRIC_F_EXP</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ADD_NARITY_OP_METRIC_F_EXP</code> node.
     * @return the multi divide function expression build
     * @throws ParserException if an error occurs while parsing.
     */
    private NArityAdd add_narity_op_metric_f_exp(SimpleNode node) throws ParserException {
        NArityAdd exp = new NArityAdd();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTMETRIC_F_EXP:
                exp.add(this.metric_f_exp(cn));
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }
    
    /**
     * Extracts the object structures from the <code>MULTIPLY_NARITY_OP_METRIC_F_EXP</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>MULTIPLY_NARITY_OP_METRIC_F_EXP</code> node.
     * @return the multi divide function expression build
     * @throws ParserException if an error occurs while parsing.
     */
    private NArityMultiply multiply_narity_op_metric_f_exp(SimpleNode node) throws ParserException {
        NArityMultiply exp = new NArityMultiply();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTMETRIC_F_EXP:
                exp.add(this.metric_f_exp(cn));
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }
    
    /**
     * Extracts the object structures from the <code>BINARY_OP_METRIC_F_EXP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>BINARY_OP_METRIC_F_EXP</code> node.
     * @return the binray numeric operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryOp binary_op_metric_f_exp(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTADD_OP_METRIC_F_EXP:
                return this.add_op_metric_f_exp(cn);
            case LexerTreeConstants.JJTSUBSTRACT_OP_METRIC_F_EXP:
                return this.substract_op_metric_f_exp(cn);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>ADD_OP_METRIC_F_EXP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ADD_OP_METRIC_F_EXP</code> node.
     * @return the add operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryAdd add_op_metric_f_exp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTMETRIC_F_EXP
                        && cn2.getId() == LexerTreeConstants.JJTMETRIC_F_EXP) {
                return new BinaryAdd(this.metric_f_exp(cn1), this.metric_f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>SUBSTRACT_OP_METRIC_F_EXP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>SUBSTRACT_OP_METRIC_F_EXP</code> node.
     * @return the substract operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinarySubstract substract_op_metric_f_exp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTMETRIC_F_EXP
                        && cn2.getId() == LexerTreeConstants.JJTMETRIC_F_EXP) {
                return new BinarySubstract(this.metric_f_exp(cn1), this.metric_f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    
    /**
     * Extracts the object structures from the <code>UNARY_OP_METRIC_F_EXP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>UNARY_OP_METRIC_F_EXP</code> node.
     * @return the unary op metric function expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private FExp unary_op_metric_f_exp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTMETRIC_F_EXP) {
                return new BinarySubstract(new Number(), this.metric_f_exp(cn));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>INIT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>INIT</code> node.
     * @return the initial state structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp goal(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTPRE_GD) {
                obj.goal = this.pre_gd(cn);
                return obj.goal;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>INIT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>INIT</code> node.
     * @return the initial state structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ArrayList<InitEl> init(SimpleNode node) throws ParserException {
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTINIT_EL:
                obj.init.add(this.init_el(cn));
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return obj.init;
    }
    
    /**
     * Extracts the object structures from the <code>INIT_EL</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>INIT_EL</code> node.
     * @return initial state element.
     * @throws ParserException if an error occurs while parsing.
     */
    private InitEl init_el(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTLITERAL:
                return this.ground_literal(cn);
            case LexerTreeConstants.JJTEQUAL_INIT_EL:
                return this.equal_init_el(cn);
            case LexerTreeConstants.JJTTIMED_LITERAL:
                return this.timed_literal(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    
    /**
     * Extracts the object structures from the <code>INIT_EL</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>INIT_EL</code> node.
     * @return initial state element.
     * @throws ParserException if an error occurs while parsing.
     */
    private TimedLiteral timed_literal(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTNUMBER
                        && cn2.getId() == LexerTreeConstants.JJTLITERAL) {
                Number number = this.number(cn1);
                if (number.getValue() < 0) {
                    this.mgr.logParserError("time stamp of timed literal must be positive", 
                                file, cn1.getLine(), cn1.getColumn());
                }
                return new TimedLiteral(number, this.ground_literal(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>INIT_EL</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>INIT_EL</code> node.
     * @return initial state element.
     * @throws ParserException if an error occurs while parsing.
     */
    private EqualComp equal_init_el(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                        && cn2.getId() == LexerTreeConstants.JJTNUMBER) {
                return new EqualComp(this.f_head(cn1), this.number(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>OBJECT_DECLARATION</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>OBJECT_DECLARATION</code> node.
     * @return the object declartion structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Map<String, Constant> object_declaration(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() ==  1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTTYPED_LIST) {
                this.constantDef = true;
                this.constant_typed_list(cn, obj.constants);
                this.constantDef = false;
                return obj.constants;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
    }
    
    
    /**
     * Extracts the object structures from a <code>DOMAIN</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DOMAIN</code> node.
     * @return the domain structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private PDDLObject domain(SimpleNode node) throws ParserException {
        this.obj = new PDDLObject();
        this.obj.content = PDDLObject.Content.DOMAIN;
        this.obj.domainFile = this.file;
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);

            switch (cn.getId()) {
            case LexerTreeConstants.JJTDOMAIN_NAME:
                obj.domainName = cn.getImage();
                String name = file.getName();
                if (!cn.getImage().equals(name.substring(0, name.lastIndexOf(".")))) {
                    this.mgr.logParserWarning("Domain \"" + cn.getImage() + "\" should be defined in a file \""
                                + cn.getImage() + ".pddl\".", this.file, cn.getLine(),
                                cn.getColumn());
                }
                break;
            case LexerTreeConstants.JJTREQUIRE_DEF:
                this.requireDef(cn);
                break;
            case LexerTreeConstants.JJTTYPE_DEF:
                this.types_def(cn);
                break;
            case LexerTreeConstants.JJTCONSTANT_DEF:
                this.constants_def(cn);
                break;
            case LexerTreeConstants.JJTPREDICATE_DEF:
                this.predicates_def(cn);
                break;
            case LexerTreeConstants.JJTFUNCTION_DEF:
                this.functions_def(cn);
                break;
            case LexerTreeConstants.JJTCONSTRAINTS:
                this.constraints(cn);
                break;
            case LexerTreeConstants.JJTSTRUCTURE_DEF:
                this.structure_def(cn);
                break;
            default:
                
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return obj;
    }
    
    /**
     * Extracts the object structures from the <code>STRUCTURE_DEF</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>STRUCTURE_DEF</code> node.
     * @throws ParserException if an error occurs while parsing.
     */
    private void structure_def(SimpleNode node) throws ParserException {
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTACTION_DEF:
                this.action_def(cn);
                break;
            case LexerTreeConstants.JJTDURATION_ACTION_DEF:
                if (!this.obj.requirements.contains(RequireKey.DURATIVE_ACTIONS)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.DURATIVE_ACTIONS
                                + "\" needed to specify durative actions.",
                                this.file, node.getLine(), node.getColumn());
                }
                this.durative_action_def(cn);
                break;
            case LexerTreeConstants.JJTDERIVED_DEF:
                this.derived_def(cn);
                break;
            default:
                this.actionCtx = null;
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
    }

    /**
     * Extracts the object structures from the <code>DURATIVE_ACTION</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DURATIVE_ACTION</code> node.
     * @return the durative actionCtx structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private DurativeAction durative_action_def(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 3) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            SimpleNode cn3 = (SimpleNode) node.jjtGetChild(2);
            if (cn1.getId() == LexerTreeConstants.JJTDA_SYMBOL
                && cn2.getId() == LexerTreeConstants.JJTTYPED_LIST
                && cn3.getId() == LexerTreeConstants.JJTDA_DEF_BODY) {
                DurativeAction action = new DurativeAction(this.action_name(cn1));
                LinkedHashMap<String, Variable> parameters = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn2, parameters);
                for (Variable param : parameters.values()) {
                    action.add(param);
                }
                this.da_def_body(cn3, action);
                if (this.obj.actions.containsKey(action.getName())) {
                    this.mgr.logParserError("Duplicated action \"" + action.getName() 
                                + " \"." , this.file, cn1.getLine(), cn1.getColumn());
                } else {
                    this.obj.actions.put(action.getName(), action);
                }
                return action;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>DA_DEF_BODY</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_DEF_BODY</code> node.
     * @param action the duratiive actionCtx to initialize.
     * @throws ParserException if an error occurs while parsing.
     */
    private void da_def_body(SimpleNode node, DurativeAction action) throws ParserException {
        this.actionCtx = action;
        if (node.jjtGetNumChildren() == 3) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            SimpleNode cn3 = (SimpleNode) node.jjtGetChild(2);
            if (cn1.getId() == LexerTreeConstants.JJTDURATION_CONSTRAINT
                && cn2.getId() == LexerTreeConstants.JJTDA_GD
                && cn3.getId() == LexerTreeConstants.JJTDA_EFFECT) {
                action.setConstraint(this.duration_constraint(cn1));
                action.setCondition(this.da_gd(cn2));
                action.setEffect(this.da_effect(cn3));
            } else {
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
        this.actionCtx = null;
    }
    
    /**
     * Extracts the object structures from the <code>DA_EFFECT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_EFFECT</code> node.
     * @return the durative actionCtx effect  expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp da_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTAND_DA_EFFECT:
                return this.and_da_effect(cn);
            case LexerTreeConstants.JJTTIMED_EFFECT:
                return this.timed_effect(cn);
            case LexerTreeConstants.JJTFORALL_DA_EFFECT:
                if (!this.obj.requirements.contains(RequireKey.CONDITIONAL_EFFECTS)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.CONDITIONAL_EFFECTS
                                + "\" needed to specify conditional durative effect.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.forall_da_effect(cn);
            case LexerTreeConstants.JJTWHEN_DA_EFFECT:
                if (!this.obj.requirements.contains(RequireKey.CONDITIONAL_EFFECTS)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.CONDITIONAL_EFFECTS
                                + "\" needed to specify conditional durative effect.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.when_da_effect(cn);
            case LexerTreeConstants.JJTDA_ASSIGN_OP:
                if (!this.obj.requirements.contains(RequireKey.FLUENTS)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.FLUENTS
                                + "\" needed to specify assign operation in durative actionCtx effect.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.da_assign_op(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>F_ASSIGN_DA</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_END_F_ASSIGN_DA_EFFECT</code> node.
     * @return the durative assign function expression.
     * @throws ParserException if an error occurs while parsing.
     */
    private AssignOpExp da_assign_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTASSIGN:
                return this.da_assign(cn);
            case LexerTreeConstants.JJTSCALE_UP:
                return this.da_scale_up(cn);
            case LexerTreeConstants.JJTSCALE_DOWN:
                return this.da_scale_down(cn);
            case LexerTreeConstants.JJTINCREASE:
                return this.da_increase(cn);
            case LexerTreeConstants.JJTDECREASE:
                return this.da_decrease(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>WHEN_DA_EFFECT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>WHEN_DA_EFFECT</code> node.
     * @return the when duratve actionCtx effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private WhenExp when_da_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTDA_GD
                        && cn2.getId() == LexerTreeConstants.JJTTIMED_EFFECT) {
                Exp cond = this.da_gd(cn1);
                Exp effect = this.timed_effect(cn2);
                return new WhenExp(cond, effect);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>FORALL_DA_EFFECT</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FORALL_DA_EFFECT</code> node.
     * @return the universal durative effect expression structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ForallExp forall_da_effect(SimpleNode node) throws ParserException {
        
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTTYPED_LIST
                        && cn2.getId() == LexerTreeConstants.JJTDA_EFFECT) {
                ActionDef tmp = this.actionCtx;
                this.actionCtx = null;
                ForallExp forall = new ForallExp();
                LinkedHashMap<String, Variable> vars = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn1, vars);
                this.actionCtx = tmp;
                for (Variable var : vars.values()) {
                    forall.add(var);
                }
                this.quantifiedExpCtx = forall;
                Exp exp = this.da_effect(cn2);
                forall.setExp(exp);
                this.quantifiedExpCtx = null;
                return forall;
            }
        }
        this.quantifiedExpCtx = null;
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>TIMED_EFFECT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TIMED_EFFECT</code> node.
     * @return the timed effect expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp timed_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTAT_DA_EFFECT:
                return this.at_da_effect(cn);
            case LexerTreeConstants.JJTAT_F_ASSIGN_DA_EFFECT:
                return this.at_f_assign_da_effect(cn);
            case LexerTreeConstants.JJTASSIGN_OP_T:
                if (!this.obj.requirements.contains(RequireKey.CONDITIONAL_EFFECTS)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.CONDITIONAL_EFFECTS
                                + "\" needed to specify conditional durative effect.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.assign_op_t(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>ASSIGN_OP_T</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ASSIGN_OP_T</code> node.
     * @return the durative assign operation expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AssignOpExp assign_op_t(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTINCREASE:
                return this.increase_assign_op_t(cn);
            case LexerTreeConstants.JJTDECREASE:
                return this.decrease_assign_op_t(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>DECREASE_ASSIGN_OP_T</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DECREASE_ASSIGN_OP_T</code> node.
     * @return the decrease durative expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Decrease decrease_assign_op_t(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_T) {
                return new Decrease(this.f_head(cn1), this.f_exp_t(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>INCREASE_ASSIGN_OP_T</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>INCREASE_ASSIGN_OP_T</code> node.
     * @return the increase durative expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Increase increase_assign_op_t(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_T) {
                return new Increase(this.f_head(cn1), this.f_exp_t(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>F_EXP_T</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>F_EXP_T</code> node.
     * @return the durative fonction expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Term f_exp_t(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTCONTINOUS_VARIABLE) {
                 return new Variable(Variable.CONTINOUS_VARIABLE, Type.NUMBER);
            }
        } else if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            Variable cv = new Variable(Variable.CONTINOUS_VARIABLE, Type.NUMBER);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTCONTINOUS_VARIABLE) {
                return new BinaryMultiply(this.f_exp(cn1), cv);
            } else if (cn1.getId() == LexerTreeConstants.JJTCONTINOUS_VARIABLE
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new BinaryMultiply(cv, this.f_exp(cn1));
                
            }
        } 
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AT_F_ASSIGN_DA_EFFECT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_F_ASSIGN_DA_EFFECT</code> node.
     * @return the at durative actionCtx effect expression that contains a fonction assign.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtTimedExp at_f_assign_da_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTAT_END_F_ASSIGN_DA_EFFECT:
                return this.at_end_f_assign_da_effect(cn);
            case LexerTreeConstants.JJTAT_START_F_ASSIGN_DA_EFFECT:
                return this.at_start_f_assign_da_effect(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>AT_START_F_ASSIGN_DA_EFFECT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_START_F_ASSIGN_DA_EFFECT</code> node.
     * @return the at start durative actionCtx effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtStartTimedExp at_start_f_assign_da_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTF_ASSIGN_DA) {
                Exp exp = this.f_assign_da(cn);
                return new AtStartTimedExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AT_END_F_ASSIGN_DA_EFFECT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_END_F_ASSIGN_DA_EFFECT</code> node.
     * @return the at start durative actionCtx effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtEndTimedExp at_end_f_assign_da_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTF_ASSIGN_DA) {
                Exp exp = this.f_assign_da(cn);
                return new AtEndTimedExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>F_ASSIGN_DA</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_END_F_ASSIGN_DA_EFFECT</code> node.
     * @return the durative assign function expression.
     * @throws ParserException if an error occurs while parsing.
     */
    private AssignOpExp f_assign_da(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTASSIGN:
                return this.da_assign(cn);
            case LexerTreeConstants.JJTSCALE_UP:
                return this.da_scale_up(cn);
            case LexerTreeConstants.JJTSCALE_DOWN:
                return this.da_scale_down(cn);
            case LexerTreeConstants.JJTINCREASE:
                return this.da_increase(cn);
            case LexerTreeConstants.JJTDECREASE:
                return this.da_decrease(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    
    /**
     * Extracts the object structures from the <code>DA_SCALE_UP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_SCALE_UP</code> node.
     * @return the durative actionCtx assign scale up expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ScaleUp da_scale_up(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new ScaleUp(this.f_head(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>DA_DECREASE</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_DECREASE</code> node.
     * @return the durative actionCtx descrease expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Decrease da_decrease(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new Decrease(this.f_head(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>DA_INCREASE</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_INCREASE</code> node.
     * @return the durative actionCtx increase expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Increase da_increase(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new Increase(this.f_head(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>DA_SCALE_DOWN</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_SCALE_DOWN</code> node.
     * @return the surative actionCtx assign scale up expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ScaleDown da_scale_down(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new ScaleDown(this.f_head(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>DA_ASSIGN</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_ASSIGN</code> node.
     * @return the durative assign effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Assign da_assign(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new Assign(this.f_head(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>F_EXP_DA</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>F_EXP_DA</code> node.
     * @return the durative fonction expression structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Term f_exp_da(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTF_EXP:
                return this.f_exp(cn);
            case LexerTreeConstants.JJTDA_BINARY_OP:
                return this.da_binary_op(cn);
            case LexerTreeConstants.JJTDA_UNARY_OP:
                return this.da_unary_op(cn);
            case LexerTreeConstants.JJTVAR_DURATION:
                if (!this.obj.requirements.contains(RequireKey.DURATION_INEQUALITIES)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.DURATION_INEQUALITIES
                                + "\" needed to specify durative inequalities in actionCtx effect.",
                                this.file, node.getLine(), node.getColumn());
                }
                return new Variable(Variable.DURATIVE_VARIABLE, Type.NUMBER);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>DA_UNARY_OP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_UNARY_OP</code> node.
     * @return the durative unary operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryOp da_unary_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new BinarySubstract(new Number(0.0), this.f_exp_da(cn));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>DA_BINARY_OP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_BINARY_OP</code> node.
     * @return the surative binary numeric operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryOp da_binary_op(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTADD_OP:
                return this.da_add_op(cn);
            case LexerTreeConstants.JJTSUBSTRACT_OP:
                return this.da_substract_op(cn);
            case LexerTreeConstants.JJTMULTIPLY_OP:
                return this.multiply_op(cn);
            case LexerTreeConstants.JJTDIVIDE_OP:
                return this.divide_op(cn);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>DA_ADD_OPERATION</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_ADD_OPERATION</code> node.
     * @return the add operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryAdd da_add_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP_DA
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new BinaryAdd(this.f_exp_da(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>DA_SUBSTRACT_OP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_SUBSTRACT_OP</code> node.
     * @return the substract operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinarySubstract da_substract_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP_DA
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new BinarySubstract(this.f_exp_da(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>DA_MULTIPLY_OP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_MULTIPLY_OP</code> node.
     * @return the durative multiply operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryMultiply da_multiply_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP_DA
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new BinaryMultiply(this.f_exp_da(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>DA_DIVIDE_OP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_DIVIDE_OP</code> node.
     * @return the durative divide operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryDivide da_divide_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP_DA
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP_DA) {
                return new BinaryDivide(this.f_exp_da(cn1), this.f_exp_da(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AT_DA_EFFECT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_DA_EFFECT</code> node.
     * @return the at durative actionCtx effect expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private AtTimedExp at_da_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTAT_END_DA_EFFECT:
                return this.at_end_da_effect(cn);
            case LexerTreeConstants.JJTAT_START_DA_EFFECT:
                return this.at_start_da_effect(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>AT_END_DA_EFFECT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_END_DA_EFFECT</code> node.
     * @return the at start durative actionCtx effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtEndTimedExp at_end_da_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTLITERAL) {
                Exp exp = this.literal(cn);
                return new AtEndTimedExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AT_START_DA_EFFECT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_START_DA_EFFECT</code> node.
     * @return the at start durative actionCtx effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtStartTimedExp at_start_da_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTLITERAL) {
                Exp exp = this.literal(cn);
                return new AtStartTimedExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AND_DA_EFFECT</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AND_DA_EFFECT</code> node.
     * @return the conjunctive durative actionCtx effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AndExp and_da_effect(SimpleNode node) throws ParserException {
        AndExp exp = new AndExp();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTDA_EFFECT:
                exp.add(this.da_effect(cn));
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }
    
    /**
     * Extracts the object structures from the <code>DA_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DA_GD</code> node.
     * @return the durative actionCtx goal description expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp da_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTPREF_TIMED_GD:
                return this.pref_timed_gd(cn);
            case LexerTreeConstants.JJTAND_DA_GD:
                return this.and_da_gd(cn);
            case LexerTreeConstants.JJTFORALL_DA_GD:
                if (!this.obj.requirements.contains(RequireKey.UNIVERSAL_PRECONDITIONS)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.PREFERENCES
                                + "\" needed to specify preferences.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.forall_da_gd(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>FORALL_DA_GD</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FORALL_DA_GD</code> node.
     * @return the universal duartive actionCtx goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ForallExp forall_da_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTTYPED_LIST
                        && cn2.getId() == LexerTreeConstants.JJTDA_GD) {
                ActionDef tmp = this.actionCtx;
                this.actionCtx = null;
                ForallExp forall = new ForallExp();
                LinkedHashMap<String, Variable> vars = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn1, vars);
                this.actionCtx = tmp;
                for (Variable var : vars.values()) {
                    forall.add(var);
                }
                this.quantifiedExpCtx = forall;
                Exp exp = this.da_gd(cn2);
                forall.setExp(exp);
                this.quantifiedExpCtx = null;
                return forall;
            }
        }
        this.quantifiedExpCtx = null;
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AND_DA_GD</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AND_DA_GD</code> node.
     * @return the conjunctive durative actionCtx goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AndExp and_da_gd(SimpleNode node) throws ParserException {
        AndExp exp = new AndExp();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTDA_GD:
                exp.add(this.da_gd(cn));
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }
    
    /**
     * Extracts the object structures from the <code>PREF_TIMED_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREF_TIMED_GD</code> node.
     * @return the preference timed goal description expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp pref_timed_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTTIMED_GD:
                return this.timed_gd(cn);
            case LexerTreeConstants.JJTNAMED_PREF_TIMED_GD:
                if (!this.obj.requirements.contains(RequireKey.PREFERENCES)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.PREFERENCES
                                + "\" needed to specify preferences.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.named_pref_timed_gd(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>NAMED_PREF_TIMED_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>NAMED_PREF_TIMED_GD</code> node.
     * @return the name preference timed goal description expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private PrefExp named_pref_timed_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            if (cn1.getId() == LexerTreeConstants.JJTTIMED_GD) {
                Exp exp = this.timed_gd(cn1);
                return new PrefExp(exp);
            }
        } else if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTPREF_NAME
                        && cn2.getId() == LexerTreeConstants.JJTPREF_TIMED_GD) {
                String name = this.pref_name(cn1);
                Exp exp = this.timed_gd(cn2);
                return new PrefExp(name, exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
   /**
    * Extracts the object structures from the <code>TIMED_GD</code> node
    * of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>TIMED_GD</code> node.
    * @return the timed goal description expression built
    * @throws ParserException if an error occurs while parsing.
    */
   private Exp timed_gd(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           switch (cn.getId()) {
           case LexerTreeConstants.JJTAT_TIMED_GD:
               return this.at_timed_gd(cn);
           case LexerTreeConstants.JJTOVER_TIMED_GD:
               return this.over_timed_gd(cn);
           default:
               throw new ParserException("An internal parser error occurs: node "
                           + node.getLabel() + " unexpected.");
           }
       } else {
           throw new ParserException("An internal parser error occurs: node "
                           + node.getLabel() + " unexpected.");
       }
   }
    
   /**
    * Extracts the object structures from the <code>OVER_TIMED_GD</code> node
    * of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>OVER_TIMED_GD</code> node.
    * @return the over timed goal description expression built
    * @throws ParserException if an error occurs while parsing.
    */
   private OverTimedExp over_timed_gd(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           switch (cn.getId()) {
           case LexerTreeConstants.JJTOVER_ALL_TIMED_GD:
               return this.over_all_timed_gd(cn);
           default:
               throw new ParserException("An internal parser error occurs: node "
                           + node.getLabel() + " unexpected.");
           }
       } else {
           throw new ParserException("An internal parser error occurs: node "
                           + node.getLabel() + " unexpected.");
       }
   }
   
   /**
    * Extracts the object structures from the <code>OVER_ALL_TIMED_GD</code> node
    * of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>OVER_ALL_TIMED_GD</code> node.
    * @return the over all timed goal description expression built
    * @throws ParserException if an error occurs while parsing.
    */
   private OverTimedExp over_all_timed_gd(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           if (cn.getId() == LexerTreeConstants.JJTGD) {
               Exp exp = this.gd(cn);
               return new OverAllTimedExp(exp);
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                                       + node.getLabel() + " unexpected.");
   }
   
   /**
    * Extracts the object structures from the <code>AT_TIMED_GD</code> node
    * of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>AT_TIMED_GD</code> node.
    * @return the at timed goal description expression built
    * @throws ParserException if an error occurs while parsing.
    */
   private AtTimedExp at_timed_gd(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           switch (cn.getId()) {
           case LexerTreeConstants.JJTAT_START_TIMED_GD:
               return this.at_start_timed_gd(cn);
           case LexerTreeConstants.JJTAT_END_TIMED_GD:
               return this.at_end_timed_gd(cn);
           default:
               throw new ParserException("An internal parser error occurs: node "
                           + node.getLabel() + " unexpected.");
           }
       } else {
           throw new ParserException("An internal parser error occurs: node "
                           + node.getLabel() + " unexpected.");
       }
   }
   
   /**
    * Extracts the object structures from the <code>AT_START_TIMED_GD</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>AT_START_TIMED_GD</code> node.
    * @return the at start timed goal description expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private AtStartTimedExp at_start_timed_gd(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           if (cn.getId() == LexerTreeConstants.JJTGD) {
               Exp exp = this.gd(cn);
               return new AtStartTimedExp(exp);
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                                       + node.getLabel() + " unexpected.");
   }
   
   /**
    * Extracts the object structures from the <code>AT_END_TIMED_GD</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>AT_END_TIMED_GD</code> node.
    * @return the at end timed goal description expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private AtEndTimedExp at_end_timed_gd(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           if (cn.getId() == LexerTreeConstants.JJTGD) {
               Exp exp = this.gd(cn);
               return new AtEndTimedExp(exp);
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                                       + node.getLabel() + " unexpected.");
   }
   
    /**
     * Extracts the object structures from the <code>DURATION_CONSTRAINT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DURATION_CONSTRAINT</code> node.
     * @return the duration constraint expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp duration_constraint(SimpleNode node) throws ParserException {
        Exp exp = null;
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTEMPTY_OR:
                 exp = this.empty_or(cn);
                 break;
            case LexerTreeConstants.JJTAND_SIMPLE_DURATION_CONSTRAINT:
                if (!this.obj.requirements.contains(RequireKey.DURATION_INEQUALITIES)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.DURATION_INEQUALITIES
                                + "\" needed to specify conjunction of durative constraints.",
                                this.file, node.getLine(), node.getColumn());
                }
                exp = this.and_simple_duration_constraint(cn);
                break;
            case LexerTreeConstants.JJTSIMPLE_DURATION_CONSTRAINT:
                exp = this.simple_duration_constraint(cn);
                break;
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
        return exp;
    }
    
    /**
     * Extracts the object structures from the <code>AND_SIMPLE_DURATION_CONSTRAINT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AND_SIMPLE_DURATION_CONSTRAINT</code> node.
     * @return the conjuncive simple duration constraint expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AndExp and_simple_duration_constraint(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTSIMPLE_DURATION_CONSTRAINT) {
                AndExp exp = new AndExp();
                exp.add(this.simple_duration_constraint(cn));
                return exp;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>SIMPLE_DURATION_CONSTRAINT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>SIMPLE_DURATION_CONSTRAINT</code> node.
     * @return the simple duration constraint expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp simple_duration_constraint(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTD_OP:
                return this.d_op(cn);
            case LexerTreeConstants.JJTAT_SIMPLE_DURATION_CONSTRAINT:
                return this.at_simple_duration_constraint(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>AT_SIMPLE_DURATION_CONSTRAINT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_SIMPLE_DURATION_CONSTRAINT</code> node.
     * @return the at simple duration constraint expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtTimedExp at_simple_duration_constraint(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTAT_END_SIMPLE_DURATION_CONSTRAINT:
                return this.at_end_simple_duration_constraint(cn);
            case LexerTreeConstants.JJTAT_START_SIMPLE_DURATION_CONSTRAINT:
                return this.at_start_simple_duration_constraint(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>AT_END_SIMPLE_DURATION_CONSTRAINT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_END_SIMPLE_DURATION_CONSTRAINT</code> node.
     * @return the at end simple durative constraint expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtEndTimedExp at_end_simple_duration_constraint(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTSIMPLE_DURATION_CONSTRAINT) {
                Exp exp = this.simple_duration_constraint(cn);
                return new AtEndTimedExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AT_START_SIMPLE_DURATION_CONSTRAINT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_START_SIMPLE_DURATION_CONSTRAINT</code> node.
     * @return the at start simple durative constraint expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtStartTimedExp at_start_simple_duration_constraint(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTSIMPLE_DURATION_CONSTRAINT) {
                Exp exp = this.simple_duration_constraint(cn);
                return new AtStartTimedExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>D_OP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>D_OP</code> node.
     * @return the durative operation expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private FCompExp d_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTGEQUAL_D_OP:
                if (!this.obj.requirements.contains(RequireKey.DURATION_INEQUALITIES)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.DURATION_INEQUALITIES
                                + "\" needed to specify durative inequalitities.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.gequal_d_op(cn);
            case LexerTreeConstants.JJTLEQUAL_D_OP:
                if (!this.obj.requirements.contains(RequireKey.DURATION_INEQUALITIES)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.DURATION_INEQUALITIES
                                + "\" needed to specify durative inequalitities.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.lequal_d_op(cn);
            case LexerTreeConstants.JJTEQUAL_D_OP:
                return this.equal_d_op(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>EQUAL_D_OP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>EQUAL_D_OP</code> node.
     * @return the equal durative operation expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private FCompExp equal_d_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTVAR_DURATION
                        && cn2.getId() == LexerTreeConstants.JJTD_VALUE) {
                Variable cv = new Variable(Variable.DURATIVE_VARIABLE, Type.NUMBER);
                return new EqualComp(cv, this.d_value(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");   
    }
    
    /**
     * Extracts the object structures from the <code>LEQUAL_D_OP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>LEQUAL_D_OP</code> node.
     * @return the less equal durative operation expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private FCompExp lequal_d_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTVAR_DURATION
                        && cn2.getId() == LexerTreeConstants.JJTD_VALUE) {
                Variable cv = new Variable(Variable.DURATIVE_VARIABLE, Type.NUMBER);
                return new LEqualComp(cv, this.d_value(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");   
    }
    
    /**
     * Extracts the object structures from the <code>GEQUAL_D_OP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>GEQUAL_D_OP</code> node.
     * @return the greater equal durative operation expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private FCompExp gequal_d_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTVAR_DURATION
                        && cn2.getId() == LexerTreeConstants.JJTD_VALUE) {
                Variable cv = new Variable(Variable.DURATIVE_VARIABLE, Type.NUMBER);
                return new GEqualComp(cv, this.d_value(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");   
    }
    
    /**
     * Extracts the object structures from the <code>GEQUAL_D_OP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>D_OP</code> node.
     * @return the greater equal durative operation expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private Term d_value(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTNUMBER:
                return this.number(cn);
            case LexerTreeConstants.JJTF_EXP:
                if (!this.obj.requirements.contains(RequireKey.FLUENTS)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.FLUENTS
                                + "\" needed to specify fluent expressions.",
                                this.file, node.getLine(), node.getColumn());
                }
                return this.f_exp(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        } else {
            throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
        }
    }
    
    /**
     * Extracts the object structures from the <code>EMPTY_OR</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>EMPTY_OR</code> node.
     * @return the empty or expression built
     * @throws ParserException if an error occurs while parsing.
     */
    private OrExp empty_or(SimpleNode node) throws ParserException {
        return new OrExp();
    }
    
    /**
     * Extracts the object structures from the <code>DURATION_CONSTRAINT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DURATION_CONSTRAINT</code> node.
     * @return the duration constraint expression built
     * @throws ParserException if an error occurs while parsing.
     */
    
    /**
     * Extracts the object structures from the <code>DERIVED_DEF</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DERIVED_DEF</code> node.
     * @return the actionCtx structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private DerivedPredicate derived_def(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTATOMIC_FORMULA 
                       && cn2.getId() == LexerTreeConstants.JJTGD) {
                AtomicFormula head = this.atomic_formula(cn1);
                Exp body = this.gd(cn2);
                DerivedPredicate axiom = new DerivedPredicate(head, body);
                this.obj.axioms.add(axiom);
                return axiom;
            }
           
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    /**
     * Extracts the object structures from the <code>ACTION_DEF</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ACTION_DEF</code> node.
     * @return the actionCtx structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Action action_def(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 3) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            SimpleNode cn3 = (SimpleNode) node.jjtGetChild(2);
            if (cn1.getId() == LexerTreeConstants.JJTACTION_NAME
                && cn2.getId() == LexerTreeConstants.JJTTYPED_LIST
                && cn3.getId() == LexerTreeConstants.JJTACTION_DEF_BODY) {
                Action action = new Action(this.action_name(cn1));
                LinkedHashMap<String, Variable> parameters = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn2, parameters);
                for (Variable param : parameters.values()) {
                    action.add(param);
                }
                this.action_def_body(cn3, action);
                if (this.obj.actions.containsKey(action.getName())) {
                    this.mgr.logParserError("Duplicated action \"" + action.getName() 
                                + " \"." , this.file, cn1.getLine(), cn1.getColumn());
                } else {
                    this.obj.actions.put(action.getName(), action);
                }
                return action;
            }
        }
        return null;        
    }
    
    
    /**
     * Extracts the object structures from the <code>ACTION_DEF_BODY</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ACTION_DEF_BODY</code> node.
     * @param action the actionCtx to initialize.
     * @throws ParserException if an error occurs while parsing.
     */
    private void action_def_body(SimpleNode node, Action action) throws ParserException {
        this.actionCtx = action;
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTPRE_GD
                && cn2.getId() == LexerTreeConstants.JJTEFFECT) {
                action.setPrecondition(this.pre_gd(cn1));
                action.setEffect(this.effect(cn2));
            } else {
                this.actionCtx = null;
            }
        } else {
            this.actionCtx = null;
        }
        this.actionCtx = null;
    }
    
    /**
     * Extracts the object structures from the <code>EFFECT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>EFFECT</code> node.
     * @return the effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTAND_C_EFFECT:
                return this.and_c_effect(cn);
            case LexerTreeConstants.JJTC_EFFECT:
                return this.c_effect(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        return null;
    }
    
    /**
     * Extracts the object structures from the <code>EFFECT</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>EFFECT</code> node.
     * @return the effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp c_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTFORALL_EFFECT:
                return this.forall_effect(cn);
            case LexerTreeConstants.JJTWHEN_CON_EFFECT:
                return this.when_con_effect(cn);
            case LexerTreeConstants.JJTP_EFFECT:
                return this.p_effect(cn);
            case LexerTreeConstants.JJTPROB_EFFECT:
            	return this.prob_effect(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * For cleanliness, this returns an AndExp which is the conjunction of 
     * all the individual probability expressions.
     * 
     * @param node
     * @return
     * @throws ParserException
     */
    private ProbExp prob_effect(SimpleNode node) throws ParserException {

            Iterator<SimpleNode> children = node.iterator();
            
            ProbExp prob_term = new ProbExp();
            
            for(int i=0; i < node.jjtGetNumChildren(); i++){
            	SimpleNode cn = (SimpleNode)node.jjtGetChild(i);
            	SimpleNode real_cn = (SimpleNode)cn.jjtGetChild(1);
            	
            	Exp child = null;
            	
            	switch(real_cn.getId()){
            	case LexerTreeConstants.JJTC_EFFECT:
            		child = this.c_effect(real_cn);
            		break;
            	case LexerTreeConstants.JJTAND_C_EFFECT:
            		child = this.and_c_effect(real_cn);
            		break;
            	
            	}
            	
            	prob_term.addExp(child, this.number(((SimpleNode)cn.jjtGetChild(0))).getValue());
            	
             }
            
            return prob_term;
        
    }
    
    
    /**
     * Extracts the object structures from the <code>WHEN_CON_EFFECT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>WHEN_CON_EFFECT</code> node.
     * @return the when effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private WhenExp when_con_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTGD
                        && cn2.getId() == LexerTreeConstants.JJTCOND_EFFECT) {
                Exp cond = this.gd(cn1);
                Exp effect = this.cond_effect(cn2);
                return new WhenExp(cond, effect);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
   /**
    * Extracts the object structures from the <code>WHEN_C_EFFECT</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>WHEN_C_EFFECT</code> node.
    * @return the when effect expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private Exp cond_effect(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           switch (cn.getId()) {
           case LexerTreeConstants.JJTAND_P_EFFECT:
               return this.and_p_effect(cn);
           case LexerTreeConstants.JJTP_EFFECT:
               return this.p_effect(cn);
           case LexerTreeConstants.JJTAND_C_EFFECT:
           		return this.and_c_effect(cn);
           case LexerTreeConstants.JJTC_EFFECT:
           		return this.c_effect(cn);
           default:
               throw new ParserException("An internal parser error occurs: node "
                           + cn.getLabel() + " unexpected.");
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
   }
   
   /**
    * Extracts the object structures from the <code>AND_P_EFFECT</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>AND_P_EFFECT</code> node.
    * @return the conjuncive propositional effect expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private AndExp and_p_effect(SimpleNode node) throws ParserException {
       AndExp exp = new AndExp();
       for (int i = 0; i < node.jjtGetNumChildren(); i++) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
           switch (cn.getId()) {
           case LexerTreeConstants.JJTP_EFFECT:
               exp.add(this.p_effect(cn));
               break;
           default:
               throw new ParserException(
                           "An internal parser error occurs: node "
                                       + cn.getLabel() + " unexpected.");
           }
       }
       return exp;
   }
   
   /**
    * Extracts the object structures from the <code>P_EFFECT</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>P_EFFECT</code> node.
    * @return the propositional effect expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private Exp p_effect(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           switch (cn.getId()) {
           case LexerTreeConstants.JJTASSIGN_OP:
               if (!this.obj.requirements.contains(RequireKey.FLUENTS)) {
                   this.mgr.logParserError("Require key \"" + RequireKey.FLUENTS
                               + "\" missing.", this.file, 
                               node.getLine(), node.getColumn());
               }
               return this.assign_op(cn);
           case LexerTreeConstants.JJTLITERAL:
               return this.literal(cn);
           default:
               throw new ParserException("An internal parser error occurs: node "
                           + cn.getLabel() + " unexpected.");
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
   }
   
   /**
    * Extracts the object structures from the <code>ASSIGN_OP_EXP</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>ASSIGN_OP_EXP</code> node.
    * @return the assign operation expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private AssignOpExp assign_op(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 1) {
           SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
           switch (cn.getId()) {
           case LexerTreeConstants.JJTASSIGN:
               return this.assign(cn);
           case LexerTreeConstants.JJTSCALE_UP:
               return this.scale_up(cn);
           case LexerTreeConstants.JJTSCALE_DOWN:
               return this.scale_down(cn);
           case LexerTreeConstants.JJTINCREASE:
               return this.increase(cn);
           case LexerTreeConstants.JJTDECREASE:
               return this.decrease(cn);
           default:
               throw new ParserException("An internal parser error occurs: node "
                           + cn.getLabel() + " unexpected.");
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
   }
    
   /**
    * Extracts the object structures from the <code>SCALE_UP</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>SCALE_UP</code> node.
    * @return the assign scale up expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private ScaleUp scale_up(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 2) {
           SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
           SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
           if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                       && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
               return new ScaleUp(this.f_head(cn1), this.f_exp(cn2));
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
   }
   
   /**
    * Extracts the object structures from the <code>DECREASE</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>DECREASE</code> node.
    * @return the descrease expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private Decrease decrease(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 2) {
           SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
           SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
           if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                       && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
               return new Decrease(this.f_head(cn1), this.f_exp(cn2));
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
   }
   
   /**
    * Extracts the object structures from the <code>INCREASE</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>INCREASE</code> node.
    * @return the increase expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private Increase increase(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 2) {
           SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
           SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
           if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                       && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
               return new Increase(this.f_head(cn1), this.f_exp(cn2));
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
   }
   
   /**
    * Extracts the object structures from the <code>SCALE_DOWN</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>SCALE_DOWN</code> node.
    * @return the assign scale up expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private ScaleDown scale_down(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 2) {
           SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
           SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
           if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                       && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
               return new ScaleDown(this.f_head(cn1), this.f_exp(cn2));
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
   }
   
   /**
    * Extracts the object structures from the <code>ASSIGN</code>
    * node of the syntaxic tree and implements the semantic check.
    * 
    * @param node the <code>ASSIGN</code> node.
    * @return the propositional effect expression built.
    * @throws ParserException if an error occurs while parsing.
    */
   private Assign assign(SimpleNode node) throws ParserException {
       if (node.jjtGetNumChildren() == 2) {
           SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
           SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
           if (cn1.getId() == LexerTreeConstants.JJTF_HEAD
                       && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
               return new Assign(this.f_head(cn1), this.f_exp(cn2));
           }
       }
       throw new ParserException("An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
   }
               
    /**
     * Extracts the object structures from the <code>FORALL_C_EFFECT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FORALL_C_EFFECT</code> node.
     * @return the universal effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ForallExp forall_effect(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTTYPED_LIST
                        && cn2.getId() == LexerTreeConstants.JJTEFFECT) {
                ActionDef tmp = this.actionCtx;
                this.actionCtx = null;
                ForallExp forall = new ForallExp();
                LinkedHashMap<String, Variable> vars = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn1, vars);
                this.actionCtx = tmp;
                for (Variable var : vars.values()) {
                    forall.add(var);
                }
                this.quantifiedExpCtx = forall;
                Exp exp = this.effect(cn2);
                forall.setExp(exp);
                this.quantifiedExpCtx = null;
                return forall; 
            }
        }
        this.quantifiedExpCtx = null;
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AND_C_EFFECT</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AND_C_EFFECT</code> node.
     * @return the conjuncive effect expression built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AndExp and_c_effect(SimpleNode node) throws ParserException {
        AndExp effect = new AndExp();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            if (cn.getId() == LexerTreeConstants.JJTC_EFFECT) {
                effect.add(this.c_effect(cn));
            } else {
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        }
        return effect;
    }
    
    /**
     * Extracts the object structures from the <code>PRE_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PRE_GD</code> node.
     * @return the precondition goal description built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp pre_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTEMPTY_OR:
                return this.empty_or(cn);
            case LexerTreeConstants.JJTPREF_GD:
                return this.pref_gd(cn);
            case LexerTreeConstants.JJTAND_PRE_GD:
                return this.and_pre_gd(cn);
            case LexerTreeConstants.JJTFORALL_PRE_GD:
                if (!this.obj.requirements.contains(RequireKey.UNIVERSAL_PRECONDITIONS)
                            && !this.obj.requirements.contains(RequireKey.QUANTIFIED_PRECONDITIONS)) {
                    this.mgr.logParserError("Universal formula cannot be defined without require keys \""
                               + RequireKey.UNIVERSAL_PRECONDITIONS
                               + "\" or \""
                               + RequireKey.QUANTIFIED_PRECONDITIONS
                               + "\".", this.file, node.getLine(), node.getColumn());
                }
                return this.forall_pre_gd(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
   
    /**
     * Extracts the object structures from the <code>FORALL_PRE_GD</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FORALL_PRE_GD</code> node.
     * @return the universal precondition goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ForallExp forall_pre_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTTYPED_LIST
                        && cn2.getId() == LexerTreeConstants.JJTPRE_GD) {
                ActionDef tmp = this.actionCtx;
                this.actionCtx = null;
                ForallExp forall = new ForallExp();
                LinkedHashMap<String, Variable> vars = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn1, vars);
                this.actionCtx = tmp;
                for (Variable var : vars.values()) {
                    forall.add(var);
                }
                this.quantifiedExpCtx = forall;
                Exp exp = this.pre_gd(cn2);
                forall.setExp(exp);
                this.quantifiedExpCtx = null;
                return forall;
            }
        }
        this.quantifiedExpCtx = null;
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>AND_PRE_GD</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AND_PRE_GD</code> node.
     * @return the conjuncive precondition goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AndExp and_pre_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTPRE_GD) {
                AndExp pre_gd = new AndExp();
                pre_gd.add(this.pre_gd(cn));
                return pre_gd;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>PREF_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREF_GD</code> node.
     * @return the precondition goal description built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp pref_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTNAMED_PREF_GD:
                return this.named_pref_gd(cn);
            case LexerTreeConstants.JJTGD:
                return this.gd(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }   

    /**
     * Extracts the object structures from the <code>PREF_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREF_GD</code> node.
     * @return the precondition goal description built
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp named_pref_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            if (cn1.getId() == LexerTreeConstants.JJTGD) {
                Exp exp = this.gd(cn1);
                return new PrefExp(exp);
            }
        } else if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTPREF_NAME
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                String name = this.pref_name(cn1);
                Exp exp = this.gd(cn2);
                return new PrefExp(name, exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>ACTION_NAME</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ACTION_NAME</code> node.
     * @return the actionCtx name structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private String action_name(SimpleNode node) throws ParserException {
        return node.getImage();
    }

    /**
     * Extracts the object structures from the <code>REQUIRE_DEF</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>REQUIRE_DEF</code> node.
     * @return the require definition structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Set<RequireKey> requireDef(SimpleNode node) throws ParserException {
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTREQUIRE_KEY:
                RequireKey rk = RequireKey.getRequireKey(cn.getImage());
                
                if (rk == null) {
                    this.mgr.logParserError("Invalid require key \"" + cn.getImage() + "\".",
                                file, node.getLine(), node.getColumn());
                } else if (rk != null && this.options.get(rk) == null) {
                    this.mgr.logParserError("Require key \"" + rk.getImage() 
                                + "\" forbidden due to parser requirement options." , 
                                file, cn.getLine(), cn.getColumn());
                } else {
                    if (rk.equals(RequireKey.ADL)) {
                        obj.requirements.add(RequireKey.ADL);
                        obj.requirements.add(RequireKey.STRIPS);
                        obj.requirements.add(RequireKey.TYPING);
                        obj.requirements.add(RequireKey.EXISTENTIAL_PRECONDITIONS);
                        obj.requirements.add(RequireKey.UNIVERSAL_PRECONDITIONS);
                        obj.requirements.add(RequireKey.QUANTIFIED_PRECONDITIONS);
                        obj.requirements.add(RequireKey.CONDITIONAL_EFFECTS);
                        obj.requirements.add(RequireKey.DISJUNCTIVE_PRECONDITIONS);
                        obj.requirements.add(RequireKey.EQUALITY);
                        obj.requirements.add(RequireKey.FLUENTS);
                        obj.requirements.add(RequireKey.NEGATIVE_PRECONDITIONS);
                    } else if (rk.equals(RequireKey.QUANTIFIED_PRECONDITIONS)) {
                        obj.requirements.add(RequireKey.QUANTIFIED_PRECONDITIONS);
                        obj.requirements.add(RequireKey.EXISTENTIAL_PRECONDITIONS);
                        obj.requirements.add(RequireKey.UNIVERSAL_PRECONDITIONS);
                    } else {
                        obj.requirements.add(rk);
                    }
                }
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return obj.requirements;
    }

    /**
     * Extracts the object structures from the <code>TYPE_DEF</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TYPE_DEF</code> node.
     * @return the type definition structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Map<String, Set<String>> types_def(SimpleNode node) throws ParserException {
        if (!this.obj.requirements.contains(RequireKey.TYPING)) {
            this.mgr.logParserError("Require key \"" + RequireKey.TYPING
                        + "\" needed to specify typed terms.", this.file, 
                        node.getLine(), node.getColumn());
        }
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTTYPED_LIST:
                this.type_typed_list(cn, obj.types);
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return obj.types;
    }


    /**
     * Extracts the object structures from the <code>TYPED_LIST</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TYPED_LIST</code> node.
     * @param tl the types list of primitive type already built.
     * @return the typed list structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Map<String, Set<String>> type_typed_list(SimpleNode node,
                Map<String, Set<String>> tl) throws ParserException {
        boolean typed = false;
        Set<String> ptl = new LinkedHashSet<String>(); 
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTPRIMITIVE_TYPE:
                String pt = primitive_type(cn);
                if (pt.equals(Type.NUMBER_SYMBOL) || pt.equals(Type.OBJECT_SYMBOL)) {
                    this.mgr.logParserError("Type \"" + pt + "\" is a reserved type.", 
                                this.file, cn.getLine(), cn.getColumn());
                } else if (tl.containsKey(pt)) {
                    this.mgr.logParserError("Type \"" + pt + "\" duplicated definition.", 
                                this.file, cn.getLine(), cn.getColumn());
                } else {
                    tl.put(pt, new LinkedHashSet<String>());
                    ptl.add(pt);
                }
                break;
            case LexerTreeConstants.JJTTYPE:
                typed = true;
                Set<String> type = type(cn);
                for (String pti : type) {
                    if (pti.equals(Type.NUMBER_SYMBOL)) {
                       this.mgr.logParserError("Type \"" + pti + "\" not allow in type declaration.", 
                                   this.file, node.getLine(), node.getColumn());
                   } else if (!tl.containsKey(pti)) {
                       this.mgr.logParserError("Type \"" + pti + "\" undefinied.", 
                                   this.file, node.getLine(), node.getColumn());
                   } else {
                       Set<String> ti = tl.get(pti);
                       ti.addAll(ptl);
                   }
                }
                break;
            case LexerTreeConstants.JJTTYPED_LIST:                
                if (!this.obj.requirements.contains(RequireKey.TYPING)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.TYPING
                                + "\" needed to specify types.", this.file, 
                                node.getLine(), node.getColumn());
                }
                Map<String, Set<String>> typed_list = this.type_typed_list(cn, tl);
                tl.putAll(typed_list);
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        if (!typed) {
            Set<String> obj = tl.get(Type.OBJECT_SYMBOL);
            for (String t : tl.keySet()) {
                if (!t.equals(Type.NUMBER_SYMBOL) && !t.equals(Type.OBJECT_SYMBOL)) {
                    obj.add(t);
                }
            }
            
        }
        return tl;
    }

    
    
    /**
     * Extracts the object structures from the <code>TYPED_LIST</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TYPED_LIST</code> node.
     * @param tl the typed list of variables already built.
     * @return the typed list structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Map<String, Variable> var_typed_list(SimpleNode node,
                LinkedHashMap<String, Variable> tl) throws ParserException {
        List<Variable> vtl = new ArrayList<Variable>();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTVARIABLE:
                Variable v = this.variable(cn);
                if (tl.containsKey(v.getImage())) {
                    this.mgr.logParserError("Variable \"" + v + "\" duplicated definition.",
                                this.file, cn.getLine(), cn.getColumn());
                } else {
                    vtl.add(v);
                    tl.put(v.getImage(), v);
                }
                
                break;
            case LexerTreeConstants.JJTTYPE:
                Set<String> type = type(cn);
                TypeSet newType = new TypeSet();
                for (String pti : type) {
                    if (pti.equals(Type.NUMBER_SYMBOL)) {
                        this.mgr.logParserError("Type \"" + pti + "\" not allow in type declaration.", 
                                    this.file, node.getLine(), node.getColumn());
                    } else if (!this.obj.types.containsKey(pti)) {
                        this.mgr.logParserError("Type \"" + pti + "\" undefinied.", 
                                    this.file, node.getLine(), node.getColumn());
                    } else {
                        newType.add(new Type(pti, obj.types));
                    }
                }
                for (Variable vi : vtl) {
                    vi.setTypeSet(newType);
                }
               break;
            case LexerTreeConstants.JJTTYPED_LIST:
                
                if (!this.obj.requirements.contains(RequireKey.TYPING)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.TYPING
                                + "\" needed to specify typed variables.", this.file,
                                node.getLine(), node.getColumn());
                }
                this.var_typed_list(cn, tl);
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return tl;
    }


    /**
     * Extracts the object structures from the <code>TYPED_LIST</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TYPED_LIST</code> node.
     * @param tl the typed list of constants already built.
     * @return the typed list structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Map<String, Constant> constant_typed_list(SimpleNode node,
                Map<String, Constant> tl) throws ParserException {
        List<Constant> ctl = new ArrayList<Constant>();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTCONSTANT:
                Constant c = this.constant(cn);
                if (tl.containsKey(c.getImage())) {
                    this.mgr.logParserError("Constant \"" + c + "\" duplicated definition.",
                                this.file, cn.getLine(), cn.getColumn());
                } else {
                    ctl.add(c);
                    tl.put(c.getImage(), c);
                }
                break;
            case LexerTreeConstants.JJTTYPE:
               Set<String> type = type(cn);
               TypeSet newType = new TypeSet();
               for (String pti : type) {
                   if (pti.equals(Type.NUMBER_SYMBOL)) {
                       this.mgr.logParserError("Type \"" + pti + "\" not allow in type declaration.", 
                                   this.file, node.getLine(), node.getColumn());
                   } else if (!this.obj.types.containsKey(pti)
                               && !this.obj.getContent().equals(PDDLObject.Content.PARTIAL_PROBLEM)) {
                       this.mgr.logParserError("Type \"" + pti + "\" undefinied.", 
                                   this.file, node.getLine(), node.getColumn());
                   } else {
                       newType.add(new Type(pti, obj.types));
                   }
               }
               for (Constant ci : ctl) {
                    ci.setTypeSet(newType);
               }
                break;
            case LexerTreeConstants.JJTTYPED_LIST:
                if (!this.obj.requirements.contains(RequireKey.TYPING)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.TYPING
                                + "\" needed to specify typed constants.", this.file, 
                                node.getLine(), node.getColumn());
                }
                this.constant_typed_list(cn, tl);
                break;
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        return tl;
    }


    /**
     * Extracts the object structures from the <code>PRIMITIVE_TYPE</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PRIMITIVE_TYPE</code> node.
     * @return the primitive type structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private String primitive_type(SimpleNode node) throws ParserException {
        return node.getImage();
    }


    /**
     * Extracts the object structures from the <code>TYPE</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TYPE</code> node.
     * @return the type structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Set<String> type(SimpleNode node) throws ParserException {
        Set<String> type = new LinkedHashSet<String>();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            if (cn.getId() == LexerTreeConstants.JJTPRIMITIVE_TYPE) {
                type.add(this.primitive_type(cn));
            } else {
                throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        }
        return type;
    }

    /**
     * Extracts the object structures from the <code>CONSTANTS_DEF</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>CONSTANTS_DEF</code> node.
     * @return the constants definition structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Map<String, Constant> constants_def(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() ==  1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTTYPED_LIST) {
                this.constantDef = true;
                this.constant_typed_list(cn, obj.constants);
                for (Constant cst : obj.constants.values()) {
                    for (Type pt : cst.getTypeSet()) {
                        Set<Constant> domain = this.obj.typeDomains.get(pt);
                        if (domain == null) {
                            domain = new LinkedHashSet<Constant>();
                        }
                        domain.add(cst);
                    }
                }
                this.constantDef = false;
                return obj.constants;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>CONSTANT</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>CONSTANT</code> node.
     * @return the constant structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Constant constant(SimpleNode node) throws ParserException {
        Constant cons = new Constant(node.getImage(), new Type(Type.OBJECT_SYMBOL, obj.types));
        if (!this.constantDef) {
            Constant cst = this.obj.constants.get(cons.getImage());
            if (cst == null) {
                this.mgr.logParserError("Constant \"" + cons +"\" undefined." , 
                            this.file, node.getLine(), node.getColumn());
            } else {
                cons = cst;
            }
        }
        return cons;
    }

    /**
     * Extracts the object structures from the <code>PREDICATES_DEF</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREDICATES_DEF</code> node.
     * @return the predicates definition structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Set<AtomicFormula> predicates_def(SimpleNode node) throws ParserException {
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTATOMIC_FORMULA_SKELETON:
                obj.predicates.add(this.atomic_formula_skeleton(cn));
                break;
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        return obj.predicates;
    }

    /**
     * Extracts the object structures from the <code>PREDICATES_DEF</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREDICATES_DEF</code> node.
     * @return the predicates definition structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtomicFormula atomic_formula_skeleton(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTPREDICATE
                        && cn2.getId() == LexerTreeConstants.JJTTYPED_LIST) {
                AtomicFormula afs = new AtomicFormula(this.predicate(cn1));
                LinkedHashMap<String, Variable> arguments = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn2, arguments);
                for (Variable arg : arguments.values()) {
                    afs.add(arg);
                }
                return afs;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
        
    }

    /**
     * Extracts the object structures from the <code>PREDICATE</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREDICATE</code> node.
     * @return the predicate structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private String predicate(SimpleNode node) throws ParserException {
        return node.getImage();
    }

    /**
     * Extracts the object structures from the <code>FUNCTIONS_DEF</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FUNCTIONS_DEF</code> node.
     * @return the functions definition structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Set<FHead> functions_def(SimpleNode node) throws ParserException {
        if (!this.obj.requirements.contains(RequireKey.FLUENTS)) {
            this.mgr.logParserError("Require key \"" + RequireKey.FLUENTS
                        + "\" needed to defined functions.", this.file, node.getLine(),
                        node.getColumn());
        }
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTFUNCTION_TYPED_LIST:
                this.atomic_function_skeleton_typed_list(cn, obj.functions);
                break;
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        return obj.functions;
    }

    /**
     * Extracts the object structures from the <code>TYPED_LIST</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TYPED_LIST</code> node.
     * @param tl the typed list of constants already built.
     * @return the typed list structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Set<FHead> atomic_function_skeleton_typed_list(
                SimpleNode node, Set<FHead> tl)
                throws ParserException {
        List<FHead> atftl = new ArrayList<FHead>();
        TypeSet number = new TypeSet();
        number.add(Type.NUMBER);
        
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTATOMIC_FUNCTION_SKELETON:
                FHead func = this.atomic_function_skeleton(cn);
                func.setTypeSet(number);
                tl.add(func);
                break;
            case LexerTreeConstants.JJTFUNCTION_TYPE:
                if (!cn.getImage().equals(Type.NUMBER_SYMBOL)) {
                    this.mgr.logParserError("FExp can be only typed as number.", 
                                    this.file, node.getLine(), node.getColumn());
                }
                break;
            case LexerTreeConstants.JJTFUNCTION_TYPED_LIST:
                if (!this.obj.requirements.contains(RequireKey.TYPING)) {
                    this.mgr.logParserError("Require key \"" + RequireKey.TYPING
                                + "\" needed to specify typed functions.", this.file, 
                                node.getLine(), node.getColumn());
                }
                this.atomic_function_skeleton_typed_list(cn, tl);
                break;
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        return tl;
    }


    /**
     * Extracts the object structures from the
     * <code>ATOMIC_FUNCTION_SKELETON</code> node of the syntaxic tree and
     * implements the semantic check.
     * 
     * @param node the <code>ATOMIC_FUNCTION_SKELETON</code> node.
     * @return the atomic function skeleton structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private FHead atomic_function_skeleton(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTFUNCTOR 
                        && cn2.getId() == LexerTreeConstants.JJTTYPED_LIST) {
                FHead afs = new FHead(this.functor(cn1), Type.NUMBER);
                LinkedHashMap<String, Variable> arguments = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn2, arguments);
                for (Variable arg : arguments.values()) {
                    afs.add(arg);
                }
                return afs;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>FUNCTOR</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FUNCTOR</code> node.
     * @return the functor structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private String functor(SimpleNode node) throws ParserException {
        return node.getImage();
    }

    /**
     * Extracts the object structures from the <code>TERM</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TERM</code> node.
     * @return the exp structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Variable variable(SimpleNode node) throws ParserException {
        Variable var = new Variable(node.getImage(), new Type(Type.OBJECT_SYMBOL, obj.types));
        if (this.actionCtx != null) {
            TypeSet qt = null;
            if (this.quantifiedExpCtx != null) {
                Iterator<Variable> i = this.quantifiedExpCtx.iterator();
                while (i.hasNext() && qt == null) {
                    Variable vi = i.next();
                    if (var.getImage().equals(vi.getImage())) {
                        qt = vi.getTypeSet();
                    }
                }
            }
            TypeSet pt = null;
            if (this.actionCtx != null) {
                Iterator<Term> i = this.actionCtx.iterator();
                while (i.hasNext() && pt == null) {
                    Term ti = i.next();
                    if (var.getImage().equals(ti.getImage())) {
                        pt = ti.getTypeSet();
                    }
                }
            }
            if (qt == null && pt == null) {
                this.mgr.logParserError("Variable \"" + var +"\" undefined in action \"" 
                            + this.actionCtx.getName() + "\".",                        
                            this.file, node.getLine(), node.getColumn());
            } else if (qt != null && pt != null && !qt.equals(pt)) {
                this.mgr.logParserError("Variable \"" + var +"\" conflicted type in action \"" 
                            + this.actionCtx.getName() + "\".",                        
                            this.file, node.getLine(), node.getColumn());
            } else {
                if (qt != null) {
                    var.setTypeSet(qt);
                } else {
                    var.setTypeSet(pt);
                }
            }
        } 
        return var;
        
    }

    /**
     * Extracts the object structures from the <code>CONSTRAINTS</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>CONSTRAINTS</code> node.
     * @return the constraints definition structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Set<Exp> constraints(SimpleNode node) throws ParserException {
        if (!this.obj.requirements.contains(RequireKey.CONSTRAINTS)) {
            this.mgr.logParserError("Require key \"" + RequireKey.CONSTRAINTS
                        + "\" missing to define constraints.", this.file, 
                        node.getLine(),
                        node.getColumn());
        }
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTPREF_CON_GD) {
                obj.constraints.add(this.pref_con_gd(cn));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>PREF_CON_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREF_CON_GD</code> node.
     * @return the conditional goal description preference structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp pref_con_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTAND_PREF_CON_GD:
                return this.and_pref_con_gd(cn);
            case LexerTreeConstants.JJTFORALL_PREF_CON_GD:
                return this.forall_pref_con_gd(cn);
            case LexerTreeConstants.JJTNAMED_PREF_CON_GD:
                return this.named_pref_con_gd(cn);
            case LexerTreeConstants.JJTCON_GD:
                return this.con_gd(cn);
            default:
                throw new ParserException("An internal parser error occurs: node "
                            + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>AND_PREF_CON_GD</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AND_PREF_CON_GD</code> node.
     * @return the and condition goal description preference structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AndExp and_pref_con_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTPREF_CON_GD) {
                AndExp pre_con_gd = new AndExp();
                pre_con_gd.add(this.pref_con_gd(cn));
                return pre_con_gd;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>FORALL_PREF_CON_GD</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FORALL_PREF_CON_GD</code> node.
     * @return the forall condition goal description preference structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ForallExp forall_pref_con_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTTYPED_LIST
                        && cn2.getId() == LexerTreeConstants.JJTPREF_CON_GD) {
                ActionDef tmp = this.actionCtx;
                this.actionCtx = null;
                ForallExp forall = new ForallExp();
                LinkedHashMap<String, Variable> vars = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn1, vars);
                this.actionCtx = tmp;
                for (Variable var : vars.values()) {
                    forall.add(var);
                }
                this.quantifiedExpCtx = forall;
                Exp exp = this.pref_con_gd(cn2);
                forall.setExp(exp);
                this.quantifiedExpCtx = null;
                return forall;
            }
        }
        this.quantifiedExpCtx = null;
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>PREFERENCE</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREFERENCE</code> node.
     * @return the preference structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private PrefExp named_pref_con_gd(SimpleNode node) throws ParserException {
        if (!this.obj.requirements.contains(RequireKey.PREFERENCES)) {
            this.mgr.logParserError("Require key \"" + RequireKey.PREFERENCES
                        + "\" missing.", this.file, node.getLine(), node.getColumn());
            if (node.jjtGetNumChildren() == 1) {
                SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
                if (cn1.getId() == LexerTreeConstants.JJTCON_GD) {
                    Exp exp = this.con_gd(cn1);
                    return new PrefExp(exp);
                }
                
            } else if (node.jjtGetNumChildren() == 2) {
                SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
                SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
                if (cn1.getId() == LexerTreeConstants.JJTPREF_NAME
                            && cn2.getId() == LexerTreeConstants.JJTCON_GD) {
                    String name = this.pref_name(cn1);
                    Exp exp = this.con_gd(cn2);
                    return new PrefExp(name, exp);
                }
            } else {
                throw new ParserException("aAn internal parser error occurs: node "
                            + node.getLabel() + " unexpected.");
            }
        }
       throw new ParserException("1An internal parser error occurs: node "
                   + node.getLabel() + " unexpected.");
       
    }

    /**
     * Extracts the object structures from the <code>PREF_NAME</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>PREF_NAME</code> node.
     * @return the preference image structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private String pref_name(SimpleNode node) throws ParserException {
        return node.getImage();
    }

    /**
     * Extracts the object structures from the <code>CON_GD</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>CON_GD</code> node.
     * @return the conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp con_gd(SimpleNode node) throws ParserException {
        Exp exp = null;
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTAND_CON_GD:
                exp = this.and_con_gd(cn);
                break;
            case LexerTreeConstants.JJTFORALL_CON_GD:
                exp = this.forall_con_gd(cn);
                break;
            case LexerTreeConstants.JJTAT_END_CON_GD:
                exp = this.at_end_con_gd(cn);
                break;
            case LexerTreeConstants.JJTALWAYS_CON_GD:
                exp = this.always_con_gd(cn);
                break;
            case LexerTreeConstants.JJTSOMETIME_CON_GD:
                exp = this.sometime_con_gd(cn);
                break;
            case LexerTreeConstants.JJTWITHIN_CON_GD:
                exp = this.within_con_gd(cn);
                break;
            case LexerTreeConstants.JJTAT_MOST_ONCE_CON_GD:
                exp = this.at_most_once_con_gd(cn);
                break;
            case LexerTreeConstants.JJTSOMETIME_AFTER_CON_GD:
                exp = this.sometime_after_con_gd(cn);
                break;
            case LexerTreeConstants.JJTSOMETIME_BEFORE_CON_GD:
                exp = this.sometime_before_con_gd(cn);
                break;
            case LexerTreeConstants.JJTALWAYS_WITHIN_CON_GD:
                exp = this.always_within_con_gd(cn);
                break;
            case LexerTreeConstants.JJTHOLD_DURING_CON_GD:
                exp = this.hold_during_con_gd(cn);
                break;
            case LexerTreeConstants.JJTHOLD_AFTER_CON_GD:
                exp = this.hold_after_con_gd(cn);
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }

    /**
     * Extracts the object structures from the <code>HOLD_AFTER</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>HOLD_AFTER</code> node.
     * @return the "hold after" conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private HoldAfterExp hold_after_con_gd(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTNUMBER
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                Number time = this.number(cn1);
                Exp exp = this.gd(cn2);
                return new HoldAfterExp(exp, time);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>HOLD_DURING</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>HOLD_DURING</code> node.
     * @return the "hold during" conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private HoldDuringExp hold_during_con_gd(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 3) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            SimpleNode cn3 = (SimpleNode) node.jjtGetChild(2);
            if (cn1.getId() == LexerTreeConstants.JJTNUMBER
                        && cn2.getId() == LexerTreeConstants.JJTNUMBER
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                Number lt = this.number(cn1);
                Number ut = this.number(cn2);
                Exp exp = this.gd(cn3);
                return new HoldDuringExp(lt, ut, exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>ALWAYS_WITHIN</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ALWAYS_WITHIN</code> node.
     * @return the "always within" conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AlwaysWithinExp always_within_con_gd(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 3) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            SimpleNode cn3 = (SimpleNode) node.jjtGetChild(2);
            if (cn1.getId() == LexerTreeConstants.JJTNUMBER
                        && cn2.getId() == LexerTreeConstants.JJTGD
                        && cn2.getId() == LexerTreeConstants.JJTGD) { 
                Number time = this.number(cn1);
                Exp arg1 = this.gd(cn2);
                Exp arg2 = this.gd(cn3);
                return new AlwaysWithinExp(arg1, arg2, time);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>SOMETIME_BEFORE</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>SOMETIME_BEFORE</code> node.
     * @return the "sometime before" conditional goal description structure
     *         built.
     * @throws ParserException if an error occurs while parsing.
     */
    private SometimeBeforeExp sometime_before_con_gd(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTGD
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                Exp arg1 = this.gd(cn1);
                Exp arg2 = this.gd(cn2);
                return new SometimeBeforeExp(arg1, arg2);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>SOMETIME_AFTER</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>SOMETIME_AFTER</code> node.
     * @return the "sometime after" conditional goal description structure
     *         built.
     * @throws ParserException if an error occurs while parsing.
     */
    private SometimeAfterExp sometime_after_con_gd(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTGD
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                Exp arg1 = this.gd(cn1);
                Exp arg2 = this.gd(cn2);
                return new SometimeAfterExp(arg1, arg2);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>AT_MOST_ONCE</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_MOST_ONCE</code> node.
     * @return the "at most once" conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtMostOnceExp at_most_once_con_gd(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTGD) {
                Exp exp = this.gd(cn);
                return new AtMostOnceExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>WITHIN</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>WITHIN</code> node.
     * @return the "within" conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private WithinExp within_con_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTNUMBER
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                Number time = this.number(cn1);
                Exp exp = this.gd(cn2);
                return new WithinExp(exp, time);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>SOMETIME</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>SOMETIME</code> node.
     * @return the "sometime" conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private SometimeExp sometime_con_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTGD) {
                Exp exp = this.gd(cn);
                return new SometimeExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>ALWAYS</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ALWAYS</code> node.
     * @return the "always" conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AlwaysExp always_con_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTGD) {
                Exp exp = this.gd(cn);
                return new AlwaysExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>AND_CON_GD</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AND_CON_GD</code> node.
     * @return the and conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AndExp and_con_gd(SimpleNode node) throws ParserException {
        AndExp and = new AndExp();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTCON_GD:
                and.add(this.con_gd(cn));
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return and;
    }

    /**
     * Extracts the object structures from the <code>FORALL_CON_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FORALL_CON_GD</code> node.
     * @return the universal conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ForallExp forall_con_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTTYPED_LIST
                        && cn2.getId() == LexerTreeConstants.JJTCON_GD) {
                ActionDef tmp = this.actionCtx;
                this.actionCtx = null;
                ForallExp forall = new ForallExp();
                LinkedHashMap<String, Variable> vars = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn1, vars);
                this.actionCtx = tmp;
                for (Variable var : vars.values()) {
                    forall.add(var);
                }
                this.quantifiedExpCtx = forall;
                Exp exp = this.con_gd(cn2);
                forall.setExp(exp);
                this.quantifiedExpCtx = null;
                return forall;
            }
        }
        this.quantifiedExpCtx = null;
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
 
    /**
     * Extracts the object structures from the <code>AT_END_CON_GD</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AT_END_CON_GD</code> node.
     * @return the at end conditional goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtEndTimedExp at_end_con_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTCON_GD) {
                Exp exp = this.gd(cn);
                return new AtEndTimedExp(exp);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>GD</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>GD</code> node.
     * @return the goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Exp gd(SimpleNode node) throws ParserException {
        Exp exp = null;
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTATOMIC_FORMULA:
                exp = this.atomic_formula(cn);
                break;
            case LexerTreeConstants.JJTAND_GD:
                exp = this.and_gd(cn);
                break;
            case LexerTreeConstants.JJTOR_GD:
                if (!this.obj.requirements.contains(RequireKey.DISJUNCTIVE_PRECONDITIONS)) {
                    this.mgr.logParserError("Disjunctive formula cannot be defined without require key \""
                               + RequireKey.DISJUNCTIVE_PRECONDITIONS
                               + "\".", this.file, node.getLine(), node.getColumn());
                }
                exp = this.or_gd(cn);
                break;
            case LexerTreeConstants.JJTNOT_GD:
                NotExp notExp = this.not_gd(cn);
                if (!notExp.getExp().getExpID().equals(ExpID.F_COMP) && !this.obj.requirements.contains(RequireKey.NEGATIVE_PRECONDITIONS)) {
                    this.mgr.logParserError("Negative formula cannot be defined without require key \""
                               + RequireKey.NEGATIVE_PRECONDITIONS
                               + "\".", this.file, node.getLine(), node.getColumn());
                }
                exp = notExp;
                break;
            case LexerTreeConstants.JJTIMPLY_GD:
                if (!this.obj.requirements.contains(RequireKey.DISJUNCTIVE_PRECONDITIONS)) {
                    this.mgr.logParserError("Implication formula cannot be defined without require key \""
                                + RequireKey.DISJUNCTIVE_PRECONDITIONS
                                + "\".", this.file, node.getLine(), node.getColumn());
                }
                exp = this.imply_gd(cn);
                break;
            case LexerTreeConstants.JJTFORALL_GD:
                if (!this.obj.requirements.contains(RequireKey.UNIVERSAL_PRECONDITIONS)
                            && !this.obj.requirements.contains(RequireKey.QUANTIFIED_PRECONDITIONS)) {
                    this.mgr.logParserError("Universal formula cannot be defined without require keys \""
                               + RequireKey.UNIVERSAL_PRECONDITIONS
                               + "\" or \""
                               + RequireKey.QUANTIFIED_PRECONDITIONS
                               + "\".", this.file, node.getLine(), node.getColumn());
                }
                exp = this.forall_gd(cn);
                break;
            case LexerTreeConstants.JJTEXISTS_GD:
                if (!this.obj.requirements.contains(RequireKey.EXISTENTIAL_PRECONDITIONS)
                            && !this.obj.requirements.contains(RequireKey.QUANTIFIED_PRECONDITIONS)) {
                    this.mgr.logParserError("Existential formula cannot be defined without require keys \""
                               + RequireKey.EXISTENTIAL_PRECONDITIONS
                               + "\" or \""
                               + RequireKey.QUANTIFIED_PRECONDITIONS
                               + "\".", this.file, node.getLine(), node.getColumn());
                }
                exp = this.exists_gd(cn);
                break;
            case LexerTreeConstants.JJTF_COMP:
                FCompExp fCompExp = this.f_comp(cn);
                if (!this.obj.requirements.contains(RequireKey.FLUENTS)) {
                    if (!(fCompExp.getOp().equals(Comp.EQUAL)
                                && fCompExp.getArg1().getExpID().equals(ExpID.TERM)
                                && fCompExp.getArg2().getExpID().equals(ExpID.TERM) 
                                && this.obj.requirements.contains(RequireKey.EQUALITY))) {
                        this.mgr.logParserError("FExp expression cannot be defined without require keys \""
                                    + RequireKey.FLUENTS + "\" or \"" +RequireKey.EQUALITY + "\".", this.file, node.getLine(), node.getColumn());
                    }
                }
                exp = fCompExp;
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }

    /**
     * Extracts the object structures from the <code>FCOMP</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FCOMP</code> node.
     * @return the comparaison function structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private FCompExp f_comp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTBINARY_COMP) {
                return this.binary_comp(cn);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>BINARY_COMP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>BINARY_COMP</code> node.
     * @return the binray numeric operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private FCompExp binary_comp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTGREATER_COMP:
                return this.greater_comp(cn);
            case LexerTreeConstants.JJTGREATER_EQUAL_COMP:
                return this.greater_equal_comp(cn);
            case LexerTreeConstants.JJTLESS_COMP:
                return this.less_comp(cn);
            case LexerTreeConstants.JJTLESS_EQUAL_COMP:
                return this.less_equal_comp(cn);
            case LexerTreeConstants.JJTEQUAL_COMP:
                return this.equal_comp(cn);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>GREATER_COMP</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>GREATER_COMP</code> node.
     * @return the greater compraison structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private GreaterComp greater_comp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new GreaterComp(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>LESS_COMP</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>LESS_COMP</code> node.
     * @return the less compraison structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private LessComp less_comp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new LessComp(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>EQUAL_COMP</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>EQUAL_COMP</code> node.
     * @return the equal compraison structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private EqualComp equal_comp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new EqualComp(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>GREATER_EQUAL_COMP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>GREATER_EQUAL_COMP</code> node.
     * @return the greater equal compraison structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private GEqualComp greater_equal_comp(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new GEqualComp(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>LESS_EQUAL_COMP</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>LESS_EQUAL_COMP</code> node.
     * @return the less equal compraison structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private LEqualComp less_equal_comp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new LEqualComp(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>F_EXP</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>F_EXP</code> node.
     * @return the fonction expression structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Term f_exp(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTNUMBER:
                return this.number(cn);
            case LexerTreeConstants.JJTOP:
                return this.op(cn);
            case LexerTreeConstants.JJTF_HEAD:
                return this.f_head(cn);
            case LexerTreeConstants.JJTVARIABLE:
                return this.variable(cn);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>F_HEAD</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>F_HEAD</code> node.
     * @return the function head structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private FHead f_head(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() > 0) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            if (cn1.getId() == LexerTreeConstants.JJTFUNCTOR) {
                FHead f_head = new FHead(this.functor(cn1), Type.NUMBER);
                for (int i = 1; i < node.jjtGetNumChildren(); i++) {
                    SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
                    switch (cn.getId()) {
                    case LexerTreeConstants.JJTTERM:
                        f_head.add(this.term(cn));
                        break;
                    default:
                        throw new ParserException("An internal parser error occurs: node "
                                    + cn.getLabel()
                                    + " unexpected.");
                    }
                }
                return f_head;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>OPERATION</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>OPERATION</code> node.
     * @return the operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryOp op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTBINARY_OPERATION:
                return this.binary_operation(cn);
            case LexerTreeConstants.JJTUNARY_OP:
                return this.unary_operation(cn);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>UNARY_OPERATION</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>UNARY_OPERATION</code> node.
     * @return the unary operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryOp unary_operation(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTF_EXP) {
                return new BinarySubstract(new Number(0.0), this.f_exp(cn));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>BINRAY_OPERATION</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>BINRAY_OPERATION</code> node.
     * @return the binray numeric operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryOp binary_operation(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTADD_OP:
                return this.add_op(cn);
            case LexerTreeConstants.JJTSUBSTRACT_OP:
                return this.substract_op(cn);
            case LexerTreeConstants.JJTMULTIPLY_OP:
                return this.multiply_op(cn);
            case LexerTreeConstants.JJTDIVIDE_OP:
                return this.divide_op(cn);
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>ADD_OPERATION</code> node
     * of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ADD_OPERATION</code> node.
     * @return the add operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryAdd add_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new BinaryAdd(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>SUBSTRACT_OPERATION</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>SUBSTRACT_OPERATION</code> node.
     * @return the substract operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinarySubstract substract_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new BinarySubstract(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>MULTIPLY_OPERATION</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>MULTIPLY_OPERATION</code> node.
     * @return the multiply operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryMultiply multiply_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new BinaryMultiply(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>DIVIDE_OPERATION</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>DIVIDE_OPERATION</code> node.
     * @return the divide operation structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private BinaryDivide divide_op(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTF_EXP
                        && cn2.getId() == LexerTreeConstants.JJTF_EXP) {
                return new BinaryDivide(this.f_exp(cn1), this.f_exp(cn2));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>NUMBER</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>NUMBER</code> node.
     * @return the number structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Number number(SimpleNode node) throws ParserException {
        return new Number(Double.valueOf(node.getImage()));
    }

    /**
     * Extracts the object structures from the <code>AND_GD</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>AND_GD</code> node.
     * @return the conjunctive goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AndExp and_gd(SimpleNode node) throws ParserException {
        AndExp exp = new AndExp();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTGD:
                exp.add(this.gd(cn));
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }

    /**
     * Extracts the object structures from the <code>OR_GD</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>OR_GD</code> node.
     * @return the disjunctive goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private OrExp or_gd(SimpleNode node) throws ParserException {
        OrExp exp = new OrExp();
        for (int i = 0; i < node.jjtGetNumChildren(); i++) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTGD:
                exp.add(this.gd(cn));
                break;
            default:
                throw new ParserException(
                            "An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }

    /**
     * Extracts the object structures from the <code>NOT_GD</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>NOT_GD</code> node.
     * @return the negative goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private NotExp not_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTGD) {
                return new NotExp(this.gd(cn));
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>IMPLY_GD</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>IMPLY_GD</code> node.
     * @return the implicative goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ImplyExp imply_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTGD
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                Exp head = this.gd(cn1);
                Exp body = this.gd(cn2);
                return new ImplyExp(head, body);
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>FORALL_GD</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>FORALL_GD</code> node.
     * @return the universal goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ForallExp forall_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTTYPED_LIST
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                ActionDef tmp = this.actionCtx;
                this.actionCtx = null;
                ForallExp forall = new ForallExp();
                LinkedHashMap<String, Variable> vars = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn1, vars);
                this.actionCtx = tmp;
                for (Variable var : vars.values()) {
                    forall.add(var);
                }
                this.quantifiedExpCtx = forall;
                Exp exp = this.gd(cn2);
                forall.setExp(exp);
                this.quantifiedExpCtx = null;
                return forall;
            }
        }
        this.quantifiedExpCtx = null;
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>EXISTS_GD</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>EXISTS_GD</code> node.
     * @return the existential goal description structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private ExistsExp exists_gd(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() == 2) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            SimpleNode cn2 = (SimpleNode) node.jjtGetChild(1);
            if (cn1.getId() == LexerTreeConstants.JJTTYPED_LIST
                        && cn2.getId() == LexerTreeConstants.JJTGD) {
                ActionDef tmp = this.actionCtx;
                this.actionCtx = null;
                ExistsExp exists = new ExistsExp();
                LinkedHashMap<String, Variable> vars = new LinkedHashMap<String, Variable>();
                this.var_typed_list(cn1, vars);
                this.actionCtx = tmp;
                for (Variable var : vars.values()) {
                    exists.add(var);
                }
                this.quantifiedExpCtx = exists;
                Exp exp = this.gd(cn2);
                exists.setExp(exp);
                this.quantifiedExpCtx = null;
                return exists;
            }
        }
        this.quantifiedExpCtx = null;
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>LITERAL</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>LITERAL</code> node.
     * @return the exp literal structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Literal literal(SimpleNode node) throws ParserException {
        Literal exp = null;
        if (node.jjtGetNumChildren() ==  1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTATOMIC_FORMULA:
                exp = this.atomic_formula(cn);
                break;
            case LexerTreeConstants.JJTNOT_ATOMIC_FORMULA:
                exp = this.not_atomic_formula(cn);
                break;
            default:
                throw new ParserException("An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }

    /**
     * Extracts the object structures from the <code>LITERAL</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>LITERAL</code> node.
     * @return the exp literal structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Literal ground_literal(SimpleNode node) throws ParserException {
        Literal exp = null;
        if (node.jjtGetNumChildren() ==  1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTATOMIC_FORMULA:
                exp = this.ground_atomic_formula(cn);
                break;
            case LexerTreeConstants.JJTNOT_ATOMIC_FORMULA:
                exp = this.not_ground_atomic_formula(cn);
                break;
            default:
                throw new ParserException("An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return exp;
    }
    
    /**
     * Extracts the object structures from the <code>LITERAL</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>LITERAL</code> node.
     * @return the exp literal structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Literal not_atomic_formula(SimpleNode node)
                throws ParserException {
        Literal exp = null;
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTATOMIC_FORMULA) {
                return new NotAtomicFormula(this.atomic_formula(cn));
            }   
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>LITERAL</code> node of
     * the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>LITERAL</code> node.
     * @return the exp literal structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Literal not_ground_atomic_formula(SimpleNode node)
                throws ParserException {
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            if (cn.getId() == LexerTreeConstants.JJTATOMIC_FORMULA) {
                return new NotAtomicFormula(this.ground_atomic_formula(cn));
            }   
        }
        throw new ParserException("An internal parser error occurs: node "
                                        + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>ATOMIC_FORMULA</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ATOMIC_FORMULA</code> node.
     * @return the atomic formula structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtomicFormula atomic_formula(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() > 0) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            if (cn1.getId() == LexerTreeConstants.JJTPREDICATE) {
                AtomicFormula atomic_formula = new AtomicFormula(this.predicate(cn1));
                for (int i = 1; i < node.jjtGetNumChildren(); i++) {
                    SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
                    if (cn.getId() == LexerTreeConstants.JJTTERM) {
                        atomic_formula.add(this.term(cn));
                    } else {
                        throw new ParserException("An internal parser error occurs: node "
                                                + cn.getLabel() + " unexpected.");
                    }
                }
                if (!this.obj.getContent().equals(Content.PARTIAL_PROBLEM)) {
                    boolean match = false;
                    Iterator<AtomicFormula> i = obj.predicates.iterator();
                    while (i.hasNext() && !match) {
                        AtomicFormula copy = i.next().clone().standardize();
                        Substitution sigma = copy.match(atomic_formula);
                       match = sigma != null;
                        
                    }
                    if (!match) {
                        this.mgr.logParserWarning("predicate \""
                                    + atomic_formula.toTypedString()
                                    + "\" undefined.", this.file, node
                                    .getLine(), node.getColumn());
                        obj.predicates.add(atomic_formula);
                    }
                }
                return atomic_formula;
            } 
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }

    /**
     * Extracts the object structures from the <code>ATOMIC_FORMULA</code>
     * node of the syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>ATOMIC_FORMULA</code> node.
     * @return the atomic formula structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private AtomicFormula ground_atomic_formula(SimpleNode node) throws ParserException {
        if (node.jjtGetNumChildren() > 0) {
            SimpleNode cn1 = (SimpleNode) node.jjtGetChild(0);
            if (cn1.getId() == LexerTreeConstants.JJTPREDICATE) {
                AtomicFormula atomic_formula = new AtomicFormula(this.predicate(cn1));
                for (int i = 1; i < node.jjtGetNumChildren(); i++) {
                    SimpleNode cn = (SimpleNode) node.jjtGetChild(i);
                    if (cn.getId() == LexerTreeConstants.JJTCONSTANT) {
                        atomic_formula.add(this.constant(cn));
                    } else {
                        throw new ParserException("An internal parser error occurs: node "
                                            + cn.getLabel() + " unexpected.");
                    }
                }
                return atomic_formula;
            }
        }
        throw new ParserException("An internal parser error occurs: node "
                    + node.getLabel() + " unexpected.");
    }
    
    /**
     * Extracts the object structures from the <code>TERM</code> node of the
     * syntaxic tree and implements the semantic check.
     * 
     * @param node the <code>TERM</code> node.
     * @return the exp structure built.
     * @throws ParserException if an error occurs while parsing.
     */
    private Term term(SimpleNode node) throws ParserException {
        Term t = null;
        if (node.jjtGetNumChildren() == 1) {
            SimpleNode cn = (SimpleNode) node.jjtGetChild(0);
            switch (cn.getId()) {
            case LexerTreeConstants.JJTCONSTANT:
                t = this.constant(cn);
                break;
            case LexerTreeConstants.JJTVARIABLE:
                t = this.variable(cn);
                break;
            default:
                throw new ParserException("An internal parser error occurs: node "
                                        + cn.getLabel() + " unexpected.");
            }
        }
        return t;
    }
}
