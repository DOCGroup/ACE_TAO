// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Log_Msg
//
// = FILENAME
//    test_callback.cpp
//
// = DESCRIPTION
//     This program tests the <ACE_Log_Msg> class wrt writing to user
//     defined callback objects.  In particular, it tests to make sure
//     that nested callbacks don't deadlock.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"

ACE_RCSID(Log_Msg, test_callback, "$Id$")

class Logger : public ACE_Log_Msg_Callback
{
public:
  Logger (int be_recursive = 1);
  // Constructor sets whether we're testing "recursive" callback
  // logging!

  void log (ACE_Log_Record &log_record);
  // Logging callback

  void verbose (int be_verbose);

private:
  int verbose_logging_;
  // Flag for testing verbose logging.

  int recursive_;
  // Flag for testing recursive callback logging.
};

void
Logger::verbose (int be_verbose)
{
  this->verbose_logging_ = be_verbose;
}

Logger::Logger (int be_recursive)
  : recursive_ (be_recursive)
{
}

void
Logger::log (ACE_Log_Record &log_record)
{
  int use_log_msg = 0;
  if (this->recursive_)
    {
      this->recursive_ = 0;
      use_log_msg = 1;
    }

  if (!this->verbose_logging_)
    {
      if (use_log_msg)
        ACE_DEBUG ((LM_DEBUG,
                    "Logger::log->%s\n",
                    log_record.msg_data ()));
      else
        ACE_OS::printf ("Recursive Logger callback = %s\n",
                        log_record.msg_data ());
    }
  else
    {
      ACE_TCHAR verbose_msg[ACE_Log_Record::MAXVERBOSELOGMSGLEN];
      int result = log_record.format_msg (ACE_LOG_MSG->local_host (),
                                          ACE_LOG_MSG->flags (),
                                          verbose_msg);
      if (result == 0)
        {
          if (use_log_msg)
            ACE_DEBUG ((LM_DEBUG,
                        "Logger::log->%s\n",
                        verbose_msg));
          else
            ACE_OS::printf ("Recursive Logger callback = %s\n",
                            verbose_msg);
        }
    }

  // Cleanup on the way out.
  if (use_log_msg)
    this->recursive_ = 1;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // This message should show up in stderr.
  ACE_DEBUG ((LM_DEBUG,
              "(%t) first message\n"));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

  // This message should not show up anywhere.
  ACE_DEBUG ((LM_DEBUG,
              "(%t) second message\n"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);

  // This message should not show up anywhere since no callback object
  // has been specified.
  ACE_DEBUG ((LM_DEBUG,
              "(%t) third message\n"));

  // Create a callback object and make it "verbose".
  Logger logger;
  logger.verbose (1);

  // Set the callback object.
  ACE_LOG_MSG->msg_callback (&logger);

  // This message should show up via the Logger callback.
  ACE_DEBUG ((LM_DEBUG,
              "(%t) forth message\n"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);

  // This message should show up via the Logger callback (somewhat
  // verbosely).
  ACE_DEBUG ((LM_DEBUG,
              "(%t) fifth message\n"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE);

  // This message should show up via the Logger callback (really
  // verbosely).
  ACE_DEBUG ((LM_DEBUG,
              "(%t) sixth message\n"));

  logger.verbose (0);

  // This message should show up via the Logger callback (not
  // verbosely).
  ACE_DEBUG ((LM_DEBUG,
              "(%t) seventh message\n"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

  // This message should show up in stderr and the Logger callback.
  // The one from the Logger callback will not be verbose, but the one
  // from stderr should be verbose.
  ACE_DEBUG ((LM_DEBUG,
              "(%t) eight message\n"));
  return 0;
}
