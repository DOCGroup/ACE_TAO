// -*- C++ -*-


// ============================================================================
/**
 *  @file test_config.h
 *
 *  $Id$
 *
 *   This file factors out common macros and other utilities used by the
 *   ACE automated regression tests.
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 *  @author Tim Harrison <harrison@cs.wustl.edu>
 *  @author David Levine <levine@cs.wustl.edu>
 */
// ============================================================================

#ifndef ACE_TEST_CONFIG_H
#define ACE_TEST_CONFIG_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_NLOGGING)
// ACE_NLOGGING must not be set if the tests are to produce any output.
#undef ACE_NLOGGING
#endif /* ACE_NLOGGING */

// This first #undef protects against command-line definitions.
#undef ACE_NDEBUG
#include "ace/streams.h"
#include "ace/Log_Msg.h"

#if defined (ACE_HAS_WINCE)
// Note that Pocket PC 2002 will NOT create a directory if it does not start with a leading '\'.
// PPC 2002 only accepts '\log' as a valid directory name, while 'log\' works under WinCE 3.0.
# define ACE_LOG_DIRECTORY_FOR_MKDIR ACE_TEXT ("\\log")
# define ACE_LOG_DIRECTORY           ACE_TEXT ("\\log\\")
# define MAKE_PIPE_NAME(X) ACE_TEXT ("\\\\.\\pipe\\"#X)
#elif defined (ACE_WIN32)
# define ACE_LOG_DIRECTORY ACE_TEXT ("log\\")
# define MAKE_PIPE_NAME(X) ACE_TEXT ("\\\\.\\pipe\\"#X)
#else
# define ACE_LOG_DIRECTORY ACE_TEXT ("log/")
# define MAKE_PIPE_NAME(X) ACE_TEXT (X)
#endif /* ACE_WIN32 */

#if defined (ACE_HAS_WINCE)
#define ACE_LOG_FILE_EXT_NAME ACE_TEXT (".txt")
#else
#define ACE_LOG_FILE_EXT_NAME ACE_TEXT (".log")
#endif /* ACE_HAS_WINCE */

#if defined (ACE_HAS_WINCE) || defined (ACE_HAS_PHARLAP)
const size_t ACE_MAX_CLIENTS = 4;
#else
const size_t ACE_MAX_CLIENTS = 30;
#endif /* ACE_HAS_WINCE */

const size_t ACE_NS_MAX_ENTRIES = 1000;
const size_t ACE_DEFAULT_USECS = 1000;
const size_t ACE_MAX_TIMERS = 4;
const size_t ACE_MAX_DELAY = 10;
const size_t ACE_MAX_INTERVAL = 0;
const size_t ACE_MAX_ITERATIONS = 10;
const size_t ACE_MAX_PROCESSES = 10;
const size_t ACE_MAX_THREADS = 4;

#define ACE_START_TEST(NAME) \
  const ACE_TCHAR *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE); \
  if (ace_file_stream::instance()->set_output (program) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), program))

#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n"), program)); \
  ace_file_stream::instance()->close ()

#define ACE_CLOSE_TEST_LOG ace_file_stream::instance()->close ()

#if !defined (ACE_WIN32)
#define ACE_APPEND_LOG(NAME) \
  const ACE_TCHAR *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE); \
  ace_file_stream::instance()->close (); \
  if (ace_file_stream::instance()->set_output (program, 1) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), program));
#else /* ACE_WIN32 */
#define ACE_APPEND_LOG(NAME) \
  const ACE_TCHAR *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE); \
  if (ace_file_stream::instance()->set_output (program, 1) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), program));
#endif /* ACE_WIN32 */

#define ACE_END_LOG \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n\n"), program)); \
  ACE_LOG_MSG->set_flags(ACE_Log_Msg::SILENT); \
  ace_file_stream::instance()->close ();

#if defined (VXWORKS)
  // This is the only way I could figure out to avoid an error
  // about attempting to unlink a non-existant file.
#define ACE_INIT_LOG(NAME) \
  ACE_TCHAR temp[MAXPATHLEN]; \
  ACE_OS::sprintf (temp, ACE_TEXT ("%s%s%s"), \
                   ACE_LOG_DIRECTORY, \
                   ACE::basename (NAME, ACE_DIRECTORY_SEPARATOR_CHAR), \
                   ACE_LOG_FILE_EXT_NAME); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Deleting old log file %s (if any)\n\n"), temp)); \
  int fd_init_log; \
  if ((fd_init_log = ACE_OS::open (temp, \
                                   O_WRONLY|O_CREAT, \
                                   S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) != ERROR) \
    { \
      ACE_OS::close (fd_init_log); \
      ACE_OS::unlink (temp); \
    }

#if defined (ghs)
# // Rename main to ace_main for compatibility with run_tests.vxworks.
# undef ACE_MAIN
# define ACE_MAIN ace_main
#endif /* ghs */
#else /* ! VXWORKS */
#define ACE_INIT_LOG(NAME) \
  ACE_TCHAR temp[MAXPATHLEN]; \
  ACE_OS::sprintf (temp, ACE_TEXT ("%s%s%s"), \
                   ACE_LOG_DIRECTORY, \
                   ACE::basename (NAME, ACE_DIRECTORY_SEPARATOR_CHAR), \
                   ACE_LOG_FILE_EXT_NAME); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Deleting old log file %s (if any)\n\n"), temp)); \
  ACE_OS::unlink (temp);
#endif /* ! VXWORKS */

#if defined (ACE_LACKS_IOSTREAM_TOTALLY)
#define OFSTREAM FILE
#else
#define OFSTREAM ofstream
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

#include "tests/Test_Output_Export.h"

class Test_Output_Export ACE_Test_Output
{
public:
  ACE_Test_Output (void);
  ~ACE_Test_Output (void);
  static ACE_Test_Output *instance ();
  int set_output (const ACE_TCHAR *filename, int append = 0);
  OFSTREAM *output_file (void);
  void close (void);

private:
  OFSTREAM *output_file_;
};

typedef ACE_Test_Output ace_file_stream;

void randomize (int array[], size_t size);

#endif /* ACE_TEST_CONFIG_H */
