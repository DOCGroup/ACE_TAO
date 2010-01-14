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
  

  // Get SA-POP Planner.
  planner = builder.get_exp_eu_planner ();

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
  UserInterface::QuestionInt steps_ques("Max steps to run (10000 step limit): ", 0, 10000);
  if (user_input.ask (steps_ques))
    max_steps = steps_ques.get_answer_int ();

  

  //Configure the OutAdapters to use
  SA_POP::LogScreenOut screen_out (std::cout);
  planner->add_out_adapter (&screen_out);
  planner->add_out_adapter (&graph_out);
  //SA_POP::SchemaOut s_out	(std::cout, kconds);
  //planner->add_out_adapter (&s_out);

  // Set planner to ask whether continue after each plan is generated.
  UserInterface::QuestionBool ques_cont_plan ("Continue planning to find next plan? [(Y)es or (N)o]:", false);
  planner->set_pause (&user_input, &ques_cont_plan);

  // Run experiment.
  planner->plan (max_steps, goal);

//  SA_POP::Plan plan = planner->get_plan ();
//  SA_POP::Utility plan_eu = planner->calc_plan_eu (plan);
//  std::cout << "Expected utility of generated plan:  " << plan_eu << std::endl;


  delete planner;

  // Wait for user to end program.
//  UserInterface::Question end_ques ("Enter any character to end program:");
//  user_input.ask (end_ques);

  _CrtDumpMemoryLeaks();

  return 0;
};


