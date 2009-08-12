import java.io.File;
import java.io.FileNotFoundException;
import java.util.Properties;

import Translator.PDDL_Translator;

import pddl4j.Domain;
import pddl4j.ErrorManager;
import pddl4j.PDDLObject;
import pddl4j.Parser;
import pddl4j.Problem;
import pddl4j.RequireKey;
import pddl4j.Source;
import pddl4j.ErrorManager.Message;

public class PDDLtoSAN {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		if(args.length != 5){
			System.out.println("Wrong number of arguments: "+args.length);
			System.exit(0);
		}

		String domain_path = args[0];
		String problem_path = args[1];
		boolean include_init = new Boolean(args[2]);
		int cond_combine = new Integer(args[3]);
		String output_name = args[4];
		
//		String domain_path = "ferry.pddl";
//		String problem_path = "pb1ferry.pddl";
//		boolean include_init = false;
//		int cond_combine = 3;
//		String output_name = "ferry";
		
		Domain domain = null;
		Problem problem = null;

		Properties options = new Properties();
		options.put("source", Source.V3_0);
		options.put(RequireKey.STRIPS, true);
		options.put(RequireKey.TYPING, true);
		options.put(RequireKey.EQUALITY, true);
		options.put(RequireKey.NEGATIVE_PRECONDITIONS, true);
		options.put(RequireKey.DISJUNCTIVE_PRECONDITIONS, true);
		options.put(RequireKey.CONDITIONAL_EFFECTS, true);
		// options.put(RequireKey.EXISTENTIAL_PRECONDITIONS, true);
		// options.put(RequireKey.UNIVERSAL_PRECONDITIONS, true);

		Parser parser = new Parser(options);

		File domain_file = new File(domain_path);
		File problem_file = new File(problem_path);

		try {
			domain = parser.parse(domain_file);
		} catch (FileNotFoundException e) {
			System.out.println("Error: domain file "+domain_file+" not found");
			System.exit(0);
		}
		
		try {
			problem = parser.parse(problem_file);
		} catch (FileNotFoundException e) {
			System.out.println("Error: problem file "+problem_file+" not found");
			System.exit(0);
		}
			
		PDDLObject obj = parser.link(domain, problem);
	
		ErrorManager mgr = parser.getErrorManager();
			
		if (mgr.contains(Message.ERROR)) {
			mgr.print(Message.ALL);
		}
		else {
			mgr.print(Message.WARNING);
	
			System.out.println();
			System.out.println("Translating:");
			
			PDDL_Translator n = new PDDL_Translator(obj, cond_combine, include_init);
			
			System.out.println("Done translating");
			System.out.println("  SAN number of conditions: "+ n.getConditonNodeCount());
			System.out.println("  SAN number of actions: " + n.getActionNodeCount());

			n.printSAN();
			n.write_SAN_to_xml(output_name+"-san.xml");
			n.write_TM_to_xml(output_name+"-tm.xml");
			n.write_goals_to_file(output_name+"-goals");
		}
	}
}
