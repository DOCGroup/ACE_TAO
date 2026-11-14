#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/Synch.h"
#include "ace/Guard_T.h"

class LogMessage
{
public:
  static constexpr int CRITICAL {0};
  static constexpr int NORMAL {1};

  virtual ~LogMessage () = default;

  virtual int priority () const
  {
    return NORMAL;
  }
};

class CriticalLogMessage : public LogMessage
{
  int priority () const override
  {
    return LogMessage::CRITICAL;
  }
};

// Listing 1 code/ch14
using MUTEX = ACE_Thread_Mutex;
class Logger
{
public:
  Logger () = default;
  Logger (Logger const &) = delete;
  Logger (Logger &&) = delete;
  Logger &operator= (Logger const &) = delete;
  Logger &operator= (Logger &&) = delete;

  void log (LogMessage const &message)
  {
    ACE_GUARD (MUTEX, mon, mutex_);
    if (message.priority () == LogMessage::CRITICAL)
      logCritical (message);
  }

  void logCritical (LogMessage const &)
  {
    // Acquires the same mutex as log()!
    ACE_GUARD (MUTEX, mon, mutex_);
  }

private:
  MUTEX mutex_ {};
};

static Logger logger {};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  CriticalLogMessage cm;
  logger.log (cm);  // Will cause deadlock.
  return 0;
}
// Listing 1

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
