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
#include "orbsvcs/FtRtEvent/Utils/ftrtevent_export.h"
#include "tao/Versioned_Namespace.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_FTRTEC {

  /**
   * A utility class for logging messages.
   */
  class  TAO_FtRtEvent_Export Log
  {
  public:
    Log (unsigned int log_level, const ACE_TCHAR* format, ...);
    static void level(unsigned int log_level);
    static unsigned int level();
    static void hexdump(unsigned int log_level, const char* buf, size_t len, const ACE_TCHAR* msg);
  private:
    friend class Trace;
    static unsigned log_level_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/FtRtEvent/Utils/Log.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif
