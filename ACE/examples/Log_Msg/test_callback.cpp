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

#include "ace/OS_main.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"
#include "ace/OS_NS_stdio.h"



class Logger : public ACE_Log_Msg_Callback
// Subclassing from ACE_Log_Msg_Callback means that an instance of
// Logger can be a target of a callback.
{
public:
  // Constructor sets whether we're testing "recursive" callback
  // logging!
  Logger (int be_recursive = 1);

  virtual void log (ACE_Log_Record &log_record);
  // Logging callback hook.

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
                    "Logger::log->%s",
                    log_record.msg_data ()));
      else
        ACE_OS::printf ("Recursive Logger callback = %s",
                        ACE_TEXT_ALWAYS_CHAR (log_record.msg_data ()));
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
                        "Logger::log->%s",
                        verbose_msg));
          else
            ACE_OS::printf ("Recursive Logger callback = %s",
                            ACE_TEXT_ALWAYS_CHAR (verbose_msg));
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

  // This message should not show up anywhere since we disabled STDERR.
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
              "(%t) fourth message\n"));

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
              "(%t) eighth message\n"));
  return 0;
}
