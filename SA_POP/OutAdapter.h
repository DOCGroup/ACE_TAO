// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  OutAdapter.h
 *
 * This file contains the OutAdapter abstract base class definition for output
 * adapters.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_OUT_ADAPTER_H_
#define SA_POP_OUT_ADAPTER_H_

#include "SA_POP_Types.h"

namespace SA_POP {

  // Forward declaration of Planner.
  class Planner;

  /**
   * @class OutAdapter
   *
   * @brief Abstract base class for output adapters.
   */
  class OutAdapter {
  public:
    /// Constructor.
    OutAdapter (void) { };

    /// Destructor.
    virtual ~OutAdapter (void) { };

    /// Notify about task future expected utility changes.
    /**
     * @param planner  Planner for which changes have occurred.
     */
    virtual void notify_eu (SA_POP::Planner *planner) = 0;

    /// Notify about plan changes.
    /**
     * @param planner  Planner for which changes have occurred.
     */
    virtual void notify_plan (SA_POP::Planner *planner) = 0;

  };

};  /* SA_POP namespace */

#endif /* SA_POP_OUT_ADAPTER_H_ */
