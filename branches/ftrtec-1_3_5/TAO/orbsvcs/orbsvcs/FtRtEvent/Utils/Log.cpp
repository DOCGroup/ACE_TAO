//$Id$
#include "Log.h"

#if !defined(__ACE_INLINE__)
#include "Log.inl"
#endif /* __ACE_INLINE__ */

namespace TAO_FTRTEC {

  unsigned int Log::log_level_;

#ifndef NDEBUG
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

  char buffer [1024*100];
  char* now=buffer;

  TimeLogger::TimeLogger(const char* msg)
    : msg_(msg)
  {
    start_time_ = ACE_OS::gettimeofday ();
  }
  
  TimeLogger::~TimeLogger()
  {
    ACE_Time_Value result = ACE_OS::gettimeofday () - start_time_;
    int time_in_usec = result.sec()*1000000+result.usec();
    int n = ACE_OS::snprintf(now, buffer-now+sizeof(buffer), "%s %d\n", msg_, time_in_usec);
    now+=n;
  }

  void TimeLogger::output() 
  {
    ACE_DEBUG((LM_DEBUG, buffer));
  }
#endif

}
