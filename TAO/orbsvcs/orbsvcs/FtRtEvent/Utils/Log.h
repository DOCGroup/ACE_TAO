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

#include /**/ "ace/pre.h"
#include "ftrtevent_export.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"

namespace TAO_FTRTEC {

  class TAO_FtRtEvent_Export Trace
  {
  public:
    Trace(const char* msg);
    ~Trace();
  private:
    const char* msg_;
  };
  /**
   * A utility class for logging messages.
   */
  class  TAO_FtRtEvent_Export Log
  {
  public:
    Log (unsigned int log_level, const ACE_TCHAR* format, ...);
    static void level(unsigned int log_level);
  private:
    friend Trace;
    static unsigned log_level_;
  };
}

#define FTRTEC_TRACE(x) TAO_FTRTEC::Trace __trace_obj(x)

#if defined(__ACE_INLINE__)
#include "Log.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif
