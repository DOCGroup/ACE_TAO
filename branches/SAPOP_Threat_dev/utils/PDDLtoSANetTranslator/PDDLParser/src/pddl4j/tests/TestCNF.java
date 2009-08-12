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

import pddl4j.exp.AndExp;
import pddl4j.exp.AtomicFormula;
import pddl4j.exp.ExistsExp;
import pddl4j.exp.Exp;
import pddl4j.exp.ForallExp;
import pddl4j.exp.ImplyExp;
import pddl4j.exp.NotAtomicFormula;
import pddl4j.exp.NotExp;
import pddl4j.exp.OrExp;
import pddl4j.exp.term.Constant;
import pddl4j.exp.term.Variable;
import pddl4j.exp.type.Type;

public class TestCNF {

    /**
     * @param args
     */
    public static void main(String[] args) {
        
        // (and (forall (?x) (p1 ?x ?y)) (p2 ?x ?y))
        
        Variable x = new Variable("?x");
        Variable y = new Variable("?y");
        
        AtomicFormula p1 = new AtomicFormula("p1");
        p1.add(x);
        p1.add(y);
        
        AtomicFormula p2 = new AtomicFormula("p2");
        p2.add(x);
        p2.add(y);
        
        ForallExp forall = new ForallExp();
        forall.add(x);
        forall.setExp(p1);
        
        AndExp and = new AndExp();
        and.add(forall);
        and.add(p2);
        
        System.out.println(and);
        System.out.println(and.standardize());
        //System.out.println(and.getFreeVariables());
        
        AndExp a2 = new AndExp();
        a2.add(p1);
        a2.add(p2);
        
        //System.out.println(a2);
        //System.out.println(a2.standardize());
        
        
        ExistsExp e1 = new ExistsExp();
        e1.add(y);
        e1.setExp(p1);
        
        ForallExp f1 = new ForallExp();
        f1.add(x);
        f1.setExp(e1);
        
        ExistsExp e2 = new ExistsExp();
        e2.add(x);
        e2.add(y);
        e2.setExp(p2);
        
        AndExp a1 = new AndExp();
        a1.add(f1);
        a1.add(e2);
        
        //System.out.println(a1);
        //System.out.println(a1.standardize());
        
        
        
        //System.out.println(a1.getFreeVariables());
        
        
        
        /*Variable x = new Variable("?x");
        Variable y = new Variable("?y");
        Variable z = new Variable("?z");
        
        AtomicFormula animal = new AtomicFormula("animal");
        animal.add(y);
        
        AtomicFormula loves1 = new AtomicFormula("loves");
        loves1.add(x);
        loves1.add(y);
        
        System.out.println(loves1);
        System.out.println(loves1.normalize());
        
        
        /*AtomicFormula loves2 = new AtomicFormula("loves");
        loves2.add(y);
        loves2.add(x);
        
        ExistsExp e1 = new ExistsExp();
        e1.add(y);
        e1.setExp(loves2);
        
        ImplyExp e3 = new ImplyExp(animal, loves1);
        
        ForallExp e2 = new ForallExp();
        e2.add(y);
        e2.setExp(e3);
        
        ImplyExp e4 = new ImplyExp(e2, e1);
        
        ForallExp e5 = new ForallExp();
        e5.add(x);
        e5.setExp(e4);
        
        ForallExp e7 = new ForallExp();
        e7.add(x);
        e7.setExp(loves2);
        
        
        AndExp e6 = new AndExp();
        e6.add(e7);
        e6.add(loves1);
        e6.add(e1);
        
        //System.out.println(e6);
        //Exp e8 = e6.toPNF();
        //System.out.println(e8);
        //System.out.println(e8.toPNF());
        
        
        
        
        
        System.out.println("INIT: \n" + e5);
        System.out.println(e5.standardize());
        System.out.println(e5.standardize().moveQuantifierOutward());
        //System.out.println(e5.standardize().moveQuantifierOutward().toNNF());
        //System.out.println(e5.toNNF());
        Exp e9 = e5.toPrenexNormaForm();
        System.out.println("PNF:\n" + e9);
        //System.out.println(e9.toNNF());
        System.out.println("CNF:\n" + e9.toConjunctiveNormalForm());
        
        
        /*Variable v1 = new Variable("v1");
        Variable v2 = new Variable("v2");
        
        Constant c1 = new Constant("c1");
        Constant c2 = new Constant("c2");
        
        AtomicFormula a1 = new AtomicFormula("a1");
        a1.add(v1);
        a1.add(v2);
        
        AtomicFormula a2 = new AtomicFormula("a2");
        a2.add(c1);
        a2.add(c2);
        
        NotAtomicFormula nota2 = new NotAtomicFormula(a2);
        
        AndExp and = new AndExp();
        and.add(a1);
        and.add(nota2);
        
        OrExp or = new OrExp();
        or.add(a1);
        or.add(nota2);
        
        
        
        ForallExp forall = new ForallExp(or);
        forall.add(v1);
        forall.add(v2);
        
        NotExp not = new NotExp(forall);
        
        System.out.println(not);
        System.out.println(not.standardize());*/
        
        /*AtomicFormula A = new AtomicFormula("A");
        AtomicFormula B = new AtomicFormula("B");
        AtomicFormula C = new AtomicFormula("C");
        AtomicFormula D = new AtomicFormula("D");
        AtomicFormula E = new AtomicFormula("E");
        AtomicFormula F = new AtomicFormula("F");
        
        
        AndExp AetB = new AndExp();
        AetB.add(A);
        AetB.add(B);
        
        AndExp CetD = new AndExp();
        CetD.add(C);
        CetD.add(D);
        
        OrExp CouD = new OrExp();
        CouD.add(C);
        CouD.add(D);
        
        OrExp AouB = new OrExp();
        AouB.add(A);
        AouB.add(B);
        
        
        AndExp AouBetCouD = new AndExp();
        AouBetCouD.add(AouB);
        AouBetCouD.add(CouD);
        
        OrExp AetBouCetD = new OrExp();
        AetBouCetD.add(AetB);
        AetBouCetD.add(CetD);
        
        OrExp AetBouCouD = new OrExp();
        AetBouCouD.add(AetB);
        AetBouCouD.add(CouD);
        
        AndExp and1 = new AndExp();
        and1.add(F);
        and1.add(AetBouCetD);
        //and1.add(F);
        
        ImplyExp AimplyAnd1 = new ImplyExp(A, and1);
        
        NotExp notE = new NotExp(E);
        
        
        //and2.add(F);
        
        AndExp and3 = new AndExp();
        and3.add(A);
        and3.add(CouD);
        
        AndExp and2 = new AndExp();
        and2.add(F);
        and2.add(and3);
        
        
        System.out.println("IN:  "+ AimplyAnd1);
        System.out.println("IN:  "+ AimplyAnd1.standardize());
        System.out.println("DNF: "+ AimplyAnd1.toDNF());
        
        /*System.out.println("IN:  "+ and1);
        Exp nnf = and1.toNNF();
        System.out.println("NNF: " + nnf);
        Exp cnf = nnf.toDNF();
        System.out.println("CNF: " + cnf);*/
        
    }

}
