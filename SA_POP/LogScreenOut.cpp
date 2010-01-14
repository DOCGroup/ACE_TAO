// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  LogScreenOut.cpp
 *
 * This file contains the LogScreenOut class implementation for the output adapter
 * that displays plan changes on the screen as they occur.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include <iostream>
#include <time.h>
#include "SA_POP_Types.h"
#include "LogScreenOut.h"
#include "Planner.h"

using namespace SA_POP;

// Constructor.
LogScreenOut::LogScreenOut (std::ostream &out)
: out_ (out)
{
  // Nothing to do.
};

// Destructor.
LogScreenOut::~LogScreenOut (void)
{
  // Nothing to do.
};

// Notify about task future expected utility changes.
void LogScreenOut::notify_eu (SA_POP::Planner *planner)
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
void LogScreenOut::notify_plan (SA_POP::Planner *planner)
{
  Plan plan = planner->get_plan ();

  time_t cur_time = time(0);

  this->out_ << std::endl;
  this->out_ << "Plan (" << plan.name << ")" << " changed at " << ctime (&cur_time);
  this->out_ << "ID: " << plan.plan_id << std::endl;
  this->out_ << "Goal Condition(s):";
  for (GoalMap::iterator goal_iter = plan.goal.goal_conds.begin ();
    goal_iter != plan.goal.goal_conds.end (); goal_iter++)
  {
    this->out_ << "   " << planner->get_cond_name (goal_iter->first);
    if (goal_iter->second >= 0)
      this->out_ << " (TRUE) = " << goal_iter->second;
    else
      this->out_ << " (FALSE) = " << (-1.0 * goal_iter->second);
  }
  this->out_ << std::endl;
  this->out_ << "Start Window: [" << plan.start_window.first << ", ";
  this->out_ << plan.start_window.second << "]" << std::endl;
  this->out_ << "End Window: [" << plan.end_window.first << ", ";
  this->out_ << plan.end_window.second << "]" << std::endl;
  this->out_ << "Expected Utility: " << planner->calc_plan_eu (plan) << std::endl << std::endl;


  this->out_ << "ALL Task Instances:" << std::endl;
  for (PlanInstSet::iterator inst_iter = plan.task_insts.begin ();
    inst_iter != plan.task_insts.end ();
    inst_iter++)
  {
    PlanTaskInst inst = *inst_iter;
    this->out_ << "  Task Instance (" << inst.name << "):" << std::endl;

    this->out_ << "    ID: " << inst.inst_id << std::endl;

    this->out_ << "    Start Window: [" << inst.start_window.first << ", ";
    this->out_ << inst.start_window.second << "]" << std::endl;

    this->out_ << "    End Window: [" << inst.end_window.first << ", ";
    this->out_ << inst.end_window.second << "]" << std::endl;

    this->out_ << "    Task ID: " << inst.task_id << std::endl;

    this->out_ << "    Task Type: " << inst.type_id << std::endl;

    this->out_ << "    Suggested Implementation: ";
    this->out_ << inst.suggested_impl << std::endl;
  }
  this->out_ << std::endl;

  if (plan.connections.size () > 0) {
    this->out_ << "ALL Data Connections:" << std::endl;
    for (PlanConnSet::iterator conn_iter = plan.connections.begin ();
      conn_iter != plan.connections.end ();
      conn_iter++)
    {
      PlanConnection conn = *conn_iter;
      this->out_ << "  Data Connection:" << std::endl;
      this->out_ << "  " << conn.first_task_inst << " (";
      this->out_ << planner->get_task_name (
        planner->get_task_from_inst (conn.first_task_inst)) << "--";
      this->out_ << conn.first_port << ")  -";
      this->out_ << "(" << planner->get_cond_name (conn.cond) << ")-> ";
      this->out_ << conn.second_task_inst << " (";
      this->out_ << planner->get_task_name (
        planner->get_task_from_inst (conn.second_task_inst)) << "--";
      this->out_ << conn.second_port << ")" << std::endl;
    }
    this->out_ << std::endl;
  }


  if (plan.causal_links.size () > 0) {
    this->out_ << "ALL Causal Links:" << std::endl;
    for (CLSet::iterator cl_iter = plan.causal_links.begin ();
      cl_iter != plan.causal_links.end ();
      cl_iter++)
    {
      CausalLink clink = *cl_iter;
      this->out_ << "  " << clink.first << " (";
      this->out_ << planner->get_task_name (
        planner->get_task_from_inst (clink.first)) << ") -";
      this->out_ << "(" << planner->get_cond_name (clink.cond.id) << ")-> ";
      this->out_ << clink.second << " (";
      this->out_ << planner->get_task_name (
        planner->get_task_from_inst (clink.second)) << ")" << std::endl;
    }
    this->out_ << std::endl;
  }


  if (plan.sched_links.size () > 0) {
    this->out_ << "ALL Scheduling Links:" << std::endl;
    for (SchedLinkSet::iterator sched_iter = plan.sched_links.begin ();
      sched_iter != plan.sched_links.end ();
      sched_iter++)
    {
      SchedLink sched_link = (*sched_iter);
      this->out_ << "  " << sched_link.first  << " (";
      this->out_ << planner->get_task_name (
        planner->get_task_from_inst (sched_link.first)) << ") -> ";
      this->out_ << sched_link.second  << " (";
      this->out_ << planner->get_task_name (
        planner->get_task_from_inst (sched_link.second)) << ")" << std::endl;

    }
    this->out_ << std::endl;
  }

  if (plan.threat_links.size () > 0) {
    this->out_ << "ALL Threat Links:" << std::endl;
    for (ThreatLinkSet::iterator threat_iter = plan.threat_links.begin ();
      threat_iter != plan.threat_links.end ();
      threat_iter++)
    {
      ThreatLink threat_link = (*threat_iter);
      this->out_ << "  " << threat_link.first  << " (";
      this->out_ << planner->get_task_name (
        planner->get_task_from_inst (threat_link.first)) << ") -> ";
      this->out_ << threat_link.second  << " (";
      this->out_ << planner->get_task_name (
        planner->get_task_from_inst (threat_link.second)) << ")" << std::endl;

    }
    this->out_ << std::endl;
  }

};
