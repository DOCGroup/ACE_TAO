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
#include "Planner.h"
#include <fstream>
#include <vector>
//#include "gvc.h"
//#include "mfg_draw_graph.h"
//#include "mfg_graph_drawer.h"

using namespace SA_POP;

// Constructor.
LogGraphOut::LogGraphOut (std::ostream &out, int startstep)
: out_ (out)
{
  graphn = startstep;
  // Nothing to do.
}

// Destructor.
LogGraphOut::~LogGraphOut (void)
{
  // Nothing to do.
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
	//system("step.gif");
	return 0;
}


// Notify about plan changes.
void LogGraphOut::notify_plan (SA_POP::Planner *planner)
{
  Plan plan = planner->get_plan ();

  time_t cur_time = time(0);

  /*
  GVC_t *gvc;
  graph_t *g;
  FILE *fp;

  mfg::DrawGraph * graph;
  */
  
  //gvc = gvContext();
  //This 
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
  if(graphn > 0)
  {
	  for (PlanInstSet::iterator inst_iter = plan.task_insts.begin ();
		inst_iter != plan.task_insts.end ();
		inst_iter++)
	  {
		PlanTaskInst inst = *inst_iter;
		startimes.push_back(inst.start_window.first);
	  }

	  std::sort(startimes.begin(), startimes.end());
	  if(graphn < startimes.size())
	  {
		lastime = startimes[graphn];
	  }
	  else
	  {
		lastime = startimes[startimes.size() - 1];
	  }

  }
  
  //Open File we are going to save to, constant for the moment.

  ofstream gfile;
  gfile.open("GViz.dot");
  
  gfile << "strict digraph graph" << graphn << " {\n";
  if(graphn == 0)
  {
	  for (CLSet::iterator cl_iter = plan.causal_links.begin ();
		cl_iter != plan.causal_links.end ();
		cl_iter++)
	  {
		CausalLink clink = *cl_iter;
		//this->out_ << "Build Graph from links.." << std::endl;
		gfile << "\t" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << "[shape=box];\n";
		gfile << "\t" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << " -> ";
		gfile <<  planner->get_cond_name (clink.cond.id) << ";\n";
		gfile << "\t" << planner->get_task_name (planner->get_task_from_inst (clink.second)) << "[shape=box];\n";
		gfile << "\t" << planner->get_cond_name (clink.cond.id) << " -> ";
		gfile << planner->get_task_name (
		  planner->get_task_from_inst (clink.second)) << "\n";
	  }
  }
  else
  {
	for (CLSet::iterator cl_iter = plan.causal_links.begin ();
		cl_iter != plan.causal_links.end ();
		cl_iter++)
	 {
		CausalLink clink = *cl_iter;
		//this->out_ << "Build Graph from links.." << std::endl;

		if(planner->get_start_window (clink.first).first <= lastime)
		{
			gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << " " << planner->get_task_from_inst (clink.first) << "\" " << "[shape=box];\n";
			gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.first)) << " " << planner->get_task_from_inst (clink.first) << "\" " << " -> ";
			gfile <<  "\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << ";\n";
			if(planner->get_start_window (clink.second).first <= lastime)
			{
				gfile << "\t" << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.second)) << " " << planner->get_task_from_inst (clink.second) << "\" " << "[shape=box];\n";
				gfile << "\t" << "\"" << planner->get_cond_name (clink.cond.id) << " " << clink.cond.id << "\" " << " -> ";
				gfile << "\"" << planner->get_task_name (planner->get_task_from_inst (clink.second)) << " " << planner->get_task_from_inst (clink.second) << "\" " << "\n";
			}
		}
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
  //system("dot -Tgif GViz.dot -o step.gif");
  //system("step.gif");

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


void LogGraphOut::moveStep()
{
	graphn++;
};

void LogGraphOut::resetStep()
{
	graphn = 1;
};