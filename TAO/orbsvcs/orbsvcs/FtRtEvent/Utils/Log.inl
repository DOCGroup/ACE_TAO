// $Id$

namespace TAO_FTRTEC {
#ifndef NDEBUG
  ACE_INLINE
  Trace::Trace(const char* msg)
  : msg_(msg)
  {
    if (Log::log_level_ >=5)
      ACE_DEBUG((LM_DEBUG, "Enter %s\n",msg));
  }

  ACE_INLINE
  Trace::~Trace()
  {
    if (Log::log_level_ >=5)
      ACE_DEBUG((LM_DEBUG, "Leave %s\n",msg_));
  }

  ACE_INLINE
  void Log::level(unsigned int log_level)
  {
    log_level_ = log_level;
  }
#else // NDEBUG
  ACE_INLINE
  Trace::Trace(const char* )
  {
  }

  ACE_INLINE
  Trace::~Trace()
  {
  }

  ACE_INLINE
  Log::Log (unsigned int log_level, const ACE_TCHAR* format, ...)
  {
  }

  ACE_INLINE
  void Log::level(unsigned int )
  {
  }
#endif
}
