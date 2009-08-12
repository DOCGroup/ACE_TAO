package Translator;

import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import pddl4j.exp.Exp;
import pddl4j.exp.action.Action;
import pddl4j.exp.term.Constant;
import pddl4j.exp.term.Substitution;
import pddl4j.exp.term.Term;
import pddl4j.exp.term.Variable;
import pddl4j.exp.type.Type;
import pddl4j.exp.type.TypeSet;

public class ActionData{
	
	private Action a;
	private Substitution s;
	private List<Variable> free_parameters;
	private List<Variable> all_params;
	EqualityConstraintManager manager;
	private Map<Type, List<Constant> > type_list;
	
	public ActionData(Action p_a, Map<Type, List<Constant> > p_type_list){
		
		a = p_a;
		s = new Substitution();
		free_parameters = new LinkedList<Variable>();
		all_params = new LinkedList<Variable>();
		manager = new EqualityConstraintManager();
		type_list = p_type_list;
	}
	
	/**
	 * To save room when recursing, only copies certain data
	 * @param copy
	 */
	public ActionData(ActionData copy){
		
		a = copy.a;
		s =copy.s.clone();
		free_parameters = new LinkedList<Variable>(copy.free_parameters);
		all_params = copy.all_params;
		manager = new EqualityConstraintManager(copy.manager);
		type_list = copy.type_list;
	}
	
	public void add_free_variable(Variable v){
		free_parameters.add(v);
		all_params.add(v);
		manager.add_variable(v);
	}
	
	/**
	 * done first
	 * @param v
	 * @param not
	 */
	public void fix_variable_not_equal_variable(Variable v, Variable not){
		
		manager.assign_inequal_var_var(v, not);
	}	
	
	/**
	 * Second
	 * @param v
	 * @param to
	 */
	public void fix_variable_to_variable(Variable v, Variable to){

		manager.assign_equal_var_var(v, to);
	}

	/**
	 * third
	 * @param v
	 * @param not
	 */
	public void fix_variable_not_equal_constant(Variable v, Constant not){
		
		manager.assign_inequal_var_const(v, not);
	}
	
	
	/**
	 * note--only call this when reading in the preconditions.  Not while 
	 * enumerating!
	 * @param v
	 * @param to
	 */
	public void fix_variable_to_constant(Variable v, Constant to){
		
		s.bind(v, to);
		manager.assign_equal_var_const(v, to);
	}
	
	public void temp_fix_variable_to_constant(Variable v, Constant to){
		
		s.bind(v, to);
		free_parameters.remove(v);
		manager.temp_assign_equal_var_const(v, to);
		
	}
	
	public boolean has_free_variable(){
		return !free_parameters.isEmpty();
	}
	
	public Variable get_first_free_variable(){
		return free_parameters.get(0);
	}
	
	public List<String> get_all_substituted_parameters(){
		
		List<String> params = new LinkedList<String>();
		for(Term t: a.getParameters()){
			params.add(s.getBinding((Variable)t).toString());
		}
		
		return params;
	}
	
	public List<Constant> get_possible_values(Variable v){
		
		if(manager.variable_to_constraint.get(v).fixed_to == null)
			return manager.variable_to_constraint.get(v).possible_constant_values;
		else{
			
			List<Constant> possible = new LinkedList<Constant>();
			possible.add(manager.variable_to_constraint.get(v).fixed_to);
			return possible;
		}
	}
	
	public String get_action_name(){
		
		String action_name = a.getName();
		
		for(Variable v: all_params){
			action_name+=(" "+s.getBinding(v).toString());
		}
		
		return action_name;
	}
	
	public Exp get_precondition_expression(){
		return a.getPrecondition().apply(s);
	}
	
	public Exp get_effect_expression(){
		return a.getEffect().apply(s);
	}

	class EqualityConstraintManager{
		
		Map<Variable, VariableConstraint> variable_to_constraint = new LinkedHashMap<Variable, VariableConstraint>();

		public EqualityConstraintManager(EqualityConstraintManager copy){
			
			
			
			for(VariableConstraint constr: copy.variable_to_constraint.values()){
				

				VariableConstraint new_entry = new VariableConstraint(constr);
				for(Variable v: new_entry.equal_variables){
					variable_to_constraint.put(v, new_entry);
				}
			}
			
		}
		
