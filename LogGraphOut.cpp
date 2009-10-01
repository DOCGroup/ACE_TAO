// -*- C++ -*-
// 

//=============================================================================
/**
 * @file  LogGraphOut.cpp
 *
 * This file contains the LogGraphOut class implementation for the output adapter
 * that displays graph visualizations of the plan.
 *
 * @author  Daniel L.C. Mack <daniel.l.mack@vanderbilt.edu>
 */
//=============================================================================

//For Spawning a new Thread:
//#include <windows.h>


#include <iostream>
#include <algorithm>
#include <time.h>
#include "SA_POP_Types.h"
#include "LogGraphOut.h"
#include "SANet/SANetFileIn.h"
#include "Planner.h"
#include <fstream>
#include <vector>
//#include "Thread.h"
//#include "gvc.h"
//#include "mfg_draw_graph.h"
//#include "mfg_graph_drawer.h"

using namespace SA_POP;

// Constructor.
LogGraphOut::LogGraphOut (std::ostream &out, bool startstep)
: out_ (out)
{
  // Set up the current step
  if(startstep)
  {
    curStep = 1;
  }
  else
  {
    curStep = 0;
  }
  byStep = startstep;
  hasTracks = false;
}

// Destructor.
LogGraphOut::~LogGraphOut (void)
{
  // Nothing to do.
}


//add Tracking
void LogGraphOut::addTracking(std::vector<SANet::CondID> checks)
{
  tracks = checks;
  hasTracks = true;
}

// Notify about task future expected utility changes.
void LogGraphOut::notify_eu (SA_POP::Planner *planner)
{
  time_t cur_time = time(0);
  this->out_ << std::endl;
  this->out_ << "EU Changes:" << std::endl;
  this->out_ << ctime (&cur_time) << std::endl;

  TaskEUMap eu_changes = planner->get_eu_changes ();
  for (TaskEUMap::iterator iter = eu_changes.begin ();
    iter != eu_changes.end ();
    iter++)
  {
    this->out_ << "Task " << iter->first << " = " << iter->second;
    this->out_ << std::endl;
  }
}


unsigned long WINAPI SecondThread(PVOID pvParam)
{
	system("dot -Tgif GViz.dot -o step.gif");
	system("step.gif");
	return 0;
}



