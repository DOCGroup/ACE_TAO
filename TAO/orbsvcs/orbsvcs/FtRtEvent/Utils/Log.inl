// -*- C++ -*-
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
  void Log::hexdump(unsigned int level, char const *buf, size_t len, ACE_TCHAR const *msg)
  {
    if (Log::log_level_ >= level)
      ORBSVCS_HEX_DUMP((LM_DEBUG, buf, len, msg));
  }
#else // NDEBUG
  ACE_INLINE
  Log::Log (unsigned int, ACE_TCHAR const *, ...)
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
  void Log::hexdump(unsigned int, char const *, size_t, ACE_TCHAR const *)
  {
  }
#endif
}

TAO_END_VERSIONED_NAMESPACE_DECL
