package Translator.SAN_Data;

import java.util.LinkedList;
import java.util.List;


/**
 * Boolean state variable
 * @author Ben
 *
 */
public class CondNode{
	
	private static int id_count = 0;
	
	final String name;
	final String nodeID;
	
	double probTrue;
	double utility;
	
	String kind = "ENVIRON";
	
	String attenFactor ="";

	private List<EffectLink> affected_by;
	private List<PrecondLink> precondition_to;
	
	public CondNode(String p_name, double p_initial_probability, double p_init_util){
		
		name = p_name;
		nodeID = ("1"+id_count++);
		probTrue = p_initial_probability;
		utility = p_init_util;
		
		affected_by = new LinkedList<EffectLink>();
		precondition_to = new LinkedList<PrecondLink>();
	}
	
	public void destroy(){
		for(EffectLink l: affected_by){
			if(l.getWeight() > 0){
				l.action.get_positive_effect_links().remove(l);
			}else{
				l.action.get_negative_effect_links().remove(l);
			}
		}
		
		for(PrecondLink l: precondition_to){
			if(l.getRequiredState()){
				l.action.get_positive_precondition_links().remove(l);
			}else{
				l.action.get_negative_precondition_links().remove(l);
			}
		}
	}
	
	public List<EffectLink> getAffectedBy(){
		return affected_by;
	}
	
	public List<PrecondLink> getPreconditionTo(){
		return precondition_to;
	}
	
	public void setProbability(double prob){
		probTrue = prob;
	}
	
	public String getNodeID(){
		return nodeID;
	}
	
	public String getName(){
		return name;
	}
	
	public double getProbability(){
		return probTrue;
	}
	
	public double getUtility(){
		return utility;
	}
	
	
	public String getKind(){
		return kind;
	}
	
	public String getAttenFactor(){
		return attenFactor;
	}
	
	public String toString(){
		return ("Condition node: ["+name + "] ID: "+ nodeID +" Probability: "+ probTrue+ " Utility: "+ utility);
	}
	
	public void attach_precondition_link(PrecondLink l){
		precondition_to.add(l);
	}
	
	public void attach_effect_link(EffectLink l){
		affected_by.add(l);
	}
}