// Notify about plan changes.
void LogGraphOut::notify_plan (SA_POP::Planner *planner)
{
  Plan plan = planner->get_plan ();

  time_t cur_time = time(0);
  bool done = false;


  //This sends planning info not included in the graph to the console,  about timing windows.
  this->out_ << std::endl;
  this->out_ << "Plan (" << plan.name << ") Changed at " << std::endl;
  this->out_ << ctime (&cur_time) << std::endl;
  this->out_ << "ID: " << plan.plan_id << std::endl;

  this->out_ << "Start Window: [" << plan.start_window.first << ", ";
  this->out_ << plan.start_window.second << "]" << std::endl;

  this->out_ << "End Window: [" << plan.end_window.first << ", ";
  this->out_ << plan.end_window.second << "]" << std::endl;
  
  std::vector<int> startimes;

  //Find the plan, and find the ealiest time for the latest time step
  //Make sure we don't run out of bounds
  int lastime = 0;
  int plansize = 0;
  if(byStep)
  {
	  for (PlanInstSet::iterator inst_iter = plan.task_insts.begin ();
		inst_iter != plan.task_insts.end ();
		inst_iter++)
	  {
	  	PlanTaskInst inst = *inst_iter;
		  startimes.push_back(inst.start_window.first);
	  }
    //Sort the tasks and grab the current latest time
	  std::sort(startimes.begin(), startimes.end());
	  if(curStep < startimes.size())
	  {
		  lastime = startimes[curStep];
	  }
	  else
	  {
	  	lastime = startimes[startimes.size() - 1];
      done = true;
	  }

  }
  
  //Open File we are going to save to, constant for the moment.

  ofstream gfile;
  gfile.open("GViz.dot");

  
  gfile << "strict digraph graph" << curStep << " {\n";
  
  //Just draw out the graph from the causal links from the plan.
  if(!byStep)
  {
    //Print out the entire graph first.
    planner->print_graph(gfile, graphmap);
    //Now print out the graph of the causal links in the plan.
	  for (CLSet::iterator cl_iter = plan.causal_links.begin ();
		cl_iter != plan.causal_links.end ();
		cl_iter++)
	  {
		  CausalLink clink = *cl_iter;
		  gfile << "\t" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << "[shape=box];\n";
		  gfile << "\t" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << " -> ";
		  gfile <<  planner->get_cond_name (clink.cond.id) << ";\n";
		  gfile << "\t" << planner->get_task_name (planner->get_task_from_inst (clink.second)) << "[shape=box];\n";
		  gfile << "\t" << planner->get_cond_name (clink.cond.id) << " -> ";
		  gfile << planner->get_task_name (planner->get_task_from_inst (clink.second)) << "\n";
	  }
  }
  // Draw out the graph as if taking the plan step by step
  else if(byStep)
  {
    //Format a dot file with the entire graph to get the entire structure ready to print.
    planner->print_graph(gfile, graphmap);
    //Propogate the image of taking a step in the plan by modifying the colors of the nodes.
    color_step(planner);
    //Use the timing to find the current step and color as necessary
	  for (CLSet::iterator cl_iter = plan.causal_links.begin ();cl_iter != plan.causal_links.end ();cl_iter++)
	  {
		  CausalLink clink = *cl_iter;
		  if(planner->get_start_window (clink.first).first <= lastime)
		  {
        //Color the current step green
        if(planner->get_start_window (clink.first).first == lastime && !done)
        {
          std::map<std::string, std::string>::iterator citer = graphmap.find(planner->get_task_name (planner->get_task_from_inst (clink.first)));
          if(citer != graphmap.end())
          {
            (*citer).second = "green";
          }
          citer = graphmap.find(planner->get_cond_name (clink.cond.id));
          if(citer != graphmap.end())
          {
            (*citer).second = "green";
          }
          gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << " " << planner->get_task_from_inst (clink.first) << "\" " << "[shape=box, color= green];\n";
          gfile <<  "\t\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << " [style=filled ,color = orange];\n";
		  	  gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << " " << planner->get_task_from_inst (clink.first) << "\" " << " -> ";
		  	  gfile <<  "\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << ";\n";
        }
        else
        {
          //Go through and color the path to the current step blue.
          std::map<std::string, std::string>::iterator citer = graphmap.find(planner->get_task_name (planner->get_task_from_inst (clink.first)));
          if(citer != graphmap.end())
          {
            (*citer).second = "blue";
          }
          citer = graphmap.find(planner->get_cond_name (clink.cond.id));
          if(citer != graphmap.end())
          {
            (*citer).second = "blue";
          }
			    gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << " " << planner->get_task_from_inst (clink.first) << "\" " << "[shape=box, color= turquoise1];\n";
          gfile <<  "\t\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << "[style=filled, color = turquoise1];\n";
		  	  gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << " " << planner->get_task_from_inst (clink.first) << "\" " << " -> ";
		  	  gfile <<  "\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << ";\n";
        }
      
        //Otherwise print the graph with colors that indicate that the graph has been traversed through all the steps in the plan.
        if(planner->get_start_window (clink.second).first <= lastime)
			  {
          if(planner->get_start_window (clink.second).first == lastime &&  !done)
          {
            gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.second)) << " " << planner->get_task_from_inst (clink.second) << "\" " << "[shape=box, color = green];\n";
            //gfile <<  "\t\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << "[color = green];\n";
				    gfile << "\t" << "\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << " -> ";
				    gfile << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.second)) << " " << planner->get_task_from_inst (clink.second) << "\" " << ";\n";
          }
          else
          {
				    gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.second)) << " " << planner->get_task_from_inst (clink.second) << "\" " << "[shape=box, color = turquoise1];\n";
            gfile <<  "\t\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << "[color = turquoise1];\n";
				    gfile << "\t" << "\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << " -> ";
				    gfile << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.second)) << " " << planner->get_task_from_inst (clink.second) << "\" " << ";\n";
  			
          }
        }
		  }
	  }
  }
  
  //If we are tracking conditions not controlled by the plan, but that impact it(such as environmental conditions
  if(hasTracks)
  {
    for(int i = 0; i < tracks.size(); i++)
    {
      SANet::CondID ccid = tracks[i];
      double prob = planner->get_cond_val(ccid);
      if(prob == 1)
      {
        gfile <<  "\t\"" << planner->get_cond_name (ccid) << " " << ccid << "\" " << "[color = green];\n";
      }
      else
      {
        gfile <<  "\t\"" << planner->get_cond_name (ccid) << " " << ccid << "\" " << "[color = red];\n";
      }
    }
  }

  // If we are actually done with the graph, color the goal as true, and done.
  if(done)
  {
    SA_POP::GoalMap curgoals = planner->get_goals();
    SA_POP::GoalMap::iterator giter;
    for(giter = curgoals.begin(); giter != curgoals.end(); giter++)
    {
      
      gfile <<  "\t\"" << planner->get_cond_name (giter->first) << " " << giter->first << "\" " << "[color = green];\n";
    }
  }
  gfile << "}\n";
  
  gfile.close();
  this->out_ << std::endl;

  //Regardless, we turn it into a gif and display it Now with Threading.
  unsigned long dwThreadId;
  // Create a new thread.
  char *imageName = "step.gif";
  HANDLE hThread = CreateThread(NULL, 0, SecondThread, (PVOID) imageName,
								0, &dwThreadId);
  CloseHandle(hThread);
  //int spawn (ACE_THR_FUNC SecondThread);

  //system("dot -Tgif GViz.dot -o step.gif");
  //system("step.gif");

  //This is debug output to the console about scheduling links
  for (SchedLinkSet::iterator sched_iter = plan.sched_links.begin ();
    sched_iter != plan.sched_links.end ();
    sched_iter++)
  {
    SchedLink sched_link = (*sched_iter);
    this->out_ << "Scheduling Link:" << std::endl;
    this->out_ << "  " << sched_link.first  << " (";
    this->out_ << planner->get_task_name (
      planner->get_task_from_inst (sched_link.first)) << ") -> ";
    this->out_ << sched_link.second  << " (";
    this->out_ << planner->get_task_name (
      planner->get_task_from_inst (sched_link.second)) << ")" << std::endl;

  }


  this->out_ << std::endl;
};

