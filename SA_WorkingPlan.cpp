// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_WorkingPlan.cpp
 *
 * This file contains the implementation of the SA_WorkingPlan concrete class,
 * which implements a WorkingPlan that uses precedence graphs for scheduling.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SA_POP_Types.h"
#include "SA_WorkingPlan.h"
#include "Planner.h"
#include "PlanCommands.h"
#include "SA_PlanCommands.h"

#include <stack>
#include <fstream>

using namespace SA_POP;

// Constructor.
SA_WorkingPlan::SA_WorkingPlan (SA_POP::Planner *planner)
: WorkingPlan (planner),
has_cmds_ (false),
next_inst_id_ (1),
add_task_cmd_ (0),
assoc_impl_cmd_ (0),
resolve_threat_cmd_ (0),
resolve_sched_cmd_ (0),
adj_min_times_cmd_ (0),
adj_max_times_cmd_ (0),
add_threats_cmd_ (0)
{
  this->task_insts_.clear ();
  this->task_impls_.clear ();
  this->causal_links_.clear ();
  this->reused_insts_.clear();
  this->sched_links_.clear();

  // Clear goal.
  this->goal_.goal_id = "NULL";
  this->goal_.name = "NULL";
  this->goal_.abs_times.clear ();
  this->goal_.rel_times.clear ();
  this->goal_.goal_conds.clear ();
  this->goal_.start_window = std::make_pair (0, 0);

  // Clear plan.
  this->plan_.causal_links.clear ();
  this->plan_.connections.clear ();
  this->plan_.sched_links.clear ();
  this->plan_.task_insts.clear ();
  this->plan_.threat_links.clear ();

  this->add_task_cmd_ = new SA_AddTaskCmd (this);
  this->assoc_impl_cmd_ = new SA_AssocTaskImplCmd (this);
  this->resolve_threat_cmd_ = new SA_ResolveCLThreatCmd (this);
  this->resolve_sched_cmd_ = new SA_ResolveSchedOrderCmd (this);
  this->adj_min_times_cmd_ = new SA_AdjustMinTimesCmd (this);
  this->adj_max_times_cmd_ = new SA_AdjustMaxTimesCmd (this);

  PrecedenceSet temp;
  this->precedence_graph_.insert(std::make_pair(BEFORE,temp));
  this->precedence_graph_.insert(std::make_pair(AFTER,temp));
  this->precedence_graph_.insert(std::make_pair(SIMUL,temp));
  this->precedence_graph_.insert(std::make_pair(UNRANKED,temp));

  ////////////////////TEMPORARY///////////TEMPORARY/////////////////
///For SPACE
//this->durations.insert(std::make_pair(1,2));
//this->durations.insert(std::make_pair(2,5));
//this->durations.insert(std::make_pair(3,2));
//this->durations.insert(std::make_pair(4,10));
//this->durations.insert(std::make_pair(5,1));
//this->durations.insert(std::make_pair(6,4));
//
//this->init_start.insert(std::make_pair(1,(TimeWindow)std::make_pair(22,28)));
//this->init_end.insert(std::make_pair(1,(TimeWindow)std::make_pair(24,30)));
//this->init_start.insert(std::make_pair(2,(TimeWindow)std::make_pair(17,23)));
//this->init_end.insert(std::make_pair(2,(TimeWindow)std::make_pair(22,28)));
//this->init_start.insert(std::make_pair(3,(TimeWindow)std::make_pair(15,21)));
//this->init_end.insert(std::make_pair(3,(TimeWindow)std::make_pair(17,23)));
//this->init_start.insert(std::make_pair(4,(TimeWindow)std::make_pair(5,19)));
//this->init_end.insert(std::make_pair(4,(TimeWindow)std::make_pair(15,21)));
//this->init_start.insert(std::make_pair(5,(TimeWindow)std::make_pair(0,18)));
//this->init_end.insert(std::make_pair(5,(TimeWindow)std::make_pair(1,19)));
//this->init_start.insert(std::make_pair(6,(TimeWindow)std::make_pair(0,9)));
//this->init_end.insert(std::make_pair(6,(TimeWindow)std::make_pair(4,13)));
///For sched_test1
//this->durations.insert(std::make_pair(1,10));
//this->durations.insert(std::make_pair(2,2));
//this->durations.insert(std::make_pair(3,8));
//this->durations.insert(std::make_pair(4,8));
//this->durations.insert(std::make_pair(5,10));
//this->durations.insert(std::make_pair(7,15));
//this->durations.insert(std::make_pair(6,10));
//
//this->init_start.insert(std::make_pair(1,(TimeWindow)std::make_pair(10,30)));
//this->init_end.insert(std::make_pair(1,(TimeWindow)std::make_pair(20,40)));
//this->init_start.insert(std::make_pair(2,(TimeWindow)std::make_pair(8,28)));
//this->init_end.insert(std::make_pair(2,(TimeWindow)std::make_pair(10,30)));
//this->init_start.insert(std::make_pair(3,(TimeWindow)std::make_pair(0,20)));
//this->init_end.insert(std::make_pair(3,(TimeWindow)std::make_pair(8,28)));
//this->init_start.insert(std::make_pair(4,(TimeWindow)std::make_pair(0,20)));
//this->init_end.insert(std::make_pair(4,(TimeWindow)std::make_pair(8,28)));
//this->init_start.insert(std::make_pair(5,(TimeWindow)std::make_pair(0,20)));
//this->init_end.insert(std::make_pair(5,(TimeWindow)std::make_pair(10,30)));
//this->init_start.insert(std::make_pair(7,(TimeWindow)std::make_pair(10,NULL_TIME)));
//this->init_end.insert(std::make_pair(7,(TimeWindow)std::make_pair(25,NULL_TIME)));
//this->init_start.insert(std::make_pair(6,(TimeWindow)std::make_pair(25,NULL_TIME)));
//this->init_end.insert(std::make_pair(6,(TimeWindow)std::make_pair(35,NULL_TIME)));
//this->init_start.insert(std::make_pair(1,(TimeWindow)std::make_pair(10,NULL_TIME)));
//this->init_end.insert(std::make_pair(1,(TimeWindow)std::make_pair(20,NULL_TIME)));
//this->init_start.insert(std::make_pair(2,(TimeWindow)std::make_pair(8,NULL_TIME)));
//this->init_end.insert(std::make_pair(2,(TimeWindow)std::make_pair(10,NULL_TIME)));
//this->init_start.insert(std::make_pair(3,(TimeWindow)std::make_pair(0,NULL_TIME)));
//this->init_end.insert(std::make_pair(3,(TimeWindow)std::make_pair(8,NULL_TIME)));
//this->init_start.insert(std::make_pair(4,(TimeWindow)std::make_pair(0,NULL_TIME)));
//this->init_end.insert(std::make_pair(4,(TimeWindow)std::make_pair(8,NULL_TIME)));
//this->init_start.insert(std::make_pair(5,(TimeWindow)std::make_pair(0,NULL_TIME)));
//this->init_end.insert(std::make_pair(5,(TimeWindow)std::make_pair(10,NULL_TIME)));

/////For sched_test4
//this->durations.insert(std::make_pair(1,10));
//this->durations.insert(std::make_pair(2,5));
//this->durations.insert(std::make_pair(3,5));
//this->durations.insert(std::make_pair(4,5));
//
//this->init_start.insert(std::make_pair(1,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_end.insert(std::make_pair(1,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_start.insert(std::make_pair(2,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_end.insert(std::make_pair(2,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_start.insert(std::make_pair(3,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_end.insert(std::make_pair(3,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_start.insert(std::make_pair(4,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_end.insert(std::make_pair(4,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));

/////For sched_test5
//this->durations.insert(std::make_pair(1,0));
//this->durations.insert(std::make_pair(2,0));
//this->durations.insert(std::make_pair(3,0));
//this->durations.insert(std::make_pair(4,10));
//
//this->init_start.insert(std::make_pair(1,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_end.insert(std::make_pair(1,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_start.insert(std::make_pair(2,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_end.insert(std::make_pair(2,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_start.insert(std::make_pair(3,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_end.insert(std::make_pair(3,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_start.insert(std::make_pair(4,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
//this->init_end.insert(std::make_pair(4,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));

///For sched_main_test
//this->durations.insert(std::make_pair(1,0));
//this->durations.insert(std::make_pair(2,0));
//this->durations.insert(std::make_pair(3,0));
//this->durations.insert(std::make_pair(4,0));
//this->durations.insert(std::make_pair(5,0));
//this->durations.insert(std::make_pair(6,0));
//this->durations.insert(std::make_pair(7,0));
//this->durations.insert(std::make_pair(8,0));
//this->durations.insert(std::make_pair(9,1));
//this->durations.insert(std::make_pair(10,0));
//this->durations.insert(std::make_pair(11,0));
//this->durations.insert(std::make_pair(12,0));
//this->durations.insert(std::make_pair(13,0));
//this->durations.insert(std::make_pair(14,0));
//this->durations.insert(std::make_pair(15,0));
//this->durations.insert(std::make_pair(16,0));
//this->durations.insert(std::make_pair(17,0));

this->init_start.insert(std::make_pair(1,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(1,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(2,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(2,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(3,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(3,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(4,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(4,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(5,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(5,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(6,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(6,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(7,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(7,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(8,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(8,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(9,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(9,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(10,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(10,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(11,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(11,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(12,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(12,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(13,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(13,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(14,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(14,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(15,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(15,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(16,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(16,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(17,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(17,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(17,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(17,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(18,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(18,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(19,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(19,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(20,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(20,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(21,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(21,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(22,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(22,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(23,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(23,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(24,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(24,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(25,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(25,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(26,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(26,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(27,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(27,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(28,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(28,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(29,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(29,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(30,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(30,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(31,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(31,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(32,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(32,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
};

// Destructor.
SA_WorkingPlan::~SA_WorkingPlan (void)
{
  // Nothing to do.
};

//Reset Plan.

void SA_WorkingPlan::reset_plan ()
{
	
	next_inst_id_ =1;
	this->task_insts_.clear ();
	this->task_impls_.clear ();
  this->causal_links_.clear ();
  this->reused_insts_.clear();
  this->sched_links_.clear();

  // Clear goal.
  this->goal_.goal_id = "NULL";
  this->goal_.name = "NULL";
  this->goal_.abs_times.clear ();
  this->goal_.rel_times.clear ();
  this->goal_.goal_conds.clear ();
  this->goal_.start_window = std::make_pair (0, 0);

  // Clear plan.
  this->plan_.causal_links.clear ();
  this->plan_.connections.clear ();
  this->plan_.sched_links.clear ();
  this->plan_.task_insts.clear ();
  this->plan_.threat_links.clear ();

  this->precedence_graph_.clear();

  this->init_start.clear();
  this->init_end.clear();

  PrecedenceSet temp;
  this->precedence_graph_.insert(std::make_pair(BEFORE,temp));
  this->precedence_graph_.insert(std::make_pair(AFTER,temp));
  this->precedence_graph_.insert(std::make_pair(SIMUL,temp));
  this->precedence_graph_.insert(std::make_pair(UNRANKED,temp));

  

this->init_start.insert(std::make_pair(1,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(1,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(2,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(2,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(3,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(3,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(4,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(4,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(5,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(5,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(6,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(6,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(7,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(7,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(8,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(8,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(9,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(9,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(10,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(10,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(11,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(11,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(12,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(12,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(13,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(13,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(14,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(14,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(15,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(15,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(16,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(16,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(17,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(17,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(17,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(17,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(18,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(18,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(19,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(19,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(20,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(20,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(21,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(21,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(22,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(22,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(23,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(23,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(24,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(24,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(25,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(25,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(26,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(26,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(27,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(27,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(28,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(28,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(29,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(29,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(30,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(30,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(31,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(31,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_start.insert(std::make_pair(32,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));
this->init_end.insert(std::make_pair(32,(TimeWindow)std::make_pair(NULL_TIME,NULL_TIME)));


}

// Set goal.
void SA_WorkingPlan::set_goal (const SA_POP::Goal &goal)
{
  this->goal_ = goal; 
};

// Set command prototypes to use.
void SA_WorkingPlan::set_commands (AddOpenThreatsCmd *add_threats_cmd)
{
  if (add_threats_cmd == 0)
    return;

  this->add_threats_cmd_ = add_threats_cmd;
  this->has_cmds_ = true;
};

// Helper function to create next instance id.
TaskInstID SA_WorkingPlan::get_next_inst_id (void)
{
  TaskInstID inst_id = this->next_inst_id_;
  this->next_inst_id_++;
  return inst_id;
};

// Get current plan.
const Plan& SA_WorkingPlan::get_plan (void)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // Should track whether plan has changed rather than regenerating every time.

  // Clear plan.
  this->plan_.causal_links.clear ();
  this->plan_.connections.clear ();
  this->plan_.sched_links.clear ();
  this->plan_.task_insts.clear ();
  this->plan_.threat_links.clear ();

  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // (Need absolute windows from goal)
  // Set time windows.
  this->plan_.start_window = std::make_pair (0, 0);
  this->plan_.end_window = std::make_pair (0, 0);
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // (Need goal structure to set plan ID & name)
  // Set plan ID and name from goal.
  this->plan_.plan_id = "1";
  this->plan_.name = this->plan_.plan_id;
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

  // Add task instances to plan.
  for (SA_WorkingPlan::InstToTaskMap::iterator iter =
    this->task_insts_.begin ();
    iter != this->task_insts_.end (); iter++)
  {
    // Creat task instance and set instance and task IDs from task instance map.
    PlanTaskInst inst;
    inst.inst_id = iter->first;
    inst.task_id = iter->second;

    //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    // (Need to add separate Type to task nodes instead of using name.)
    // Set instance type.
    inst.type_id = this->planner_->get_task_name (inst.task_id);
    // Give instance a name.
    inst.name = "TaskInst";
    inst.name += to_string (inst.inst_id);
    inst.name += "_";
    inst.name += inst.type_id;
    //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

    // Set start and end windows for this instance.
    SA_WorkingPlan::InstToWinMap::iterator win_iter;
    win_iter = this->start_window_map_.find (inst.inst_id);
    if (win_iter != this->start_window_map_.end ())
      inst.start_window = win_iter->second;
    else
      throw "SA_POP::SA_WorkingPlan::get_plan (): Unable to find start window for task instance.";
    win_iter = this->end_window_map_.find (inst.inst_id);
    if (win_iter != this->end_window_map_.end ())
      inst.end_window = win_iter->second;
    else
      throw "SA_POP::SA_WorkingPlan::get_plan (): Unable to find end window for task instance.";

    // Set suggested implementation for this instance.
    InstToImplMap::iterator impl_iter;
    impl_iter = this->task_impls_.find (inst.inst_id);
    if (impl_iter != this->task_impls_.end ())
      inst.suggested_impl = impl_iter->second;
    else
      throw "SA_POP::SA_WorkingPlan::get_plan (): Unable to find implementation from task instance.";

    // Add instance to plan.
    this->plan_.task_insts.insert (inst);
  }

  // Add scheduling links.
  for (SA_WorkingPlan::SchedulingLinks::iterator sched_iter =
    this->sched_links_.begin ();
    sched_iter != this->sched_links_.end (); sched_iter++)
  {
    this->plan_.sched_links.insert (std::make_pair (
      sched_iter->first, sched_iter->second));
  }

  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // (Need to include threat links.)
  // Add threat links.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP


  // Add plan data connections and causal links.
  for (SA_WorkingPlan::CondToCLinksMap::iterator cl_iter = 
    this->causal_links_.begin ();
    cl_iter != this->causal_links_.end (); cl_iter++)
  {
    // If this is the placeholder goal task instance, don't add connection.
    if (cl_iter->second.second == SA_POP::GOAL_TASK_INST_ID)
      continue;

    // If this condition is a data condition, add data connection to plan.
    if (cl_iter->first.kind == SA_POP::DATA) {
      PlanConnection plan_conn;

      // Set condition.
      plan_conn.cond = cl_iter->first.id;

      // Set task instances.
      plan_conn.first_task_inst = cl_iter->second.first;
      plan_conn.second_task_inst = cl_iter->second.second;

      // Get task ids from instances.
      SA_WorkingPlan::InstToTaskMap::iterator inst_task_iter;

      inst_task_iter = this->task_insts_.find (cl_iter->second.first);
      if (inst_task_iter == this->task_insts_.end ())
        throw "SA_POP::SA_WorkingPlan::get_plan (): couldn't find task id for task instance to get port for plan connection.";
      TaskID first_task = inst_task_iter->second;

      inst_task_iter = this->task_insts_.find (cl_iter->second.second);
      if (inst_task_iter == this->task_insts_.end ())
        throw "SA_POP::SA_WorkingPlan::get_plan (): couldn't find task id for task instance to get port for plan connection.";
      TaskID second_task = inst_task_iter->second;

      // Get ports.
      LinkPorts ports = this->planner_->get_clink_ports
        (first_task, cl_iter->first.id, second_task);
      plan_conn.first_port = ports.first;
      plan_conn.second_port = ports.second;

      this->plan_.connections.insert (plan_conn);
    } else {  // This is a non-data condition so add causal link to plan.
      this->plan_.causal_links.insert (cl_iter->second);
    }
  }

  return this->plan_;
};

// Get set of task instances for a precedence relation to a task instance.
const TaskInstSet *SA_WorkingPlan::get_prec_set (TaskInstID task_inst, PrecedenceRelation prec_rel)
{
  PrecedenceGraph::iterator piter = this->precedence_graph_.find(prec_rel);
  PrecedenceSet::iterator titer = piter->second.find(task_inst);
  return &(titer->second);
};

// Get task id of a task instance.
TaskID SA_WorkingPlan::get_task_from_inst (TaskInstID inst_id)
{
  SA_WorkingPlan::InstToTaskMap::iterator iter =
    this->task_insts_.find (inst_id);
  if (iter == this->task_insts_.end ())
    throw "SA_POP::SA_WorkingPlan::get_task_from_inst (): requested instance id does not exist.";

  return iter->second;
};

/// Get task implementation id of a task instance.
TaskImplID SA_WorkingPlan::get_task_impl_from_inst(TaskInstID inst_id)
{
	return this->task_impls_.find(inst_id)->second;
}

void SA_WorkingPlan::generate_all_threats(void)
{
  threat_set.clear();

  std::ostringstream debug_text;

  



  debug_text << "SA_WorkingPlan::generate_all_threats:  All Causal Links: " << std::endl;
  
  
  
  for(CondToCLinksMap::iterator nit = causal_links_.begin(); nit != causal_links_.end(); nit++){

            CausalLink causal_threatened = (*nit).second;
            TaskID threatened_task = this->task_insts_.find(causal_threatened.first)->second;
            if(causal_threatened.second != -1)
              debug_text << "  Task ("<<threatened_task<<") " << "Inst ("<<causal_threatened.first<<") -(" << causal_threatened.cond.id << ")-> Task ("<<this->task_insts_.find(causal_threatened.second)->second<< ") Inst ("<< causal_threatened.second << ")" << std::endl;           

  }
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());

/*

    for(InstToImplMap::iterator iterator = this->task_impls_.begin(); iterator != this->task_impls_.end(); iterator++){
    TaskInstID threat_possibility = (*iterator).first;
    TaskID threat_possibility_taskid = this->task_insts_.find(threat_possibility)->second;
    
    file_op<<"  Task :"<<iterator->first<<"("<<threat_possibility_taskid<<")"<<std::endl;
  }


  for(InstToImplMap::iterator iterator = this->task_impls_.begin(); iterator != this->task_impls_.end(); iterator++){

    TaskInstID threat_possibility = iterator->first;
    TaskID threat_possibility_taskid = this->task_insts_.find(threat_possibility)->second;
    CondSet set = this->planner_->get_effects(threat_possibility_taskid);

    for(CondSet::iterator arr = set.begin(); arr != set.end(); arr++){ 

        Condition condition = (*arr);

        std::pair<
          CondToCLinksMap::iterator,
          CondToCLinksMap::iterator
            > ret = this->causal_links_.equal_range(condition);
      

        for(CondToCLinksMap::iterator nit = ret.first; nit != ret.second; nit++){

            CausalLink causal_threatened = (*nit).second;
            TaskID threatened_task = this->task_insts_.find(causal_threatened.first)->second;

            SANet::LinkWeight threat_effect = this->planner_->get_link(threat_possibility_taskid, condition.id);
            SANet::LinkWeight causal_effect = this->planner_->get_link(threatened_task, causal_threatened.cond.id);

           
            if((threat_effect > 0 && causal_effect < 0 )|| (threat_effect < 0 && causal_effect > 0)){

              if(causal_threatened.first != threat_possibility && causal_threatened.second != threat_possibility)
              {
                TaskID threatened_task1 = this->task_insts_.find(causal_threatened.first)->second;
                TaskID threatened_task2 = this->task_insts_.find(causal_threatened.second)->second;

                CLThreat new_threat;
                new_threat.clink = causal_threatened;
                new_threat.threat = threat_possibility;
                threat_set.insert(new_threat);

                file_op<<"        New threat: causal link from "<<causal_threatened.first<<" ("<<threatened_task1<<") to "<<causal_threatened.second<<" ("<<threatened_task2<<") using condition "<<causal_threatened.cond.id<<" threatened by "<<threat_possibility<<" ("<<threat_possibility_taskid<<")\n";

              }
             }
        }
      }
   }

  file_op.close();


*/

  debug_text.clear();

  debug_text << "SA_WorkingPlan::generate_all_threats:  All Tasks Instances: " << std::endl;
    
  for(InstToTaskMap::iterator iterator = this->task_insts_.begin(); iterator != this->task_insts_.end(); iterator++){
    TaskInstID threat_possibility = iterator->first;
    TaskID threat_possibility_taskid = iterator->second;
    
    debug_text <<"  Task (" <<threat_possibility_taskid << ")"<< ": Inst (" <<iterator->first << ")" << std::endl;
  }
  
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
  debug_text.clear();

  debug_text << "SA_WorkingPlan::generate_all_threats:  All Causal Threats: " << std::endl;


  for(InstToTaskMap::iterator iterator = this->task_insts_.begin(); iterator != this->task_insts_.end(); iterator++){
    TaskInstID threat_possibility = iterator->first;
    TaskID threat_possibility_taskid = iterator->second;
    CondSet set = this->planner_->get_effects(threat_possibility_taskid);

    for(CondSet::iterator arr = set.begin(); arr != set.end(); arr++){ 

        Condition condition = (*arr);

        std::pair<
          CondToCLinksMap::iterator,
          CondToCLinksMap::iterator
            > ret = this->causal_links_.equal_range(condition);
      

        for(CondToCLinksMap::iterator nit = ret.first; nit != ret.second; nit++){

            CausalLink causal_threatened = (*nit).second;
            TaskID threatened_task = this->task_insts_.find(causal_threatened.first)->second;

            SANet::LinkWeight threat_effect = this->planner_->get_link(threat_possibility_taskid, condition.id);
            SANet::LinkWeight causal_effect = this->planner_->get_link(threatened_task, causal_threatened.cond.id);

           
            if((threat_effect > 0 && causal_effect < 0 )|| (threat_effect < 0 && causal_effect > 0)){

              if(causal_threatened.first != threat_possibility && causal_threatened.second != threat_possibility)
              {
                TaskID threatened_task1 = this->task_insts_.find(causal_threatened.first)->second;
                TaskID threatened_task2 = this->task_insts_.find(causal_threatened.second)->second;

                CLThreat new_threat;
                new_threat.clink = causal_threatened;
                new_threat.threat = threat_possibility;
                threat_set.insert(new_threat);

                debug_text <<"  Causal link from Task ("<< threatened_task1 <<") Inst ("<< causal_threatened.first <<") to Task ("<< threatened_task2 <<") Inst ("<< causal_threatened.second <<") using condition "<<causal_threatened.cond.id<<" threatened by Task ("<< threat_possibility_taskid <<") Inst ("<<threat_possibility<<")" << std::endl;
             
              
              }
             }
        }
      }
   }

  
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
  debug_text.clear();

  
  
}


// Get all current causal link threats.
CLThreatSet SA_WorkingPlan::get_all_threats (void)
{ 
  return threat_set;
};

//Get all the causal and data links TO the task instance
CLSet SA_WorkingPlan::get_before(TaskInstID task_inst)
{
  CLSet ret;
  for(CondToCLinksMap::iterator iter = this->causal_links_.begin();iter!=this->causal_links_.end();iter++)
    if(iter->second.second == task_inst) ret.insert(iter->second);
  return ret;
}
//Get all the causal and data links FROM the task instance
CLSet SA_WorkingPlan::get_after(TaskInstID task_inst)
{
  CLSet ret;
  for(CondToCLinksMap::iterator iter = this->causal_links_.begin();iter!=this->causal_links_.end();iter++)
	  if(iter->second.second != GOAL_TASK_INST_ID && iter->second.second != NULL_TASK_INST_ID && iter->second.first == task_inst) ret.insert(iter->second);
  return ret;
}

/// Get the Causal and Scheduling orderings to this task instance
TaskInstSet SA_WorkingPlan::before_orderings (TaskInstID inst_id)
{
  CLSet before_links = this->get_before(inst_id);
  TaskInstSet ret;
  for(CLSet::iterator iter=before_links.begin();iter!=before_links.end();iter++)
  {
    if(iter->cond.kind!=DATA) ret.insert(iter->first);
  }
  for(SchedulingLinks::iterator iter=sched_links_.begin();iter!=sched_links_.end();iter++)
    if(iter->second==inst_id) ret.insert(iter->first);
  return ret;
}
/// Get the Causal and Scheduling orderings from this task instance
TaskInstSet SA_WorkingPlan::after_orderings (TaskInstID inst_id)
{
  CLSet after_links = this->get_after(inst_id);
  TaskInstSet ret;
  for(CLSet::iterator iter=after_links.begin();iter!=after_links.end();iter++)
    if(iter->cond.kind!=DATA) ret.insert(iter->second);
  for(SchedulingLinks::iterator iter=sched_links_.begin();iter!=sched_links_.end();iter++)
    if(iter->first==inst_id) ret.insert(iter->second);
  return ret;
}
/// Get a PlanCommand prototype for adding a task to the plan.
AddTaskCmd *SA_WorkingPlan::get_AddTaskCmd (void)
{
  return static_cast<AddTaskCmd *> (this->add_task_cmd_->clone ());
};

// Get a PlanCommand prototype for associating an implementation with a
// task instance in the plan.
AssocTaskImplCmd *SA_WorkingPlan::get_AssocTaskImplCmd (void)
{
  return static_cast<AssocTaskImplCmd *> (this->assoc_impl_cmd_->clone ());
};

// Get a PlanCommand prototype for resolving a causal link threat in the
// plan (with promotion or demotion).
ResolveCLThreatCmd *SA_WorkingPlan::get_ResolveCLThreatCmd (void)
{
  return static_cast<ResolveCLThreatCmd *> (this->resolve_threat_cmd_->clone ());
};

// Get a PlanCommand prototype for resolving a scheduling conflict (i.e.
// non-causal-link ordering constraint (with promotion or demotion)
// between two task instances in the plan.
ResolveSchedOrderCmd *SA_WorkingPlan::get_ResolveSchedOrderCmd (void)
{
  return static_cast<ResolveSchedOrderCmd *> (this->resolve_sched_cmd_->clone ());
};

// Get a PlanCommand prototype for adjusting the minimum times of a
// task instance in the plan.
AdjustMinTimesCmd *SA_WorkingPlan::get_AdjustMinTimesCmd (void)
{
  return static_cast<AdjustMinTimesCmd *> (this->adj_min_times_cmd_->clone ());
};

// Get a PlanCommand prototype for adjusting the maximum times of a
// task instance in the plan.
AdjustMaxTimesCmd *SA_WorkingPlan::get_AdjustMaxTimesCmd (void)
{
  return static_cast<AdjustMaxTimesCmd *> (this->adj_max_times_cmd_->clone ());
};

void SA_WorkingPlan::execute (SA_AddTaskCmd *cmd)
{
	// Get task ID, condition, and instance ID.
  TaskID task = cmd->tasks_.front ();

  

  Condition cond = cmd->cond_;
  TaskInstID task_inst;
  
  // If trying a new task (i.e., not still trying task instances for same task), get existing task instances for that task from working plan. 
  if(cmd->last_task_ != task)
  {
	  // Get existing task instances for current task.
    for(InstToTaskMap::iterator iter = this->task_insts_.begin(); iter!=this->task_insts_.end();iter++)
      if(iter->second==cmd->tasks_.front()) 
        cmd->used_task_insts_.insert(iter->first);
  }

  // If there are no/no-more existing task instances to try, create new task instance.
  if(cmd->used_task_insts_.empty()) 
  {
    if(task == 20 && this->planner_->init_added)
    {
      throw "Reached SA_WorkingPlan::execute (SA_AddTaskCmd *cmd) for Special Initial Action after it was already existing instance tried";
    }

    this->planner_->init_added =  true;
  
    task_inst = this->get_next_inst_id ();
    // Add task instance.
    this->task_insts_.insert (std::make_pair (task_inst, task));
    //   Remove this task from tasks still to try.
    cmd->tasks_.pop_front ();
    
    
    
  }
  else
  {
	  // Reuse the task instance
    task_inst = *cmd->used_task_insts_.begin();
		this->reused_insts_.insert(task_inst);

    // NOTE: task instance removed from existing task instances to try in undo.
	}

  std::ostringstream debug_text;
  debug_text << "SA_WorkingPlan::execute (SA_AddTaskCmd *cmd): Adding task (" << task << ") instance (" << task_inst << ") for condition (" << cond.id << ").";

  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());

  // Create causal links for each task instance depending on the condition.
  for (TaskInstSet::iterator inst_iter = cmd->task_insts_.begin ();
    inst_iter != cmd->task_insts_.end (); inst_iter++)
  {
    CausalLink clink;
    clink.cond = cond;
    clink.first = task_inst;
    clink.second = *inst_iter;
    CondToCLinksMap::iterator links_iter;

    // Check whether this causal link already exists in working plan.
    for (links_iter = this->causal_links_.lower_bound (cond);links_iter != this->causal_links_.upper_bound (cond);links_iter++)
      if(links_iter->second == clink)
        break;
    // If causal link not found in working plan, add it to causal links and ordering links.
    if(links_iter==this->causal_links_.upper_bound(cond))
    {
      std::ostringstream debug_text2;
      debug_text2 << "SA_WorkingPlan::execute (SA_AddTaskCmd *cmd): Adding causal link (" << clink.first << " -" << clink.cond.id << "-> " << clink.second << ")";
      SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text2.str ());

      this->causal_links_.insert (std::make_pair (cond, clink));

      if(clink.second != GOAL_TASK_INST_ID){
        this->ordering_links.insert(std::pair<TaskInstID, TaskInstID>(clink.first, clink.second));
        this->reverse_ordering_links.insert(std::pair<TaskInstID, TaskInstID>(clink.second, clink.first));
      }
       cmd->added_links_.insert(clink);
    }
  }

  // Set this task and task instance as last used by command.
	cmd->last_task_ = task;
	cmd->last_task_inst_ = task_inst;
};

void SA_WorkingPlan::undo (SA_AddTaskCmd *cmd)
{



  if(cmd->used_task_insts_.empty())
  {
    // Remove task instance.
    this->task_insts_.erase (this->task_insts_.find(cmd->last_task_inst_));  
  }
  else {
	// Remove the task instance from the set of reusable task instances
	  cmd->used_task_insts_.erase(cmd->used_task_insts_.begin());
    this->reused_insts_.erase(this->reused_insts_.find(cmd->last_task_inst_));
  }






  // Remove causal links.
  for (SA_WorkingPlan::CondToCLinksMap::iterator cl_iter =
    this->causal_links_.lower_bound (cmd->cond_);
    cl_iter != this->causal_links_.upper_bound (cmd->cond_);)
  {
    SA_WorkingPlan::CondToCLinksMap::iterator prev_iter = cl_iter;
    cl_iter++;
    if (cmd->task_insts_.find (prev_iter->second.second) != cmd->task_insts_.end ())
    {
      for(CLSet::iterator iter = cmd->added_links_.begin();iter!=cmd->added_links_.end();iter++)
      {
        if(prev_iter->second == *iter)  
        {
          
          CausalLink clink = prev_iter->second;
          
          this->causal_links_.erase (prev_iter);
          cmd->added_links_.erase(iter);

          
          std::pair<SchedulingLinks::iterator, SchedulingLinks::iterator> ret =
          ordering_links.equal_range(clink.first);
          SchedulingLinks::iterator it;
          for(it = ret.first; it != ret.second; it++){
            if(it->second == clink.second){
              break;
            }
          }              
          this->ordering_links.erase(it);

          ret = reverse_ordering_links.equal_range(clink.second);
          for(it = ret.first; it != ret.second; it++){
            if(it->second == clink.first){
              break;
            }
          }
          this->reverse_ordering_links.erase(it);
       
          break;
        }
      }
    }
  }
};

// Execute a command to associate an implementation with a
// task instance in the plan.
bool SA_WorkingPlan::execute (SA_AssocTaskImplCmd *cmd)
{
  // Associate task instance with first implementation in list from command.
  if(this->reused_insts_.find(cmd->task_inst_)==this->reused_insts_.end())
  {
    this->task_impls_.insert (std::make_pair(cmd->task_inst_, cmd->impls_.front ()));
    this->durations.insert(std::make_pair(cmd->task_inst_, this->planner_->get_impl(cmd->impls_.front())->get_duration()));
  }
  // Update last implementation to this one and remove it from command.
  cmd->last_impl_ = cmd->impls_.front ();
  cmd->impls_.pop_front ();
	// Update the precedence graph

  //Ben changed this.  undo by returning that stuff

  cmd->got_to_scheduling = true;

  if(is_cycle_in_ordering()){
    cmd->got_to_scheduling = false;
    return false;
  }

/*

  std::map<TaskInstID, bool> un_visited_map;
  std::map<TaskInstID, bool> visited_map;
  std::stack<TaskInstID> s;

  for(InstToTaskMap::iterator it = this->task_insts_.begin(); it != this->task_insts_.end(); it++){
    un_visited_map.insert(std::pair<TaskInstID, bool>(it->first, true));

  }

  int total_num = un_visited_map.size();
  while(!(s.size()== total_num)){

    std::map<TaskInstID, bool>::iterator some_it = un_visited_map.begin();
    TaskInstID next = some_it->first;
    dfs_aux(next, s, visited_map, un_visited_map);
  }

  visited_map.clear();

  cmd->got_to_scheduling = true;

  while(!s.empty()){
    TaskInstID next = s.top();
    s.pop();

    if(!dfs_aux2(next, visited_map)){
      cmd->got_to_scheduling = false;     
      return false;
    }
  }
*/

  return this->init_prec_insert(cmd->task_inst_,cmd);
};


bool SA_WorkingPlan::is_cycle_in_ordering(){

  std::ostringstream debug_text;
  


  std::map<TaskInstID, bool> un_visited_map;
  std::map<TaskInstID, bool> visited_map;
  std::stack<TaskInstID> s;

  for(InstToTaskMap::iterator it = this->task_insts_.begin(); it != this->task_insts_.end(); it++){
    un_visited_map.insert(std::pair<TaskInstID, bool>(it->first, true));

  }

  int total_num = un_visited_map.size();
  while(!(s.size()== total_num)){

    std::map<TaskInstID, bool>::iterator some_it = un_visited_map.begin();
    TaskInstID next = some_it->first;
    dfs_aux(next, s, visited_map, un_visited_map);
  }

  visited_map.clear();


  while(!s.empty()){
    TaskInstID next = s.top();
    s.pop();

    if(!dfs_aux2(next, visited_map)){  

      
      debug_text << "SA_WorkingPlan::is_cycle_in_ordering:  FOUND A LOOP IN THREAT SCHEDULE" << std::endl;
      SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
      return true;
    }
  }

  return false;
}

// Undo a command to associate an implementation with a
// task instance in the plan.
void SA_WorkingPlan::undo (SA_AssocTaskImplCmd *cmd)
{
	// Undo the time window adjustments and the precedence graph updations.
  if(cmd->got_to_scheduling){
    this->undo(&cmd->max_adj_cmd);
    this->undo(&cmd->min_adj_cmd);
	  this->prec_erase(cmd->task_inst_,cmd);
    cmd->causal_insertions.clear();
    cmd->simul_insertions.clear();
  }

  if(this->reused_insts_.find(cmd->task_inst_)==this->reused_insts_.end())
  {
    this->durations.erase(this->durations.find(cmd->task_inst_));
    this->task_impls_.erase (cmd->task_inst_);
  }
};

// Execute a command to resolve a causal link threat in the
// plan (with promotion or demotion).
bool SA_WorkingPlan::execute (SA_ResolveCLThreatCmd * cmd)
{

  std::ostringstream debug_text;
 




	TaskInstID first_task_inst = cmd->first;
	TaskInstID second_task_inst = cmd->second;
  Condition condition = cmd->condition;

 // CausalLink cl;
 // cl.cond = condition;
 // cl.first = first_task_inst;
 // cl.second = second_task_inst;

 // this->causal_links_.insert(std::pair<Condition, CausalLink>(condition, cl));
  this->ordering_links.insert(std::pair<TaskInstID, TaskInstID>(first_task_inst, second_task_inst));
  this->reverse_ordering_links.insert(std::pair<TaskInstID, TaskInstID>(second_task_inst, first_task_inst));

/*


  std::map<TaskInstID, bool> un_visited_map;
  std::map<TaskInstID, bool> visited_map;
  std::stack<TaskInstID> s;

  for(InstToTaskMap::iterator it = this->task_insts_.begin(); it != this->task_insts_.end(); it++){
    un_visited_map.insert(std::pair<TaskInstID, bool>(it->first, true));
  }

  int total_num = un_visited_map.size();

  while(!(s.size()== total_num)){

    std::map<TaskInstID, bool>::iterator some_it = un_visited_map.begin();
    TaskInstID next = some_it->first;
    dfs_aux(next, s, visited_map, un_visited_map);

  }

  visited_map.clear();

  while(!s.empty()){
    TaskInstID next = s.top();
    s.pop();

    if(!dfs_aux2(next, visited_map))
      return false;
  }

  */

  cmd->got_to_change_precedences = true;
  if(is_cycle_in_ordering()){

    debug_text << "SA_WorkingPlan::execute (SA_ResolveCLThreatCmd * cmd):  Cannot schedule task inst"<<cmd->first<<" before task inst"<<cmd->second<<std::endl;
    SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
    debug_text.clear();


    cmd->got_to_change_precedences = false;
    return false;
  }

 // this->init_prec_insert(this->associate_cmd->task_inst_,this->associate_cmd);

 // this->after_orderings

  PrecedenceSet* befores = &this->precedence_graph_.find(BEFORE)->second;
  PrecedenceSet* afters = &this->precedence_graph_.find(AFTER)->second;
  PrecedenceSet* simuls = &this->precedence_graph_.find(SIMUL)->second;
  PrecedenceSet* unrankeds = &this->precedence_graph_.find(UNRANKED)->second;

  TaskInstSet *before_A = &befores->find(cmd->first)->second;
  TaskInstSet *after_A = &afters->find(cmd->first)->second;
  TaskInstSet *simul_A = &simuls->find(cmd->first)->second;
  TaskInstSet *unranked_A = &unrankeds->find(cmd->first)->second;
	
  TaskInstSet *before_B = &befores->find(cmd->second)->second;
  TaskInstSet *after_B = &afters->find(cmd->second)->second;
  TaskInstSet *simul_B = &simuls->find(cmd->second)->second;
  TaskInstSet *unranked_B = &unrankeds->find(cmd->second)->second;

  /*
  cmd->before_A_old = *before_A;
  cmd->after_A_old = *after_A;
  cmd->simul_A_old = *simul_A;
  cmd->unsched_A_old = *unranked_A;

  cmd->before_B_old = *before_B;
  cmd->after_B_old = *after_B;
  cmd->simul_B_old = *simul_B;
  cmd->unsched_B_old = *unranked_B;
*/
  cmd->befores = *befores;
  cmd->afters = *afters;
  cmd->simuls = *simuls;
  cmd->unrankeds = *unrankeds;

  TaskInstSet tmp;

  /*
  for(TaskInstSet::iterator it = unranked_A->begin(); it != unranked_A->end(); it++){
    if(unranked_B->find(*it)!=unranked_B->end()){
      tmp.insert(*it);
    }
  }


  unranked_A->clear();
  unranked_B->clear();
  for(TaskInstSet::iterator it = tmp.begin(); it != tmp.end(); it++){
    unranked_A->insert(*it);
    unranked_B->insert(*it);
  }
  */

  before_B->insert(cmd->first);
  unranked_B->erase(cmd->first);

  for(TaskInstSet::iterator it = before_A->begin(); it != before_A->end(); it++){
    before_B->insert(*it);
    unranked_B->erase(*it);

    afters->find(*it)->second.insert(cmd->second);
    unrankeds->find(*it)->second.erase(cmd->second);
  }

  after_A->insert(cmd->second);
  unranked_A->erase(cmd->second);

  for(TaskInstSet::iterator it = after_B->begin(); it != after_B->end(); it++){
    after_A->insert(*it);
    unranked_A->erase(*it);

    befores->find(*it)->second.insert(cmd->first);
    unrankeds->find(*it)->second.erase(cmd->first);
  }

  debug_text << "SA_WorkingPlan::execute (SA_ResolveCLThreatCmd * cmd): Now scheduling task "<<cmd->first<<" before "<<cmd->second<<std::endl;
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
  debug_text.clear();
  









    PrecedenceSet* new_befores = &this->precedence_graph_.find(BEFORE)->second;
    PrecedenceSet* new_afters = &this->precedence_graph_.find(AFTER)->second;
    PrecedenceSet* new_simuls = &this->precedence_graph_.find(SIMUL)->second;
    PrecedenceSet* new_unrankeds = &this->precedence_graph_.find(UNRANKED)->second;

    debug_text << "SA_WorkingPlan::execute (SA_ResolveCLThreatCmd * cmd): Precendence Sets: BEFORE: " << std::endl;
    for(PrecedenceSet::iterator it = new_befores->begin(); it != new_befores->end(); it++){
      debug_text<<"  TaskInst ID "<<it->first<<" : ";
      for(TaskInstSet::iterator jt = it->second.begin(); jt != it->second.end(); jt++){
        debug_text<<" "<<*jt;
      }
      debug_text<<std::endl;
    }
    SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
    debug_text.clear();

    debug_text << "SA_WorkingPlan::execute (SA_ResolveCLThreatCmd * cmd): Precendence Sets: AFTER: " << std::endl;
    for(PrecedenceSet::iterator it = new_afters->begin(); it != new_afters->end(); it++){
      debug_text<<"  TaskInst ID "<<it->first<<" : ";
      for(TaskInstSet::iterator jt = it->second.begin(); jt != it->second.end(); jt++){
        debug_text<<" "<<*jt;
      }
            debug_text<<std::endl;
    }
    
    SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
    debug_text.clear();


    debug_text << "SA_WorkingPlan::execute (SA_ResolveCLThreatCmd * cmd): Precendence Sets: SIMUL: " << std::endl;
    for(PrecedenceSet::iterator it = new_simuls->begin(); it != new_simuls->end(); it++){
      debug_text<<"  TaskInst ID "<<it->first<<" : ";
      for(TaskInstSet::iterator jt = it->second.begin(); jt != it->second.end(); jt++){
        debug_text<<" "<<*jt;
      }
            debug_text<<std::endl;
    }

    SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
    debug_text.clear();

    debug_text << "SA_WorkingPlan::execute (SA_ResolveCLThreatCmd * cmd): Precendence Sets: UNRANKED: " << std::endl;
    for(PrecedenceSet::iterator it = new_unrankeds->begin(); it != new_unrankeds->end(); it++){
      debug_text<<"  TaskInst ID "<<it->first<<" : ";
      for(TaskInstSet::iterator jt = it->second.begin(); jt != it->second.end(); jt++){
        debug_text<<" "<<*jt;
      }
            debug_text<<std::endl;
    }
    
    SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
    debug_text.clear();

  return true;

/*

	TimeWindow first_start,second_start;
	TimeWindow first_end,second_end;
	first_start = this->get_start_window(first_task_inst);
	first_end = this->get_end_window(first_task_inst);
	second_start = this->get_start_window(second_task_inst);
	second_end = this->get_end_window(second_task_inst);
	
	if(second_start.second!= NULL_TIME && (first_end.second==NULL_TIME || second_start.second<first_end.second))
	{
  fstream file_op("outfile.txt",ios::app);
  file_op<<"place 1"<<std::endl;
  file_op.close();


		// There is a need to adjust the max times
		cmd->adj_max_times_cmd_ = static_cast<SA_AdjustMaxTimesCmd *> (this->get_AdjustMaxTimesCmd());
		cmd->adj_max_times_cmd_->set_times(first_task_inst,second_start.second-this->get_duration(first_task_inst),second_start.second);
		if(!this->execute(cmd->adj_max_times_cmd_))
			return false;
	}
	else cmd->adj_max_times_cmd_=NULL;
	if(first_end.first!= NULL_TIME && second_start.first<first_end.first)
	{
  fstream file_op("outfile.txt",ios::app);
  file_op<<"place 2"<<std::endl;
  file_op.close();


		// There is a need to adjust the min times
		cmd->adj_min_times_cmd_ = static_cast<SA_AdjustMinTimesCmd *> (this->get_AdjustMinTimesCmd());
		cmd->adj_min_times_cmd_->set_times(second_task_inst,first_end.first,first_end.first+this->get_duration(second_task_inst));
		if(!this->execute(cmd->adj_min_times_cmd_))
			return false;
	}
	else cmd->adj_min_times_cmd_=NULL;
	
	PrecedenceSet *before = &this->precedence_graph_.find(BEFORE)->second;
	PrecedenceSet *after = &this->precedence_graph_.find(AFTER)->second;
	PrecedenceSet *simul = &this->precedence_graph_.find(SIMUL)->second;
	PrecedenceSet *unranked = &this->precedence_graph_.find(UNRANKED)->second;
	
	before->find(second_task_inst)->second.insert(first_task_inst);
	after->find(first_task_inst)->second.insert(second_task_inst);
	unranked->find(first_task_inst)->second.erase(unranked->find(first_task_inst)->second.find(second_task_inst));
	unranked->find(second_task_inst)->second.erase(unranked->find(second_task_inst)->second.find(first_task_inst));
	
	TaskInstSet after_second = this->after_orderings(second_task_inst);
	for(TaskInstSet::iterator iter=simul->find(second_task_inst)->second.begin();iter!=simul->find(second_task_inst)->second.end();iter++)
		after_second.insert(*iter);
	TaskInstSet before_first = this->before_orderings(first_task_inst);
	for(TaskInstSet::iterator iter=simul->find(first_task_inst)->second.begin();iter!=simul->find(first_task_inst)->second.end();iter++)
		before_first.insert(*iter);
	// All the task instances after and simultaneous to the second task instance should
	// be after all the task instances before and simultaneous to the first task instance
	for(TaskInstSet::iterator iter=after_second.begin();iter!=after_second.end();iter++)
	{
		if(before->find(*iter)->second.find(first_task_inst)==before->find(*iter)->second.end())
		{
			SA_ResolveCLThreatCmd *temp = static_cast<SA_ResolveCLThreatCmd *> (this->get_ResolveCLThreatCmd());
			temp->set_task_insts(first_task_inst,*iter);
			cmd->cmds_.push_back(temp);
			if(!this->execute(temp)) return false;
		}
	}
	// All the task instances before and simultaneous to the first task instance should
	// be before all the task instances after and simultaneous to the second task instance
	for(TaskInstSet::iterator iter=before_first.begin();iter!=before_first.end();iter++)
	{
		if(after->find(*iter)->second.find(second_task_inst)==after->find(*iter)->second.end())
		{
			SA_ResolveCLThreatCmd *temp = static_cast<SA_ResolveCLThreatCmd *> (this->get_ResolveCLThreatCmd());
			temp->set_task_insts(*iter,second_task_inst);
			cmd->cmds_.push_back(temp);
			if(!this->execute(temp)) return false;
		}
	}
	return true;

  */
};

void SA_WorkingPlan::dfs_aux(TaskInstID current, std::stack<TaskInstID>& s, std::map<TaskInstID, bool>& visited, std::map<TaskInstID, bool>& unvisited){



  unvisited.erase(current);
  visited.insert(std::pair<TaskInstID, bool>(current, true));

    std::pair<
        SchedulingLinks::iterator,
        SchedulingLinks::iterator
            > ret = this->ordering_links.equal_range(current);

    for(SchedulingLinks::iterator it = ret.first; it != ret.second; it++){
      TaskInstID hah = it->second;
      if(visited.find(hah)==visited.end()){
        dfs_aux(it->second, s, visited, unvisited);
      }
    }

   s.push(current);

}

bool SA_WorkingPlan::dfs_aux2(TaskInstID current, std::map<TaskInstID, bool>& visited){

  visited.insert(std::pair<TaskInstID, bool>(current, true));

    std::pair<
        SchedulingLinks::iterator,
        SchedulingLinks::iterator
            > ret = this->reverse_ordering_links.equal_range(current);

    for(SchedulingLinks::iterator it = ret.first; it != ret.second; it++){
      TaskInstID hah = it->second;
      if(visited.find(hah)==visited.end()){
        return false;
      }
    }

    return true;
}

// Undo a command to resolve a causal link threat in the
// plan (with promotion or demotion).
void SA_WorkingPlan::undo (SA_ResolveCLThreatCmd * cmd)
{

  std::ostringstream debug_text;

  debug_text << "SA_WorkingPlan::undo (SA_ResolveCLThreatCmd * cmd): Undoing scheduling task "<<cmd->first<<" before "<<cmd->second<<std::endl;
  

  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
  debug_text.clear();

  if(cmd->got_to_change_precedences){


    PrecedenceSet* new_befores = &this->precedence_graph_.find(BEFORE)->second;
    PrecedenceSet* new_afters = &this->precedence_graph_.find(AFTER)->second;
    PrecedenceSet* new_simuls = &this->precedence_graph_.find(SIMUL)->second;
    PrecedenceSet* new_unrankeds = &this->precedence_graph_.find(UNRANKED)->second;

    for(PrecedenceSet::iterator it = cmd->befores.begin(); it != cmd->befores.end(); it++){
      new_befores->find(it->first)->second.clear();
      for(TaskInstSet::iterator jt = it->second.begin(); jt != it->second.end(); jt++){
        new_befores->find(it->first)->second.insert(*jt);
      }
    }

    for(PrecedenceSet::iterator it = cmd->afters.begin(); it != cmd->afters.end(); it++){
      new_afters->find(it->first)->second.clear();
      for(TaskInstSet::iterator jt = it->second.begin(); jt != it->second.end(); jt++){
        new_afters->find(it->first)->second.insert(*jt);
      }
    }

    for(PrecedenceSet::iterator it = cmd->simuls.begin(); it != cmd->simuls.end(); it++){
      new_simuls->find(it->first)->second.clear();
      for(TaskInstSet::iterator jt = it->second.begin(); jt != it->second.end(); jt++){
        new_simuls->find(it->first)->second.insert(*jt);
      }
    }

    for(PrecedenceSet::iterator it = cmd->unrankeds.begin(); it != cmd->unrankeds.end(); it++){
      new_unrankeds->find(it->first)->second.clear();
      for(TaskInstSet::iterator jt = it->second.begin(); jt != it->second.end(); jt++){
        new_unrankeds->find(it->first)->second.insert(*jt);
      }
    }

/*

  TaskInstSet *before_A = &this->precedence_graph_.find(BEFORE)->second.find(cmd->first)->second;
  TaskInstSet *after_A = &this->precedence_graph_.find(AFTER)->second.find(cmd->first)->second;
  TaskInstSet *simul_A = &this->precedence_graph_.find(SIMUL)->second.find(cmd->first)->second;
  TaskInstSet *unranked_A = &this->precedence_graph_.find(UNRANKED)->second.find(cmd->first)->second;
	
  TaskInstSet *before_B = &this->precedence_graph_.find(BEFORE)->second.find(cmd->second)->second;
  TaskInstSet *after_B = &this->precedence_graph_.find(AFTER)->second.find(cmd->second)->second;
  TaskInstSet *simul_B = &this->precedence_graph_.find(SIMUL)->second.find(cmd->second)->second;
  TaskInstSet *unranked_B = &this->precedence_graph_.find(UNRANKED)->second.find(cmd->second)->second;

  before_A->clear();
  for(TaskInstSet::iterator it = cmd->before_A_old.begin(); it != cmd->before_A_old.end(); it++){
    before_A->insert(*it);
  }

  after_A->clear();
  for(TaskInstSet::iterator it = cmd->after_A_old.begin(); it != cmd->after_A_old.end(); it++){
    after_A->insert(*it);
  }

  simul_A->clear();
  for(TaskInstSet::iterator it = cmd->simul_A_old.begin(); it != cmd->simul_A_old.end(); it++){
    simul_A->insert(*it);
  }

  unranked_A->clear();
  for(TaskInstSet::iterator it = cmd->unsched_A_old.begin(); it != cmd->unsched_A_old.end(); it++){
    unranked_A->insert(*it);
  }

  before_B->clear();
  for(TaskInstSet::iterator it = cmd->before_B_old.begin(); it != cmd->before_B_old.end(); it++){
    before_B->insert(*it);
  }

  after_B->clear();
  for(TaskInstSet::iterator it = cmd->after_B_old.begin(); it != cmd->after_B_old.end(); it++){
    after_B->insert(*it);
  }

  simul_B->clear();
  for(TaskInstSet::iterator it = cmd->simul_B_old.begin(); it != cmd->simul_B_old.end(); it++){
    simul_B->insert(*it);
  }

  unranked_B->clear();
  for(TaskInstSet::iterator it = cmd->unsched_B_old.begin(); it != cmd->unsched_B_old.end(); it++){
    unranked_B->insert(*it);
  }

  */
  }

  //before_A = cmd->before_A_old;
 // after_A = cmd->after_A_old
  //simul_A = cmd->simul_A_old;
  //unranked_A = cmd->unsched_A_old;

  //before_B = cmd->before_B_old;
 // after_B = cmd->after_B_old;
  //simul_B = cmd->simul_B_old;
 // unranked_B = cmd->unsched_B_old;

  //return;

//  std::pair<CondToCLinksMap::iterator, CondToCLinksMap::iterator> firstmap
//    = this->causal_links_.equal_range(cmd->condition);

//  CondToCLinksMap::iterator firstit;

//  for(firstit = firstmap.first; firstit != firstmap.second; firstit++){
 //   if(firstit->second.first == cmd->first && firstit->second.second == cmd->second)
 //     break;
 // }

//  this->causal_links_.erase(firstit);

  std::pair<
      std::multimap<TaskInstID, TaskInstID>::iterator,
      std::multimap<TaskInstID, TaskInstID>::iterator
      > ret = this->ordering_links.equal_range(cmd->first);

  std::multimap<TaskInstID, TaskInstID>::iterator it;

  for(it = ret.first; it != ret.second; it++){
    if(it->second == cmd->second)
      break;
  //    this->ordering_links.erase(it);
  }

  this->ordering_links.erase(it);

  ret = this->reverse_ordering_links.equal_range(cmd->second);

  for(it = ret.first; it != ret.second; it++){
    if(it->second == cmd->first)
        break;
  }

  this->reverse_ordering_links.erase(it);


  /*
  
  for(std::list<SA_ResolveCLThreatCmd*>::reverse_iterator iter=cmd->cmds_.rbegin();iter!=cmd->cmds_.rend();iter++)
		this->undo(*iter);
	
	TaskInstID first_task_inst = cmd->first;
	TaskInstID second_task_inst = cmd->second;

	PrecedenceSet *before = &this->precedence_graph_.find(BEFORE)->second;
	PrecedenceSet *after = &this->precedence_graph_.find(AFTER)->second;
	PrecedenceSet *unranked = &this->precedence_graph_.find(UNRANKED)->second;
	
	before->find(second_task_inst)->second.erase(before->find(second_task_inst)->second.find(first_task_inst));
	after->find(first_task_inst)->second.erase(after->find(first_task_inst)->second.find(second_task_inst));
	unranked->find(first_task_inst)->second.insert(second_task_inst);
	unranked->find(second_task_inst)->second.insert(first_task_inst);
	if(cmd->adj_max_times_cmd_ != NULL) {
		this->undo(cmd->adj_max_times_cmd_);
	}
	if(cmd->adj_min_times_cmd_ != NULL)this->undo(cmd->adj_min_times_cmd_);

  
  */
  };

// Execute a command to resolve a scheduling conflict (i.e.
// non-causal-link ordering constraint with promotion or demotion)
// between two task instances in the plan.
bool SA_WorkingPlan::execute (SA_ResolveSchedOrderCmd *cmd)
{
	std::cout<<"Scheduling "<<cmd->first<<" before "<<cmd->second<<std::endl;
	TaskInstID first_task_inst = cmd->first;
	TaskInstID second_task_inst = cmd->second;
	TimeWindow first_start,second_start;
	TimeWindow first_end,second_end;
	first_start = this->get_start_window(first_task_inst);
	first_end = this->get_end_window(first_task_inst);
	second_start = this->get_start_window(second_task_inst);
	second_end = this->get_end_window(second_task_inst);
	
	if(second_start.second!= NULL_TIME && (first_end.second==NULL_TIME || second_start.second<first_end.second))
	{
		// There is a need to adjust the max times
		cmd->adj_max_times_cmd_ = static_cast<SA_AdjustMaxTimesCmd *> (this->get_AdjustMaxTimesCmd());
		cmd->adj_max_times_cmd_->set_times(first_task_inst,second_start.second-this->get_duration(first_task_inst),second_start.second);
		if(!this->execute(cmd->adj_max_times_cmd_))
			return false;
	}
	else cmd->adj_max_times_cmd_=NULL;
	if(first_end.first!= NULL_TIME && second_start.first<first_end.first)
	{
		// There is a need to adjust the min times
		cmd->adj_min_times_cmd_ = static_cast<SA_AdjustMinTimesCmd *> (this->get_AdjustMinTimesCmd());
		cmd->adj_min_times_cmd_->set_times(second_task_inst,first_end.first,first_end.first+this->get_duration(second_task_inst));
		if(!this->execute(cmd->adj_min_times_cmd_))
			return false;
	}
	else cmd->adj_min_times_cmd_=NULL;
	
	PrecedenceSet *before = &this->precedence_graph_.find(BEFORE)->second;
	PrecedenceSet *after = &this->precedence_graph_.find(AFTER)->second;
	PrecedenceSet *simul = &this->precedence_graph_.find(SIMUL)->second;
	PrecedenceSet *unranked = &this->precedence_graph_.find(UNRANKED)->second;
	
	before->find(second_task_inst)->second.insert(first_task_inst);
	after->find(first_task_inst)->second.insert(second_task_inst);
	unranked->find(first_task_inst)->second.erase(unranked->find(first_task_inst)->second.find(second_task_inst));
	unranked->find(second_task_inst)->second.erase(unranked->find(second_task_inst)->second.find(first_task_inst));
	
	TaskInstSet after_second = this->after_orderings(second_task_inst);
	for(TaskInstSet::iterator iter=simul->find(second_task_inst)->second.begin();iter!=simul->find(second_task_inst)->second.end();iter++)
		after_second.insert(*iter);
	TaskInstSet before_first = this->before_orderings(first_task_inst);
	for(TaskInstSet::iterator iter=simul->find(first_task_inst)->second.begin();iter!=simul->find(first_task_inst)->second.end();iter++)
		before_first.insert(*iter);
	// All the task instances after and simultaneous to the second task instance should
	// be after all the task instances before and simultaneous to the first task instance
	for(TaskInstSet::iterator iter=after_second.begin();iter!=after_second.end();iter++)
	{
		if(before->find(*iter)->second.find(first_task_inst)==before->find(*iter)->second.end())
		{
			SA_ResolveSchedOrderCmd *temp = static_cast<SA_ResolveSchedOrderCmd *> (this->get_ResolveSchedOrderCmd());
			temp->set_task_insts(first_task_inst,*iter);
			cmd->cmds_.push_back(temp);
			if(!this->execute(temp)) return false;
		}
	}
	// All the task instances before and simultaneous to the first task instance should
	// be before all the task instances after and simultaneous to the second task instance
	for(TaskInstSet::iterator iter=before_first.begin();iter!=before_first.end();iter++)
	{
		if(after->find(*iter)->second.find(second_task_inst)==after->find(*iter)->second.end())
		{
			SA_ResolveSchedOrderCmd *temp = static_cast<SA_ResolveSchedOrderCmd *> (this->get_ResolveSchedOrderCmd());
			temp->set_task_insts(*iter,second_task_inst);
			cmd->cmds_.push_back(temp);
			if(!this->execute(temp)) return false;
		}
	}
	return true;
};

// Undo a command to resolve a scheduling conflict (i.e.
// non-causal-link ordering constraint with promotion or demotion)
// between two task instances in the plan.
void SA_WorkingPlan::undo (SA_ResolveSchedOrderCmd *cmd)
{
  	for(std::list<SA_ResolveSchedOrderCmd*>::reverse_iterator iter=cmd->cmds_.rbegin();iter!=cmd->cmds_.rend();iter++)
		this->undo(*iter);
	
	TaskInstID first_task_inst = cmd->first;
	TaskInstID second_task_inst = cmd->second;

	PrecedenceSet *before = &this->precedence_graph_.find(BEFORE)->second;
	PrecedenceSet *after = &this->precedence_graph_.find(AFTER)->second;
	PrecedenceSet *unranked = &this->precedence_graph_.find(UNRANKED)->second;
	
	before->find(second_task_inst)->second.erase(before->find(second_task_inst)->second.find(first_task_inst));
	after->find(first_task_inst)->second.erase(after->find(first_task_inst)->second.find(second_task_inst));
	unranked->find(first_task_inst)->second.insert(second_task_inst);
	unranked->find(second_task_inst)->second.insert(first_task_inst);
	if(cmd->adj_max_times_cmd_ != NULL) {
		this->undo(cmd->adj_max_times_cmd_);
	}
	if(cmd->adj_min_times_cmd_ != NULL)this->undo(cmd->adj_min_times_cmd_);
};

// Execute a command to adjust the minimum times of a
// task instance in the plan.
bool SA_WorkingPlan::execute (SA_AdjustMinTimesCmd *cmd)
{
	std::cout<<"adjusting min time for "<<cmd->task_inst_<<std::endl;
	TimeWindow *start_win = &this->start_window_map_.find(cmd->task_inst_)->second;
  TimeWindow *end_win = &this->end_window_map_.find(cmd->task_inst_)->second;
  // If this adjustment violated and time constraints, return false.
  if(start_win->second!=NULL_TIME &&(cmd->new_end_min==NULL_TIME || cmd->new_end_min>end_win->second || cmd->new_start_min>start_win->second)) 
	  return false;
  // Set the new time window
  start_win->first = cmd->new_start_min;
  end_win->first = cmd->new_end_min;
	
  TimeWindow ts = this->get_start_window(cmd->task_inst_);
  TimeWindow es = this->get_end_window(cmd->task_inst_);
  CLSet after_links = this->get_after(cmd->task_inst_);
  if(after_links.empty()) std::cout<<"after links is empty"<<std::endl;
  TaskInstSet causal,sched,data;
  for(CLSet::iterator iter=after_links.begin();iter!=after_links.end();iter++)
  {    if(iter->cond.kind!=DATA) causal.insert(iter->second);
       else data.insert(iter->second);
  }
  for(SchedulingLinks::iterator iter=sched_links_.begin();iter!=sched_links_.end();iter++)
    if(iter->first==cmd->task_inst_) sched.insert(iter->second);
  
  // Propagate this time window change to all the task instances ordered after this task instance
  for(TaskInstSet::iterator iter=causal.begin();iter!=causal.end();iter++)
  {
    TimeWindow temp_start = this->get_start_window(*iter);
    TimeWindow temp_end = this->get_end_window(*iter);
    if(end_win->first>temp_start.first)
    {
		std::cout<<"adjusting time for causal "<<*iter<<std::endl;
		SA_AdjustMinTimesCmd* temp = static_cast<SA_AdjustMinTimesCmd *> (this->get_AdjustMinTimesCmd());
		TimeValue dur = this->durations.find(*iter)->second;
		if(dur!=0) temp->set_times(*iter,end_win->first,end_win->first+this->durations.find(*iter)->second);
		else temp->set_times(*iter,end_win->first,temp_end.first);
		cmd->min_adjust_cmds.push_back(temp);
    }
  }

  if(sched.empty()) std::cout<<"sched is empty"<<std::endl;


  for(TaskInstSet::iterator iter=sched.begin();iter!=sched.end();iter++)
  {
    TimeWindow temp_start = this->get_start_window(*iter);
    TimeWindow temp_end = this->get_end_window(*iter);
    if(end_win->first>temp_start.first)
    {
      SA_AdjustMinTimesCmd* temp = static_cast<SA_AdjustMinTimesCmd *> (this->get_AdjustMinTimesCmd());
		  TimeValue dur = this->durations.find(*iter)->second;
		  if(dur!=0) 
        temp->set_times(*iter,end_win->first,end_win->first+this->durations.find(*iter)->second);
      else 
        temp->set_times(*iter,end_win->first,temp_end.first);
      cmd->min_adjust_cmds.push_back(temp);
  
    }
  }
	// Do the same change for the task instances simultaneous to this one
  for(TaskInstSet::iterator iter=data.begin();iter!=data.end();iter++)
  {
    TimeWindow temp_start = this->get_start_window(*iter);
    if(start_win->first>temp_start.first)
    {
		std::cout<<"adjusting time for data "<<*iter<<std::endl;
      SA_AdjustMinTimesCmd* temp = static_cast<SA_AdjustMinTimesCmd *> (this->get_AdjustMinTimesCmd());
      temp->set_times(*iter,start_win->first,end_win->first);
      cmd->min_adjust_cmds.push_back(temp);
    }
  }


  for(SA_AdjustMinTimesCmd::MinTimesAdjustList::iterator iter=cmd->min_adjust_cmds.begin();iter!=cmd->min_adjust_cmds.end();iter++)
  {

    if(!this->execute(*iter)) return false;
  }
  std::cout<<"the task inst is "<<cmd->task_inst_<<std::endl;
  // Check to see whether any task instance should be removed from the unranked set due to this time window change
  TaskInstSet *unranked = &this->precedence_graph_.find(UNRANKED)->second.find(cmd->task_inst_)->second;
  for(TaskInstSet::iterator iter=unranked->begin();iter!=unranked->end();iter++)
  {
	  TimeWindow temp_end = this->get_end_window(*iter);
	  if(temp_end.second!=NULL_TIME && temp_end.second<start_win->first)
	  {
		  this->precedence_graph_.find(BEFORE)->second.find(cmd->task_inst_)->second.insert(*iter);
		  this->precedence_graph_.find(AFTER)->second.find(*iter)->second.insert(cmd->task_inst_);
		  this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second.erase(this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second.find(*iter));
		  cmd->ranked.insert(*iter);
	  }
  }
  for(TaskInstSet::iterator iter=cmd->ranked.begin();iter!=cmd->ranked.end();iter++)
  {
	  unranked->erase(unranked->find(*iter));
  }
  std::cout<<"min times of "<<cmd->task_inst_<<" : "<<cmd->new_start_min<<" "<<cmd->new_end_min<<std::endl;
  return true;
};

// Undo a command to adjust the minimum times of a
// task instance in the plan.
void SA_WorkingPlan::undo (SA_AdjustMinTimesCmd *cmd)
{
  TimeWindow *start_win = &this->get_start_window(cmd->task_inst_);
  TimeWindow *end_win = &this->get_end_window(cmd->task_inst_);
	
  if(start_win->first==cmd->old_start_min && end_win->first==cmd->old_end_min) return;
  // Undo all the removal of the tasks from the unranked set
  TaskInstSet *unranked1 = &this->precedence_graph_.find(UNRANKED)->second.find(cmd->task_inst_)->second;
  TaskInstSet *before = &this->precedence_graph_.find(BEFORE)->second.find(cmd->task_inst_)->second;
  for(TaskInstSet::iterator iter=cmd->ranked.begin();iter!=cmd->ranked.end();iter++)
  {
	  TaskInstSet *unranked2 = &this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second;
	  TaskInstSet *after = &this->precedence_graph_.find(AFTER)->second.find(*iter)->second;
	  unranked1->insert(*iter);
	  unranked2->insert(cmd->task_inst_);
	  before->erase(before->find(*iter));
	  after->erase(after->find(cmd->task_inst_));
  }
  
  // Undo all the time window changes caused by this command
  for(SA_AdjustMinTimesCmd::MinTimesAdjustList::reverse_iterator iter=cmd->min_adjust_cmds.rbegin();iter!=cmd->min_adjust_cmds.rend();iter++)
  {
    this->undo(*iter);
  }
	// Reset the time window to the old one  
  start_win->first=cmd->old_start_min;
  end_win->first=cmd->old_end_min;
  
  return;
};

// Execute a command to adjust the maximum times of a
// task instance in the plan.
bool SA_WorkingPlan::execute (SA_AdjustMaxTimesCmd *cmd)
{
	std::cout<<"adjusting max time for "<<cmd->task_inst_<<std::endl;
	TimeWindow *start_win = &this->start_window_map_.find(cmd->task_inst_)->second;
  TimeWindow *end_win = &this->end_window_map_.find(cmd->task_inst_)->second;
  std::cout<<"for "<<cmd->task_inst_<<" "<<cmd->new_end_max<<" "<<end_win->first<<" "<<cmd->new_start_max<<" "<<start_win->first<<std::endl;
  // If this adjustment violated and time constraints, return false.
  if((end_win->first==NULL_TIME && cmd->new_end_max!= NULL_TIME) || cmd->new_end_max<end_win->first || cmd->new_start_max<start_win->first) return false;
  // Set the new time window
  start_win->second = cmd->new_start_max;
  end_win->second = cmd->new_end_max;
  CLSet before_links = this->get_before(cmd->task_inst_);
  TaskInstSet causal,sched,data;
  for(CLSet::iterator iter=before_links.begin();iter!=before_links.end();iter++)
  {    if(iter->cond.kind!=DATA) causal.insert(iter->first);
       else data.insert(iter->first);
  }
  for(SchedulingLinks::iterator iter=sched_links_.begin();iter!=sched_links_.end();iter++)
    if(iter->second==cmd->task_inst_) sched.insert(iter->first);
  
  // Propagate this time window change to all the task instances ordered before this task instance
  for(TaskInstSet::iterator iter=causal.begin();iter!=causal.end();iter++)
  {
    TimeWindow temp_end = this->get_end_window(*iter);
	if(temp_end.second==NULL_TIME || (temp_end.second!=NULL_TIME && start_win->second<temp_end.second))
    {
      SA_AdjustMaxTimesCmd* temp = static_cast<SA_AdjustMaxTimesCmd *> (this->get_AdjustMaxTimesCmd());
      temp->set_times(*iter,start_win->second-this->durations.find(*iter)->second,start_win->second);
      cmd->max_adjust_cmds.push_back(temp);
    }
  }

  if(sched.empty()) std::cout<<"msched is empty ("<<cmd->task_inst_<<")"<<std::endl;
  else
  std::cout<<"mnot sched is empty ("<<cmd->task_inst_<<")"<<std::endl;
  
  for(TaskInstSet::iterator iter=sched.begin();iter!=sched.end();iter++)
  {
    TimeWindow temp_end = this->get_end_window(*iter);
    if(temp_end.second==NULL_TIME || (temp_end.second!=NULL_TIME && start_win->second<temp_end.second))
    {
      SA_AdjustMaxTimesCmd* temp = static_cast<SA_AdjustMaxTimesCmd *> (this->get_AdjustMaxTimesCmd());
      temp->set_times(*iter,start_win->second-this->durations.find(*iter)->second,start_win->second);
      cmd->max_adjust_cmds.push_back(temp);
    }
  }

	// Do the same change for the task instances simultaneous to this one
  for(TaskInstSet::iterator iter=data.begin();iter!=data.end();iter++)
  {
    TimeWindow temp_end = this->get_end_window(*iter);
    if(temp_end.second==NULL_TIME || (temp_end.second!=NULL_TIME && end_win->second<temp_end.second))
    {
      SA_AdjustMaxTimesCmd* temp = static_cast<SA_AdjustMaxTimesCmd *> (this->get_AdjustMaxTimesCmd());
      temp->set_times(*iter,start_win->second,end_win->second);
      cmd->max_adjust_cmds.push_back(temp);
    }
  }
  for(SA_AdjustMaxTimesCmd::MaxTimesAdjustList::iterator iter=cmd->max_adjust_cmds.begin();iter!=cmd->max_adjust_cmds.end();iter++)
  {
    if(!this->execute(*iter)) return false;
  }
  // Check to see whether any task instance should be removed from the unranked set due to this time window change
  TaskInstSet *unranked = &this->precedence_graph_.find(UNRANKED)->second.find(cmd->task_inst_)->second;
  for(TaskInstSet::iterator iter=unranked->begin();iter!=unranked->end();iter++)
  {
	  TimeWindow temp_start = this->get_start_window(*iter);
	  if(temp_start.first>end_win->second)
	  {
		  this->precedence_graph_.find(AFTER)->second.find(cmd->task_inst_)->second.insert(*iter);
		  this->precedence_graph_.find(BEFORE)->second.find(*iter)->second.insert(cmd->task_inst_);
		  this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second.erase(this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second.find(*iter));
		  cmd->ranked.insert(*iter);
	  }
  }
  for(TaskInstSet::iterator iter=cmd->ranked.begin();iter!=cmd->ranked.end();iter++)
  {
	  unranked->erase(unranked->find(*iter));
  }
  std::cout<<"max times for "<<cmd->task_inst_<<" : "<<cmd->new_start_max<<" "<<cmd->new_end_max<<std::endl;
  return true;
};

// Undo a command to adjust the maximum times of a
// task instance in the plan.
void SA_WorkingPlan::undo (SA_AdjustMaxTimesCmd *cmd)
{
	std::cout<<"task inst is "<<cmd->task_inst_<<std::endl;
  TimeWindow *start_win = &this->get_start_window(cmd->task_inst_);
  TimeWindow *end_win = &this->get_end_window(cmd->task_inst_);
  if(start_win->second==cmd->old_start_max && end_win->second==cmd->old_end_max) return;
  // Undo all the removal of the tasks from the unranked set
  TaskInstSet *unranked1 = &this->precedence_graph_.find(UNRANKED)->second.find(cmd->task_inst_)->second;
  TaskInstSet *after = &this->precedence_graph_.find(AFTER)->second.find(cmd->task_inst_)->second;
  for(TaskInstSet::iterator iter=cmd->ranked.begin();iter!=cmd->ranked.end();iter++)
  {
	  TaskInstSet *unranked2 = &this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second;
	  TaskInstSet *before = &this->precedence_graph_.find(BEFORE)->second.find(*iter)->second;
	  unranked1->insert(*iter);
	  unranked2->insert(cmd->task_inst_);
	  after->erase(after->find(*iter));
	  before->erase(before->find(cmd->task_inst_));
  }
  
  // Undo all the time window changes caused by this command
  for(SA_AdjustMaxTimesCmd::MaxTimesAdjustList::reverse_iterator iter=cmd->max_adjust_cmds.rbegin();iter!=cmd->max_adjust_cmds.rend();iter++)
  {
    this->undo(*iter);
  }
  
	// Reset the time window to the old one  
  start_win->second=cmd->old_start_max;
  end_win->second=cmd->old_end_max;
  
  return;
};
// Get the start window of the task instance. If one doesn't exist, then assign it one.
TimeWindow SA_WorkingPlan::get_start_window(TaskInstID task_inst)
{
	InstToWinMap::iterator iter = this->start_window_map_.find(task_inst);
	if(iter==this->start_window_map_.end()) {
		// The window doesn't exist
		// The default start window.
		TimeWindow temp_start = std::make_pair(0,NULL_TIME);
		TimeWindow temp_end;
		if(this->durations.find(task_inst)->second!=0)
		{
			// The default end window for non data tasks.
			temp_end = std::make_pair(this->durations.find(task_inst)->second,NULL_TIME);
		}
		else 
		{
			// The default end window for data tasks.		
			temp_end = std::make_pair(NULL_TIME,NULL_TIME);
		}
		this->start_window_map_.insert(std::make_pair(task_inst,temp_start));
		this->end_window_map_.insert(std::make_pair(task_inst,temp_end));
	}
	return this->start_window_map_.find(task_inst)->second;
};

TimeWindow SA_WorkingPlan::get_end_window(TaskInstID task_inst)
{
	InstToWinMap::iterator iter = this->end_window_map_.find(task_inst);
	if(iter==this->end_window_map_.end()) {
		// The window doesn't exist
		// The default start window.
		TimeWindow temp_start = std::make_pair(0,NULL_TIME);
		TimeWindow temp_end;
		if(this->durations.find(task_inst)->second!=0) 
		{
			// The default end window for non data tasks.
			temp_end = std::make_pair(this->durations.find(task_inst)->second,NULL_TIME);
		}
		else
		{
			// The default end window for data tasks.		
			temp_end = std::make_pair(NULL_TIME,NULL_TIME);
		}
		this->start_window_map_.insert(std::make_pair(task_inst,temp_start));
		this->end_window_map_.insert(std::make_pair(task_inst,temp_end));
	}
  return this->end_window_map_.find(task_inst)->second;
};

//Get the duration of a task instance
TimeValue SA_WorkingPlan::get_duration(TaskInstID task_inst)
{
  return this->durations.find(task_inst)->second;
};

/// Adds the sched order to the sched_links_ map by putting the first task instance before the second
void SA_WorkingPlan::add_sched_link(TaskInstID first_task_inst, TaskInstID second_task_inst)
{
  std::cout<<"Adding sched link insert"<<std::endl;
	this->sched_links_.insert(std::make_pair(first_task_inst,second_task_inst));
}
/// Removes the sched order from the sched_links_ map
void SA_WorkingPlan::remove_sched_link(TaskInstID first_task_inst, TaskInstID second_task_inst)
{
	for(SchedulingLinks::iterator iter=this->sched_links_.begin();iter!=this->sched_links_.end();iter++)
	{
		if(iter->first==first_task_inst && iter->second==second_task_inst)
		{
			this->sched_links_.erase(iter);
			break;
		}
	}
}
// Update the precedence graph by inserting the task instance.
bool SA_WorkingPlan::init_prec_insert(TaskInstID task_inst, SA_AssocTaskImplCmd *cmd)
{
  TaskInstSet temp;
  this->precedence_graph_.find(BEFORE)->second.insert(std::make_pair(task_inst,temp));
  this->precedence_graph_.find(AFTER)->second.insert(std::make_pair(task_inst,temp));
  this->precedence_graph_.find(UNRANKED)->second.insert(std::make_pair(task_inst,temp));
  this->precedence_graph_.find(SIMUL)->second.insert(std::make_pair(task_inst,temp));

  TimeWindow win_start = this->init_start.find(task_inst)->second;
  TimeWindow win_end = this->init_end.find(task_inst)->second;
  TimeWindow start_win = this->get_start_window(task_inst);
  TimeWindow end_win = this->get_end_window(task_inst);
  std::cout<<"for "<<task_inst<<win_start.first<<" "<<win_start.second<<" "<<win_end.first<<" "<<win_end.second<<std::endl;
  std::cout<<"for "<<task_inst<<start_win.first<<" "<<start_win.second<<" "<<end_win.first<<" "<<end_win.second<<std::endl;
  if(win_start.first!=NULL_TIME)
  {
	  // An initial time constraint is set on the minimum times
    if(win_start.second!=NULL_TIME)
    {
		  // An initial time constraint is set on the maximum times
  		cmd->min_adj_cmd.set_times(task_inst,win_start.first,win_end.first);
	  	cmd->max_adj_cmd.set_times(task_inst,win_start.second,win_end.second);
		  if(!this->execute(&cmd->min_adj_cmd))
		  {
		    this->undo(&cmd->min_adj_cmd);
			  return false;
		  }
		  if(!this->execute(&cmd->max_adj_cmd))
		  {
			  this->undo(&cmd->max_adj_cmd);
			  this->undo(&cmd->min_adj_cmd);
			  return false;
		  }
    }
    else
    {
		// No initial time constraint is set on the maximum times
  	  cmd->min_adj_cmd.set_times(task_inst,win_start.first,win_end.first);
		  if(!this->execute(&cmd->min_adj_cmd))
		  {
			  this->undo(&cmd->min_adj_cmd);
			  return false;
		  }
    }
  }
  else
  {
	  // No initial time constraint is set on the minimum times
    if(win_start.second!=NULL_TIME)
    {
 		 // An initial time constraint is set on the maximum times
 		cmd->max_adj_cmd.set_times(task_inst,win_start.second,win_end.second);
	  	if(!this->execute(&cmd->max_adj_cmd))
		  {
			  this->undo(&cmd->min_adj_cmd);
			  return false;
		  }
    }
    else
    {
		// No initial time constraint.
  		cmd->min_adj_cmd.set_times(task_inst,start_win.first,end_win.first);
	  	cmd->max_adj_cmd.set_times(task_inst,start_win.second,end_win.second);
      std::cout<<"for "<<task_inst<<" "<<start_win.first<<" "<<start_win.second<<" "<<end_win.first<<" "<<end_win.second<<std::endl;
		  if(!this->execute(&cmd->min_adj_cmd))
		  {
		    this->undo(&cmd->min_adj_cmd);
			  return false;
		  }
		  //if(!this->execute(&cmd->max_adj_cmd))
		  //{
			 // this->undo(&cmd->max_adj_cmd);
			 // this->undo(&cmd->min_adj_cmd);
			 // return false;
		  //}
    }
  }
  start_win = this->get_start_window(task_inst);
  end_win = this->get_end_window(task_inst);
  TaskInstSet *before,*after,*simul,*unranked;
  before = &this->precedence_graph_.find(BEFORE)->second.find(task_inst)->second;
  after = &this->precedence_graph_.find(AFTER)->second.find(task_inst)->second;
  unranked = &this->precedence_graph_.find(UNRANKED)->second.find(task_inst)->second;
  simul = &this->precedence_graph_.find(SIMUL)->second.find(task_inst)->second;
  // If this task instance is not reused, insert all the task instances in it unranked set.
  if(unranked->empty())
  {
    for(PrecedenceSet::iterator iter=this->precedence_graph_.find(BEFORE)->second.begin();iter!=this->precedence_graph_.find(BEFORE)->second.end();iter++)
    {
	    if(iter->first!=task_inst)
	    {
		    std::cout<<"inserting "<<iter->first<<" in unranked of "<<task_inst<<std::endl;
		    unranked->insert(iter->first);
	    }
    }
    if(unranked->empty()) return true;
  }
  CLSet after_links = this->get_after(task_inst);
    
  TaskInstSet causal,data;
  for(CLSet::iterator iter=after_links.begin();iter!=after_links.end();iter++)
  { 
    if(iter->second==GOAL_TASK_INST_ID || iter->second==NULL_TASK_INST_ID) continue;
    if(iter->cond.kind!=DATA) causal.insert(iter->second);
    else data.insert(iter->second);
  }

  for(TaskInstSet::iterator iter=causal.begin();iter!=causal.end();iter++)
  {
	  TaskInstSet *iter_after = &this->precedence_graph_.find(AFTER)->second.find(*iter)->second;
	  TaskInstSet *iter_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter)->second;
	  TaskInstSet *iter_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second;
	  TaskInstSet *iter_simul = &this->precedence_graph_.find(SIMUL)->second.find(*iter)->second;
    if(after->find(*iter)!=after->end()) continue;
    std::cout<<"inserting "<<*iter<<" in after set of "<<task_inst<<std::endl;
	  TaskInstSet temp = *before;
    temp.insert(task_inst);
    for(TaskInstSet::iterator iter2=simul->begin();iter2!=simul->end();iter2++)
      temp.insert(*iter2);
    for(TaskInstSet::iterator iter2=temp.begin();iter2!=temp.end();iter2++)
    {
      TaskInstSet *iter2_before,*iter2_after,*iter2_simul,*iter2_unranked;
      iter2_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter2)->second;
      iter2_after = &this->precedence_graph_.find(AFTER)->second.find(*iter2)->second;
      iter2_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter2)->second;
      iter2_simul = &this->precedence_graph_.find(SIMUL)->second.find(*iter2)->second;
      for(TaskInstSet::iterator iter1=iter_after->begin();iter1!=iter_after->end();iter1++)
	    {
		    std::cout<<*iter1<<" is after "<<*iter<<" so it is after "<<*iter2<<std::endl;
	        TaskInstSet *iter1_before,*iter1_unranked;
		    iter1_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter1)->second;
			iter1_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter1)->second;
		    if(iter1_before->find(*iter2)!=iter1_before->end()) continue;
			else cmd->causal_insertions.insert(std::make_pair(*iter2,*iter1));
			iter1_before->insert(*iter2);
		    iter2_after->insert(*iter1);
		    if(iter1_unranked->find(*iter2)!=iter1_unranked->end()) iter1_unranked->erase(*iter2);
		    if(iter2_unranked->find(*iter1)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter1));
	    }
	    for(TaskInstSet::iterator iter1=iter_simul->begin();iter1!=iter_simul->end();iter1++)
	    {
		    std::cout<<*iter1<<" is siml to "<<*iter<<" so it is after "<<*iter2<<std::endl;
	        TaskInstSet *iter1_before,*iter1_unranked;
		    iter1_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter1)->second;
			iter1_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter1)->second;
		    if(iter1_before->find(*iter2)!=iter1_before->end()) continue;
			else cmd->causal_insertions.insert(std::make_pair(*iter2,*iter1));
		    iter1_before->insert(*iter2);
		    iter2_after->insert(*iter1);
		    if(iter1_unranked->find(*iter2)!=iter1_unranked->end()) iter1_unranked->erase(*iter2);
		    if(iter2_unranked->find(*iter1)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter1));
	    }
	    if(iter_before->find(*iter2)!=iter_before->end()) continue;
		else cmd->causal_insertions.insert(std::make_pair(*iter2,*iter));
	    iter_before->insert(*iter2);
	    iter2_after->insert(*iter);
		if(iter_unranked->find(*iter2)!=iter_unranked->end()) iter_unranked->erase(*iter2);
	    if(iter2_unranked->find(*iter)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter));
    }
  }
  for(TaskInstSet::iterator iter=data.begin();iter!=data.end();iter++)
  {
	  TaskInstSet *iter_after = &this->precedence_graph_.find(AFTER)->second.find(*iter)->second;
	  TaskInstSet *iter_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter)->second;
	  TaskInstSet *iter_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second;
	  TaskInstSet *iter_simul = &this->precedence_graph_.find(SIMUL)->second.find(*iter)->second;
    if(simul->find(*iter)!=simul->end()) continue;
	  std::cout<<"inserting "<<*iter<<" in simul set of "<<task_inst<<std::endl;
    TaskInstSet temp = *before;
    for(TaskInstSet::iterator iter2=temp.begin();iter2!=temp.end();iter2++)
    {
		std::cout<<"iter2 is "<<*iter2<<std::endl;
		TaskInstSet *iter2_before,*iter2_after,*iter2_simul,*iter2_unranked;
      iter2_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter2)->second;
      iter2_after = &this->precedence_graph_.find(AFTER)->second.find(*iter2)->second;
      iter2_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter2)->second;
      iter2_simul = &this->precedence_graph_.find(SIMUL)->second.find(*iter2)->second;
	    for(TaskInstSet::iterator iter1=iter_after->begin();iter1!=iter_after->end();iter1++)
	    {
		    std::cout<<*iter1<<" is after "<<*iter<<" so it is after "<<*iter2<<std::endl;
	        TaskInstSet *iter1_before,*iter1_unranked;
		    iter1_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter1)->second;
			iter1_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter1)->second;
		    if(iter1_before->find(*iter2)!=iter1_before->end()) continue;
			else cmd->causal_insertions.insert(std::make_pair(*iter2,*iter1));
		    iter1_before->insert(*iter2);
		    iter2_after->insert(*iter1);
		    if(iter1_unranked->find(*iter2)!=iter1_unranked->end()) iter1_unranked->erase(*iter2);
		    if(iter2_unranked->find(*iter1)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter1));
	    }
	    for(TaskInstSet::iterator iter1=iter_simul->begin();iter1!=iter_simul->end();iter1++)
	    {
		    std::cout<<*iter1<<" is simul to "<<*iter<<" so it is after "<<*iter2<<std::endl;
	        TaskInstSet *iter1_before,*iter1_unranked;
		    iter1_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter1)->second;
			iter1_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter1)->second;
		    if(iter1_before->find(*iter2)!=iter1_before->end()) continue;
			else cmd->causal_insertions.insert(std::make_pair(*iter2,*iter1));
		    iter1_before->insert(*iter2);
		    iter2_after->insert(*iter1);
		    if(iter1_unranked->find(*iter2)!=iter1_unranked->end()) iter1_unranked->erase(*iter2);
		    if(iter2_unranked->find(*iter1)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter1));
	    }    
	    if(iter_before->find(*iter2)!=iter_before->end()) continue;
		else cmd->causal_insertions.insert(std::make_pair(*iter2,*iter));
	    iter_before->insert(*iter2);
	    iter2_after->insert(*iter);
		if(iter_unranked->find(*iter2)!=iter_unranked->end()) iter_unranked->erase(*iter2);
	    if(iter2_unranked->find(*iter)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter));
    }
    temp = *simul;
    temp.insert(task_inst);
    for(TaskInstSet::iterator iter2=temp.begin();iter2!=temp.end();iter2++)
    {
      TaskInstSet *iter2_before,*iter2_after,*iter2_simul,*iter2_unranked;
      iter2_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter2)->second;
      iter2_after = &this->precedence_graph_.find(AFTER)->second.find(*iter2)->second;
      iter2_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter2)->second;
      iter2_simul = &this->precedence_graph_.find(SIMUL)->second.find(*iter2)->second;
	    for(TaskInstSet::iterator iter1=iter_after->begin();iter1!=iter_after->end();iter1++)
	    {
		    std::cout<<*iter1<<" is after "<<*iter<<" so it is after "<<*iter2<<std::endl;
	        TaskInstSet *iter1_before,*iter1_unranked;
		    iter1_before = &this->precedence_graph_.find(BEFORE)->second.find(*iter1)->second;
			iter1_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter1)->second;
		    if(iter1_before->find(*iter2)!=iter1_before->end()) continue;
			else cmd->causal_insertions.insert(std::make_pair(*iter2,*iter1));
		    iter1_before->insert(*iter2);
		    iter2_after->insert(*iter1);
		    if(iter1_unranked->find(*iter2)!=iter1_unranked->end()) iter1_unranked->erase(*iter2);
		    if(iter2_unranked->find(*iter1)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter1));
	    }
	    for(TaskInstSet::iterator iter1=iter_simul->begin();iter1!=iter_simul->end();iter1++)
	    {
		    std::cout<<*iter1<<" is simul "<<*iter<<" so it is simul "<<*iter2<<std::endl;
	        TaskInstSet *iter1_simul,*iter1_unranked;
		    iter1_simul = &this->precedence_graph_.find(SIMUL)->second.find(*iter1)->second;
			iter1_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter1)->second;
		    if(iter1_simul->find(*iter2)!=iter1_simul->end()) continue;
			else 
			{
				cmd->simul_insertions.insert(std::make_pair(*iter2,*iter1));
			}
			iter1_simul->insert(*iter2);
		    iter2_simul->insert(*iter1);
		    if(iter1_unranked->find(*iter2)!=iter1_unranked->end()) iter1_unranked->erase(*iter2);
		    if(iter2_unranked->find(*iter1)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter1));
	    }
	    for(TaskInstSet::iterator iter1=iter_before->begin();iter1!=iter_before->end();iter1++)
	    {
		    std::cout<<*iter1<<" is before "<<*iter<<" so it is before "<<*iter2<<std::endl;
	        TaskInstSet *iter1_after,*iter1_unranked;
		    iter1_after = &this->precedence_graph_.find(AFTER)->second.find(*iter1)->second;
			iter1_unranked = &this->precedence_graph_.find(UNRANKED)->second.find(*iter1)->second;
		    if(iter1_after->find(*iter2)!=iter1_after->end()) continue;
			else cmd->causal_insertions.insert(std::make_pair(*iter1,*iter2));
		    iter1_after->insert(*iter2);
		    iter2_before->insert(*iter1);
		    if(iter1_unranked->find(*iter2)!=iter1_unranked->end()) iter1_unranked->erase(*iter2);
		    if(iter2_unranked->find(*iter1)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter1));
	    }
	    if(iter_simul->find(*iter2)!=iter_simul->end()) continue;
		else cmd->simul_insertions.insert(std::make_pair(*iter2,*iter));
	    iter2_simul->insert(*iter);
	    iter_simul->insert(*iter2);
		if(iter_unranked->find(*iter2)!=iter_unranked->end()) iter_unranked->erase(*iter2);
	    if(iter2_unranked->find(*iter)!=iter2_unranked->end()) iter2_unranked->erase(iter2_unranked->find(*iter));
    }
  }
  std::cout<<"for "<<task_inst<<" start window: "<<start_win.first<<"-"<<start_win.second<<" and end window "<<end_win.first<<"-"<<end_win.second<<std::endl;
  for(TaskInstSet::iterator iter = unranked->begin();iter!=unranked->end();iter++)
  {
	  TimeWindow temp_start = this->get_start_window(*iter);
	  TimeWindow temp_end = this->get_end_window(*iter);
	  std::cout<<"checking "<<*iter<<" in all with start window: "<<temp_start.first<<"-"<<temp_start.second<<" and end window "<<temp_end.first<<"-"<<temp_end.second<<std::endl;
	  if(end_win.second!=NULL_TIME && temp_start.first>end_win.second)
	  {
		  after->insert(*iter);
		  this->precedence_graph_.find(BEFORE)->second.find(*iter)->second.insert(task_inst);
	  }
	  else if(temp_end.second!=NULL_TIME && temp_end.second<start_win.first)
	  {
		  before->insert(*iter);
		  this->precedence_graph_.find(AFTER)->second.find(*iter)->second.insert(task_inst);
	  }
	  else
	  {
		  this->precedence_graph_.find(UNRANKED)->second.find(*iter)->second.insert(task_inst);
	  }
  }
	this->print_prec();
  for(InstToWinMap::iterator iter=this->start_window_map_.begin();iter!=this->start_window_map_.end();iter++)
  {
    std::cout<<"The start window for "<<iter->first<<" is "<<iter->second.first<<" - "<<iter->second.second<<std::endl;
  }
  for(InstToWinMap::iterator iter=this->end_window_map_.begin();iter!=this->end_window_map_.end();iter++)
  {
    std::cout<<"The end window for "<<iter->first<<" is "<<iter->second.first<<" - "<<iter->second.second<<std::endl;
  }
  return true;
};

// Print the precedence graph
void SA_WorkingPlan::print_prec()
{
	std::cout<<"BEFORE : "<<BEFORE<<", AFTER : "<<AFTER<<", SIMUL : "<<SIMUL<<", UNRANKED : "<<UNRANKED<<std::endl;
	for(PrecedenceGraph::iterator iter1 = this->precedence_graph_.begin(); iter1 != this->precedence_graph_.end(); iter1++)
	{
		std::cout<<"The precedence set "<<iter1->first<<": "<<std::endl;
		for(PrecedenceSet::iterator iter2 = iter1->second.begin(); iter2 != iter1->second.end(); iter2++)
		{
			std::cout<<"The task inst set of "<<iter2->first<<" is ";
			for(TaskInstSet::iterator iter3 = iter2->second.begin(); iter3 != iter2->second.end(); iter3++)
			{
				std::cout<<*iter3<<",";
			}
			std::cout<<std::endl;
		}
	}
};

// Undo all the updation to the precedence graph due to the chosen task instance
void SA_WorkingPlan::prec_erase(TaskInstID task_inst, SA_AssocTaskImplCmd *cmd)
{
PrecedenceSet *before = &this->precedence_graph_.find(BEFORE)->second;
PrecedenceSet *simul = &this->precedence_graph_.find(SIMUL)->second;
PrecedenceSet *after = &this->precedence_graph_.find(AFTER)->second;
PrecedenceSet *unranked = &this->precedence_graph_.find(UNRANKED)->second;
	for(std::set< std::pair<TaskInstID,TaskInstID> >::iterator iter = cmd->causal_insertions.begin();iter!=cmd->causal_insertions.end();iter++)
	{
		// iter->second has been inserted in the after set of iter->first
		std::cout<<"removing "<<iter->second<<" from after of "<<iter->first<<std::endl;
		before->find(iter->second)->second.erase(iter->first);
		after->find(iter->first)->second.erase(iter->second);
		unranked->find(iter->first)->second.insert(iter->second);
		unranked->find(iter->second)->second.insert(iter->first);
	}
	for(std::set< std::pair<TaskInstID,TaskInstID> >::iterator iter = cmd->simul_insertions.begin();iter!=cmd->simul_insertions.end();iter++)
	{
		// iter->second has been inserted in the simul set of iter->first
		std::cout<<"removing "<<iter->second<<" from simul of "<<iter->first<<std::endl;
		simul->find(iter->second)->second.erase(iter->first);
		simul->find(iter->first)->second.erase(iter->second);
		unranked->find(iter->first)->second.insert(iter->second);
		unranked->find(iter->second)->second.insert(iter->first);
	}
	//if(before->find(task_inst)->second.empty() && after->find(task_inst)->second.empty() && simul->find(task_inst)->second.empty())
	//{
	//	CondToCLinksMap::iterator iter;
	//	for(iter = this->causal_links_.begin();iter!=this->causal_links_.end();iter++)
	//		if(iter->second.second == GOAL_TASK_INST_ID && iter->second.first == task_inst) break;
	//	if(iter==this->causal_links_.end() || cmd->impls_.size()!=0)
	//	{
	std::cout<<this->reused_insts_.size()<<std::endl;
	if(this->reused_insts_.find(task_inst)==this->reused_insts_.end())
	{
			before->erase(before->find(task_inst));
			after->erase(after->find(task_inst));
			simul->erase(simul->find(task_inst));
			unranked->erase(unranked->find(task_inst));
			this->start_window_map_.erase(this->start_window_map_.find(task_inst));
			this->end_window_map_.erase(this->end_window_map_.find(task_inst));
			for(PrecedenceSet::iterator iter1 = unranked->begin();iter1!=unranked->end();iter1++)
				iter1->second.erase(task_inst);
		}
//	}
	//std::cout<<"after erasing the precedence graph is "<<std::endl;
	//this->print_prec();
};
/// Get all the task instances
TaskInstSet SA_WorkingPlan::get_all_insts()
{
	TaskInstSet all;
	for(PrecedenceSet::iterator iter=this->precedence_graph_.find(BEFORE)->second.begin();iter!=this->precedence_graph_.find(BEFORE)->second.end();iter++)
	{
		all.insert(iter->first);
	}	  
  return all;
}
/// Check if the instance id already exists and is being reused.
bool SA_WorkingPlan::inst_exists (TaskInstID task_inst)
{
  if(this->task_impls_.find(task_inst)!=this->task_impls_.end()) return true;
  else return false;
}

/// Get task implementation for a task instance.
TaskImplID SA_WorkingPlan::get_impl_id (TaskInstID task_inst)
{
  return this->task_impls_.find(task_inst)->second;
}

