package Translator;

import java.io.FileWriter;
import java.io.StringWriter;
import java.util.Collection;
import java.util.Date;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import Translator.SAN_Data.CondNode;
import Translator.SAN_Data.EffectLink;
import Translator.SAN_Data.PrecondLink;
import Translator.SAN_Data.TaskNode;
import Translator.TM_Data.ImplToResource;
import Translator.TM_Data.Resource;
import Translator.TM_Data.TaskImpl;
import Translator.TM_Data.TaskToImpl;

import pddl4j.PDDLObject;
import pddl4j.exp.AndExp;
import pddl4j.exp.AtomicFormula;
import pddl4j.exp.Exp;
import pddl4j.exp.ExpID;
import pddl4j.exp.InitEl;
import pddl4j.exp.NotExp;
import pddl4j.exp.OrExp;
import pddl4j.exp.ProbExp;
import pddl4j.exp.WhenExp;
import pddl4j.exp.action.Action;
import pddl4j.exp.action.ActionDef;
import pddl4j.exp.fcomp.EqualComp;
import pddl4j.exp.term.Constant;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;

import org.apache.commons.collections.MultiMap;
import org.apache.commons.collections.map.MultiValueMap;
import org.w3c.dom.*;

import javax.xml.parsers.*;

import javax.xml.transform.*;
import javax.xml.transform.dom.*;
import javax.xml.transform.stream.*;


public class PDDL_Translator {
	
	StatisticsReport stat_report;

	Map<String, CondNode> condition_nodes = new LinkedHashMap<String, CondNode>();
	Map<String, TaskNode> task_nodes= new LinkedHashMap<String, TaskNode>();
	
	List<PrecondLink> precondition_links = new LinkedList<PrecondLink>();
	List<EffectLink> effect_links = new LinkedList<EffectLink>();
	
	Map<Type, List<Constant> > typeset_to_constant= new LinkedHashMap<Type, List<Constant> >();
	Map<String, Boolean> goal_states = new LinkedHashMap<String, Boolean>();
	
	Map<String, TaskImpl> task_impls = new LinkedHashMap<String, TaskImpl>();
	Map<String, Resource> resources = new LinkedHashMap<String, Resource>();
	
	List<TaskToImpl> task_to_impl_links = new LinkedList<TaskToImpl>();
	List<ImplToResource> impl_to_resource_links = new LinkedList<ImplToResource>();
	
	Map<TaskNode, List<String> > task_to_resources = new LinkedHashMap<TaskNode, List<String> >();
	
	boolean init_act_visible;
	
	/**
	 * 
	 * @param pddl_object
	 * 
	 * The object the parser uses to extract the SAN and TM
	 * 
	 * @param use_initial_action
	 * 
	 * Include an initial action, with effects to all conditions, in the SAN
	 */
	public PDDL_Translator(PDDLObject pddl_object, int condition_combine_levels, boolean use_initial_action){
		
		stat_report = new StatisticsReport(pddl_object.getDomainName(), pddl_object.getProblemName());
		
		long start_time = new Date().getTime();
		
		init_act_visible = use_initial_action;
		
		List<Action> action_list;
		List<ActionData> action_data;

		//Even if we don't use init act, we can't give SA-POP an action named 20.
		TaskNode init_act = new TaskNode("initact");
		
		//First, maps constants to their types; second, creates a resource for every constant
		record_constants(pddl_object);
		
		//Read in the goals and remember what they are.
		get_goals(pddl_object);

		//Creates conditions for every proposition, sets them all false at first
		enumerate_propositions_default(pddl_object);
		
    	//Now replace the ones that were asserted true in the problem
		set_initial_propositions(pddl_object);
     
		//Get all the initial actions
		action_list = get_all_actions(pddl_object);
      
		//Splices when constructs out of the effects and turns them into multiple actions
		action_list = remove_conditionals(action_list);
		
		//Turns the preconditions to disjunctive normal form and splits the terms into multiple
		//actions
        action_list = remove_precondition_disjunctions(action_list);
		
        //Not sure if this step is necessary, but goes through and normalizes stuff, mainly to get
        //rid of double nested not clauses.
        action_list = convert_preconditions_to_prenex(action_list);
        
        //Go through and look at the equality/inequality constraints remaining.  
        action_data = parse_equality_constraints(action_list);
        
       
        
        //Make sure that there aren't untranslatable probabilities encoded
        if(!are_probabilistics_translatable(action_list)){
        	System.out.println("Probabilistics this complex cannot currently be translated to a SANet");
        	System.exit(0);
        }

        //Propositionalize all actions
        enumerate_actions(action_data);
        

               
        //Add the initial action in as an anchor--otherwise it would seem that none
        //of the conditions can come true
        setup_initial_action(init_act);

    //    calculateNetworkStatistics("before_impossibility_removal");

        System.out.println("Before optimizing: "+ this.task_nodes.size()+" tasks "+ this.condition_nodes.size()+" conditions");
        System.out.println("Elapsed: "+ (new Date().getTime()-start_time));
        
        boolean keep_looping;
        do {
        	
	        //Propagate up through all impossible tasks and get rid of ones that can never
	        //come true
	        keep_looping = eliminate_impossible_tasks();
	
	        //Basically cut down through the tasks and conditions and only take the relevant
	        //ones 
	  //      keep_looping = eliminate_irreleevant_tasks_and_conditions()||keep_looping;
	        //Get rid of tasks that take plan backwards and where getting there
	        //helps nothing else
	 ////////////       keep_looping = eliminate_dominated_tasks()||keep_looping;
        } while(keep_looping);
        
        //Go through and look to see if any conditions can be replaced by a set of other
        //conditions
        //TODO maybe try something like a kmap here?
        

   //     calculateNetworkStatistics("before_cond_combining");

        combine_conditions(condition_combine_levels);
        
 //       calculateNetworkStatistics("after_cond_combining");
        
        System.out.println("End elapsed: "+ (new Date().getTime()-start_time));
        
        //Give preconditions links the proper probabilities
        set_probabilities();
        
        //Make task impls for every task
        record_task_impls();
	}
	
	private void calculateNetworkStatistics(String label){
		
		System.out.println();
		System.out.println("Network stats at: "+label);
		System.out.println("   Number of conditions: "+this.condition_nodes.size());
		System.out.println("   Number of tasks: "+ this.task_nodes.size());
		System.out.println();
		
		int total_precond_links = 0;
		int num_sat_links = 0;
		
		int num_threat_links = 0;
		
		int total_effect_links = 0;
		
//		int[] effects_per_cond = new int[condition_nodes.size()];
//		int[] preconds_per_cond = new int[condition_nodes.size()];
//		int[] effects_per_task = new int[task_nodes.size()-1];
//		int[] preconds_per_task = new int[task_nodes.size()-1];
		
		List<Integer> effects_per_cond = new LinkedList<Integer>();
		List<Integer> preconds_per_cond = new LinkedList<Integer>();
		List<Integer> effects_per_task = new LinkedList<Integer>();
		List<Integer> preconds_per_task = new LinkedList<Integer>();
		
		List<Integer> ways_to_sat_each_precond = new LinkedList<Integer>();
		List<Integer> threats_to_each_precond = new LinkedList<Integer>();
		
		int count = 0;
		for(CondNode cnd: this.condition_nodes.values()){

			int num_pos_effect = 0;
			int num_neg_effect = 0;
			
			for(EffectLink el: cnd.getAffectedBy()){
				
				if(el.action.getName().equals("initact")){
					continue;
				}
				
				if(el.getWeight() == 1){
					num_pos_effect++;
				}else if(el.getWeight() == -1){
					num_neg_effect++;
				}
				
				total_effect_links++;
			}
			
			for(PrecondLink pl: cnd.getPreconditionTo()){
				if(pl.getRequiredState()){
					num_sat_links+=num_pos_effect;
					ways_to_sat_each_precond.add(num_pos_effect);
					
					num_threat_links+=num_neg_effect;
					threats_to_each_precond.add(num_neg_effect);
				}else{
					num_sat_links+=num_neg_effect;
					ways_to_sat_each_precond.add(num_neg_effect);
					
					num_threat_links+=num_neg_effect;
					threats_to_each_precond.add(num_pos_effect);
				}
				
				total_precond_links++;
			}
			
			preconds_per_cond.add(cnd.getPreconditionTo().size());
			effects_per_cond.add(cnd.getAffectedBy().size());
			
			count++;
		}
		
		count = 0;
		for(TaskNode tnd: this.task_nodes.values()){
			
			if(tnd.getName().equals("initact")){
				continue;
			}
			
			effects_per_task.add(tnd.get_negative_effect_links().size()+tnd.get_positive_effect_links().size());
			preconds_per_task.add(tnd.get_positive_precondition_links().size()+tnd.get_negative_precondition_links().size());
			count++;
		}
		
		InfoSet info_set = new InfoSet();
		
		info_set.total_tasks = this.task_nodes.size() - 1;
		info_set.total_conds = this.condition_nodes.size();
		info_set.total_preconds = this.precondition_links.size();
		info_set.total_effects = this.effect_links.size();
		
		info_set.effects_per_cond = effects_per_cond;
		info_set.preconds_per_cond = preconds_per_cond;
		info_set.effects_per_act = effects_per_task;
		info_set.preconds_per_act = preconds_per_task;
		info_set.satisfiers_per_precond = ways_to_sat_each_precond;
		info_set.threats_per_precond = threats_to_each_precond;
		
		info_set.avg_effects_per_cond = total_effect_links/(double)condition_nodes.size();
		info_set.avg_preconds_per_cond = total_precond_links/(double)condition_nodes.size();
		info_set.avg_effects_per_act = total_effect_links/(double)task_nodes.size();
		info_set.avg_preconds_per_act = total_effect_links/(double)task_nodes.size();
		info_set.avg_satisfiers_per_precond = num_sat_links/(double)total_precond_links;
		info_set.avg_threats_per_precond = num_threat_links/(double)total_precond_links;
		
		info_set.std_dev_effects_per_cond = get_std_dev(info_set.avg_effects_per_cond, effects_per_cond);
		info_set.std_dev_preconds_per_cond = get_std_dev(info_set.avg_preconds_per_cond, preconds_per_cond);
		info_set.std_dev_effects_per_act = get_std_dev(info_set.avg_effects_per_act, effects_per_task);
		info_set.std_dev_preconds_per_act = get_std_dev(info_set.avg_preconds_per_act, preconds_per_task);
		info_set.std_dev_satisfiers_per_precond = get_std_dev(info_set.avg_satisfiers_per_precond , ways_to_sat_each_precond);
		info_set.std_dev_threats_per_precond = get_std_dev(info_set.avg_threats_per_precond, threats_to_each_precond);
		
		//Find the probability that an arbitrary precondition has an effect opposite it
		
		double precond_with_negation = 0;
		for(PrecondLink el: this.precondition_links){
			TaskNode tnd = el.action;
			
			List<EffectLink> all_effects = new LinkedList<EffectLink>(tnd.get_negative_effect_links());
			all_effects.addAll(tnd.get_positive_effect_links());
			
			for(EffectLink elk: all_effects){
				if(elk.effect  == el.precondition && el.getRequiredState() != (elk.getWeight() == 1)){
					precond_with_negation++;
					break;
				}
			}
		}
		
		info_set.reverse_link_prob = precond_with_negation/this.precondition_links.size();
	//	System.out.println(info_set.reverse_link_prob);
		
		//TODO generate reverse link prob
		//TODO generate clustering coeff
		//TODO diameter!
		
	//	if(!label.equals("before_impossibility_removal")){
		
			System.out.println("Generating simplified graph");
			Map<String, SimpleNode> simple_graph = generate_simplified_graph();
			System.out.println("Done generating simplified graph");


			List<Double> connectednesses = new LinkedList<Double>();
			
			for(SimpleNode sn: simple_graph.values()){
				double connected = 0;
				
				for(SimpleNode sn_child: sn.neighbors){
					for(SimpleNode sn_child_check: sn.neighbors){
						if(sn_child == sn_child_check){
							continue;
						}
						if(sn_child.neighbors.contains(sn_child_check)){
							connected++;
						}
					}
				}
			//	System.out.println("Connected: "+connected);
			//	System.out.println("Neighbors: "+sn.neigbors.size());
			//	if(sn.neigbors.size() > 1)
				connectednesses.add((connected)/(sn.neighbors.size()*(sn.neighbors.size()-1)));
			//	System.out.println((connected)/(sn.neigbors.size()*(sn.neigbors.size()-1)));
			}
			
			double sum = 0;
			
			for(Double d: connectednesses){
				sum+=d;
			}
			info_set.clustering_coeff = sum/connectednesses.size();
	//	}
			
		stat_report.reports.put(label, info_set);
	}
	