		public EqualityConstraintManager(){
			
		}
		
		public void add_variable(Variable v){
			variable_to_constraint.put(v, new VariableConstraint(v));
		}
		
		public void assign_equal_var_var(Variable a, Variable b){
			
			VariableConstraint b_const = variable_to_constraint.get(b);
			VariableConstraint a_const = variable_to_constraint.get(a);
			
			for(Variable v: b_const.inequality_variable_constraints){
				if(a_const.equal_variables.contains(v)){
					a_const.possible_constant_values.clear();
				}
			}

			List<Variable> new_equal_variables = new LinkedList<Variable>(a_const.equal_variables);
			for(Variable v: b_const.equal_variables){
				variable_to_constraint.put(v, a_const);
				new_equal_variables.add(v);
			}
			
			a_const.equal_variables = new_equal_variables;
			a_const.possible_constant_values.retainAll(b_const.possible_constant_values);
			a_const.inequality_variable_constraints.addAll(b_const.inequality_variable_constraints);
		}
		
		public void assign_inequal_var_var(Variable a, Variable b){
			
			VariableConstraint b_const = variable_to_constraint.get(b);
			VariableConstraint a_const = variable_to_constraint.get(a);
			
			a_const.inequality_variable_constraints.add(b);
			b_const.inequality_variable_constraints.add(a);
			
			if(a.equals(b)){
				a_const.possible_constant_values.clear();
			}
		}
		
		public void assign_inequal_var_const(Variable a, Constant b){
			variable_to_constraint.get(a).possible_constant_values.remove(b);
		}
		
		public void assign_equal_var_const(Variable a, Constant b){
			
			VariableConstraint a_const = variable_to_constraint.get(a);
			
			if(a_const.possible_constant_values.contains(b)){
			
				a_const.possible_constant_values.clear();
				a_const.possible_constant_values.add(b);
				
				for(Variable v: a_const.inequality_variable_constraints){
					variable_to_constraint.get(v).possible_constant_values.remove(b);
				}
			}else{
				a_const.possible_constant_values.clear();
			}
		}
		
		public void temp_assign_equal_var_const(Variable a, Constant b){
			
			VariableConstraint a_const = variable_to_constraint.get(a);
			
			if(a_const.fixed_to != null){
				
				for(Variable v: a_const.inequality_variable_constraints){
					
					VariableConstraint v_const = variable_to_constraint.get(v);
					if(v_const.denied_by.contains(a)){
						v_const.possible_constant_values.add(a_const.fixed_to);
					}
				}
			}
			
			a_const.fixed_to = b;
			
			for(Variable v: a_const.inequality_variable_constraints){
				
				VariableConstraint v_const = variable_to_constraint.get(v);
				
				if(v_const.possible_constant_values.contains(b)){
					v_const.denied_by.add(a);
					v_const.possible_constant_values.remove(b);
				}
			}
		}
		
		class VariableConstraint{

			public VariableConstraint(VariableConstraint copy){
				equal_variables = copy.equal_variables;
				inequality_variable_constraints = copy.inequality_variable_constraints;
				possible_constant_values = new LinkedList<Constant>(copy.possible_constant_values);
				denied_by = new LinkedList<Variable>(copy.denied_by);
				fixed_to = copy.fixed_to;
			}
			
			public VariableConstraint(Variable v){
				equal_variables.add(v);
				
				possible_constant_values = new LinkedList<Constant>();
				
				Iterator<Type> type_it = v.getTypeSet().iterator();
				
				while(type_it.hasNext()){
					Type typ = type_it.next();
					if(type_list.get(typ) != null){
						possible_constant_values.addAll(type_list.get(typ));
					}
				}
				
				denied_by = new LinkedList<Variable>();
				fixed_to = null;
			}
			
			public List<Variable> equal_variables = new LinkedList<Variable>();
			public List<Variable> inequality_variable_constraints = new LinkedList<Variable>();
			public List<Constant> possible_constant_values;
			public List<Variable> denied_by;
			
			public Constant fixed_to;
		}
	}
}