package Translator;

import java.util.List;

public class InfoSet {

	public int total_tasks;
	public int total_conds;
	public int total_preconds;
	public int total_effects;
	public List<Integer> effects_per_cond;
	public List<Integer> preconds_per_cond;
	public List<Integer> effects_per_act;
	public List<Integer> preconds_per_act;
	public List<Integer> satisfiers_per_precond;
	public List<Integer> threats_per_precond;
	public double avg_effects_per_cond;
	public double avg_preconds_per_cond;
	public double avg_effects_per_act;
	public double avg_preconds_per_act;
	public double avg_satisfiers_per_precond;
	public double avg_threats_per_precond;
	public double std_dev_effects_per_cond;
	public double std_dev_preconds_per_cond;
	public double std_dev_effects_per_act;
	public double std_dev_preconds_per_act;
	public double std_dev_satisfiers_per_precond;
	public double std_dev_threats_per_precond;
	public double reverse_link_prob;
	public double clustering_coeff;

}
