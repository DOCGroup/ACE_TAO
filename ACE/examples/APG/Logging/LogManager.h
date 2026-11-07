#include "ace/streams.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Callback.h"
#include <memory>

#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

// Listing 1 code/ch03
class LogManager
{
public:
  LogManager () = default;
  ~LogManager () = default;
  LogManager (LogManager const &) = delete;
  LogManager (LogManager &&) = delete;
  LogManager &operator= (LogManager const &) = delete;
  LogManager &operator= (LogManager &&) = delete;

  void redirectToDaemon
    (ACE_TCHAR const *prog_name = ACE_TEXT (""));
  void redirectToSyslog
    (ACE_TCHAR const *prog_name = ACE_TEXT (""));
  void redirectToOStream (ACE_OSTREAM_TYPE *output);
  void redirectToFile (char const *filename);
  void redirectToStderr ();
  ACE_Log_Msg_Callback *redirectToCallback
    (ACE_Log_Msg_Callback *callback);

  // Exclude 1
private:
  std::unique_ptr<ofstream> log_stream_ {};
  // ACE_Log_Msg retains this borrowed stream pointer while OSTREAM logging is active.
  ACE_OSTREAM_TYPE *output_stream_ {};
  // Exclude 1
};
// Listing 1

// Listing 2 code/ch03
void LogManager::redirectToSyslog (ACE_TCHAR const *prog_name)
{
  ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::SYSLOG, prog_name);
}

void LogManager::redirectToDaemon (ACE_TCHAR const *prog_name)
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

void LogManager::redirectToFile (char const *filename)
{
  log_stream_ = std::make_unique<ofstream> ();
  log_stream_->open (filename, ios::out | ios::app);
  this->redirectToOStream (log_stream_.get ());
}

void LogManager::redirectToStderr ()
{
  ACE_LOG_MSG->clr_flags
    (ACE_Log_Msg::OSTREAM | ACE_Log_Msg::LOGGER);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
}

ACE_Log_Msg_Callback *
LogManager::redirectToCallback (ACE_Log_Msg_Callback *callback)
{
  ACE_Log_Msg_Callback *previous =
    ACE_LOG_MSG->msg_callback (callback);
  if (callback == nullptr)
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::MSG_CALLBACK);
  else
    ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);
  return previous;
}
// Listing 2

// Listing 3 code/ch03
using LogManagerSingleton = ACE_Singleton<LogManager, ACE_Null_Mutex>;
#define LOG_MANAGER LogManagerSingleton::instance()
// Listing 3

#endif /* LOG_MANAGER_H */
