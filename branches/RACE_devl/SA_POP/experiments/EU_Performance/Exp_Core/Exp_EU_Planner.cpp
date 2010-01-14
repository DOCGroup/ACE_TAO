// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Exp_EU_Planner.cpp
 *
 * This file contains the Exp_EU_Planner class implementation for a planner,
 * which implements a Planner that finds all possible plans during planning
 * and outputs statistics comparing expected utility of each to the SA-POP
 * preferred plan.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include <string>
#include <set>
#include <map>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include "SA_POP_Types.h"
#include "Exp_EU_Planner.h"

//#include "Planner.h"
//#include "PlanStrategy.h"
//#include "SchedStrategy.h"
//#include "WorkingPlan.h"
//#include "TaskMap.h"
//#include "OutAdapter.h"
//#include "PlanCommands.h"

using namespace SA_POP;

// Constructor.
Exp_EU_Planner::Exp_EU_Planner (void)
:do_pause_ (false),
 input_ (0),
 ques_ (0)
{
  // All initialization handled by Planner base class constructor.
};


/// Destructor.
Exp_EU_Planner::~Exp_EU_Planner (void)
{
  // All deletion handled by Planner base class destructor.
}


// Set to pause and ask user whether to continue after each plan is generated.
void Exp_EU_Planner::set_pause (UserInterface::InputCL *input_cl, UserInterface::QuestionBool *ques)
{
  this->input_ = input_cl;
  this->ques_ = ques;
  this->do_pause_ = true;
};

// Set to generate all plans without user input (instead of asking whether to continue after each plan is generated).
void Exp_EU_Planner::unset_pause (void)
{
  this->do_pause_ = false;
};


// Satisfy scheduling constraints in fully instantiated plan (no
// recursive call backs).
bool Exp_EU_Planner::full_sched ()
{
  if (this->sched_strat_->satisfy_full_sched ()) {
    this->plan_ = this->working_plan_->get_plan ();
    this->notify_plan_changed ();
    Utility plan_eu = this->calc_plan_eu (this->plan_);

//    std::ostringstream eu_str;
//    eu_str << "Plan EU: " << plan_eu << std::endl << std::endl;
//    SA_POP_DEBUG_STR(SA_POP_DEBUG_QUIET, eu_str.str ());

    // If pausing to ask user whether to continue, ask question
    // and return true (so the current plan will be returned instead
    // of continuing planning) if user replies "No" to continuation
    // question.
    if (this->do_pause_) {
      this->input_->ask ((*this->ques_));
      if (!this->ques_->get_answer_bool ())
        return true;
    }
  }

  return false;
};

