// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  LogScreenOut.h
 *
 * This file contains the LogScreenOut class definition for the output adapter
 * that displays plan changes on the screen as they occur.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_LOG_SCREEN_OUT_H_
#define SA_POP_LOG_SCREEN_OUT_H_

#include <iostream>
#include "OutAdapter.h"
#include "LogScreenOut_Export.h"

namespace SA_POP {

  /**
   * @class LogScreenOut
   *
   * @brief Output adapter that displays plan changes on the screen as they
   *        occur.
   */
  class LOGSCREENOUT_Export LogScreenOut : public OutAdapter {
  public:
    /// Constructor.
    /**
     * @param strm  Output stream to log to.
     */
    LogScreenOut (std::ostream &out);

    /// Destructor.
    virtual ~LogScreenOut (void);

    /// Notify about task future expected utility changes.
    /**
     * @param planner  Planner for which changes have occurred.
     */
    virtual void notify_eu (SA_POP::Planner *planner);

    /// Notify about plan changes.
    /**
     * @param planner  Planner for which changes have occurred.
     */
    virtual void notify_plan (SA_POP::Planner *planner);

  protected:
    /// Output stream to log to.
    std::ostream &out_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_LOG_SCREEN_OUT_H_ */
