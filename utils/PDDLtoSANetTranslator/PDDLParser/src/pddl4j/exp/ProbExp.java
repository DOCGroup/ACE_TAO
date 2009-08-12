package pddl4j.exp;

import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;

public class ProbExp extends AbstractExp{
	
	protected Map<Exp, Double> terms;
	
	
	
    public ProbExp() {
        super(ExpID.PROB_EXP);

        terms = new LinkedHashMap<Exp, Double>();
    }
    
    //TODO make this prettier.  Maybe refactor to make a ListExp?
    public void addExp(Exp exp, double prob){
    	
    	if(exp.getExpID() == ExpID.PROB_EXP){
    		
    		ProbExp prob_child = (ProbExp)exp;
    		
    		Iterator<Exp> child_children = prob_child.iterator();
    		
    		while(child_children.hasNext()){
    			Exp toAdd = child_children.next();
    			addExp(toAdd, (prob*prob_child.getProbability(toAdd)));
    		}
    	}
    	else if(exp.getExpID() == ExpID.AND){
    		
    		AndExp and_child = (AndExp)exp;
    		if(and_child.size() == 1){
    			addExp(and_child.iterator().next(), prob);
    		}else if(and_child.size() == 0){
    			
    		}else{
        		terms.put(exp, prob);	
    		}
    	}
    	else{
    		terms.put(exp, prob);
    	}
    }
    
    public void removeExp(Exp exp){
    	terms.remove(exp);
    }
    
    public ProbExp apply(Substitution sigma) {
        
    	ProbExp p = new ProbExp();
    	
    	for(Exp e: terms.keySet()){
    		p.addExp(e.clone().apply(sigma), terms.get(e));
    	}
    	
    	
    	
    	return p;
    }

	@Override
	public Set<Variable> getFreeVariables() {
		
		Set<Variable> vars = new HashSet<Variable>();
		
		for(Exp e: terms.keySet()){
			for(Variable v: e.getFreeVariables()){
				vars.add(v);
			}
		}

		return vars;
	}

	@Override
	public boolean isGround() {
		
		for(Exp e: terms.keySet()){
			if(!e.isGround()){
				return false;
			}
		}
		
		return true;
	}

	@Override
	public boolean occurs(Term term) {
		
		for(Exp e: terms.keySet()){
			if(e.occurs(term)){
				return true;
			}
		}
		
		return false;
	}

	@Override
	public Exp standardize() {
		

		return this.standardize(new LinkedHashMap<String, String>());
	}

	@Override
	public Exp standardize(Map<String, String> images) {

		ProbExp p = new ProbExp();
		
		for(Exp e: terms.keySet()){
			p.addExp(e.standardize(images), terms.get(e));
		}
		
		return p;
	}

	public String toString(){
		
		String s = "(probabilistic ";
		for(Exp e: terms.keySet()){
			s+=terms.get(e)+" "+e.toString()+" ";
		}
		
		return s;
	}
	
	@Override
	public String toTypedString() {
		
		String s = "(probabilistic ";
		for(Exp e: terms.keySet()){
			s+=terms.get(e)+" "+e.toTypedString()+" ";
		}
		
		return s;
	}
	
	public Iterator<Exp> iterator(){
		return terms.keySet().iterator();
	}
	
	public double getProbability(Exp e){
		return terms.get(e);
	}
	
	public ProbExp clone(){
		
		ProbExp clone = new ProbExp();
		
		for(Exp e: terms.keySet()){
			clone.addExp(e.clone(), terms.get(e));
		}
		
		return clone;
	}
	
	public int size(){
		return terms.size();
	}
	
    
//    
//    public final void setExp(Exp exp) {
//        if (exp == null)
//            throw new NullPointerException();
//        this.exp = exp;
//    }
//    
//    public Exp getExp() {
//        return this.exp;
//    }
//    
//    public final boolean occurs(Term term) {
//        if (term == null)
//            throw new NullPointerException();
//        return this.exp.occurs(term);
//    }
//    
//    public NotExp apply(Substitution sigma) {
//        if (sigma == null)
//            throw new NullPointerException();
//        return new NotExp(this.exp.apply(sigma));
//    }
//    
//    public NotExp standardize() {
//        return this.standardize(new LinkedHashMap<String, String>());
//    }
//    
//    public NotExp standardize(Map<String, String> images) {
//        if (images == null)
//            throw new NullPointerException();
//        return new NotExp(this.exp.standardize(images));
//    }
//    
//    public final boolean isGround() {
//        return this.exp.isGround();
//    }
//    
//    public boolean equals(Object obj) {
//        if (obj != null && obj instanceof ProbExp) {
//            ProbExp other = (ProbExp) obj;
//            return this.getExpID().equals(other.getExpID())
//                        && this.exp.equals(other.exp)
//                        && this.probability == other.probability;
//        }
//        return false;
//    }
//    
//    public int hashCode() {
//        return this.getExpID().hashCode() + this.exp.hashCode();
//    }
//    
//    public ProbExp clone() {
//        ProbExp other = (ProbExp) super.clone();
//        other.exp = this.exp.clone();
//        other.probability = this.probability;
//        return other;
//    }
//    
//    public Exp moveQuantifierOutward() {
////        Exp exp = null;
////        ExistsExp exists = null;
////        ForallExp forall = null;
////        
////        switch(this.exp.getExpID()) {
////        case EXIST:
////            exists = (ExistsExp) this.exp;
////            forall = new ForallExp();
////            for (Variable var : exists) {
////                forall.add(var.clone());
////            }
////            forall.setExp(new NotExp(exists.getExp()).moveQuantifierOutward());
////            exp = forall.moveQuantifierOutward();
////            break;
////        case FORALL:
////            forall = (ForallExp) this.exp;
////            exists = new ExistsExp();
////            for (Variable var : forall) {
////                exists.add(var.clone());
////            }
////            exists.setExp(new NotExp(forall.getExp()).moveQuantifierOutward());
////            exp = exists.moveQuantifierOutward();
////            break;
////        default:
////            exp = this.clone();
////        }
//        return exp; 
//    }
//    
//    public OrExp toDisjunctiveNormalForm() {
//            OrExp dnf = new OrExp();
//            dnf.add(this.clone());
//
//        return dnf;
//    }
//    
//    public AndExp toConjunctiveNormalForm() {
//  
//        AndExp cnf = new AndExp();
//        cnf.add(this.clone());
//        return cnf;
//    }
//    public Exp toNegativeNormalForm() {
//    	return this.clone();
//    }
//    
//    public Set<Variable> getFreeVariables() {
//        return this.exp.getFreeVariables();
//    }
//    
//    public String toString() {
//        StringBuffer str = new StringBuffer();
//        str.append("(probabilistic ");
//        str.append(this.probability+" ");
//        str.append(this.exp.toString());
//        str.append(")");
//        return str.toString();
//    }
//    
//    public String toTypedString() {
//        StringBuffer str = new StringBuffer();
//        str.append("(probabilistic ");
//        str.append(this.probability+" ");
//        str.append(this.exp.toTypedString());
//        str.append(")");
//        return str.toString();
//    }
}