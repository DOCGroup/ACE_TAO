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
  
  ACE_INLINE
  void Log::hexdump(unsigned int level, const char* buf, size_t len, const char* msg)
  {
    if (Log::log_level_ >= level)
      ACE_HEX_DUMP((LM_DEBUG, buf, len, msg));
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
  Log::Log (unsigned int, const ACE_TCHAR*, ...)
  {
  }

  ACE_INLINE
  void Log::level(unsigned int )
  {
  }

  ACE_INLINE
  void Log::hexdump(unsigned int, const char*, size_t, const char*)
  {
  }
#endif
}