	public StatisticsReport getStatReport(){
		return stat_report;
	}
	
	private Map<String, SimpleNode> generate_simplified_graph(){
		Map<String, SimpleNode> node_name_to_simple = new LinkedHashMap<String, SimpleNode>();
		
		for(String tn: this.task_nodes.keySet()){
			
			if(tn.equals("initact")){
				continue;
			}
			
			node_name_to_simple.put(task_nodes.get(tn).getName(), new SimpleNode(tn));
		}
		
		for(TaskNode tn: this.task_nodes.values()){
			
			if(tn.getName().equals("initact")){
				continue;
			}

			List<EffectLink> all_effects = new LinkedList<EffectLink>(tn.get_negative_effect_links());
			all_effects.addAll(tn.get_positive_effect_links());
			
			for(EffectLink elk: all_effects){
				CondNode condition = elk.effect;
				
				//All precond from cond
				for(PrecondLink pl: condition.getPreconditionTo()){
					TaskNode tn2 = pl.action;
					if(tn2 == tn || tn2.getName().equals("initact")){
						continue;
					}
					

					SimpleNode.connect(node_name_to_simple.get(tn.getName()), node_name_to_simple.get(tn2.getName()));
				}
					
				//All effect from cond
				for(EffectLink el: condition.getAffectedBy()){
					TaskNode tn2 = el.action;
					if(tn2 == tn || tn2.getName().equals("initact")){
						continue;		
					}
					
		//			System.out.println(tn2.getNodeID());
		//			System.out.println(tn.getName());
		//			System.out.println(tn2.getName());
					SimpleNode.connect(node_name_to_simple.get(tn.getName()), node_name_to_simple.get(tn2.getName()));
				}
			}
			
			List<PrecondLink> all_preconds = new LinkedList<PrecondLink>(tn.get_negative_precondition_links());
			all_preconds.addAll(tn.get_positive_precondition_links());
			
			for(PrecondLink plk: all_preconds){
				CondNode condition  = plk.precondition;
				
				//All precond from cond
				for(PrecondLink pl: condition.getPreconditionTo()){
					TaskNode tn2 = pl.action;
					if(tn2 == tn || tn2.getName().equals("initact")){
						continue;
					}
					
					SimpleNode.connect(node_name_to_simple.get(tn.getName()), node_name_to_simple.get(tn2.getName()));
				}
				
				//All effect from cond
				for(EffectLink el: condition.getAffectedBy()){
					TaskNode tn2 = el.action;
					if(tn2 == tn || tn2.getName().equals("initact")){
						continue;
					}
					
					SimpleNode.connect(node_name_to_simple.get(tn.getName()), node_name_to_simple.get(tn2.getName()));
				}
			}
		}
		
		return node_name_to_simple;
	}
	
	private double get_std_dev(double mean, int[] data){

		double sum_squares = 0;
		for(int i = 0; i < data.length; i++){
			sum_squares+=Math.pow(mean-data[i], 2);
		}
		double avg = sum_squares/data.length;
		return Math.sqrt(avg);
	}
	
	private double get_std_dev(double mean, List<Integer> data){
		double sum_squares = 0;
		for(Integer theint: data){
			sum_squares+=Math.pow(mean-theint, 2);
		}
		double avg = sum_squares/data.size();
		return Math.sqrt(avg);
	}
	
	MultiMap opp_effect_same_task;
	MultiMap common_task;
	
	private void combine_conditions(int max_search){
		
		long intv1 = new Date().getTime();
		
		opp_effect_same_task = new MultiValueMap();
		common_task = new MultiValueMap();
		
		for(TaskNode tn: this.task_nodes.values()){
			for(EffectLink efln: tn.get_positive_effect_links()){
				for(EffectLink o_efln: tn.get_negative_effect_links()){
					opp_effect_same_task.put(efln.effect, o_efln.effect);
					opp_effect_same_task.put(o_efln.effect, efln.effect);
				}
			}
			
			for(EffectLink efln: tn.get_positive_effect_links()){
				for(EffectLink ef2: tn.get_positive_effect_links()){
					common_task.put(efln.effect, ef2.effect);
					common_task.put(ef2.effect, efln.effect);
				}
				for(EffectLink ef2: tn.get_negative_effect_links()){
					common_task.put(efln.effect, ef2.effect);
					common_task.put(ef2.effect, efln.effect);
				}
			}
			for(EffectLink efln: tn.get_negative_effect_links()){
				for(EffectLink ef2: tn.get_positive_effect_links()){
					common_task.put(efln.effect, ef2.effect);
					common_task.put(ef2.effect, efln.effect);
				}
				for(EffectLink ef2: tn.get_negative_effect_links()){
					common_task.put(efln.effect, ef2.effect);
					common_task.put(ef2.effect, efln.effect);
				}
			}
		}
		
		for(int i = 1; i <= max_search; i++)
		{
			boolean found_combine = true;
			
			while(found_combine){
	
				found_combine = combine_conditions_aux(new LinkedList<CondNode>(), i);
				
				if(found_combine){
					if(!neg_combine){
						
						for(PrecondLink lnk: replaced.getPreconditionTo()){
							for(CondNode cnd: replacers){
								precondition_links.add(lnk.action.attach_precondition(cnd, lnk.required_state));
							}
						}
						
						if(this.goal_states.containsKey(replaced.getName())){
							boolean value = this.goal_states.get(replaced.getName());
							this.goal_states.remove(replaced.getName());
							for(CondNode cnd: replacers){

								this.goal_states.put(cnd.getName(), value);
							}
						}
						
						destroy_condition(replaced);
					}else{
						
						
						for(PrecondLink lnk: replaced.getPreconditionTo()){
							
							for(CondNode cnd: replacers){
								precondition_links.add(lnk.action.attach_precondition(cnd, !lnk.required_state));
							}
						}
						
						if(this.goal_states.containsKey(replaced.getName())){
							boolean value = this.goal_states.get(replaced.getName());
							this.goal_states.remove(replaced.getName());
							for(CondNode cnd: replacers){

								this.goal_states.put(cnd.getName(), !value);
							}
						}
						
						destroy_condition(replaced);
						
					}
				}
			}
		}
		
        long intv2 = new Date().getTime();
	}
	
	
	private boolean combine_conditions_aux(List<CondNode> fixed_cnds, int still_to_fix){
		if(still_to_fix != 0){
			for(CondNode cn: this.condition_nodes.values()){
				
				boolean skip = false;
				Object to_cast = opp_effect_same_task.get(cn);
				if(to_cast != null)
				{
					Collection<Object> cln = (Collection<Object>)to_cast;
					
					for(CondNode nd: fixed_cnds)
					{
						if(cln.contains(nd)){
							skip = true;
							break;
						}
					}
				}
				if(skip){
					continue;
				}
				
				if(!fixed_cnds.contains(cn)){
					List<CondNode> nodes = new LinkedList<CondNode>(fixed_cnds);
					nodes.add(cn);

					boolean to_return = combine_conditions_aux(nodes, still_to_fix-1);
					if(to_return){
						return true;
					}
				}
			}
		}else{
			for(CondNode compare: this.condition_nodes.values()){
				
				if(fixed_cnds.contains(compare)){
					continue;
				}
				
				Object to_cast = opp_effect_same_task.get(compare);
				if(!(to_cast == null)){
					Collection<Object> cln = (Collection<Object>)to_cast;

					if(!cln.containsAll(fixed_cnds)){
						continue;
					}
				}
				
				List<EffectLink> combined_effects = new LinkedList<EffectLink>();
				
				for(CondNode cn: fixed_cnds){
					combined_effects.addAll(cn.getAffectedBy());
				}
				
				//if all links in combined_effects are the same as links in compare.getaff
				
				List<EffectLink> compare_effects = compare.getAffectedBy();
				

				if(equivalent_sets(combined_effects, compare_effects)){
					
					replaced = compare;
					replacers = fixed_cnds;
					neg_combine = false;
					return true;
				}
				else if(opposite_sets(combined_effects, compare_effects)){						

						replaced = compare;
						replacers = fixed_cnds;
						neg_combine = true;
						return true;
				}
			}
		}
		
		return false;
	}
	
	private List<CondNode> replacers;
	private CondNode replaced;
	private boolean neg_combine;
	
	private boolean equivalent_sets(List<EffectLink> set1p, List<EffectLink> set2p){
		
		List<EffectLink> set1 = new LinkedList<EffectLink>(set1p);
		List<EffectLink> set2 = new LinkedList<EffectLink>(set2p);
		
		for(EffectLink ln: set1){
			
			boolean found_equiv = false;
			for(EffectLink chk: set2){
				if(chk.sameSource(ln)){
					found_equiv = true;
					break;
				}
			}
			
			if(!found_equiv){
				return false;
			}
			
		}
		
		for(EffectLink ln: set2){
			
			boolean found_equiv = false;
			for(EffectLink chk: set1){
				if(chk.sameSource(ln)){
					found_equiv = true;
					break;
				}
			}
			
			if(!found_equiv){
				return false;
			}
			
		}
		
		return true;
	}
	
	private boolean opposite_sets(List<EffectLink> set1p, List<EffectLink> set2p){
		
		List<EffectLink> set1 = new LinkedList<EffectLink>(set1p);
		List<EffectLink> set2 = new LinkedList<EffectLink>(set2p);
		
		
		for(EffectLink ln: set1){
			
			boolean found_equiv = false;
			for(EffectLink chk: set2){
				if(chk.oppositeSource(ln)){
					found_equiv = true;
					break;
				}
			}
			
			if(!found_equiv){
				return false;
			}
		}
		
		for(EffectLink ln: set2){
			
			boolean found_equiv = false;
			for(EffectLink chk: set1){
				if(chk.oppositeSource(ln)){
					found_equiv = true;
					break;
				}
			}
			
			if(!found_equiv){
				return false;
			}
		}
		
		return true;
	}
	
	
	//TODO make this work at an arbitrary depth
	/**
	 * Returns true if tasks are eliminated
	 */
	private boolean eliminate_dominated_tasks(){
		
		long intv1 = new Date().getTime();
		
		List<TaskNode> to_remove = new LinkedList<TaskNode>();
		
		boolean keep_looking = true;
		boolean removed_something = false;
		
		
		//Find all "purely consumed" conditions
		
		
		Set<CondNode> pos_only_consumed = new HashSet<CondNode>();
		Set<CondNode> neg_only_consumed = new HashSet<CondNode>();
		
		for(CondNode cnd: this.condition_nodes.values()){
			//Check the positives
			
			//If every positive precondition link has a corresponding negative 
			//effect from the same task
			
			boolean unmatched_pos_precond = false;
			boolean unmatched_neg_precond = false;
			
			for(PrecondLink prl: cnd.getPreconditionTo()){
				
				if(prl.getRequiredState()){
					//If Positive
					
					boolean found_neg_match = false;
					
					for(EffectLink el: prl.action.get_negative_effect_links()){
						if(el.effect == cnd){
							found_neg_match = true;
							break;
						}
					}
					
					if(!found_neg_match){
						unmatched_pos_precond = true;
					}
					
				}else{
					//If negative
					
					boolean found_pos_match = false;
					
					for(EffectLink el: prl.action.get_positive_effect_links()){
						if(el.effect == cnd){
							found_pos_match = true;
							break;
						}	
					}
					
					if(!found_pos_match){
						unmatched_neg_precond = true;
					}
				}
			}
			
			if(!unmatched_neg_precond){
				System.out.println("No unmatched neg "+cnd);
				neg_only_consumed.add(cnd);
			}
			
			if(!unmatched_pos_precond){
				System.out.println("No unmatched pos "+cnd);
				pos_only_consumed.add(cnd);
			}
			
			//check the negatives
			
			//If every negative precondition link has a corresponding positive 
			//effect from the same task
		}
		
		while(keep_looking){
		
			for(TaskNode t: this.task_nodes.values()){
			
	
				Map<CondNode, Boolean> preconditions = new LinkedHashMap<CondNode, Boolean>();
				for(PrecondLink l: t.get_positive_precondition_links()){
					preconditions.put(l.precondition, true);
				}
				for(PrecondLink l: t.get_negative_precondition_links()){
					preconditions.put(l.precondition, false);
				}
				
				Map<CondNode, Boolean> useful_effects = new LinkedHashMap<CondNode, Boolean>();
				for(EffectLink l: t.get_positive_effect_links()){
					if(is_effect_useful(l)){
						useful_effects.put(l.effect, true);
					}
				}
				
				for(EffectLink l: t.get_negative_effect_links()){
					if(is_effect_useful(l)){
						useful_effects.put(l.effect, false);
					}				
				}
				
				Map<CondNode, Set<TaskNode>> cond_to_satisfiers = new LinkedHashMap<CondNode, Set<TaskNode>>();
				Map<TaskNode, Map<CondNode, Boolean>> satisfiers_precond = new LinkedHashMap<TaskNode, Map<CondNode, Boolean>>();
				Map<TaskNode, Map<CondNode, Boolean>> satisfiers_effects = new LinkedHashMap<TaskNode, Map<CondNode, Boolean>>();
				
				List<CondNode> cond_list = new LinkedList<CondNode>();
				
				for(CondNode c: preconditions.keySet()){
					Set<TaskNode> satisfying = new HashSet<TaskNode>();
					cond_to_satisfiers.put(c, satisfying);
					for(EffectLink l: c.getAffectedBy()){
						
						if(l.getWeight() == 1 && preconditions.get(c) == true ||
						   l.getWeight() == -1 && preconditions.get(c) == false){
	
							satisfying.add(l.action);
	
							Map<CondNode, Boolean> preconds_for_sat = new LinkedHashMap<CondNode, Boolean>();
							for(PrecondLink pl: l.action.get_positive_precondition_links()){
								preconds_for_sat.put(pl.precondition, true);
							}
							for(PrecondLink pl: l.action.get_negative_precondition_links()){
								preconds_for_sat.put(pl.precondition, false);
							}
							
							satisfiers_precond.put(l.action, preconds_for_sat);
							
							Map<CondNode, Boolean> effects_for_sat = new LinkedHashMap<CondNode, Boolean>();
							for(EffectLink el: l.action.get_positive_effect_links()){
								effects_for_sat.put(el.effect, true);
							}
							
							for(EffectLink el: l.action.get_negative_effect_links()){
								effects_for_sat.put(el.effect, false);
							}
							
							satisfiers_effects.put(l.action, effects_for_sat);
						}
					}
					
					cond_list.add(c);
				}
				
		//		stored_sat_precond = null;
				if(expand_satisfaction_combination(cond_list, cond_to_satisfiers, satisfiers_precond,
						satisfiers_effects, new LinkedHashMap<CondNode, TaskNode>(), useful_effects)){
					
					Map<CondNode, Boolean> all_sat_precond = new LinkedHashMap<CondNode, Boolean>();
//					for(CondNode cnd: stored_sat_precond.keySet()){
//						all_sat_precond.putAll(stored_sat_precond.get(cnd));
//					}
			//		for(CondNode cnd: all_sat_precond.keySet()){
		
			//		}
					
					//Note! bah
					boolean all_necessary_preconditions_extraneous = true;
					for(CondNode cn: preconditions.keySet()){
						if(!(all_sat_precond.containsKey(cn)&&all_sat_precond.get(cn)==preconditions.get(cn))){
							for(PrecondLink pl: cn.getPreconditionTo()){
								if(pl.getRequiredState() == preconditions.get(cn) &&
										!(pl.getTaskID()== t.getNodeID())){
									all_necessary_preconditions_extraneous = false;
								}
							}
						}
					}
	//				
					if(all_necessary_preconditions_extraneous){
		
				//		to_remove.add(t);
					}
					
					
					Map<CondNode, Boolean> pruned_preconditions = 
						new LinkedHashMap<CondNode, Boolean>();
					
					for(CondNode cnd: preconditions.keySet()){
						if(!(all_sat_precond.containsKey(cnd)&&all_sat_precond.get(cnd)==preconditions.get(cnd))){
							pruned_preconditions.put(cnd, preconditions.get(cnd));
	
						}
					}
					
					boolean all_preconds_consumed = true;
					
					for(CondNode cnd: pruned_preconditions.keySet()){
						if(pruned_preconditions.get(cnd)){
							if(!pos_only_consumed.contains(cnd)){
								all_preconds_consumed = false;
							}
						}else{
							if(!neg_only_consumed.contains(cnd)){
								all_preconds_consumed = false;
							}
						}
					}
					
					if(all_preconds_consumed){
		
						to_remove.add(t);
					}
					
	//				for(EffectLink l: t.get_positive_effect_links()){
	//					if(!this.is_effect_useful(l)){
	//		
	//					}
	//				}
	//				
	//				for(EffectLink l: t.get_negative_effect_links()){
	//					if(!this.is_effect_useful(l)){
	//					
	//					}
	//				}
					
					//Now know that this task leaves the environment the same
					//as sometime before.  Now we prune it if it invalidates a condition that has to stay true
					
	//				for(CondNode cn: useful_effects.keySet()){
	//					for(PrecondLink l: cn.getPreconditionTo()){
	//						if(l.required_state == useful_effects.get(cn)){
	//				
	//							num_clinks++;
	//						}
	//					}
	//				}
					
	//				for(EffectLink el: t.get_effect_links()){
	//					if(this.goal_states.containsKey(el.effect.getName()) &&
	//						 this.goal_states.get(el.effect.getName()) != (el.getWeight()==1)){
	//						to_remove.add(t);
	//					}
	//				}
					
	//				if(num_clinks < 2){
	//
	//				}
				}
				
			}
			
			for(TaskNode tn: to_remove){
				destroy_task(tn);
			}
			

			
			keep_looking = !to_remove.isEmpty();
			removed_something = removed_something||keep_looking;
			
			to_remove.clear();
			
		}
		
		long intv2 = new Date().getTime();
		System.out.println("Removing useless tasks: "+(intv2-intv1)+" ms");
		
		return removed_something;
		
		//0.  for every task:
		//1.  Get all preconditions of task
			//Map<CondNode, boolean> preconditions
		//1'  Get all useful effects of task
			//Map<CondNode, boolean> task_effects
		//2.  For each precondition, get all tasks that satisfy that precondition
			//Map<CondNode, Set<TaskNode>> cond_to_satisfiers
		//3.  For each task that satisfies precondition, get preconditions
			//Map<TaskNode, Set<CondNode>> satisfiers_precond
		//4.  For every combination of 1 task for each entry in cond_to_satisfiers
			//get the whole collection of those tasks' preconditions from 
			//satisfiers_precond
			//remove from the collection the negated effects of the tasks in the 
			//combination
			//If for every combination, all entries in task_effects are in the collection,
			//the task is useless.  Delete task, move to next.
		//5.  If previous step didn't terminate, repeat 2 on the new collection until search is
			//as deep as desired
			//Note: to do it deeper, we would need to keep track of what task each condition
			//is for

	}

//	Map<CondNode, Map<CondNode, Boolean>> stored_sat_precond;
	
	private boolean expand_satisfaction_combination(
			List<CondNode> not_satisfied,
			Map<CondNode, Set<TaskNode>> cond_to_satisfiers, 
			Map<TaskNode, Map<CondNode, Boolean>> satisfiers_precond,
			Map<TaskNode, Map<CondNode, Boolean>> satisfiers_effects,
			Map<CondNode, TaskNode> added,
			Map<CondNode, Boolean> act_effects
			){
		
		if(!not_satisfied.isEmpty()){
			
			CondNode sat_cond = not_satisfied.get(0);
			
			for(TaskNode tn: cond_to_satisfiers.get(sat_cond)){
				
				
				List<CondNode> new_to_sat = new LinkedList<CondNode>(not_satisfied);
				new_to_sat.remove(sat_cond);
				
				Map<CondNode, TaskNode> fixed_tasks = new LinkedHashMap<CondNode, TaskNode>(added);
				fixed_tasks.put(sat_cond, tn);
				
				boolean toret = expand_satisfaction_combination(
					new_to_sat,
					cond_to_satisfiers,
					satisfiers_precond,
					satisfiers_effects,
					fixed_tasks,
					act_effects
				);
				
				if(!toret){
					return false;
				}
				
			}
			return true;
			
		}else{

			
			//0.  Make this map new so we can modify it
			
			Map<TaskNode, Map<CondNode, Boolean>> satisfiers_precond_local = new LinkedHashMap<TaskNode, Map<CondNode, Boolean>>();
			
			for(TaskNode tnd: satisfiers_precond.keySet()){
				satisfiers_precond_local.put(tnd, new LinkedHashMap<CondNode, Boolean>(satisfiers_precond.get(tnd)));
			}
			
			//1.  Build the map of effects
			Map<CondNode, Map<CondNode, Boolean>> all_sat_effects = new LinkedHashMap<CondNode, Map<CondNode, Boolean>>();
			for(CondNode cnd: added.keySet()){
				Map<CondNode, Boolean> to_add = satisfiers_effects.get(added.get(cnd));
				all_sat_effects.put(cnd, to_add);
			}
			
			
			//2.  Get rid of preconditions which must be different different places
			
			//a.  detect them
			
			List<CondNode> to_purge_from_known_preconditions = new LinkedList<CondNode>();
			
			for(CondNode root_cond1: added.keySet()){
				for(CondNode precond1: satisfiers_precond_local.get(added.get(root_cond1)).keySet()){
					for(CondNode root_cond2: added.keySet()){
						for(CondNode precond2: satisfiers_precond_local.get(added.get(root_cond2)).keySet()){
							if((precond1 == precond2) &&  
									(satisfiers_precond_local.get(added.get(root_cond1)).get(precond1) !=
										satisfiers_precond_local.get(added.get(root_cond2)).get(precond2))){
								to_purge_from_known_preconditions.add(precond1);
							}
						}
					}
				}
			}
			
			//b.  erase them
			
			for(CondNode cnd: to_purge_from_known_preconditions){
				for(TaskNode tnd: satisfiers_precond_local.keySet()){
					satisfiers_precond_local.remove(cnd);
				}
			}
			
			//3.  Build the map of precondition to conditions needed for it, but for each check
			//to see that it is nowhere in the map of effects except under the same key
			
			Map<CondNode, Map<CondNode, Boolean>> all_sat_precond = new LinkedHashMap<CondNode, Map<CondNode, Boolean>>();
			
			for(CondNode root_cond: added.keySet()){
				all_sat_precond.put(root_cond, new LinkedHashMap<CondNode, Boolean>());
				
				//Find the preconditions of the task that satisfys that cond
				for(CondNode precond: satisfiers_precond_local.get(added.get(root_cond)).keySet()){
					
					boolean opp_found_in_effects = false;
					//Check to see if it is in the effects map
					
					out:
					for(CondNode effect_root: all_sat_effects.keySet()){
						
						if(added.get(effect_root) == added.get(root_cond)){
							continue;
						}
						
						for(CondNode effect_eff: all_sat_effects.get(effect_root).keySet()){
							if((precond == effect_eff) &&
									(satisfiers_precond_local.get(added.get(root_cond)).get(precond)!=
										all_sat_effects.get(effect_root).get(effect_eff))){
								opp_found_in_effects = true;
								break out;
							}
						}
					}
					
					if(!opp_found_in_effects){
						all_sat_precond.get(root_cond).put(precond, 
								satisfiers_precond_local.get(added.get(root_cond)).get(precond));
					}else{
		
					}
					
				}
			}
			
			
			//TODO set it null after every time through
			
//			if(stored_sat_precond == null){
//				stored_sat_precond = new LinkedHashMap<CondNode, Map<CondNode, Boolean>>();
//
//				for(CondNode cnd: added.keySet()){
//					stored_sat_precond.put(cnd, new LinkedHashMap<CondNode, Boolean>(satisfiers_precond.get(added.get(cnd))));
//				}
//			}
//			else{
//				//Want to have stored_sat_precond.get(each precond) retain only the values
//				//in satisfiers_precond for that precondition
//				
//
//				
//				//For every precondition
//				for(CondNode cond: added.keySet()){
//					List<CondNode> to_remove = new LinkedList<CondNode>();
//					Map<CondNode, Boolean> tmp = stored_sat_precond.get(cond);
//					
//					//For every value in stored_sat_precond of that precond
//					for(CondNode cnd: tmp.keySet()){
//						
//						//If satisfiers_precond doesn't have it, remove it
//						if(!satisfiers_precond.get(added.get(cond)).containsKey(cnd) ||
//						!(satisfiers_precond.get(added.get(cond)).get(cnd) == tmp.get(cnd))){
//							to_remove.add(cnd);
//						}
//					}
//					for(CondNode cnd: to_remove){
//						tmp.remove(cnd);
//					}	
//				}
//			}

				
//				for(TaskNode t: satisfiers_precond.keySet()){

//					Map<CondNode, Boolean> tmp = stored_sat_precond.get(added.get(t));
//					List<CondNode> to_remove = new LinkedList<CondNode>();
//					for(CondNode cnd: tmp.keySet()){
//						if(!satisfiers_precond.get(t).containsKey(cnd) ||
//								!(satisfiers_precond.get(t).get(cnd) == tmp.get(cnd))){
//							to_remove.add(cnd);
//						}
//					}
//					
//					for(CondNode cnd: to_remove){
//						tmp.remove(cnd);
//					}
//					
//				}
				
				
//			}
			


			
			//If all preconditions in the sat_preconditions, continue
			//If any are not in it, return false

			
			for(CondNode cnd: act_effects.keySet()){
				for(CondNode cnd2: all_sat_precond.keySet()){
					if(all_sat_precond.get(cnd2).containsKey(cnd)){
						if(all_sat_precond.get(cnd2).get(cnd).booleanValue() != 
							act_effects.get(cnd).booleanValue()){
	
							return false;
						}
							
					}else{
	
						return false;
					}
				}
			}
			

			return true;
		}
	}
	
	private boolean is_effect_useful(EffectLink c){
		
		double effect_value = c.getWeight();
		CondNode cnd = c.effect;
		
		if(this.goal_states.containsKey(c.effect.getName()) && this.goal_states.get(c.effect.getName()) == (c.getWeight()==1))
			return true;
			
		for(PrecondLink l: cnd.getPreconditionTo()){
			if(l.required_state == true && effect_value > 0 ||
			   l.required_state == false && effect_value < 0){
				
				return true;
			}
		}
		
		return false;
	}
	
	private void setup_initial_action(TaskNode initial_action){
		
    	task_nodes.put("initact", initial_action);

    	for(CondNode c: condition_nodes.values()){
    		initial_action.attach_effect(c, (c.getProbability()-.5)*2);
    	}
    	
    	effect_links.addAll(initial_action.get_positive_effect_links());
    	effect_links.addAll(initial_action.get_negative_effect_links());
    	
    	this.task_to_resources.put(initial_action, new LinkedList<String>());
	}
	
	private boolean eliminate_irreleevant_tasks_and_conditions(){
		
		long intv1 = new Date().getTime();
		
        for(TaskNode t: this.task_nodes.values()){
        	t.get_positive_effect_links().clear();
        	t.get_negative_effect_links().clear();
        }
        
        for(CondNode con: this.condition_nodes.values()){
    	  con.getPreconditionTo().clear();
        }
        
        ExpansionDataSet d = new ExpansionDataSet();
        

        for(String s: goal_states.keySet()){
        	expand_relevant_condition(this.goal_states.get(s), this.condition_nodes.get(s), d);
        }
        
        Set<String> all_conds = new HashSet<String>(d.pos_condition_nodes.keySet());
        all_conds.addAll(d.neg_condition_nodes.keySet());
        
        boolean return_value = (condition_nodes.size() != all_conds.size());
        
        condition_nodes = new LinkedHashMap<String, CondNode>();
        condition_nodes.putAll(d.pos_condition_nodes);
        condition_nodes.putAll(d.neg_condition_nodes);
        
        return_value = return_value||(task_nodes.size() != d.task_nodes.size());

        task_nodes = d.task_nodes;
        precondition_links = d.relevant_preconditions;
        effect_links = d.relevant_effects;
        
        long intv2 = new Date().getTime();
        System.out.println("Pruned irrelevant tasks and conditions: "+(intv2-intv1)+" ms");
        
		
        return return_value;
	}
	
	class SplitChangePair{
		
		Exp split_partial_exp;			
		Exp additions_to_other_exp;
	}
	
	class ExpansionDataSet{
		
		Map<String, CondNode> pos_condition_nodes = new LinkedHashMap<String, CondNode>();
		Map<String, CondNode> neg_condition_nodes = new LinkedHashMap<String, CondNode>();		
		
		Map<String, TaskNode> task_nodes= new LinkedHashMap<String, TaskNode>();
		
