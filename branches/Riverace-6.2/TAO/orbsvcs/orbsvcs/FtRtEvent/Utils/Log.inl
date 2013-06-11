// -*- C++ -*-
//
// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_FTRTEC {
#ifndef NDEBUG
  ACE_INLINE
  void Log::level(unsigned int log_level)
  {
    log_level_ = log_level;
  }

  ACE_INLINE
  unsigned int Log::level()
  {
    return log_level_;
  }

  ACE_INLINE
  void Log::hexdump(unsigned int level, const char* buf, size_t len, const ACE_TCHAR* msg)
  {
    if (Log::log_level_ >= level)
      ORBSVCS_HEX_DUMP((LM_DEBUG, buf, len, msg));
  }
#else // NDEBUG
  ACE_INLINE
  Log::Log (unsigned int, const ACE_TCHAR*, ...)
  {
  }

  ACE_INLINE
  void Log::level(unsigned int )
  {
  }

  ACE_INLINE
  unsigned int Log::level()
  {
    return 0;
  }

  ACE_INLINE
  void Log::hexdump(unsigned int, const char*, size_t, const ACE_TCHAR*)
  {
  }
#endif
}

TAO_END_VERSIONED_NAMESPACE_DECL
