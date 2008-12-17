// -*- C++ -*-
// 

//=============================================================================
/**
 * @file  SchemaOut.cpp
 *
 * This file contains the SchemaOut class implementation for the output adapter
 * that creates a valid Plan Schema in XML for agents to use.
 *
 * @author  Daniel L.C. Mack <daniel.l.mack@vanderbilt.edu>
 */
//=============================================================================

#include <iostream>
#include <time.h>
#include "SA_POP_Types.h"
#include "SchemaOut.h"
#include "Planner.h"
#include <fstream>

using namespace SA_POP;

// Constructor.
SchemaOut::SchemaOut (std::ostream &out, std::vector<SA_POP::CondID> ks)
: out_ (out)
{
  graphn = 0;
  knowns = ks;

  // Nothing to do.
};

// Destructor.
SchemaOut::~SchemaOut (void)
{
  // Nothing to do.
};

// Notify about task future expected utility changes.
void SchemaOut::notify_eu (SA_POP::Planner *planner)
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
};

// Notify about plan changes.
void SchemaOut::notify_plan (SA_POP::Planner *planner)
{
  
  Plan plan = planner->get_plan ();

  time_t cur_time = time(0);
  
  SA_POP::GoalMap gm = planner->get_goals();
  
    //Open File we are going to save to
  //string fname = "GViz.dot";
  graphn += 1;
  ofstream gfile;
  gfile.open("planSchema.xml");
  
  gfile << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
  gfile << "<SA-POP:planSchema\n";
  gfile << "xmlns:SA-POP=\"http://www.vanderbilt.edu/SA-POP\"\n";
  gfile << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
  gfile << "xsi:schemaLocation=\"http://www.vanderbilt.edu/SA-POP Plan_Schema.xsd\"\n";
  gfile << "xmlns=\"http://www.vanderbilt.edu/SA-POP\">\n\n\n";

  for(SA_POP::GoalMap::iterator giter = gm.begin(); giter != gm.end(); giter++)
  {
	  gfile << "<GoalCond>\n";
	  gfile << "<condID>" << giter->first << "</condID>\n";
	  gfile << "<utility>" << giter->second << "</utility>\n";
	  gfile << "</GoalCond>\n";
	
	 
  }

  for(int conds = 0; conds < knowns.size(); conds ++)
  {
	SA_POP::CondID ccid =  knowns[conds];
	gfile << "<<KnownCond>>\n";
	gfile << "<condID>" << ccid << "</condID>\n";
	gfile << "<trueprob>" << "1" << "</trueprob>\n";
	gfile << "</<KnownCond>>\n";
  }

  SA_POP::PlanInstSet tinst = plan.task_insts;

  for(SA_POP::PlanInstSet::iterator giter = tinst.begin(); giter != tinst.end(); giter++)
  {
	  SA_POP::PlanTaskInst tnt = *giter;

	  gfile <<  "\t<Task>\n";
	  gfile << "\t\t<TaskID>";
	  gfile << tnt.task_id;
	  gfile << "</TaskID>\n";
	  gfile << "\t\t<earliestStart>";
	  gfile << tnt.start_window.first;
	  gfile << "</earliestStart>\n";
	  gfile << "\t\t<earliestEnd>";
	  gfile << tnt.end_window.first;
	  gfile << "</earliestEnd>\n";

	  gfile << "\t\t<latestStart>";
	  gfile << tnt.start_window.second;
	  gfile << "</latestStart>\n";
		
	  gfile << "\t\t<latestEnd>";
	  gfile << tnt.end_window.second;
	  gfile << "</latestEnd>\n";
	  gfile << "\t</Task>\n";
	
  }

  for(SA_POP::PlanInstSet::iterator giter = tinst.begin(); giter != tinst.end(); giter++)
  {
	  SA_POP::PlanTaskInst tnt = *giter;

	  gfile <<  "\t<TaskImpl>\n";
	  gfile << "\t\t<implID>";
	  gfile << tnt.suggested_impl;
	  gfile << "</implID>\n";
	  gfile << "\t</TaskImpl>\n";
	
  }

  for(SA_POP::PlanInstSet::iterator giter = tinst.begin(); giter != tinst.end(); giter++)
  {
	  SA_POP::PlanTaskInst tnt = *giter;

	  gfile <<  "\t<TaskToImpl>\n";
	  gfile << "\t\t<TaskID>";
	  gfile << tnt.task_id;
	  gfile << "</TaskID>\n";
	  gfile << "\t\t<implID>";
	  gfile << tnt.suggested_impl;
	  gfile << "</implID>\n";
	  gfile << "\t</TaskToImpl>\n";
	
  }

  SA_POP::CLSet cls = plan.causal_links;
  for(SA_POP::CLSet::iterator giter = cls.begin(); giter != cls.end(); giter++)
  {
	  SA_POP::CausalLink clink = *giter;
	  
	  gfile <<  "\t<CausalLink>\n";
	  gfile << "\t\t<FromTask>";
	  gfile << planner->get_task_from_inst(clink.first);
	  gfile << "</FromTask>\n";
	  gfile << "\t\t<ToTask>";
	  gfile << planner->get_task_from_inst(clink.second);
	  gfile << "</ToTask>\n";
	  gfile << "\t\t<condID>";
	  gfile << clink.cond.id;
	  gfile << "</condID>\n";
	  gfile << "\t\t<condVal>";
	  gfile << clink.cond.value;
	  gfile << "</condVal>\n";
	  gfile << "\t</CausalLink>\n";
	
	  
  }

  SA_POP::SchedLinkSet sls = plan.sched_links;
  for(SA_POP::SchedLinkSet::iterator giter = sls.begin(); giter != sls.end(); giter++)
  {
	  SA_POP::SchedLink slink = *giter;

	  gfile <<  "\t<ScheduleLink>\n";
	  gfile << "\t\t<FirstTask>";
	  gfile << planner->get_task_from_inst(slink.first);
	  gfile << "</FirstTask>\n";
	  gfile << "\t\t<SecondTask>";
	  gfile << planner->get_task_from_inst(slink.second);
	  gfile << "</SecondTask>\n";
	  gfile << "\t</ScheduleLink>\n";

  }

  SA_POP::ThreatLinkSet tls = plan.threat_links;
  for(SA_POP::ThreatLinkSet::iterator giter = tls.begin(); giter != tls.end(); giter++)
  {
	  SA_POP::ThreatLink tlink = *giter;

	  gfile <<  "\t<ThreatLink>\n";
	  gfile << "\t\t<ThreateningTask>";
	  gfile << planner->get_task_from_inst(tlink.first);
	  gfile << "</ThreateningTask>\n";
	  gfile << "\t\t<ThreatensTask>";
	  gfile << planner->get_task_from_inst(tlink.second);
	  gfile << "</ThreatensTask>\n";
	  gfile << "\t</ThreatLink>\n";

  }

  gfile << "\n</SA-POP:planSchema>";
  gfile.close();

  this->out_ << std::endl;
  this->out_ << "Plan (" << plan.name << ") Changed at " << std::endl;
  this->out_ << ctime (&cur_time) << std::endl;
  this->out_ << "ID: " << plan.plan_id << std::endl;

  this->out_ << "Start Window: [" << plan.start_window.first << ", ";
  this->out_ << plan.start_window.second << "]" << std::endl;

  this->out_ << "End Window: [" << plan.end_window.first << ", ";
  this->out_ << plan.end_window.second << "]" << std::endl;
  
  for (PlanInstSet::iterator inst_iter = plan.task_insts.begin ();
    inst_iter != plan.task_insts.end ();
    inst_iter++)
  {
    PlanTaskInst inst = *inst_iter;
  }
  


  this->out_ << std::endl;
};