		List<PrecondLink> relevant_preconditions = new LinkedList<PrecondLink>();
		List<EffectLink> relevant_effects = new LinkedList<EffectLink>();
	}
	
	private void expand_relevant_condition(boolean val, CondNode cond, ExpansionDataSet d){
		
		if(val){
			if(d.pos_condition_nodes.containsValue(cond)){
				return;
			}
			d.pos_condition_nodes.put(cond.getName(), cond);
		}else{
			if(d.neg_condition_nodes.containsValue(cond)){
				return;
			}
			d.neg_condition_nodes.put(cond.getName(), cond);
		}
		
		
		for(EffectLink l: cond.getAffectedBy()){
			if(val && l.getWeight() == 1){
				l.action.get_positive_effect_links().add(l);
				d.relevant_effects.add(l);
				expand_relevant_task(l.action, d);
			}else if (l.getWeight() == -1){
				l.action.get_negative_effect_links().add(l);
				d.relevant_effects.add(l);
				expand_relevant_task(l.action, d);				
			}
		}
	}
	
	private void expand_relevant_task(TaskNode t, ExpansionDataSet d){
		
		if(d.task_nodes.containsValue(t)){
			return;
		}
		
		d.task_nodes.put(t.getName(), t);
		
		for(PrecondLink l: t.get_positive_precondition_links()){
			
			l.precondition.getPreconditionTo().add(l);
			d.relevant_preconditions.add(l);
			expand_relevant_condition(true, l.precondition, d);
		}
		
		for(PrecondLink l: t.get_negative_precondition_links()){
			
			l.precondition.getPreconditionTo().add(l);
			d.relevant_preconditions.add(l);
			expand_relevant_condition(false, l.precondition, d);
		}
	}
	
	private boolean eliminate_impossible_tasks(){
		
		long intv1 = new Date().getTime();
		
		List<CondNode> new_values = new LinkedList<CondNode>(condition_nodes.values());
		
		boolean removed_something;
		boolean ever_removed_something = false;
		
		do{
			
			removed_something = false;
			
			for(CondNode c_node: new_values){
				if(condition_nodes.values().contains(c_node)){
					removed_something = propagate_impossible_condition(c_node)||removed_something;
				}
			}
		
			ever_removed_something = ever_removed_something||removed_something;
		
		}while(removed_something);
		
        long intv2 = new Date().getTime();
        System.out.println("Eliminated impossible tasks: "+(intv2-intv1)+" ms");
		
		return ever_removed_something;
	}
	
	private void destroy_task(TaskNode t){
		
		for(PrecondLink l:t.get_positive_precondition_links()){
			this.precondition_links.remove(l);
		}
		
		for(PrecondLink l:t.get_negative_precondition_links()){
			this.precondition_links.remove(l);
		}
		
		for(EffectLink l:t.get_positive_effect_links()){
			this.effect_links.remove(l);
		}
		
		for(EffectLink l:t.get_negative_effect_links()){
			this.effect_links.remove(l);
		}
		
		t.destroy();
	
		task_nodes.remove(t.getName());
	}
	
	private void destroy_condition(CondNode c){
		
		for(PrecondLink l:c.getPreconditionTo()){
			this.precondition_links.remove(l);
		}
		
		for(EffectLink l:c.getAffectedBy()){
			this.effect_links.remove(l);
		}
		
		
		c.destroy();
		
		condition_nodes.remove(c.getName());
	}
	
	private boolean propagate_impossible_condition(CondNode c_node){
		
			boolean isTurnedFalse = false;
			boolean isTurnedTrue = false;
			
			boolean removed_something = false;
			
			List<EffectLink> new_affected = new LinkedList<EffectLink>(c_node.getAffectedBy());
			
			for(EffectLink e: new_affected){
				
				if(c_node.getAffectedBy().contains(e)){
					if(e.getWeight() > 0){
						isTurnedTrue = true;
					}else if(e.getWeight() < 0){
						isTurnedFalse = true;
					}
				}
			}
			
			boolean condition_state = false;
			
			if(!isTurnedTrue && c_node.getProbability() == 0){
				condition_state = true;
			}
			
			if((!isTurnedTrue && c_node.getProbability() == 0)||(!isTurnedFalse && c_node.getProbability() == 1)){
				List<PrecondLink> new_preconditions = new LinkedList<PrecondLink>(c_node.getPreconditionTo());
				
				//There are actions, but they don't do anything, so get rid of them
				for(EffectLink eff: c_node.getAffectedBy()){
					if(eff.getWeight() > 0){
						eff.action.get_positive_effect_links().remove(eff);
					}else{
						eff.action.get_negative_effect_links().remove(eff);
					}
				}
				
				removed_something = true;
				destroy_condition(c_node);
				for(PrecondLink pre_link: new_preconditions){
					if(c_node.getPreconditionTo().contains(pre_link) 
							&& (condition_state == pre_link.required_state)){
						
						TaskNode t = pre_link.action;
						destroy_task(t);
						
						List<EffectLink> new_effects = new LinkedList<EffectLink>();
						new_effects.addAll(t.get_positive_effect_links());
						new_effects.addAll(t.get_negative_effect_links());
				
//						for(EffectLink l: new_effects){
//							if(
//									((l.getWeight() > 0 && t.get_positive_effect_links().contains(l))
//								  ||(l.getWeight() < 0 &&t.get_negative_effect_links().contains(l))) && 
//									condition_nodes.containsValue(l.effect)){
//								propagate_impossible_condition(l.effect);
//							}
//						}
					}else if (c_node.getPreconditionTo().contains(pre_link)){
						if(pre_link.getRequiredState()){
							pre_link.action.get_positive_precondition_links().remove(pre_link);
						}else{
							pre_link.action.get_negative_precondition_links().remove(pre_link);
						}
						
					}
				}
			}
			
			return removed_something;
	}
	
	
	private void set_probabilities(){
		for(TaskNode t: task_nodes.values()){
			t.set_precondition_probabilities();
		}
	}
	
	private boolean are_probabilistics_translatable(List<Action> action_list){
		
		for(Action a: action_list){
			
			Exp e = a.getEffect();
			
			if(are_dependent_prob_terms(e) || are_conjunctive_prob_terms(e)){
				return false;
			}
		}
		
		return true;
	}
	
	private boolean are_dependent_prob_terms(Exp e){
		
		switch(e.getExpID()){
		case AND:{
			AndExp and_exp = (AndExp)e;
			Iterator<Exp> children = and_exp.iterator();
			while(children.hasNext()){
				Exp child = children.next();
				boolean result = are_dependent_prob_terms(child);
				if(result){
					return true;
				}
			}
			return false;
		}
		case PROB_EXP:{
			ProbExp prob_exp = (ProbExp)e;
			if(prob_exp.size() > 1){
				return true;
			}
			
			Iterator<Exp> children = prob_exp.iterator();
			while(children.hasNext()){
				Exp child = children.next();
				boolean result = are_dependent_prob_terms(child);
				if(result){
					return true;
				}
			}
			
			return false;
			
		}case ATOMIC_FORMULA:{
			return false;
		}case NOT:{
			return are_dependent_prob_terms(((NotExp)e).getExp());
		} default:{
			System.out.println("Error: term "+e+" not recognized here");
			System.exit(0);
			return true;
		}
		}
		
	}
	
	private boolean are_conjunctive_prob_terms(Exp e){
		switch(e.getExpID()){
		case AND:{
			AndExp and_exp = (AndExp)e;
			Iterator<Exp> children = and_exp.iterator();
			while(children.hasNext()){
				Exp child = children.next();
				boolean result = are_conjunctive_prob_terms(child);
				if(result){
					return true;
				}
			}
			return false;
		}
		case PROB_EXP:{
			ProbExp prob_exp = (ProbExp)e;

			
			Iterator<Exp> children = prob_exp.iterator();
			while(children.hasNext()){
				Exp child = children.next();
				
				if(child.getExpID() == ExpID.AND){
					return true;
				}
				
				boolean result = are_conjunctive_prob_terms(child);
				if(result){
					return true;
				}
			}
			
			return false;
			
		}case ATOMIC_FORMULA:{
			return false;
		}case NOT:{
			return are_conjunctive_prob_terms(((NotExp)e).getExp());
		} default:{
			System.out.println("Error: term "+e+" not recognized here");
			System.exit(0);
			return true;
		}
		}
	}
	
	private void enumerate_actions(List<ActionData> action_data){
		
		long intv1 = new Date().getTime();
		
        for(ActionData a: action_data){
        	actionEnumerationAux(a);
        }
        
        long intv2 = new Date().getTime();
        System.out.println("Enumerated actions: "+(intv2-intv1)+" ms");
	}
	
	private List<ActionData> parse_equality_constraints(List<Action> action_list){
		
		long intv1 = new Date().getTime();
		
        List<ActionData> action_data = new LinkedList<ActionData>();
		
		Map<Variable, Variable> variable_equal_variable_list = new LinkedHashMap<Variable, Variable>();
		Map<Variable, Variable> variable_not_equal_variable_list = new LinkedHashMap<Variable, Variable>();
		Map<Variable, Constant> variable_equal_constant_list = new LinkedHashMap<Variable, Constant>();
		Map<Variable, Constant> variable_not_equal_constant_list = new LinkedHashMap<Variable, Constant>();
        
		
		Iterator<Action> action_iterator = action_list.iterator();
    	List<Action> new_actions = new LinkedList<Action>();
    	
		//TODO tentatively agree that this works because everything is and.  When forall and exists
		//are implemented, take another look
        while(action_iterator.hasNext()){
        	
        	variable_equal_variable_list.clear();
        	variable_not_equal_variable_list.clear();
        	variable_equal_constant_list.clear();
        	variable_not_equal_constant_list.clear();
        	
        	Action current_action = action_iterator.next();
        	ActionData this_action_data = new ActionData(current_action, typeset_to_constant);
        	new_actions.add(current_action);

        	action_data.add(this_action_data);
        	
        	List<Term> all_variables = current_action.getParameters();
        	
        	for(Term t: all_variables){
        		if(t instanceof Variable){
        			this_action_data.add_free_variable((Variable)t);
        		}
        	}
        	
        	AndExp tmp = new AndExp();
        	tmp.add(((Action)current_action).getPrecondition());
        	Iterator<Exp> precond_it = tmp.iterator();
        	
        	AndExp new_precondition = tmp.clone();
        	


        	while(precond_it.hasNext()){


        		Exp k = precond_it.next();

        		switch(k.getExpID()){
	        		case ATOMIC_FORMULA:{
	        			break;
	        		}
	        		case NOT:{
	
	        			Exp internal = ((NotExp)k).getExp();
	
	        			switch(internal.getExpID())
	        			{
		        			case F_COMP:{
		
		        				EqualComp equal_exp = (EqualComp)internal;
		
		        				Term arg1 = equal_exp.getArg1();
		        				Term arg2 = equal_exp.getArg2();
		
		        				boolean term1_var = (arg1 instanceof Variable);
		        				boolean term2_var = (arg2 instanceof Variable);
		
		        				if(term1_var && term2_var){
		        					variable_not_equal_variable_list.put((Variable)arg1, (Variable)arg2);
		        					new_precondition.remove(k);
		        				}
		        				else if(!term1_var && term2_var){
		        					variable_not_equal_constant_list.put((Variable)arg2, (Constant)arg1);
		        					new_precondition.remove(k);
		        				}
		        				else if(term1_var && !term2_var){
		        					variable_not_equal_constant_list.put((Variable)arg1, (Constant)arg2);
		        					new_precondition.remove(k);
		        				}
		        				else{
		        					if(arg1.equals(arg2)){
		
		        						new_actions.remove(current_action);
		        						break;
		
		        					}else{
		        						new_precondition.remove(k);
		
		        					}
		        					new_precondition.remove(k);
		        				}
		
		        				break;
		        			}default:{
		        				break;
		        			}
	        			}
	        			break;
	        		}
	        		case F_COMP:{
	        			EqualComp equal_exp = (EqualComp)k;
	
	        			Term arg1 = equal_exp.getArg1();
	        			Term arg2 = equal_exp.getArg2();
	
	        			boolean term1_var = (arg1 instanceof Variable);
	        			boolean term2_var = (arg2 instanceof Variable);
	
	
	        			if(term1_var && term2_var){
	        				variable_equal_variable_list.put((Variable)arg1, (Variable)arg2);
	        				new_precondition.remove(k);
	        			}
	        			else if(!term1_var && term2_var){
	        				variable_equal_constant_list.put((Variable)arg2, (Constant)arg1);
	        				new_precondition.remove(k);
	        			}
	        			else if(term1_var && !term2_var){
	        				variable_equal_constant_list.put((Variable)arg1, (Constant)arg1);
	        				new_precondition.remove(k);
	        			}
	        			else{
	        				if(arg1.equals(arg2)){
	        					new_precondition.remove(k);
	        				}else{
	        					new_actions.remove(current_action);
	        					break;
	        				}
	        				new_precondition.remove(k);
	        			}
	
	        			break;
	
	        		}default:{
	        			System.out.println("Error: term "+k+" not recognized here");
	        			break;
	        		}

        		}
        	}

        	current_action.setPrecondition(new_precondition);
        	
        	for(Variable t: variable_not_equal_variable_list.keySet()){
        		this_action_data.fix_variable_not_equal_variable(t, variable_not_equal_variable_list.get(t));
        	}
        	for(Variable t: variable_equal_variable_list.keySet()){
        		this_action_data.fix_variable_to_variable(t, variable_equal_variable_list.get(t));
        	}
        	for(Variable t: variable_not_equal_constant_list.keySet()){
        		this_action_data.fix_variable_not_equal_constant(t, variable_not_equal_constant_list.get(t));
        	}
        	for(Variable t: variable_equal_constant_list.keySet()){
        		this_action_data.fix_variable_to_constant(t,variable_equal_constant_list.get(t));
        	}
        }
        
        long intv2 = new Date().getTime();
        System.out.println("Parsed equalities: "+(intv2-intv1)+" ms");
        
        return action_data;
	}
	
	private List<Action> convert_preconditions_to_prenex(List<Action> action_list){
		
		long intv1 = new Date().getTime();
		
		List<Action> new_actions = new LinkedList<Action>();
		
       for(Action a: action_list){
    	   
    	   Action a2 = a.clone();
    	   a2.setPrecondition(a2.getPrecondition().toNegativeNormalForm());
    	   new_actions.add(a2);
          }
	       
        long intv2 = new Date().getTime();
        System.out.println("Normalized: "+(intv2 - intv1)+" ms");
	       
       return new_actions;
	}
	
	private List<Action> remove_conditionals(List<Action> action_list){
        
		long intv1 = new Date().getTime();
		
        boolean still_conditionals = true;
        while(still_conditionals){
        	still_conditionals = false;
        	List<Action> new_actions = new LinkedList<Action>();
        	
        	for(Action a: action_list){
        	
        		List<SplitChangePair> changes = split_when_conditions(a.getEffect());
        		
        		if(changes.isEmpty()){
        			new_actions.add(a);
        		}
        		
        		int i = 0;
        		for(SplitChangePair p: changes){
        			
        			Action newAct = a.clone();
        			newAct.setName(newAct.getName()+((i++==0)?"+":"-"));
        			newAct.setEffect(p.split_partial_exp);
        			AndExp newPrecondition = new AndExp();
        			newPrecondition.add(a.getPrecondition().clone());
        			newPrecondition.add(p.additions_to_other_exp);
        			newAct.setPrecondition(newPrecondition);
        			new_actions.add(newAct);
        			
        			still_conditionals = true;
        		}
        	}
        	
        	action_list = new_actions;
        }
        
        long intv2 = new Date().getTime();
		System.out.println("Removed conditionals: "+(intv2-intv1)+" ms");
        
        return action_list;
        
	}
	
	private List<Action> remove_precondition_disjunctions(List<Action> action_list){
        //TODO I'm not convinced that the toDisjunctive stuff necessarily gets rid of every or expression
        //Check if this is true, and if it's not, do the disjunction cleaner
        
		long intv1 = new Date().getTime();
		
        List<Action> actions_after_disjunctions = new LinkedList<Action>();
        
       for(Action a: action_list){
        	
        	Exp act_preconditions = ((Action)a).getPrecondition();
        	OrExp act_preconditions_disj = (OrExp)act_preconditions.toDisjunctiveNormalForm();
        	Iterator<Exp> terms_it = act_preconditions_disj.iterator();
        	
        	int i = 0;
        	while(terms_it.hasNext()){
        	
        		Exp new_act_preconditions = terms_it.next();
        		Action new_action = ((Action)a).clone();
        		new_action.setPrecondition(new_act_preconditions);
        		new_action.setName(a.getName()+"["+(i++)+"]");
        		actions_after_disjunctions.add(new_action);
        	}
        }
       
       
       long intv2 = new Date().getTime();
       System.out.println("Removed disjunctions: "+(intv2-intv1)+" ms");
       
       return actions_after_disjunctions;
       
	}
	
	private List<Action> get_all_actions(PDDLObject obj){
		
        List<Action> actions_list = new LinkedList<Action>();
        Iterator<ActionDef> action_def_it = obj.actionsIterator();
        
        while(action_def_it.hasNext()){
        	
        	ActionDef d = action_def_it.next();
        	actions_list.add(((Action)d).clone());
        }
        
        return actions_list;
	}
	
	private void set_initial_propositions(PDDLObject obj){
        for(InitEl e: obj.getInit()){
        	CondNode n = condition_nodes.get(e.toString());
        	n.setProbability(1);
        }
	}
	
	private void enumerate_propositions_default(PDDLObject obj){
		
		long intv1 = new Date().getTime();
		
        Iterator<AtomicFormula> atom_it = obj.predicatesIterator();

    	//First enumerate all propositions--all set to false
        while(atom_it.hasNext()){
        	
        	AtomicFormula form = atom_it.next();
        	List<Variable> variables = new LinkedList<Variable>(form.getFreeVariables());
        	propositionEnumerationAux(form, variables, variables, new Substitution());
        }
        
		long intv2 = new Date().getTime();
		System.out.println("Enumerated conditions: "+(intv2-intv1)+" ms");
	}
	
	private void get_goals(PDDLObject obj){
		
		Exp goals = obj.getGoal();
		Iterator<Exp> goal_it = ((AndExp)goals.toConjunctiveNormalForm()).iterator();
		
		while(goal_it.hasNext()){
			
			Exp current = goal_it.next();
			
				String seek_string = null;
				
				switch(current.getExpID())
				{
					case NOT:
					{
						seek_string = (((NotExp)current).getExp().toString());
						goal_states.put(seek_string, false);	
						break;
						
					}
					default: {
						
						seek_string = (current.toString());
						goal_states.put(seek_string, true);
					}
				}
		}
	}
	
	private void record_constants(PDDLObject obj){
		
		Iterator<Constant> const_it = obj.constantsIterator();
		
		
		//Keep track of what type each constant is, to save pain later
		while(const_it.hasNext()){
			Constant c = const_it.next();

			TypeSet type = c.getTypeSet();
			
			Set<Type> all_types = new HashSet<Type>();
			Iterator<Type> type_it = type.iterator();
			while(type_it.hasNext()){
				Type typ = type_it.next();
				all_types.add(typ);
				all_types.addAll(typ.getAllSuperTypes());
			}
			
			for(Type typ: all_types){
				
				if(typeset_to_constant.get(typ) == null){
					typeset_to_constant.put(typ, new LinkedList<Constant>());
					
				}
				
				typeset_to_constant.get(typ).add(c);
			}
			
			resources.put(c.toTypedString(), new Resource(c.toString(), "DISCRETE", "1"));
			
		}
	}
	
	/**
	 * Finds all conditions in effects and splits them into separate actions.
	 * 
	 * else false
	 */
	
	//TODO when forall and exists are implemented, see if this is still valid.
	
