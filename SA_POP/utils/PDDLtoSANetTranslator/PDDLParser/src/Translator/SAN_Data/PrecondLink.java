package Translator.SAN_Data;

public class PrecondLink{
	
	final public CondNode precondition;
	final public TaskNode action;
	
	final public boolean required_state;
	
	double true_prob;
	double false_prob;
	
	String portID = "";
	
	public PrecondLink(CondNode p_precond, TaskNode p_action, boolean required){
	
		precondition = p_precond;
		action = p_action;
		required_state = required;
		
		p_precond.attach_precondition_link(this);
	}
	
	public String getCondID(){
		return precondition.getNodeID();
	}
	
	public String getTaskID(){
		return action.getNodeID();
	}
	
	public double getTrueProb(){
		return true_prob;
	}
	
	public double getFalseProb(){
		return false_prob;
	}
	
	public boolean getRequiredState(){
		return required_state;
	}
	
	public void setProbability(double prob){
		
		if(required_state){
			true_prob = prob;
			false_prob = 0;
		}else{
			true_prob = 0;
			false_prob = prob;
		}
	}
	
	public String getPortID(){
		return portID;
	}
	
	public String toString(){
		return ("Precondition link from: ["+ precondition.name+ "] to ["+ action.name + "] t/f: "+ true_prob+ " "+ false_prob);
	}
}