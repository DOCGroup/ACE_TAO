package Translator.SAN_Data;

public class EffectLink{
	
	final public TaskNode action;
	final public CondNode effect;
	
	final double weight;
	
	String portID ="";
	
	public EffectLink(TaskNode p_action, CondNode p_effect, double p_weight){
		
		action = p_action;
		effect = p_effect;
		weight = p_weight;
		
		p_effect.attach_effect_link(this);
	}
	
	public String getTaskID(){
		return action.getNodeID();
	}
	
	public String getCondID(){
		return effect.getNodeID();
	}
	
	public String getPortID(){
		return portID;
	}
	
	public double getWeight(){
		return weight;
	}
	
	public String toString(){
		return ("Effect link from: ["+action.name+"] to ["+ effect.name + "] Weight: "+weight);
	}
	
	public boolean sameSource(EffectLink l){
		return this.action == l.action && this.weight == l.weight;
	}
	
	public boolean oppositeSource(EffectLink l){
		return this.action == l.action && this.weight != l.weight;
	}
	
//	/**
//	 * Note! this does not  
//	 */
//	public boolean equals(Object obj){
//		
//		if(!(obj instanceof EffectLink)){
//			return false;
//		}
//		
//		EffectLink other_link = (EffectLink)obj;
//		
//		return (other_link.action == action && other_link.weight == weight);
//		
//	}
}