// -*- C++ -*-

// ============================================================================
/**
 *  @file test_config.h
 *
 *  $Id$
 *
 *   This file factors out common macros and other utilities used by the
 *   ACE automated regression tests.  It also shows how to redirect ACE_DEBUG/ACE_ERROR
 *   output to a file.
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 *  @author Tim Harrison <harrison@cs.wustl.edu>
 *  @author David Levine <levine@cs.wustl.edu>
 */
// ============================================================================

#ifndef ACE_TEST_CONFIG_H
#define ACE_TEST_CONFIG_H

// This first #undef protects against command-line definitions.
#undef ACE_NDEBUG

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_NLOGGING)
// ACE_NLOGGING must not be set if the tests are to produce any output.
#undef ACE_NLOGGING
#endif /* ACE_NLOGGING */

#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Log_Msg.h"

#if defined (ACE_HAS_WINCE)
// Note that Pocket PC 2002 will NOT create a directory if it does not start with a leading '\'.
// PPC 2002 only accepts '\log' as a valid directory name, while 'log\' works under WinCE 3.0.
# define ACE_LOG_DIRECTORY_FOR_MKDIR ACE_TEXT ("\\log")
# define ACE_LOG_DIRECTORY           ACE_TEXT ("\\log\\")
# define MAKE_PIPE_NAME(X) ACE_TEXT ("\\\\.\\pipe\\"#X)
#elif defined (ACE_WIN32)
# define ACE_LOG_DIRECTORY ACE_TEXT ("log\\")
# define ACE_LOG_DIRECTORY_FOR_MKDIR ACE_TEXT ("log\\")
# define MAKE_PIPE_NAME(X) ACE_TEXT ("\\\\.\\pipe\\"#X)
#elif defined (ANDROID)
# define ACE_LOG_DIRECTORY_FOR_MKDIR ACE_TEXT ("/sdcard/log/")
# define ACE_LOG_DIRECTORY ACE_TEXT ("/sdcard/log/")
# define MAKE_PIPE_NAME(X) ACE_TEXT (X)
#else
# define ACE_LOG_DIRECTORY_FOR_MKDIR ACE_TEXT ("log/")
# define ACE_LOG_DIRECTORY ACE_TEXT ("log/")
# define MAKE_PIPE_NAME(X) ACE_TEXT (X)
#endif /* ACE_WIN32 */

#if !defined (ACE_DEFAULT_TEST_DIR)
# define ACE_DEFAULT_TEST_DIR ACE_TEXT ("")
#endif

#define ACE_LOG_FILE_EXT_NAME ACE_TEXT (".log")

#if defined (ACE_HAS_WINCE) || defined (ACE_HAS_PHARLAP)
size_t const ACE_MAX_CLIENTS = 4;
#else
size_t const ACE_MAX_CLIENTS = 30;
#endif /* ACE_HAS_WINCE */

size_t const ACE_NS_MAX_ENTRIES = 1000;
size_t const ACE_DEFAULT_USECS = 1000;
size_t const ACE_MAX_TIMERS = 4;
size_t const ACE_MAX_DELAY = 10;
size_t const ACE_MAX_INTERVAL = 0;
size_t const ACE_MAX_ITERATIONS = 10;
size_t const ACE_MAX_PROCESSES = 10;
size_t const ACE_MAX_THREADS = 4;

#ifndef ACE_START_TEST
#define ACE_START_TEST(NAME) \
  const ACE_TCHAR *program = NAME; \
  if (ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open log_msg failed")), -1); \
  if (ace_file_stream::instance()->set_output (program) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), program))
#endif /* ACE_START_TEST */

#ifndef ACE_END_TEST
#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n"), program)); \
  ace_file_stream::instance()->close ()
#endif /* ACE_END_TEST */

#define ACE_CLOSE_TEST_LOG ace_file_stream::instance()->close ()

#define ACE_APPEND_LOG(NAME) \
  const ACE_TCHAR *program = NAME; \
  if (ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open log_msg failed")), -1); \
  if (ace_file_stream::instance()->set_output (program, 1) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), program));

#define ACE_END_LOG \
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n\n"), program)); \
  ACE_LOG_MSG->set_flags(ACE_Log_Msg::SILENT); \
  ace_file_stream::instance()->close ();

#if defined (ACE_VXWORKS)
  // This is the only way I could figure out to avoid an error
  // about attempting to unlink a non-existent file.

#include "ace/OS_NS_fcntl.h"

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

#else /* ! VXWORKS */
#  if !defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
#    define ACE_INIT_LOG_FMT ACE_TEXT ("%ls%ls%ls")
#  else
#    define ACE_INIT_LOG_FMT ACE_TEXT ("%s%s%s")
#  endif /* !ACE_WIN32 && ACE_USES_WCHAR */
#define ACE_INIT_LOG(NAME) \
  ACE_TCHAR temp[MAXPATHLEN]; \
  ACE_OS::sprintf (temp, ACE_INIT_LOG_FMT, \
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

#include "Test_Output_Export.h"

class Test_Output_Export ACE_Test_Output
{
public:
  ACE_Test_Output (void);
  ~ACE_Test_Output (void);
  static ACE_Test_Output *instance (void);
  int set_output (const ACE_TCHAR *filename, int append = 0);
  OFSTREAM *output_file (void);
  void close (void);
  const ACE_TCHAR *dll_name (void);
  const ACE_TCHAR *name (void);
  static void close_singleton (void);

private:
  static ACE_Test_Output *instance_;

  OFSTREAM *output_file_;
};

typedef ACE_Test_Output ace_file_stream;

#endif /* ACE_TEST_CONFIG_H */