	private List<SplitChangePair> split_when_conditions(Exp e){
		
		switch(e.getExpID()){
		
			case AND:{
				
				AndExp exp = (AndExp)e.clone();
				Iterator<Exp> exp_it = exp.iterator();
				
				while(exp_it.hasNext()){
					
					Exp search = exp_it.next();
					List<SplitChangePair> returned = split_when_conditions(search);
					
					if(!returned.isEmpty()){
						
						for(SplitChangePair p: returned){
							
							AndExp copy = exp.clone();
							copy.remove(search);
							copy.add(p.split_partial_exp);
							p.split_partial_exp = copy;
						}
						return returned;
					}
				}
				
				//Nothing new to add, return nothing
				return new LinkedList<SplitChangePair>();
	
			}case ATOMIC_FORMULA:{
				
				return new LinkedList<SplitChangePair>();
	
			}case NOT:{
				
				NotExp exp = (NotExp)e;
				Exp inner = exp.getExp();
				List<SplitChangePair> returned = split_when_conditions(inner);
				
				if(!returned.isEmpty()){
					
					for(SplitChangePair p: returned){
						
						NotExp copy = exp.clone();
						copy.setExp(p.split_partial_exp);
						p.split_partial_exp = copy;
					}
					
					return returned;
				}
				
				return new LinkedList<SplitChangePair>();
	
			}case WHEN:{
				
				WhenExp when = (WhenExp)e;
				
				Exp condition = when.getCondition();
				Exp effect = when.getEffect();
				
				List<SplitChangePair> changes = new LinkedList<SplitChangePair>();
				
				SplitChangePair positive = new SplitChangePair();
				positive.additions_to_other_exp = condition;
				positive.split_partial_exp = effect;
				
				SplitChangePair negative = new SplitChangePair();
				negative.additions_to_other_exp = new NotExp(condition);
				negative.split_partial_exp = new AndExp();
				
				changes.add(positive);
				changes.add(negative);
				
				return changes;
	
			}case PROB_EXP:{
				
				ProbExp p = (ProbExp)e;
				
				Iterator<Exp> exp_it = p.iterator();
					
				while(exp_it.hasNext()){
					Exp expr = exp_it.next();
					
					List<SplitChangePair> returned = split_when_conditions(expr);
					
					if(!returned.isEmpty()){
						
						for(SplitChangePair pair: returned){
							
							ProbExp new_exp = p.clone();
							new_exp.removeExp(expr);
							new_exp.addExp(pair.split_partial_exp, p.getProbability(expr));
							pair.split_partial_exp = new_exp;
							
						}
						
						return returned;
						
					}
				}
				
				return new LinkedList<SplitChangePair>();
	
			}
		}

		System.out.println("Error: term "+e+" not recognized here");
		System.exit(0);
		return null;
		
	}
	
	private void actionEnumerationAux(ActionData dat){
		if(dat.has_free_variable())
		{
			ActionData clone_dat = new ActionData(dat);
			Variable to_fix = dat.get_first_free_variable();
			for(Constant c: dat.get_possible_values(to_fix)){
				clone_dat.temp_fix_variable_to_constant(to_fix, c);
				actionEnumerationAux(clone_dat);
			}
				
		}else
		{
			
			String action_name = dat.get_action_name();
			Exp preconditions = dat.get_precondition_expression();
			Exp effects = dat.get_effect_expression();
			
			
			Iterator<Exp> precond_it = ((AndExp)preconditions).iterator();
			Iterator<Exp> effect_it = ((AndExp)effects).iterator();
			
			TaskNode act = new TaskNode(action_name);
			
			
			while(precond_it.hasNext()){
				
				Exp k = precond_it.next();
				String condition_seek = null;
				
				switch(k.getExpID()){
					case ATOMIC_FORMULA:{
						condition_seek = (k.toString());
						act.attach_precondition(condition_nodes.get(condition_seek),true);
						break;
					}case NOT:{
						condition_seek = (((NotExp)k).getExp().toString());
						act.attach_precondition(condition_nodes.get(condition_seek),false);
						break;
					}default:
					{
						System.out.println("Error: term "+k+" not recognized here");
						System.exit(0);
						break;
					}
				}
			}
			
			while(effect_it.hasNext()){

				Exp k = effect_it.next();
				String condition_seek = null;

				switch(k.getExpID()){
					case ATOMIC_FORMULA:{
						condition_seek = (k.toString());
						act.attach_effect(condition_nodes.get(condition_seek), 1);
						break;
					}case NOT:{
						condition_seek = (((NotExp)k).getExp().toString());
						act.attach_effect(condition_nodes.get(condition_seek), -1);
						break;
					}case PROB_EXP:{
	
						ProbExp p = (ProbExp)k;
						if(p.size() == 1){
	
							Exp term = p.iterator().next();
	
							switch(term.getExpID()){
								case ATOMIC_FORMULA:{
		
									condition_seek = (term.toString());
									act.attach_effect(condition_nodes.get(condition_seek), p.getProbability(term));
		
									break;
								}
								case NOT:{
		
									condition_seek = ((NotExp)term).getExp().toString();
									act.attach_effect(condition_nodes.get(condition_seek), -p.getProbability(term));
		
									break;
								}
								default:{
		
									System.out.println("Error: term "+term+" not recognized here");
									System.exit(0);
								}
							}
						}else if(p.size() == 0){
							//meh
						}
						else{
							System.out.println("Error: term "+k+" not recognized here");
							System.exit(0);
						}
						break;
					}default:{
	
						System.out.println("Error: term "+k+" not recognized here");
						System.exit(0);
						break;
					}
				}
			}

			
			task_nodes.put(action_name, act);
			task_to_resources.put(act, dat.get_all_substituted_parameters());
			precondition_links.addAll(act.get_positive_precondition_links());
			precondition_links.addAll(act.get_negative_precondition_links());
			
			effect_links.addAll(act.get_positive_effect_links());
			effect_links.addAll(act.get_negative_effect_links());
		}
	}

	
	private void record_task_impls(){
		
		for(TaskNode t: this.task_nodes.values()){
			
			if(t.getName().equals("initact")){
				continue;
			}
			
			task_impls.put(t.getName(), new TaskImpl(t.getName()+"_impl"));
			
			task_to_impl_links.add(new TaskToImpl(t.getNodeID(), (t.getName()+"_impl"), "1"));
			
			for(String c: task_to_resources.get(t)){
				impl_to_resource_links.add(new ImplToResource((t.getName()+"_impl"), c, "1"));
			}
		}
	}
	
	private void propositionEnumerationAux(AtomicFormula predicate, List<Variable> free, List<Variable> all_vars, Substitution s){
		
		if(!free.isEmpty())
		{
			Variable to_fix = free.get(0);
			List<Variable> reduced = new LinkedList<Variable>(free);
			reduced.remove(0);
			
			
			Iterator<Type> type_it = to_fix.getTypeSet().iterator();
			
			while(type_it.hasNext()){
				Type typ = type_it.next();
				if(typeset_to_constant.get(typ) != null)
				for(Constant c: typeset_to_constant.get(typ)){
					
					Substitution s2 = s.clone();
					s2.bind(to_fix, c);
					
					propositionEnumerationAux(predicate, reduced, all_vars, s2);
				}
			}
				
		}else
		{

			AtomicFormula substituted = predicate.apply(s);
			String formula = (substituted.toString());
			CondNode node = new CondNode(formula, 0, 0);
			condition_nodes.put(formula, node);
			
		}
	}
	
	public void printConditionNodes(){
		System.out.println("Conditions: ");
		System.out.println();
		for(String s: condition_nodes.keySet()){
			System.out.println(condition_nodes.get(s).toString());
		}
		System.out.println();
	}
	
	public void printTaskNodes(){
		System.out.println("Actions: ");
		System.out.println();
		for(String s: task_nodes.keySet()){
			System.out.println();
			System.out.println(task_nodes.get(s).toString());
		}
		System.out.println();
	}
	
	public void printGoals(){
		System.out.println("Goals: ");
		System.out.println();
		for(String s: goal_states.keySet()){
			System.out.println(s+" Desired state: "+goal_states.get(s));
		}
		System.out.println();
	}
	
	public void printSAN(){
		printConditionNodes();
		printTaskNodes();
		printGoals();
	}
	
	public void printTaskImpls(){
		System.out.println("Task Impls:");
		System.out.println();
		for(String s: task_impls.keySet()){
			System.out.println(task_impls.get(s).toString());
		}

		System.out.println();
	}
	
	public void printResources(){
		System.out.println("Resources:");
		System.out.println();
		for(String s: resources.keySet()){
			System.out.println(resources.get(s).toString());
		}
		System.out.println();
	}
	
	public void printTaskToImpls(){
		System.out.println("TaskToImpls: ");
		System.out.println();
		for(TaskToImpl s: task_to_impl_links){
			System.out.println(s.toString());
		}
		System.out.println();
	}
	
	public void printResourceToImpls(){
		System.out.println("ResourceToImpls: ");
		System.out.println();
		for(ImplToResource r: impl_to_resource_links){
			System.out.println(r.toString());
		}
		System.out.println();		
	}
	
	public void printTM(){
		printTaskImpls();
		printResources();
		printTaskToImpls();
		printResourceToImpls();
	}
	
	public long getConditonNodeCount(){
		return condition_nodes.size();
	}
	
	public long getActionNodeCount(){
		return task_nodes.size();
	}
	
