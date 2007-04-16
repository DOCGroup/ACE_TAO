// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_POP_Demo.cpp
 *
 * This file contains the main() function for the SA-POP demo.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include <iostream>
#include "SA_POP_Types.h"
#include "SA_Builder.h"
#include "TaskMapFileIn.h"
#include "SANet/SANetFileIn.h"
#include "LogScreenOut.h"


int main (int argc, char* argv[])
{
  SA_POP::SA_Builder builder;
  SANet::SANetFileIn sanet_in;
  SA_POP::TaskMapFileIn tm_in;

  // Get filenames from user.
  std::cout << "Task Network file: ";
  std::string sanet_filename = "../examples/SPACE.san.xml";
//  std::string sanet_filename = "examples/test_graph1.san.xml";
//  std::cin >> sanet_filename;

  std::cout << "Task Map file: ";
  std::string tm_filename = "../examples/SPACE.tm.xml";
//  std::string tm_filename = "examples/test_graph.tm.xml";
//  std::cin >> tm_filename;

  SA_POP::Goal goal;
  goal.goal_id = "UserSpecifiedGoal ID";
  goal.name = "User specified goal";
  goal.abs_times.clear ();
  goal.rel_times.clear ();
  goal.goal_conds.clear ();
  goal.start_window = std::make_pair (0, 0);

  // Get goal.
  SA_POP::CondID goal_id;
  SA_POP::Utility goal_util;
  std::cout << "Goal condition ID: ";
  std::cin >> goal_id;
  std::cout << "Goal utility: ";
  std::cin >> goal_util;
  goal.goal_conds.insert (std::make_pair (goal_id, goal_util));
  
  try {
    sanet_in.build_net (sanet_filename, &builder);
    tm_in.build_task_map (tm_filename, &builder);
  } catch (std::string e) {
    std::cerr << "ERROR while building task network and task map from files:";
    std::cerr << std::endl;
    std::cerr << e;
  } catch (...) {
    std::cerr << "UNKNOWN ERROR while planning." << std::endl;
  }


  SA_POP::Planner *planner = 0;

  try {
    planner = builder.get_planner ();

    SA_POP::LogScreenOut screen_out (std::cout);
    planner->add_out_adapter (&screen_out);

    planner->plan (100, goal);
  } catch (std::string e) {
    std::cerr << "ERROR while planning:" << std::endl;
    std::cerr << e;
    delete planner;
  } catch (...) {
    std::cerr << "UNKNOWN ERROR while planning." << std::endl;
    delete planner;
  }

  delete planner;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  std::cout << "Enter any character to end program: ";
  char temp_;
  std::cin>>temp_;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

  return 0;
}
