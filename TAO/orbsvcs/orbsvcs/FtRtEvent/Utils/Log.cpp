//$Id$
#include "Log.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"

namespace TAO_FTRTEC {

  unsigned int Log::log_level_;

  Log::Log (unsigned int log_level, const ACE_TCHAR* format, ...)
  {
    if (log_level_ >= log_level) {
      va_list p;
      va_start(p, format);
      char str[1024];
      ACE_OS::vsprintf(str, format, p);
      ACE_DEBUG((LM_DEBUG, str));
      va_end(p);
    }
  }

  void Log::level(unsigned int log_level)
  {
    log_level_ = log_level;
  }
}
