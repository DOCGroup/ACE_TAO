/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    test_config.h
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 *  @author Tim Harrison <harrison@cs.wustl.edu>
 *  @author and David Levine <levine@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TEST_CONFIG_H
#define ACE_TEST_CONFIG_H

#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_stat.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#if defined (ACE_WIN32)

#define ACE_DEFAULT_TEST_FILE ACE_TEXT ("C:\\temp\\ace_test_file")
#define ACE_TEMP_FILE_NAME ACE_TEXT ("C:\\temp\\ace_temp_file")
#define ACE_LOG_DIRECTORY ACE_TEXT ("C:\\temp\\log\\")
#define MAKE_PIPE_NAME(X) ACE_TEXT ("\\\\.\\pipe\\"#X)

#else

#define ACE_DEFAULT_TEST_FILE ACE_TEXT ("/tmp/ace_test_file")
#define ACE_TEMP_FILE_NAME ACE_TEXT ("/tmp/ace_temp_file")
#define ACE_LOG_DIRECTORY ACE_TEXT ("log/")
#define MAKE_PIPE_NAME(X) ACE_TEXT (X)

#endif /* ACE_WIN32 */

#ifndef ACE_START_TEST
#define ACE_START_TEST(NAME) \
  ACE_TCHAR const *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM); \
  if (ace_file_stream.set_output (program) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) starting %s test at %D\n"), program));
#endif /* ACE_START_TEST */

#ifndef ACE_END_TEST
#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n"), program)); \
  ace_file_stream.close ();
#endif /* ACE_END_TEST */

#define ACE_NEW_THREAD \
do {\
  ACE_LOG_MSG->msg_ostream (ace_file_stream.output_file ()); \
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER); \
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM); \
} while (0)

#define ACE_APPEND_LOG(NAME) \
  ACE_TCHAR const *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM); \
  if (ace_file_stream.set_output (program, true) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), program));

#define ACE_END_LOG \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n\n"), program)); \
  ace_file_stream.close ();

#define ACE_INIT_LOG(NAME) \
  char temp[BUFSIZ]; \
  ACE_OS::sprintf (temp, "%s%s%s", \
                   ACE_TEXT_ALWAYS_CHAR (ACE_LOG_DIRECTORY), \
                   ACE_TEXT_ALWAYS_CHAR (ACE::basename (NAME, ACE_DIRECTORY_SEPARATOR_CHAR)), \
                   ".log"); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Deleting old log file %C (if any)\n\n"), temp)); \
  ACE_OS::unlink (temp);


class ACE_Test_Output
{
public:
  ACE_Test_Output () = default;
  ~ACE_Test_Output () = default;
  ACE_Test_Output (ACE_Test_Output const &) = delete;
  ACE_Test_Output (ACE_Test_Output &&) = delete;
  ACE_Test_Output &operator= (ACE_Test_Output const &) = delete;
  ACE_Test_Output &operator= (ACE_Test_Output &&) = delete;
  int set_output (ACE_TCHAR const *filename, bool append = false);
  ofstream *output_file ();
  void close ();

private:
  ofstream output_file_;
};

static ACE_Test_Output ace_file_stream;

int
ACE_Test_Output::set_output (ACE_TCHAR const *filename, bool append)
{
  char temp[BUFSIZ];
  // Ignore the error value since the directory may already exist.
  ACE_OS::mkdir (ACE_LOG_DIRECTORY);
  ACE_OS::sprintf (temp, "%s%s%s",
                   ACE_TEXT_ALWAYS_CHAR (ACE_LOG_DIRECTORY),
                   ACE_TEXT_ALWAYS_CHAR (ACE::basename (filename, ACE_DIRECTORY_SEPARATOR_CHAR)),
                   ".log");

  this->output_file_.open (temp, ios::out | (append ? ios::app : ios::trunc));
  if (this->output_file_.bad ())
    return -1;

  ACE_LOG_MSG->msg_ostream (ace_file_stream.output_file ());
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  return 0;
}

ofstream *
ACE_Test_Output::output_file ()
{
  return &this->output_file_;
}

void
ACE_Test_Output::close ()
{
  this->output_file_.flush ();
  this->output_file_.close ();
}

#endif /* ACE_TEST_CONFIG_H */
