// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_POP_Demo.cpp
 *
 * This file contains the main() function for the SA-POP demo.
 * 11/19/08 Added the ability for multiple goals - dmack
 * 11/25/08 Added the use of the LogGraph Out Adapter for Visual purposes - dmack
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
#include "SA_Builder.h"
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
  namespace C2W {
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
}

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

}

int main (int argc, char* argv[])
{
  SA_POP::SA_Builder builder;
  SANet::SANetFileIn sanet_in;
  SA_POP::TaskMapFileIn tm_in;
  SA_POP::Planner *planner = 0;
  SA_POP::LogGraphOut graph_out	(std::cout, false);

  UserInterface::InputCL user_input;
  std::string sanet_filename = "";
  std::string tm_filename = "";
  size_t max_steps = SANet::Default::SA_MAX_STEPS;

  std::vector<SA_POP::CondID> track_conds;
  std::vector<SA_POP::CondID> kconds;
  std::map<SA_POP::CondID, double> condMap;


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
  planner = builder.get_planner ();

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

  

  // Get number of conditions to track.
  int track = 0;
  //Conditions to be potentially displayed 
	std::cout << "How many conditions to track? ";
	std::cin >> track;

  // Get conditions to track.
	for(int t = 0; t < track; t++)
	{
		int cid = 0;
		std::cout << "Enter the Condition ID to track:";
		std::cin >> cid;
		SA_POP::CondID ccid = SA_POP::CondID(cid);
    condMap.insert(std::make_pair(ccid, 1));
		track_conds.push_back(ccid);
	
	}

  //Configure the OutAdapters to use
  SA_POP::LogScreenOut screen_out (std::cout);
  graph_out.addTracking(track_conds);
  planner->add_out_adapter (&graph_out);
  //SA_POP::SchemaOut s_out	(std::cout, kconds);
  //planner->add_out_adapter (&s_out);
  //planner->add_out_adapter (&screen_out);

  planner->plan (max_steps, goal);
  SA_POP::Plan plan = planner->get_plan ();
  SA_POP::Utility plan_eu = planner->calc_plan_eu (plan);
  std::cout << "Expected utility of generated plan:  " << plan_eu << std::endl;


  UserInterface::QuestionChoice<UserInterface::C2W::NextKind> ques_next ("Negate an (E)ffect, change an external (C)ondition, or e(X)it:", UserInterface::C2W::INVALID, false);
  ques_next.add_mapping ("E", UserInterface::C2W::EFFECT);
  ques_next.add_mapping ("Effect", UserInterface::C2W::EFFECT);
  ques_next.add_mapping ("C", UserInterface::C2W::CONDITION);
  ques_next.add_mapping ("Condition", UserInterface::C2W::CONDITION);
  ques_next.add_mapping ("X", UserInterface::C2W::EXIT);
  ques_next.add_mapping ("Exit", UserInterface::C2W::EXIT);


  bool stop = false;
  while(!stop)
  {
	  displayConds(planner, track_conds, &condMap);

    UserInterface::QuestionBool adv_ques ("Advance to next time step? [(Y)es or (N)o]:", false);
    user_input.ask (adv_ques);
    if (adv_ques.get_answer_bool ())
	  {
		  graph_out.moveStep();
		  planner->plan (max_steps, goal);
	  }
	  else
	  {
      UserInterface::C2W::NextKind next_val = UserInterface::C2W::INVALID;
      user_input.ask (ques_next);
      next_val = ques_next.get_answer_val ();

      switch (next_val)
      {
      case UserInterface::C2W::EFFECT:
			  SA_POP::TaskID curTask;
			  SA_POP::CondID curEff;
			  std::cout << "Enter the Task ID: ";
			  std::cin >> curTask;
			  std::cout << "Enter the Condition ID: ";
			  std::cin >> curEff;

        for(std::map<SANet::CondID, double>::iterator cIter = condMap.begin(); cIter != condMap.end(); cIter++)
        {
          planner->update_cond_val((*cIter).first, (*cIter).second);
        }
        planner->update_effect(curTask, curEff, -1);

        planner->replan(max_steps, goal);

        break;

      case UserInterface::C2W::CONDITION:
      {
			  SA_POP::CondID envi;
			  SA_POP::Probability newprob;
			  std::cout << "Enter the Condition ID: ";
			  std::cin >> envi;
			  std::cout << "Enter the Probability: ";
			  std::cin >> newprob;
        std::map<SANet::CondID, double>::iterator cmp = condMap.find(envi);
        if(cmp != condMap.end())
          (*cmp).second = newprob;

        for(std::map<SANet::CondID, double>::iterator cIter = condMap.begin(); cIter != condMap.end(); cIter++)
        {
          planner->update_cond_val((*cIter).first, (*cIter).second);
        }

        planner->replan(max_steps, goal);
      }
        break;

      case UserInterface::C2W::EXIT:
      case UserInterface::C2W::INVALID:
      default:
			  stop = true;
        break;
      }
	  }
  }

  delete planner;

  // Wait for user to end program.
//  UserInterface::Question end_ques ("Enter any character to end program:");
//  user_input.ask (end_ques);


  _CrtDumpMemoryLeaks();

  return 0;
};