	public void write_SAN_to_xml(String filename){
		
		try{
        DocumentBuilderFactory dbfac = DocumentBuilderFactory.newInstance();
        DocumentBuilder docBuilder = dbfac.newDocumentBuilder();
        Document doc = docBuilder.newDocument();
        
        Element root = doc.createElement("SANet:network");
     
        root.setAttribute("xmlns:SANet", "http://www.vanderbilt.edu/SANet");
        root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
        root.setAttribute("xsi:schemaLocation", "http://www.vanderbilt.edu/SANet SANet_Network.xsd");
        root.setAttribute("xmlns", "http://www.vanderbilt.edu/SANet");
        
        doc.appendChild(root);
        
        Element defaultAtten = doc.createElement("defaultAttenFactor");
        Text dAtten = doc.createTextNode("");
        defaultAtten.appendChild(dAtten);
        
        Element defCost = doc.createElement("defaultTaskCost");
        Text dCText = doc.createTextNode("");
        defCost.appendChild(dCText);
        
        Element defCondUtil = doc.createElement("defaultCondUtil");
        Text dCU = doc.createTextNode("");
        defCondUtil.appendChild(dCU);
        
        Element defProb = doc.createElement("defaultCondProbTrue");
        Text probTextt = doc.createTextNode("");
        defProb.appendChild(probTextt);
        
        Element linkThresh = doc.createElement("linkThresh");
        Text lThrT = doc.createTextNode("");
        linkThresh.appendChild(lThrT);
        
        root.appendChild(defaultAtten);
        root.appendChild(defCost);
        root.appendChild(defCondUtil);
        root.appendChild(defProb);
        root.appendChild(linkThresh);

		for(TaskNode a: task_nodes.values()){
			
			if(!init_act_visible && a.getName().equals("initact")){
				continue;
			}
			
			Element taskNode = doc.createElement("taskNode");
			
			Element nodeID = doc.createElement("nodeID");
			Text idText = doc.createTextNode(a.getNodeID());
			nodeID.appendChild(idText);
			
			Element name = doc.createElement("name");
			Text nameText = doc.createTextNode(a.getName());
			name.appendChild(nameText);
			
			Element priorProb = doc.createElement("priorProb");
			Text probText = doc.createTextNode(new Double(a.getPriorProb()).toString());
			priorProb.appendChild(probText);
			
			Element attenFactor = doc.createElement("attenFactor");
			Text text = doc.createTextNode(a.getAttenFactor());
			attenFactor.appendChild(text);
			
			Element cost = doc.createElement("cost");
			Text costText = doc.createTextNode(a.getCost());
			cost.appendChild(costText);
			
			taskNode.appendChild(nodeID);
			taskNode.appendChild(name);
			taskNode.appendChild(priorProb);
			taskNode.appendChild(attenFactor);
			taskNode.appendChild(cost);
			
			root.appendChild(taskNode);
		}
		
		for(CondNode c: condition_nodes.values()){
			Element condNode = doc.createElement("condNode");
			
			Element nodeID = doc.createElement("nodeID");
			Text idText = doc.createTextNode(c.getNodeID());
			nodeID.appendChild(idText);
			
			Element name = doc.createElement("name");
			Text nameText = doc.createTextNode(c.getName());
			name.appendChild(nameText);
			
			Element probTrue = doc.createElement("probTrue");
			Text probText = doc.createTextNode(new Double(c.getProbability()).toString());
			probTrue.appendChild(probText);
			
			Element utility = doc.createElement("utility");
			Text utilText = doc.createTextNode(new Double(c.getUtility()).toString());
			utility.appendChild(utilText);
			
			Element kind = doc.createElement("kind");
			Text kindText = doc.createTextNode(c.getKind());
			kind.appendChild(kindText);
			
			Element attenFactor = doc.createElement("attenFactor");
			Text attenText = doc.createTextNode(c.getAttenFactor());
			attenFactor.appendChild(attenText);
			
			condNode.appendChild(nodeID);
			condNode.appendChild(name);
			condNode.appendChild(probTrue);
			condNode.appendChild(utility);
			condNode.appendChild(kind);
			condNode.appendChild(attenFactor);
			
			root.appendChild(condNode);
		}
		
		for(PrecondLink l: precondition_links){
			

			
			Element preLink = doc.createElement("precondLink");
			
			Element condID = doc.createElement("condID");
			Text idText = doc.createTextNode(l.getCondID());
			condID.appendChild(idText);
			
			Element taskID = doc.createElement("taskID");
			Text taskText = doc.createTextNode(l.getTaskID());
			taskID.appendChild(taskText);
			
			Element portID = doc.createElement("portID");
			Text portText = doc.createTextNode(l.getPortID());
			portID.appendChild(portText);
			
			Element trueProb = doc.createElement("trueProb");
			Text trueText = doc.createTextNode(new Double(l.getTrueProb()).toString());
			trueProb.appendChild(trueText);
			
			Element falseProb = doc.createElement("falseProb");
			Text falseText = doc.createTextNode(new Double(l.getFalseProb()).toString());
			falseProb.appendChild(falseText);
			
			preLink.appendChild(condID);
			preLink.appendChild(taskID);
			preLink.appendChild(portID);
			preLink.appendChild(trueProb);
			preLink.appendChild(falseProb);
			
			root.appendChild(preLink);
		}
		
		for(EffectLink l: effect_links){
			
			if(!init_act_visible && l.action.getName().equals("initact")){
				continue;
			}
			
			Element effLink = doc.createElement("effectLink");
			
			Element taskID = doc.createElement("taskID");
			Text idText = doc.createTextNode(l.getTaskID());
			taskID.appendChild(idText);
			
			Element condID = doc.createElement("condID");
			Text condText = doc.createTextNode(l.getCondID());
			condID.appendChild(condText);
			
			Element portID = doc.createElement("portID");
			Text portText = doc.createTextNode(l.getPortID());
			portID.appendChild(portText);
			
			Element weight = doc.createElement("weight");
			Text weightText = doc.createTextNode(new Double(l.getWeight()).toString());
			weight.appendChild(weightText);
			
			effLink.appendChild(taskID);
			effLink.appendChild(condID);
			effLink.appendChild(portID);
			effLink.appendChild(weight);
			
			root.appendChild(effLink);
		}
		
        //set up a transformer
        TransformerFactory transfac = TransformerFactory.newInstance();
        Transformer trans = transfac.newTransformer();
        trans.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "no");
        trans.setOutputProperty(OutputKeys.INDENT, "yes");
        trans.setOutputProperty(OutputKeys.STANDALONE, "no");
        
        //create string from xml tree
        StringWriter sw = new StringWriter();
        StreamResult result = new StreamResult(sw);
        DOMSource source = new DOMSource(doc);
        trans.transform(source, result);
        
        FileWriter f =  new FileWriter(filename);
        f.write(sw.toString());
        f.close();

		}catch(Exception e){
			e.printStackTrace();
		}
        
	}
	
	public void write_TM_to_xml(String filename){
		
		try{
	        DocumentBuilderFactory dbfac = DocumentBuilderFactory.newInstance();
	        DocumentBuilder docBuilder = dbfac.newDocumentBuilder();
	        Document doc = docBuilder.newDocument();
	        
	        Element root = doc.createElement("SA-POP:taskMap");
	     
	        root.setAttribute("xmlns:SA-POP", "http://www.vanderbilt.edu/SA-POP");
	        root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	        root.setAttribute("xsi:schemaLocation", "http://www.vanderbilt.edu/SA-POP Task_Map.xsd");
	        root.setAttribute("xmlns", "http://www.vanderbilt.edu/SA-POP");
	        
	        for(TaskImpl t: task_impls.values()){
	        	
				if(!init_act_visible && t.getImplID().equals("initact_impl")){
					continue;
				}
	        	
	        	Element taskImpl = doc.createElement("taskImpl");
	        	
	        	Element implID = doc.createElement("implID");
	        	Text implText = doc.createTextNode(t.getImplID());
	        	implID.appendChild(implText);
	        	
	        	Element param = doc.createElement("param");
	        	
	        	Element paramID = doc.createElement("paramID");
	        	Text paramIDText = doc.createTextNode(t.getParam().getParamID());
	        	paramID.appendChild(paramIDText);
	        	
	        	Element paramKind = doc.createElement("kind");
	        	Text paramKindText = doc.createTextNode(t.getParam().getKind());
	        	paramKind.appendChild(paramKindText);
	        	
	        	Element paramValue = doc.createElement("value");
	        	Text paramValueText = doc.createTextNode(t.getParam().getValue());
	        	paramValue.appendChild(paramValueText);
	        	
	        	param.appendChild(paramID);
	        	param.appendChild(paramKind);
	        	param.appendChild(paramValue);
	        	
	        	taskImpl.appendChild(implID);
	        	taskImpl.appendChild(param);
	        	
	        	root.appendChild(taskImpl);
	        	
	        }
	        
	        for(Resource r: resources.values()){
	        	
	        	Element resource = doc.createElement("resource");
	        	
	        	Element resourceID = doc.createElement("resourceID");
	        	Text resourceText = doc.createTextNode(r.getResourceID());
	        	resourceID.appendChild(resourceText);
	        	
	        	Element kind = doc.createElement("kind");
	        	Text kindText = doc.createTextNode(r.getKind());
	        	kind.appendChild(kindText);
	        	
	        	Element capacity = doc.createElement("capacity");
	        	Text capacityText = doc.createTextNode(r.getCapacity());
	        	capacity.appendChild(capacityText);
	        	
	        	resource.appendChild(resourceID);
	        	resource.appendChild(kind);
	        	resource.appendChild(capacity);
	        	
	        	root.appendChild(resource);
	        	
	        }
	        
	        for(TaskToImpl t: task_to_impl_links){
	        	
	        	Element taskToImpl = doc.createElement("taskToImpl");
	        	
	        	Element taskID = doc.createElement("taskID");
	        	Text taskIDText = doc.createTextNode(t.getTaskID());
	        	taskID.appendChild(taskIDText);
	        	
	        	Element implID = doc.createElement("implID");
	        	Text implIDText = doc.createTextNode(t.getImplID());
	        	implID.appendChild(implIDText);
	        	
	        	Element duration = doc.createElement("duration");
	        	Text durationText = doc.createTextNode(t.getDuration());
	        	duration.appendChild(durationText);
	        	
	        	taskToImpl.appendChild(taskID);
	        	taskToImpl.appendChild(implID);
	        	taskToImpl.appendChild(duration);
	    
	        	root.appendChild(taskToImpl);
	        	
	        }
	        
	        for(ImplToResource i: impl_to_resource_links){
	        	
	        	Element implToResource = doc.createElement("implToResource");
	        	
	        	Element implID = doc.createElement("implID");
	        	Text implIDText = doc.createTextNode(i.getImplID());
	        	implID.appendChild(implIDText);
	        	
	        	Element resourceID = doc.createElement("resourceID");
	        	Text resourceIDText = doc.createTextNode(i.getResourceID());
	        	resourceID.appendChild(resourceIDText);
	        	
	        	Element utilization = doc.createElement("utilization");
	        	Text utilizationText = doc.createTextNode(i.getUtilization());
	        	utilization.appendChild(utilizationText);
	        	
	        	implToResource.appendChild(implID);
	        	implToResource.appendChild(resourceID);
	        	implToResource.appendChild(utilization);
	        	
	        	root.appendChild(implToResource);
	        }
	        
	        doc.appendChild(root);
	        
	        //set up a transformer
	        TransformerFactory transfac = TransformerFactory.newInstance();
	        Transformer trans = transfac.newTransformer();
	        trans.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "no");
	        trans.setOutputProperty(OutputKeys.INDENT, "yes");
	        trans.setOutputProperty(OutputKeys.STANDALONE, "no");

	        //create string from xml tree
	        StringWriter sw = new StringWriter();
	        StreamResult result = new StreamResult(sw);
	        DOMSource source = new DOMSource(doc);
	        trans.transform(source, result);
	        
	        FileWriter f = new FileWriter(filename);
	        f.write(sw.toString());
	        f.close();

		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void write_goals_to_file(String filename){
		try{
			FileWriter f = new FileWriter(filename);
			for(String s: goal_states.keySet()){
				f.write(condition_nodes.get(s).getNodeID()+"\n");
				f.write(new Integer((goal_states.get(s))?10:-10).toString()+"\n");
			}
			f.close();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
}
