// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SAPOP_Exp_EU.cpp
 *
 * This file contains the main() function for the SA-POP Expected Utility
 * experiment.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 * @author  Daniel L.C. Mack  <daniel.l.mack@vanderbilt.edu>
 */
//=============================================================================

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "SA_POP_Types.h"
#include "Exp_Core/Exp_EU_Builder.h"
#include "Exp_Core/Exp_EU_Planner.h"
#include "TaskMapFileIn.h"
#include "SANet/SANetFileIn.h"
#include "LogScreenOut.h"
#include "LogGraphOut.h"
//#include "SA_POP_Utils.h"

#include "InputCL.h"

// Additional SA-POP DEFAULT values for use in demo.
namespace SA_POP {
  namespace Default {
    const SA_POP::CondID CondIDMin = 1;
    const SA_POP::CondID CondIDMax = 999999;
    const SA_POP::Utility GoalUtilMin = -1000;
    const SA_POP::Utility GoalUtilMax = 1000;
    const SA_POP::Probability CondProbMin = 0.0;
    const SA_POP::Probability CondProbMax = 1.0;
    const size_t NumGoalsMin = 1;
    const size_t NumGoalsMax = 10;
    const size_t NumKnownCondsMin = 0;
    const size_t NumKnownCondsMax = 100;
    const size_t NumTrialsMin = 1;
    const size_t NumTrialsMax = 1000;

    // Default percentage of initial conditions to set as true (valid values: [0, 100]).
    const size_t NumPercentCondsInit = 30;

    // Multiplier to determine max trial attempts from number of trials to achieve.
    const size_t TrialAttemptsMult = 10;
  };  /* SA_POP::Default namespace */
};  /* SA_POP namespace */

// Additional SANet DEFAULT values for use in demo.
namespace SANet {
  namespace Default {
    const size_t SA_MAX_STEPS = 1000;
  };  /* SANet::Default namespace */
};  /* SANet namespace */


// Additional UI DEFAULT values for use in demo.
namespace UserInterface {
  namespace ExpEU {
    /// An enumerated type for user input in run loop.
    enum NextKind {EFFECT, CONDITION, EXIT, INVALID};
  };  /* UserInterface::Testing namespace */
};  /* UserInterface namespace */



//Commands to run in the thread
unsigned long WINAPI ThirdThread(PVOID pvParam)
{
	system("dot -Tgif cond.dot -o conds.gif");
	//system("conds.gif");
	return 0;
};

//for displaying conditions
void displayConds(SA_POP::Planner *plans, std::vector<SANet::CondID> checks, std::map<SANet::CondID, double> *cMap)
{

  std::ofstream cfile;
  cfile.open("Cond.dot");
  //Produce Graph
  std::cout << "Create Digraph" << std::endl;
  cfile << "strict digraph conds {\n";
  for(size_t node = 0; node< checks.size(); node++)
  {
	  std::cout << "Next Cond" << std::endl;
	 
	  if(plans->get_cond_val(checks[node]) == 1)
	  {
      std::map<SANet::CondID, double>::iterator cmp = cMap->find(checks[node]);
      if(cmp != cMap->end())
      {
        (*cmp).second = 1;
      }
		cfile <<  "\t" << "\"" << plans->get_cond_name(checks[node]) << " " << checks[node] << "\" [shape = box, color = green];\n";
	  }
	  else
	  {  
      std::map<SANet::CondID, double>::iterator cmp = cMap->find(checks[node]);
      if(cmp != cMap->end())
      {
        (*cmp).second = 0;
      }
		cfile <<  "\t" << "\"" << plans->get_cond_name(checks[node]) << " " << checks[node] << "\" [shape = box, color = red];\n";
	  }
	  std::cout << "Ready for Next Cond" << std::endl;

  }
  cfile << "}\n";
  
  cfile.close();

  unsigned long dwThreadId;
  // Create a new thread.
  char *imageName = "conds.gif";
  HANDLE tThread = CreateThread(NULL, 0, ThirdThread, (PVOID) imageName,
								0, &dwThreadId);
  CloseHandle(tThread);

};







int main (int argc, char* argv[])
{
  SA_POP::Exp_EU_Builder builder;
  SANet::SANetFileIn sanet_in;
  SA_POP::TaskMapFileIn tm_in;
  SA_POP::Exp_EU_Planner *planner = 0;
  SA_POP::LogGraphOut graph_out	(std::cout, false);
  SA_POP::LogScreenOut screen_out (std::cout);

  UserInterface::InputCL user_input;
  std::string sanet_filename = "";
  std::string tm_filename = "";
  size_t max_steps = SANet::Default::SA_MAX_STEPS;

  std::vector<SA_POP::CondID> kconds;

  // Get filenames from user.
  UserInterface::Question sanet_file_ques ("Task Network file:");
  if (user_input.ask (sanet_file_ques))
    sanet_filename = sanet_file_ques.get_answer ();
  else {
    std::cerr << "No Task Network filename provided." << std::endl;
    throw "No Task Network filename provided.";
  }
  UserInterface::Question tm_file_ques ("Task Map file:");
  if (user_input.ask (tm_file_ques))
    tm_filename = tm_file_ques.get_answer ();
  else {
    std::cerr << "No Task Map filename provided." << std::endl;
    throw "No Task Map filename provided.";
  }

  
  // Build task network and task map.
  try {
    sanet_in.build_net (sanet_filename, &builder);
    tm_in.build_task_map (tm_filename, &builder);
  } catch (std::string e) {
    std::cerr << "ERROR while building task network and task map from files:";
    std::cerr << std::endl;
    std::cerr << e;
  } catch (...) {
    std::cerr << "UNKNOWN ERROR while building task network and task map from files." << std::endl;
  }

  // Get SA-POP Planner.
  planner = builder.get_exp_eu_planner ();

  // Run manual or automatic trial based on user input.
  UserInterface::QuestionBool auto_trials_ques ("Automatic goal & initial condition generation? [(Y)es or (N)o]:", false);
  user_input.ask (auto_trials_ques);
  if (auto_trials_ques.get_answer_bool ()) {
    // Get trial log filename.
    std::string log_trials_filename = "default_trials_filename.log";
    UserInterface::Question log_trials_filename_ques ("Filename for trial log:");
    if (user_input.ask (log_trials_filename_ques))
      log_trials_filename = log_trials_filename_ques.get_answer ();

    // Get run log filename.
    std::string log_runs_filename = "default_runs_filename.log";
    UserInterface::Question log_runs_filename_ques ("Filename for run log:");
    if (user_input.ask (log_runs_filename_ques))
      log_runs_filename = log_runs_filename_ques.get_answer ();

    // Get number of experimental trials to perform.
    size_t num_trials = 1;
    UserInterface::QuestionInt num_trials_ques ("Number of trials to run:", SA_POP::Default::NumTrialsMin, SA_POP::Default::NumTrialsMax);
    if (user_input.ask (num_trials_ques))
      num_trials = num_trials_ques.get_answer_int ();

    // Get number of goal conditions.
    size_t num_goals = 1;
    UserInterface::QuestionInt num_goals_ques ("Number of goal conditions:", SA_POP::Default::NumGoalsMin, SA_POP::Default::NumGoalsMax);
    if (user_input.ask (num_goals_ques))
      num_goals = num_goals_ques.get_answer_int ();

    // Get percent of initial conditions true.
    size_t num_percent_conds_init = SA_POP::Default::NumPercentCondsInit;
    UserInterface::QuestionInt conds_init_ques ("Percentage of initial conditions true [0 to 100]:", 0, 100);
    if (user_input.ask (conds_init_ques))
      num_percent_conds_init = conds_init_ques.get_answer_int ();
    double percent_conds_init = (double) num_percent_conds_init / 100.0;

    // Get min goal utility.
    size_t min_util = 100;
    UserInterface::QuestionInt min_util_ques ("Minimum goal utility:", SA_POP::Default::GoalUtilMin, SA_POP::Default::GoalUtilMax);
    if (user_input.ask (min_util_ques))
      min_util = min_util_ques.get_answer_int ();

    // Get max goal utility.
    size_t max_util = min_util;
    UserInterface::QuestionInt max_util_ques ("Maximum goal utility:", min_util, SA_POP::Default::GoalUtilMax);
    if (user_input.ask (max_util_ques))
      max_util = max_util_ques.get_answer_int ();

    // Get maximum steps of spreading activation from user.
    UserInterface::QuestionInt steps_ques("Max steps to run spreading activation (10000 step limit): ", 0, 10000);
    if (user_input.ask (steps_ques))
      max_steps = steps_ques.get_answer_int ();

    // Get whether to output log headers.
    bool do_log_headers = false;
    UserInterface::QuestionBool log_headers_ques ("Output headers in log files? [(Y)es or (N)o]:", false);
    user_input.ask (log_headers_ques);
    do_log_headers = log_headers_ques.get_answer_bool ();

    // Output plans to screen if user wishes.
    UserInterface::QuestionBool screenout_ques ("Output plans to screen? [(Y)es or (N)o]:", false);
    user_input.ask (screenout_ques);
    if (screenout_ques.get_answer_bool ())
      planner->add_out_adapter (&screen_out);

    // Pause between plans if user wishes.
    UserInterface::QuestionBool ques_cont_plan ("Continue planning to find next plan? [(Y)es or (N)o]:", false);
    UserInterface::QuestionBool pause_ques ("Pause between plans? [(Y)es or (N)o]:", false);
    user_input.ask (pause_ques);
    if (pause_ques.get_answer_bool ()) {
      // Set planner to ask whether continue after each plan is generated.
      planner->set_pause (&user_input, &ques_cont_plan);

      // Output graphical plans if user wishes.
      UserInterface::QuestionBool graphout_ques ("Output graphical plans? [(Y)es or (N)o]:", false);
      user_input.ask (graphout_ques);
      if (graphout_ques.get_answer_bool ())
        planner->add_out_adapter (&graph_out);
    }


//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    std::cout << "Experimental parameters:" << std::endl;
    std::cout << "   Max SA steps: " << max_steps << std::endl;
    std::cout << "   Num goals: " << num_goals << std::endl;
    std::cout << "   Percent init conds true: " << percent_conds_init << std::endl;
    std::cout << "   Goal min utility: " << min_util << std::endl;
    std::cout << "   Goal max utility: " << max_util << std::endl;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

    // Run trials.
    planner->exp_run (max_steps, log_trials_filename, log_runs_filename, num_goals, percent_conds_init, min_util, max_util, num_trials * SA_POP::Default::TrialAttemptsMult, num_trials, do_log_headers);
  } else {
    
    // Create empty goal.
    SA_POP::Goal goal;
    goal.goal_id = "UserSpecifiedGoal ID";
    goal.name = "User specified goal";
    goal.abs_times.clear ();
    goal.rel_times.clear ();
    goal.goal_conds.clear ();
    goal.start_window = std::make_pair (0, 0);


    // Get number of goal conditions.
    size_t num_goals = 0;
    UserInterface::QuestionInt num_goals_ques ("Number of goal conditions:", SA_POP::Default::NumGoalsMin, SA_POP::Default::NumGoalsMax);
    if (user_input.ask (num_goals_ques))
      num_goals = num_goals_ques.get_answer_int ();
    else {
      std::cerr << "Invalid number of goals provided." << std::endl;
      throw "Invalid number of goals provided.";
    }

    // Get condition ID and utility for each goal condition.
    for(size_t g = 0; g < num_goals; g++)
    {
      SA_POP::CondID goal_id = 0;
      SA_POP::Utility goal_util = 0;
      UserInterface::QuestionInt goal_id_ques ("Goal condition ID:", SA_POP::Default::CondIDMin, SA_POP::Default::CondIDMax);
      if (user_input.ask (goal_id_ques))
        goal_id = goal_id_ques.get_answer_int ();
      else {
        std::cerr << "No valid goal condition ID provided." << std::endl;
        throw "No valid goal condition ID provided.";
      }
      UserInterface::QuestionInt goal_util_ques ("Goal utility:", SA_POP::Default::GoalUtilMin, SA_POP::Default::GoalUtilMax);
      if (user_input.ask (goal_util_ques))
        goal_util = goal_util_ques.get_answer_int ();
      else {
        std::cerr << "No valid goal condition utility provided." << std::endl;
        throw "No valid goal condition utility provided.";
      }
      goal.goal_conds.insert (std::make_pair (goal_id, goal_util));
    }
    

    // Get number of initial conditions to set (different from probability value in initial network from XML).
    size_t num_known_conds = 0;
    UserInterface::QuestionInt num_known_conds_ques ("Number of intial conditions to set:", SA_POP::Default::NumKnownCondsMin, SA_POP::Default::NumKnownCondsMax);
    if (user_input.ask (num_known_conds_ques))
      num_known_conds = num_known_conds_ques.get_answer_int ();

    // Set initial conditions.
    for(size_t k = 0; k < num_known_conds; k++)
    {
      SA_POP::CondID cond_id = 0;
      SA_POP::Probability cond_prob = 0;

      UserInterface::QuestionInt cond_id_ques ("Initial condition ID:", SA_POP::Default::CondIDMin, SA_POP::Default::CondIDMax);
      if (user_input.ask (cond_id_ques))
        cond_id = cond_id_ques.get_answer_int ();
      else
        break;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
// TO DO:  CREATE QuestionRange template class to replace QuestionInt and use one with a type of double for condition probability.
      UserInterface::QuestionInt cond_prob_ques ("Initial condition probability (of true):", SA_POP::Default::CondProbMin, SA_POP::Default::CondProbMin);
      if (user_input.ask (cond_prob_ques))
        cond_prob = cond_prob_ques.get_answer_int ();
      else
        break;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

      // Update condition to provided value.
	    planner->update_cond_val (cond_id, cond_prob);

      // Add condition to known conditions list.
	    kconds.push_back(cond_id);
    }

    // Get maximum steps of spreading activation from user.
    UserInterface::QuestionInt steps_ques("Max steps to run spreading activation (10000 step limit): ", 0, 10000);
    if (user_input.ask (steps_ques))
      max_steps = steps_ques.get_answer_int ();


    // Configure and add OutAdapters.
    planner->add_out_adapter (&screen_out);
    planner->add_out_adapter (&graph_out);
    //SA_POP::SchemaOut s_out	(std::cout, kconds);
    //planner->add_out_adapter (&s_out);

    // Set planner to ask whether continue after each plan is generated.
    UserInterface::QuestionBool ques_cont_plan ("Continue planning to find next plan? [(Y)es or (N)o]:", false);
    planner->set_pause (&user_input, &ques_cont_plan);

    // Run experiment.
    planner->plan (max_steps, goal);

//    SA_POP::Plan plan = planner->get_plan ();
//    SA_POP::Utility plan_eu = planner->calc_plan_eu (plan);
//    std::cout << "Expected utility of generated plan:  " << plan_eu << std::endl;
  }


  delete planner;

  // Wait for user to end program.
//  UserInterface::Question end_ques ("Enter any character to end program:");
//  user_input.ask (end_ques);

  _CrtDumpMemoryLeaks();

  return 0;
};


