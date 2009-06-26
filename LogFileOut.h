// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  LogFileOut.h
 *
 * This file contains the LogFileOut class definition for the output adapter
 * that logs plan changes to a file.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_LOG_FILE_OUT_H_
#define SA_POP_LOG_FILE_OUT_H_

#include <fstream>
#include "OutAdapter.h"

namespace SA_POP {

  /**
   * @class LogFileOut
   *
   * @brief Output adapter that logs plan changes to a file.
   */
  class LogFileOut : public OutAdapter {
  public:
    /// Constructor.
    /**
     * @param filename  Name of file to log to.
     */
    LogFileOut (const char *filename);

    /// Destructor.
    virtual ~LogFileOut (void);

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
    /// Output file handle.
    std::ofstream outfile_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_LOG_FILE_OUT_H_ */
