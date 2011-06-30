import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Scanner;

import Translator.PDDL_Translator;
import Translator.StatisticsReport;

import pddl4j.Domain;
import pddl4j.ErrorManager;
import pddl4j.PDDLObject;
import pddl4j.Parser;
import pddl4j.Problem;
import pddl4j.RequireKey;
import pddl4j.Source;
import pddl4j.ErrorManager.Message;

public class PDDLtoSAN
{

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		
//		if(args.length != 5){
//			System.out.println("Wrong number of arguments: "+args.length);
//			System.exit(0);
//		}
//
//		String domain_path = args[0];
//		String problem_path = args[1];
//		boolean include_init = new Boolean(args[2]);
//		int cond_combine = new Integer(args[3]);
//		String output_name = args[4];
		
//		String domain_path = "TPP-domain.pddl";
//		String problem_path = "tpp-p08.pddl";
//		boolean include_init = false;
//		int cond_combine = 2;
//		String output_name = "ferry-orig-pruned";
		
		Properties options = new Properties();
		options.put("source", Source.V3_0);
		options.put(RequireKey.STRIPS, true);
		options.put(RequireKey.TYPING, true);
		options.put(RequireKey.EQUALITY, true);
		options.put(RequireKey.NEGATIVE_PRECONDITIONS, true);
		options.put(RequireKey.DISJUNCTIVE_PRECONDITIONS, true);
		options.put(RequireKey.CONDITIONAL_EFFECTS, true);
		
		Map<String, List<String>> domains_to_problems = read_input_file(args[0]);
		
	//	List<StatisticsReport> info_sets = new LinkedList<StatisticsReport>();
		
		StatisticsCompilation all_stat_compl = new StatisticsCompilation();
		
		int all_count = 0;
		
		for(String domain_path: domains_to_problems.keySet()){
			
			StatisticsCompilation stat_compl = new StatisticsCompilation();
			
			Domain domain = null;
			File domain_file = new File(domain_path);
			Parser parser = new Parser(options);
			
			try {
				domain = parser.parse(domain_file);
			} catch (FileNotFoundException e) {
				System.out.println("Error: domain file "+domain_file+" not found");
				System.exit(0);
			}
			
			for(String problem_path: domains_to_problems.get(domain_path)){
				File problem_file = new File(problem_path);
				Problem problem = null;
				Parser problem_parser = new Parser(options);
				
				try {
					problem = problem_parser.parse(problem_file);
				} catch (FileNotFoundException e) {
					System.out.println("Error: problem file "+problem_file+" not found");
					System.exit(0);
				}
				
				PDDLObject obj = problem_parser.link(domain, problem);
				ErrorManager mgr = parser.getErrorManager();
				
				if (mgr.contains(Message.ERROR)) {
					mgr.print(Message.ALL);
					mgr.clear();
					System.exit(0);
				}
				else {
					mgr.print(Message.WARNING);
					mgr.clear();
			
					System.out.println();
					System.out.println("Translating: "+ problem_file+ " " + domain_file);
					System.out.println();
					
					PDDL_Translator n = new PDDL_Translator(obj, 3, false);
					
					System.out.println("Done translating");
					System.out.println("  SAN number of conditions: "+ n.getConditonNodeCount());
					System.out.println("  SAN number of actions: " + n.getActionNodeCount());

					stat_compl.all_reports.add(n.getStatReport());
					all_stat_compl.all_reports.add(n.getStatReport());
					
			//		n.printSAN();
					
					int this_count = all_count++;
					
					n.write_SAN_to_xml("output\\"+this_count+"_"+domain.getDomainName()+"_"+problem.getProblemName()+"-san.xml");
					n.write_TM_to_xml("output\\"+this_count+"_"+domain.getDomainName()+"_"+problem.getProblemName()+"-tm.xml");
					n.write_goals_to_file("output\\"+this_count+"_"+domain.getDomainName()+"_"+problem.getProblemName()+"-goals");
				}
			}
			
//			try {
//			//	FileWriter fileout = new FileWriter(new File(domain_path+args[1]));
//			//	fileout.write(stat_compl.toString());
//			//	fileout.close();
//				
//			} catch (IOException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}		
			
		}
		
//		FileWriter fileout;
//		try {
//			fileout = new FileWriter(new File(args[1]));
//			fileout.write(all_stat_compl.toString());
//			fileout.close();	
//			
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}

	//	System.out.println(stat_compl.toString());
	}

	public static Map<String, List<String>> read_input_file(String filename)
	{
		//TODO make this work properly
		Map<String, List<String>> to_ret = new LinkedHashMap<String, List<String>>();
		Scanner scan = null;
		try
		{
			scan = new Scanner(new FileReader(new File(filename)));
		}
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		while (scan.hasNext())
		{
			String domain_name = scan.next();

			String next = scan.next();
			List<String> problems = new LinkedList<String>();

			while (!next.equals("END"))
			{
				problems.add(next);
				next = scan.next();
			}
			to_ret.put(domain_name, problems);
		}

		return to_ret;
	}
}