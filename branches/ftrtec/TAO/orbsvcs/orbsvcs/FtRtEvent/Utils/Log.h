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

  class TAO_FtRtEvent_Export TimeLogger 
  {
  public:
    void set_message(const char* msg);
    void start();
    void stop();
    static void output();
  private:
    const char* msg_;
    ACE_Time_Value start_time_;
  };

  class TAO_FtRtEvent_Export TimeAutoLogger : public TimeLogger
  {
  public:
    TimeAutoLogger(const char* msg) { set_message(msg); start(); }
    ~TimeAutoLogger() { stop(); }
  };

  /**
   * A utility class for logging messages.
   */
  class  TAO_FtRtEvent_Export Log
  {
  public:
    Log (unsigned int log_level, const ACE_TCHAR* format, ...);
    static void level(unsigned int log_level);
    static unsigned int level();
    static void hexdump(unsigned int log_level, const char* buf, size_t len, const char* msg);
  private:
    friend class Trace;
    static unsigned log_level_;
  };
}

#define FTRTEC_TRACE(x) TAO_FTRTEC::Trace __ftrtec_trace_obj(x)
#define FTRTEC_LOGTIME(x) TAO_FTRTEC::TimeAutoLogger __ftrtec_time_logger(x)
#if defined(__ACE_INLINE__)
#include "Log.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif
