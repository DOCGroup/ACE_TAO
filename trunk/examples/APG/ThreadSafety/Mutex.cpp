// $Id$

#include "ace/Synch.h"

class LogMessage
{
public:
  enum { CRITICAL, NORMAL};

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

  void logCritical (LogMessage *msg)
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
