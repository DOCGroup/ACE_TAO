package Translator.SAN_Data;

import java.util.LinkedList;
import java.util.List;


/**
 * Action node
 * @author Ben
 *
 */
public class TaskNode{
	
	private static int id_count = 0;
	
	final String name;
	final String nodeID;
	
	String attenFactor = "";
	String cost = "";
	
//	private List<PrecondLink> preconditions;
	private List<PrecondLink> positive_preconditions;
	private List<PrecondLink> negative_preconditions;
	
	private List<EffectLink> negative_effects;
	private List<EffectLink> positive_effects;
	
	
	public TaskNode(String p_name){
		
		name = p_name;
		nodeID = "2"+(id_count++);
	
		positive_preconditions = new LinkedList<PrecondLink>();
		negative_preconditions = new LinkedList<PrecondLink>();
		
		positive_effects = new LinkedList<EffectLink>();
		negative_effects = new LinkedList<EffectLink>();
	}
	
	public void destroy(){
		
		for(PrecondLink l: positive_preconditions){
			l.precondition.getPreconditionTo().remove(l);
		}
		
		for(PrecondLink l: negative_preconditions){
			l.precondition.getPreconditionTo().remove(l);
		}
		
		for(EffectLink l: positive_effects){
			l.effect.getAffectedBy().remove(l);
		}
		
		for(EffectLink l: negative_effects){
			l.effect.getAffectedBy().remove(l);
		}
		
	}
	
	public void set_precondition_probabilities(){
		
		int links = positive_preconditions.size()+negative_preconditions.size();

		for(PrecondLink l: positive_preconditions){
			l.setProbability(Math.pow(.5, links-1));
		}

		for(PrecondLink l: negative_preconditions){
			l.setProbability(Math.pow(.5, links-1));
		}
	}
	
	public PrecondLink attach_precondition(CondNode n, boolean required_state){
		PrecondLink plk = new PrecondLink(n, this, required_state);
		
		if(required_state){
			positive_preconditions.add(plk);
		}
		else{
			negative_preconditions.add(plk);
		}
		
		return plk;
	}
	
	public EffectLink attach_effect(CondNode n, double weight){
		EffectLink elk = new EffectLink(this, n, weight);
		
		if(weight > 0){
			positive_effects.add(elk);
		}else{
			negative_effects.add(elk);
		}
		
		return elk;
	}
	
	public String getName(){
		return name;
	}
	
	public String getNodeID(){
		return nodeID;
	}
	
	public String getAttenFactor(){
		return attenFactor;
	}
	
	public String getCost(){
		return cost;
	}
	
//	public List<PrecondLink> get_precondition_links(){
//		return preconditions;
//	}
	
	public List<PrecondLink> get_positive_precondition_links(){
		return positive_preconditions;
	}
	
	public List<PrecondLink> get_negative_precondition_links(){
		return negative_preconditions;
	}
	
	public List<EffectLink> get_positive_effect_links(){
		return positive_effects;
	}
	
	public List<EffectLink> get_negative_effect_links(){
		return negative_effects;
	}
	
	public double getPriorProb(){
		return Math.pow(.5, positive_preconditions.size()+ negative_preconditions.size());
	}
	
	public String toString(){
		String s =  ("Action node: ["+ name+"] ID: "+nodeID);
		
		for(PrecondLink l: positive_preconditions){
			s+=("\n   "+l.toString());
		}
		
		for(PrecondLink l: negative_preconditions){
			s+=("\n   "+l.toString());
		}
		
		for(EffectLink l: positive_effects){
			s+=("\n   "+l.toString());
		}
		
		for(EffectLink l: negative_effects){
			s+=("\n   "+l.toString());
		}
		
		return s;
	}
}