// -*- C++ -*-
//=============================================================================
/**
 *  @file   Log.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef FTRTEC_LOG_H
#define FTRTEC_LOG_H

#include "ftrtevent_export.h"

namespace TAO_FTRTEC {

  /**
   * A utility class for logging messages.
   */
  class  TAO_FtRtEvent_Export Log
  {
  public:
    Log (unsigned int log_level, const ACE_TCHAR* format, ...);
    static void level(unsigned int log_level);
  private:
    static unsigned log_level_;
  };
}

#endif
