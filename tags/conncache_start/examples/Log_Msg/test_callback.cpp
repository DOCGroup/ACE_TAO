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
//     This program tests the Log_Msg abstraction wrt writing to user
//     defined callback objects.
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/OS.h"

ACE_RCSID(Log_Msg, test_callback, "$Id$")

class Logger : public ACE_Log_Msg_Callback
{
public:
  void log (ACE_Log_Record &log_record);
  // Logging callback

  int verbose_logging_;
  // Flag for verbose logging
};

void
Logger::log (ACE_Log_Record &log_record)
{
  if (!this->verbose_logging_)
    {
      ACE_OS::printf ("Logger::log -> %s\n",  
                      log_record.msg_data ());
    }
  else
    {
      char verbose_msg[ACE_Log_Record::MAXVERBOSELOGMSGLEN];
      int result = log_record.format_msg (ACE_LOG_MSG->local_host (), 
                                          ACE_LOG_MSG->flags (), 
                                          verbose_msg);

      if (result == 0)
        {
          ACE_OS::printf ("Logger::log -> %s\n",  
                          verbose_msg);
        }      
    }
}

int
main (int, char *[])
{
  // This message should show up in stderr.
  ACE_DEBUG ((LM_DEBUG,
              "first message\n"));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

  // This message should not show up anywhere.
  ACE_DEBUG ((LM_DEBUG,
              "second message\n"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);

  // This message should not show up anywhere since no callback object
  // has been specified.
  ACE_DEBUG ((LM_DEBUG,
              "third message\n"));

  // Create a callback object.
  Logger logger;
  logger.verbose_logging_ = 1;

  // Set the callback object.
  ACE_LOG_MSG->msg_callback (&logger);

  // This message should show up in the logger.
  ACE_DEBUG ((LM_DEBUG,
              "forth message\n"));
  
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);

  // This message should show up in the logger (somewhat loudly).
  ACE_DEBUG ((LM_DEBUG,
              "fifth message\n"));
  
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE);

  // This message should show up in the logger (really loudly).
  ACE_DEBUG ((LM_DEBUG,
              "sixth message\n"));
  
  logger.verbose_logging_ = 0;

  // This message should show up in the logger (not loudly).
  ACE_DEBUG ((LM_DEBUG,
              "seventh message\n"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

  // This message should show up in stderr and the logger. The one
  // from the logger will not be loud, but the one from stderr should
  // be loud.
  ACE_DEBUG ((LM_DEBUG,
              "eight message\n"));
  return 0;
}
