// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/Synch.h"
#include "ace/Guard_T.h"

class LogMessage
{
public:
  enum { CRITICAL, NORMAL};

  virtual ~LogMessage ()
  {
  }

  virtual int priority (void)
  {
    return NORMAL;
  }
};

class CriticalLogMessage : public LogMessage
{
  virtual int priority (void)
  {
    return LogMessage::CRITICAL;
  }
};

// Listing 1 code/ch14
typedef ACE_Thread_Mutex MUTEX;
class Logger
{
public:
  void log (LogMessage *msg)
  {
    ACE_GUARD (MUTEX, mon, mutex_);
    if (msg->priority () == LogMessage::CRITICAL)
      logCritical (msg);
  }

  void logCritical (LogMessage *)
  {
    // Acquires the same mutex as log()!
    ACE_GUARD(MUTEX, mon, mutex_);
  }

private:
  MUTEX mutex_;
};

static Logger logger;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  CriticalLogMessage cm;
  logger.log(&cm);  // Will cause deadlock.
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
