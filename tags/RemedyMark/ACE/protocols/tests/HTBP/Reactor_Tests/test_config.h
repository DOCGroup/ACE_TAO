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
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"

// The second #undef protects against being reset in a config.h file.
#undef ACE_NDEBUG

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

class ACE_Test_Output
{
public:
  ACE_Test_Output (void);
  ~ACE_Test_Output (void);
  int set_output (const ACE_TCHAR *filename, int append = 0);
  OFSTREAM *output_file (void);
  void close (void);

private:
  OFSTREAM *output_file_;
};

inline ACE_Test_Output::ACE_Test_Output (void)
  : output_file_ (0)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  this->output_file_ = new OFSTREAM;
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
}

inline ACE_Test_Output::~ACE_Test_Output (void)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY) && !defined (ACE_PSOS)
  ACE_LOG_MSG->msg_ostream (&cerr);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY && ! ACE_PSOS */

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY) && !defined (ACE_HAS_PHARLAP)
  delete this->output_file_;
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
}

inline OFSTREAM *
ACE_Test_Output::output_file (void)
{
  return this->output_file_;
}

inline int
ACE_Test_Output::set_output (const ACE_TCHAR *filename, int append)
{
#if defined (ACE_HAS_PHARLAP)
  // For PharLap, just send it all to the host console for now - redirect
  // to a file there for saving/analysis.
  EtsSelectConsole(ETS_CO_HOST);
  ACE_LOG_MSG->msg_ostream (&cout);

#else
  ACE_TCHAR temp[MAXPATHLEN];
  // Ignore the error value since the directory may already exist.
  const ACE_TCHAR *test_dir;

#if !defined (ACE_HAS_WINCE)
  test_dir = ACE_OS::getenv (ACE_TEXT ("ACE_TEST_DIR"));

  if (test_dir == 0)
#endif /* ACE_HAS_WINCE */
    test_dir = ACE_TEXT ("");

  ACE_OS::sprintf (temp,
                   ACE_TEXT ("%s%s%s%s"),
                   test_dir,
                   ACE_LOG_DIRECTORY,
                   ACE::basename (filename, ACE_DIRECTORY_SEPARATOR_CHAR),
                   ACE_LOG_FILE_EXT_NAME);

#if defined (VXWORKS)
  // This is the only way I could figure out to avoid a console
  // warning about opening an existing file (w/o O_CREAT), or
  // attempting to unlink a non-existant one.
  ACE_HANDLE fd = ACE_OS::open (temp,
                                O_WRONLY|O_CREAT,
                                S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  if (fd != ERROR)
    {
      ACE_OS::close (fd);
      ACE_OS::unlink (temp);
    }
# else /* ! VXWORKS */
  // This doesn't seem to work on VxWorks if the directory doesn't
  // exist: it creates a plain file instead of a directory.  If the
  // directory does exist, it causes a wierd console error message
  // about "cat: input error on standard input: Is a directory".  So,
  // VxWorks users must create the directory manually.
#   if defined (ACE_HAS_WINCE)
  ACE_OS::mkdir (ACE_LOG_DIRECTORY_FOR_MKDIR);
#   else
  ACE_OS::mkdir (ACE_LOG_DIRECTORY);
#   endif  // ACE_HAS_WINCE
# endif /* ! VXWORKS */

# if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  this->output_file_->open (ACE_TEXT_ALWAYS_CHAR (temp),
                            ios::out | (append ? ios::app : ios::trunc));
  if (this->output_file_->bad ())
    return -1;
#else /* when ACE_LACKS_IOSTREAM_TOTALLY */
  ACE_TCHAR *fmode = 0;
  if (append)
    fmode = ACE_TEXT ("a");
  else
    fmode = ACE_TEXT ("w");
  this->output_file_ = ACE_OS::fopen (temp, fmode);
# endif /* ACE_LACKS_IOSTREAM_TOTALLY */

  ACE_LOG_MSG->msg_ostream (this->output_file ());
#endif /* ACE_HAS_PHARLAP */

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER );
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  return 0;
}

inline void
ACE_Test_Output::close (void)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  this->output_file_->flush ();
  this->output_file_->close ();
#else
  ACE_OS::fflush (this->output_file_);
  ACE_OS::fclose (this->output_file_);
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */
  ACE_LOG_MSG->msg_ostream (0);
}

inline void
randomize (int array[], size_t size)
{
  size_t i;

  for (i = 0; i < size; i++)
    array [i] = static_cast<int> (i);

  // See with a fixed number so that we can produce "repeatable"
  // random numbers.
  ACE_OS::srand (0);

  // Generate an array of random numbers from 0 .. size - 1.

  for (i = 0; i < size; i++)
    {
      size_t index = ACE_OS::rand() % size--;
      int temp = array [index];
      array [index] = array [size];
      array [size] = temp;
    }
}

typedef ACE_Singleton<ACE_Test_Output, ACE_Null_Mutex> ace_file_stream;

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<ACE_Test_Output, ACE_Null_Mutex> *
ACE_Singleton<ACE_Test_Output, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

#endif /* ACE_TEST_CONFIG_H */
