// $Id$

#include "ace/streams.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Callback.h"

#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

// Listing 1 code/ch03
class LogManager
{
public:
  LogManager ();
  ~LogManager ();

  void redirectToDaemon
    (const ACE_TCHAR *prog_name = ACE_TEXT (""));
  void redirectToSyslog
    (const ACE_TCHAR *prog_name = ACE_TEXT (""));
  void redirectToOStream (ACE_OSTREAM_TYPE *output);
  void redirectToFile (const char *filename);
  void redirectToStderr (void);
  ACE_Log_Msg_Callback * redirectToCallback
    (ACE_Log_Msg_Callback *callback);

  // Exclude 1
private:
  ofstream *log_stream_;
  ACE_OSTREAM_TYPE *output_stream_;
  // Exclude 1
};
// Listing 1

// Listing 2 code/ch03
LogManager::LogManager ()
  : log_stream_ (0), output_stream_ (0)
{ }

LogManager::~LogManager ()
{
  if (log_stream_)
    log_stream_->close ();
  delete log_stream_;
}

void LogManager::redirectToSyslog (const ACE_TCHAR *prog_name)
{
  ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::SYSLOG, prog_name);
}

void LogManager::redirectToDaemon (const ACE_TCHAR *prog_name)
{
  ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::LOGGER,
                     ACE_DEFAULT_LOGGER_KEY);
}

void LogManager::redirectToOStream (ACE_OSTREAM_TYPE *output)
{
  output_stream_ = output;
  ACE_LOG_MSG->msg_ostream (this->output_stream_);
  ACE_LOG_MSG->clr_flags
    (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
}

void LogManager::redirectToFile (const char *filename)
{
  log_stream_ = new ofstream ();
  log_stream_->open (filename, ios::out | ios::app);
  this->redirectToOStream ((ACE_OSTREAM_TYPE *)log_stream_);
}

void LogManager::redirectToStderr (void)
{
  ACE_LOG_MSG->clr_flags
    (ACE_Log_Msg::OSTREAM | ACE_Log_Msg::LOGGER);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
}

ACE_Log_Msg_Callback *
LogManager::redirectToCallback (ACE_Log_Msg_Callback * callback)
{
  ACE_Log_Msg_Callback *previous =
    ACE_LOG_MSG->msg_callback (callback);
  if (callback == 0)
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::MSG_CALLBACK);
  else
    ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);
  return previous;
}
// Listing 2

// Listing 3 code/ch03
typedef ACE_Singleton<LogManager, ACE_Null_Mutex>
        LogManagerSingleton;
#define LOG_MANAGER LogManagerSingleton::instance()
// Listing 3

#endif /* LOG_MANAGER_H */
