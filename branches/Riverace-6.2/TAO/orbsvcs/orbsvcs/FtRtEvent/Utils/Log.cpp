//$Id$
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/FtRtEvent/Utils/Log.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/FtRtEvent/Utils/Log.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_stdio.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_FTRTEC {

unsigned int Log::log_level_;

#ifndef NDEBUG
  Log::Log (unsigned int log_level, const ACE_TCHAR* format, ...)
  {
    if (log_level_ >= log_level) {
      va_list p;
      va_start(p, format);
      ACE_TCHAR str[1024];
      ACE_OS::vsprintf(str, format, p);
      ORBSVCS_DEBUG((LM_DEBUG, str));
      va_end(p);
    }
  }
#endif
}

TAO_END_VERSIONED_NAMESPACE_DECL