int test_listmultimap (void)
{
  SA_POP::ListMultiMap<int, int> intList;
  
  intList.insert(std::make_pair(1,2));
  intList.insert(std::make_pair(5,6));
  intList.insert(std::make_pair(5,7));
  
  for (SA_POP::ListMultiMap<int, int>::iterator list_iter = intList.lower_bound (5);list_iter != intList.upper_bound (5);list_iter++)
  {
    std::cout << list_iter->second << std::endl;
  }

  for (SA_POP::ListMultiMap<int, int>::iterator list_iter = intList.begin();list_iter != intList.end();list_iter++)
  {
    std::cout << list_iter->first << " FOR JOHN " <<list_iter->second << std::endl;
  }

  intList.erase(intList.lower_bound (5), intList.upper_bound (5));

  for (SA_POP::ListMultiMap<int, int>::iterator list_iter = intList.begin();list_iter != intList.end();list_iter++)
  {
    std::cout << list_iter->first << " FOR JOHN " <<list_iter->second << std::endl;
  }

  intList.push_front(std::make_pair(9,9));
  intList.push_front(std::make_pair(8,8));
  intList.push_back(std::make_pair(7,7));

  std::cout << "Front: " <<intList.front().first << " and " <<  intList.front().second << std::endl;
  std::cout << "Back: " <<intList.back().first << " and " <<  intList.back().second << std::endl;
  intList.pop_front();
  std::cout << "Front: " <<intList.front().first << " and " <<  intList.front().second << std::endl;
  std::cout << "Back: " <<intList.back().first << " and " <<  intList.back().second << std::endl;
  intList.push_front(std::make_pair(12,12));
  intList.push_front(std::make_pair(7,7));
  for (SA_POP::ListMultiMap<int, int>::iterator list_iter = intList.begin();list_iter != intList.end();list_iter++)
  {
    std::cout << list_iter->first << " FOR JOHN " <<list_iter->second << std::endl;
  }
  std::cout << "Front: " <<intList.front().first << " and " <<  intList.front().second << std::endl;
  std::cout << "Back: " <<intList.back().first << " and " <<  intList.back().second << std::endl;
  intList.pop_back();
  std::cout << "Front: " <<intList.front().first << " and " <<  intList.front().second << std::endl;
  std::cout << "Back: " <<intList.back().first << " and " <<  intList.back().second << std::endl;
  for (SA_POP::ListMultiMap<int, int>::iterator list_iter = intList.begin();list_iter != intList.end();list_iter++)
  {
    std::cout << list_iter->first << " FOR JOHN " <<list_iter->second << std::endl;
  }
  intList.push_back(std::make_pair(7,7));
  std::cout << "Front: " <<intList.front().first << " and " <<  intList.front().second << std::endl;
  std::cout << "Back: " <<intList.back().first << " and " <<  intList.back().second << std::endl;
  intList.erase(intList.lower_bound (7));
  std::cout << "Front: " <<intList.front().first << " and " <<  intList.front().second << std::endl;
  std::cout << "Back: " <<intList.back().first << " and " <<  intList.back().second << std::endl;
  for (SA_POP::ListMultiMap<int, int>::iterator list_iter = intList.begin();list_iter != intList.end();list_iter++)
  {
    std::cout << list_iter->first << " FOR JOHN " <<list_iter->second << std::endl;
  }

  return 0;
};

