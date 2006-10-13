// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  PlanStrategy.h
 *
 * This file contains the PlanStrategy abstract base class definition for
 * the high-level planning algorithm.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_PLAN_STRATEGY_H_
#define SA_POP_PLAN_STRATEGY_H_

#include "SA_POP_Types.h"
#include "PlanCommands.h"

namespace SA_POP {

  // Forward declaration of Planner.
  class Planner;

  /**
   * @class PlanStrategy
   *
   * @brief  Abstract base class for the high-level planning/scheduling
   *         algorithm.
   */
  class PlanStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    PlanStrategy (SA_POP::Planner *planner)
      : planner_ (planner) { };

    /// Destructor.
    virtual ~PlanStrategy (void) { };



    // ************************************************************************
    // Planning methods.
    // ************************************************************************

    /// Set goals.
    /**
     * @param goals  Set of goals and associated utilities.
     */
    virtual void set_goals (GoalMap goals) = 0;

    /// Get command ID to use for next command.
    /**
     * @return  Command ID to use for next command.
     */
    virtual CommandID get_next_cmd_id (void) = 0;

    /// Recursively satisfy all open conditions (including recursive
    /// scheduling constraint satisfaction through call back).
    /**
     * @return  True if all open conditions were satisfied, false otherwise.
     */
    virtual bool satisfy_open_conds (void) = 0;


    // ************************************************************************
    // Methods for obtaining prototypes of commands that work on this strategy.
    // ************************************************************************

    /// Get a PlanCommand prototype for adding open conditions,
    /// which works on this strategy.
    /**
     * @return  An AddOpenCondsCmd prototype for this strategy.
     */
    virtual AddOpenCondsCmd *get_AddOpenCondsCmd (void) = 0;

    /// Get a PlanCommand prototype for removing open conditions,
    /// which works on this strategy.
    /**
     * @return  A RemoveOpenCondsCmd prototype for this strategy.
     */
    virtual RemoveOpenCondsCmd *get_RemoveOpenCondsCmd (void) = 0;

    /// Get an command prototype for adding causal link threats, which works on
    /// this strategy.
    /**
     * @return  An AddOpenThreatsCmd prototype for this strategy.
     */
    virtual AddOpenThreatsCmd *get_AddOpenThreatsCmd (void) = 0;

    /// Get a PlanCommand prototype for removing causal link threats,
    /// which works on this strategy.
    /**
     * @return  A RemoveOpenThreatsCmd prototype for this strategy.
     */
    virtual RemoveOpenThreatsCmd *get_RemoveOpenThreatsCmd (void) = 0;

  protected:
    /// Pointer to Planner object.
    SA_POP::Planner *planner_;

  };

};  /* SA_POP namespace */

#endif /* SA_POP_PLAN_STRATEGY_H_ */