//Advance a step 
void LogGraphOut::moveStep()
{
	curStep++;
};

//Reset current step
void LogGraphOut::resetStep()
{
	curStep = 1;
};

// Uses a color pattern to effect the nodes in the plan to give the impression that they are the pervious step
void LogGraphOut::color_step(SA_POP::Planner *planner)
{
  Plan plan = planner->get_plan ();
  for (CLSet::iterator cl_iter = plan.causal_links.begin (); cl_iter != plan.causal_links.end (); cl_iter++)
	{
		CausalLink clink = *cl_iter;
    std::map<std::string, std::string>::iterator titer = graphmap.find(planner->get_task_name (planner->get_task_from_inst (clink.first)));
    if(titer != graphmap.end())
    {
      if((*titer).second == "blue" )
      {
        (*titer).second = "cornflowerblue";
    
      }
      else if((*titer).second == "green")
      {
        (*titer).second = "red";
      }
    }
    std::map<std::string, std::string>::iterator citer = graphmap.find(planner->get_cond_name (clink.cond.id));
    if(citer != graphmap.end())
    {
      if((*citer).second == "blue" )
      {
        (*citer).second = "cornflowerblue";
    
      }
      else if((*citer).second == "green")
      {
        (*citer).second = "red";
      }
    }
  }
};