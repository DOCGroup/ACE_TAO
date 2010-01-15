// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  LogFileOut.cpp
 *
 * This file contains the LogFileOut class implementation for the output adapter
 * that logs plan changes to a file.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include <fstream>
#include <time.h>
#include "SA_POP_Types.h"
#include "LogFileOut.h"
#include "Planner.h"

using namespace SA_POP;

// Constructor.
LogFileOut::LogFileOut (const char *filename)
: outfile_ (filename)
{
  if (this->outfile_ == 0){
    std::string msg = "Unable to open ";
    msg += filename;
    msg += " for writing.";
    throw msg;
  }
};

// Destructor.
LogFileOut::~LogFileOut (void)
{
  this->outfile_.close ();
};

// Notify about task future expected utility changes.
void LogFileOut::notify_eu (SA_POP::Planner *planner)
{
  time_t cur_time = time(0);
  this->outfile_ << std::endl;
  this->outfile_ << "EU Changes (" << ctime (&cur_time) << "):" << std::endl;

  TaskEUMap eu_changes = planner->get_eu_changes ();
  for (TaskEUMap::iterator iter = eu_changes.begin ();
    iter != eu_changes.end ();
    iter++)
  {
    this->outfile_ << "Task " << iter->first << " = " << iter->second;
    this->outfile_ << std::endl;
  }
};

// Notify about plan changes.
void LogFileOut::notify_plan (SA_POP::Planner *planner)
{
  Plan plan = planner->get_plan ();

  time_t cur_time = time(0);

  this->outfile_ << std::endl;
  this->outfile_ << "Plan (" << plan.name << ")" << " changed at " << ctime (&cur_time);
  this->outfile_ << "ID: " << plan.plan_id << std::endl;
  this->outfile_ << "Goal Condition(s):";
  for (GoalMap::iterator goal_iter = plan.goal.goal_conds.begin ();
    goal_iter != plan.goal.goal_conds.end (); goal_iter++)
  {
    this->outfile_ << "   " << planner->get_cond_name (goal_iter->first);
    if (goal_iter->second >= 0)
      this->outfile_ << " (TRUE) = " << goal_iter->second;
    else
      this->outfile_ << " (FALSE) = " << (-1.0 * goal_iter->second);
  }
  this->outfile_ << std::endl;
  this->outfile_ << "Start Window: [" << plan.start_window.first << ", ";
  this->outfile_ << plan.start_window.second << "]" << std::endl;
  this->outfile_ << "End Window: [" << plan.end_window.first << ", ";
  this->outfile_ << plan.end_window.second << "]" << std::endl;
  this->outfile_ << "Expected Utility: " << planner->calc_plan_eu (plan) << std::endl << std::endl;


  this->outfile_ << "ALL Task Instances:" << std::endl;
  for (PlanInstSet::iterator inst_iter = plan.task_insts.begin ();
    inst_iter != plan.task_insts.end ();
    inst_iter++)
  {
    PlanTaskInst inst = *inst_iter;
    this->outfile_ << "  Task Instance (" << inst.name << "):" << std::endl;

    this->outfile_ << "    ID: " << inst.inst_id << std::endl;

    this->outfile_ << "    Start Window: [" << inst.start_window.first << ", ";
    this->outfile_ << inst.start_window.second << "]" << std::endl;

    this->outfile_ << "    End Window: [" << inst.end_window.first << ", ";
    this->outfile_ << inst.end_window.second << "]" << std::endl;

    this->outfile_ << "    Task ID: " << inst.task_id << std::endl;

    this->outfile_ << "    Task Type: " << inst.type_id << std::endl;

    this->outfile_ << "    Suggested Implementation: ";
    this->outfile_ << inst.suggested_impl << std::endl;
  }
  this->outfile_ << std::endl;

  if (plan.connections.size () > 0) {
    this->outfile_ << "ALL Data Connections:" << std::endl;
    for (PlanConnSet::iterator conn_iter = plan.connections.begin ();
      conn_iter != plan.connections.end ();
      conn_iter++)
    {
      PlanConnection conn = *conn_iter;
      this->outfile_ << "  Data Connection:" << std::endl;
      this->outfile_ << "  " << conn.first_task_inst << " (";
      this->outfile_ << planner->get_task_name (
        planner->get_task_from_inst (conn.first_task_inst)) << "--";
      this->outfile_ << conn.first_port << ")  -";
      this->outfile_ << "(" << planner->get_cond_name (conn.cond) << ")-> ";
      this->outfile_ << conn.second_task_inst << " (";
      this->outfile_ << planner->get_task_name (
        planner->get_task_from_inst (conn.second_task_inst)) << "--";
      this->outfile_ << conn.second_port << ")" << std::endl;
    }
    this->outfile_ << std::endl;
  }


  if (plan.causal_links.size () > 0) {
    this->outfile_ << "ALL Causal Links:" << std::endl;
    for (CLSet::iterator cl_iter = plan.causal_links.begin ();
      cl_iter != plan.causal_links.end ();
      cl_iter++)
    {
      CausalLink clink = *cl_iter;
      this->outfile_ << "  " << clink.first << " (";
      this->outfile_ << planner->get_task_name (
        planner->get_task_from_inst (clink.first)) << ") -";
      this->outfile_ << "(" << planner->get_cond_name (clink.cond.id) << ")-> ";
      this->outfile_ << clink.second << " (";
      this->outfile_ << planner->get_task_name (
        planner->get_task_from_inst (clink.second)) << ")" << std::endl;
    }
    this->outfile_ << std::endl;
  }


  if (plan.sched_links.size () > 0) {
    this->outfile_ << "ALL Scheduling Links:" << std::endl;
    for (SchedLinkSet::iterator sched_iter = plan.sched_links.begin ();
      sched_iter != plan.sched_links.end ();
      sched_iter++)
    {
      SchedLink sched_link = (*sched_iter);
      this->outfile_ << "  " << sched_link.first  << " (";
      this->outfile_ << planner->get_task_name (
        planner->get_task_from_inst (sched_link.first)) << ") -> ";
      this->outfile_ << sched_link.second  << " (";
      this->outfile_ << planner->get_task_name (
        planner->get_task_from_inst (sched_link.second)) << ")" << std::endl;

    }
    this->outfile_ << std::endl;
  }

  if (plan.threat_links.size () > 0) {
    this->outfile_ << "ALL Threat Links:" << std::endl;
    for (ThreatLinkSet::iterator threat_iter = plan.threat_links.begin ();
      threat_iter != plan.threat_links.end ();
      threat_iter++)
    {
      ThreatLink threat_link = (*threat_iter);
      this->outfile_ << "  " << threat_link.first  << " (";
      this->outfile_ << planner->get_task_name (
        planner->get_task_from_inst (threat_link.first)) << ") -> ";
      this->outfile_ << threat_link.second  << " (";
      this->outfile_ << planner->get_task_name (
        planner->get_task_from_inst (threat_link.second)) << ")" << std::endl;

    }
    this->outfile_ << std::endl;
  }
};
