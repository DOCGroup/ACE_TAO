/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//    test_config.h
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu>, Tim Harrison
//    <harrison@cs.wustl.edu>, and David Levine <levine@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_TEST_CONFIG_H
#define ACE_TEST_CONFIG_H

#include "ace/inc_user_config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_NLOGGING)
// ACE_NLOGGING must not be set if the tests are to produce any output.
#undef ACE_NLOGGING
#endif /* ACE_NLOGGING */

/* Borland C++ Builder stuff */
#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
#include /**/ <condefs.h>
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

// This first #undef protects against command-line definitions.
#undef ACE_NDEBUG
#include "ace/OS.h"
#include "ace/streams.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"

// The second #undef protects against being reset in a config.h file.
#undef ACE_NDEBUG

#if defined (ACE_HAS_WINCE)

#define ACE_DEFAULT_TEST_FILE_W L"\\temp\\ace_test_file"
#define ACE_TEMP_FILE_NAME_W L"\\temp\\ace_temp_file"
#define ACE_LOG_DIRECTORY_W L"\\temp\\log\\"
#define ACE_LOG_FILE_EXT_NAME_W L".txt"
#define MAKE_PIPE_NAME_W(X) L"\\\\.\\pipe\\"#X

#define ACE_DEFAULT_TEST_FILE_A ACE_DEFAULT_TEST_FILE_W
#define ACE_TEMP_FILE_NAME_A ACE_TEMP_FILE_NAME_W
#define ACE_LOG_DIRECTORY_A ACE_LOG_DIRECTORY_W
#define MAKE_PIPE_NAME_A(X) MAKE_PIPE_NAME_W(X)
#define ACE_LOG_FILE_EXT_NAME_A ACE_LOG_FILE_EXT_NAME_W

#elif defined (ACE_WIN32)

#define ACE_DEFAULT_TEST_FILE_A "C:\\temp\\ace_test_file"
#define ACE_TEMP_FILE_NAME_A "C:\\temp\\ace_temp_file"
#define ACE_LOG_DIRECTORY_A "C:\\temp\\log\\"
#define MAKE_PIPE_NAME_A(X) "\\\\.\\pipe\\"#X

#define ACE_DEFAULT_TEST_FILE_W L"C:\\temp\\ace_test_file"
#define ACE_TEMP_FILE_NAME_W L"C:\\temp\\ace_temp_file"
#define ACE_LOG_DIRECTORY_W L"C:\\temp\\log\\"
#define MAKE_PIPE_NAME_W(X) L"\\\\.\\pipe\\"#X

#else

#define ACE_DEFAULT_TEST_FILE_A "/tmp/ace_test_file"
#define ACE_TEMP_FILE_NAME_A "/tmp/ace_temp_file"
#define ACE_LOG_DIRECTORY_A "log/"
#define MAKE_PIPE_NAME_A(X) X

#if defined (ACE_HAS_UNICODE)
#define ACE_DEFAULT_TEST_FILE_W L"/tmp/ace_test_file"
#define ACE_TEMP_FILE_NAME_W L"/tmp/ace_temp_file"
#define ACE_LOG_DIRECTORY_W L"log/"
#define MAKE_PIPE_NAME_W(X) L##X
#else
#define ACE_DEFAULT_TEST_FILE_W "/tmp/ace_test_file"
#define ACE_TEMP_FILE_NAME_W "/tmp/ace_temp_file"
#define ACE_LOG_DIRECTORY_W "log/"
#define MAKE_PIPE_NAME_W(X) X
#endif /* ACE_HAS_UNICODE */

#endif /* ACE_WIN32 */

#if defined (ACE_HAS_WINCE)
#define ACE_LOG_FILE_EXT_NAME_W L".txt"
#define ACE_LOG_FILE_EXT_NAME_A ACE_LOG_FILE_EXT_NAME_W
#else
#define ACE_LOG_FILE_EXT_NAME_W L".log"
#define ACE_LOG_FILE_EXT_NAME_A ".log"
#endif /* ACE_HAS_WINCE */

#if defined (UNICODE)
#define ACE_DEFAULT_TEST_FILE ACE_DEFAULT_TEST_FILE_W
#define ACE_TEMP_FILE_NAME ACE_TEMP_FILE_NAME_W
#define ACE_LOG_DIRECTORY ACE_LOG_DIRECTORY_W
#define MAKE_PIPE_NAME MAKE_PIPE_NAME_W
#else
#define ACE_DEFAULT_TEST_FILE ACE_DEFAULT_TEST_FILE_A
#define ACE_TEMP_FILE_NAME ACE_TEMP_FILE_NAME_A
#define ACE_LOG_DIRECTORY ACE_LOG_DIRECTORY_A
#define MAKE_PIPE_NAME MAKE_PIPE_NAME_A
#endif /* UNICODE */

#if defined (ACE_HAS_WINCE)
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
#if defined (__Lynx__)
  const size_t ACE_MAX_PROCESSES = 4;
  const size_t ACE_MAX_THREADS = 2;
#else  /* ! __Lynx__ */
  const size_t ACE_MAX_PROCESSES = 10;
  const size_t ACE_MAX_THREADS = 4;
#endif /* ! __Lynx__ */

char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

#define ACE_START_TEST(NAME) \
  const ASYS_TCHAR *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE); \
  if (ace_file_stream::instance()->set_output (program) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("%p\n"), ASYS_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) starting %s test at %D\n"), program))

#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) Ending %s test at %D\n"), program)); \
  ace_file_stream::instance()->close ()

#if !defined (ACE_WIN32)
#define ACE_APPEND_LOG(NAME) \
  const ASYS_TCHAR *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE); \
  ace_file_stream::instance()->close (); \
  if (ace_file_stream::instance()->set_output (program, 1) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("%p\n"), ASYS_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) Starting %s test at %D\n"), program));
#else /* ACE_WIN32 */
#define ACE_APPEND_LOG(NAME) \
  const ASYS_TCHAR *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE); \
  if (ace_file_stream::instance()->set_output (program, 1) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("%p\n"), ASYS_TEXT ("set_output failed")), -1); \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) Starting %s test at %D\n"), program));
#endif /* ACE_WIN32 */

#define ACE_END_LOG \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) Ending %s test at %D\n\n"), program)); \
  ACE_LOG_MSG->set_flags(ACE_Log_Msg::SILENT); \
  ace_file_stream::instance()->close ();

#if defined (VXWORKS)
  // This is the only way I could figure out to avoid an error
  // about attempting to unlink a non-existant file.
#define ACE_INIT_LOG(NAME) \
  char temp[MAXPATHLEN]; \
  ACE_OS::sprintf (temp, "%s%s%s", \
                   ACE_LOG_DIRECTORY_A, \
                   ACE::basename (NAME, ACE_DIRECTORY_SEPARATOR_CHAR_A), \
                   ACE_LOG_FILE_EXT_NAME_A); \
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Deleting old log file %s (if any)\n\n", temp)); \
  int fd_init_log; \
  if ((fd_init_log = ACE_OS::open (temp, \
                                   O_WRONLY | O_CREAT, 0x644)) != ERROR) \
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
  char temp[MAXPATHLEN]; \
  ACE_OS::sprintf (temp, "%s%s%s", \
                   ACE_LOG_DIRECTORY_A, \
                   ACE::basename (NAME, ACE_DIRECTORY_SEPARATOR_CHAR_A), \
                   ACE_LOG_FILE_EXT_NAME_A); \
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Deleting old log file %s (if any)\n\n", temp)); \
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
  int set_output (const ASYS_TCHAR *filename, int append = 0);
  OFSTREAM *output_file (void);
  void close (void);

private:
  OFSTREAM *output_file_;
};

typedef ACE_Singleton<ACE_Test_Output, ACE_Null_Mutex> ace_file_stream;

ACE_Test_Output::ACE_Test_Output (void)
  : output_file_ (0)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  this->output_file_ = new OFSTREAM;
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

}

ACE_Test_Output::~ACE_Test_Output (void)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY) && !defined (ACE_PSOS)
  ACE_LOG_MSG->msg_ostream (&cerr);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY && ! ACE_PSOS */

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  delete this->output_file_;
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
}

int
ACE_Test_Output::set_output (const ASYS_TCHAR *filename, int append)
{
  ASYS_TCHAR temp[MAXPATHLEN];
  // Ignore the error value since the directory may already exist.
  ASYS_TCHAR *test_dir;

#if !defined (ACE_HAS_WINCE)
  test_dir = ACE_OS::getenv ("ACE_TEST_DIR");

  if (test_dir == 0)
#endif /* ACE_HAS_WINCE */
    test_dir = ASYS_TEXT ("");



  ACE_OS::sprintf (temp,
                   ASYS_TEXT ("%s%s%s%s"),
                   test_dir,
                   ACE_LOG_DIRECTORY_A,
                   ACE::basename (filename, ACE_DIRECTORY_SEPARATOR_CHAR_A),
                   ACE_LOG_FILE_EXT_NAME_A);

#if defined (VXWORKS)
  // This is the only way I could figure out to avoid a console warning
  // about opening an existing file (w/o O_CREAT), or attempting to unlink
  // a non-existant one.
  int fd;
  if ((fd = ACE_OS::open (temp, O_WRONLY | O_CREAT, 0x644)) != ERROR)
    {
      ACE_OS::close (fd);
      ACE_OS::unlink (temp);
    }
#else /* ! VXWORKS */
  // This doesn't seem to work on VxWorks if the directory doesn't
  // exist:  it creates a plain file instead of a directory.  If the
  // directory does exist, it causes a wierd console error message
  // about "cat: input error on standard input: Is a directory".  So,
  // VxWorks users must create the directory manually.
  ACE_OS::mkdir (ACE_LOG_DIRECTORY_A);
#endif /* ! VXWORKS */

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  int flags = ios::out;
  if (append)
    flags |= ios::app;
  else
    flags |= ios::trunc;

  this->output_file_->open (temp, flags);
  if (this->output_file_->bad ())
    {
      return -1;
    }
#else /* when ACE_LACKS_IOSTREAM_TOTALLY */
  ASYS_TCHAR *fmode = 0;
  if (append)
    fmode = ASYS_TEXT ("a");
  else
    fmode = ASYS_TEXT ("w");
  this->output_file_ = ACE_OS::fopen (temp, fmode);
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

  ACE_LOG_MSG->msg_ostream (this->output_file ());
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER );
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  return 0;
}

OFSTREAM *
ACE_Test_Output::output_file (void)
{
  return this->output_file_;
}

void
ACE_Test_Output::close (void)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  this->output_file_->flush ();
  this->output_file_->close ();
#else
  ACE_OS::fflush (this->output_file_);
  ACE_OS::fclose (this->output_file_);
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */
}

#if 0 /* old WinCE stuff */

#define ACE_START_TEST(NAME) \
  const ASYS_TCHAR *program = NAME; \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) starting %s test at %D\n"), program))

#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) Ending %s test at %D\n"), program)); \

#undef ACE_DEFAULT_TEST_FILE
#define ACE_DEFAULT_TEST_FILE L"\\temp\\ace_test_file"

#endif /* 0 */

void
randomize (int array[], size_t size)
{
  size_t i;

  for (i = 0; i < size; i++)
    array [i] = i;

  // See with a fixed number so that we can produce "repeatable"
  // random numbers.
  ACE_OS::srand (0);

  // Generate an array of random numbers from 0 .. size - 1.

  for (i = 0; i < size; i++)
    {
      int index = ACE_OS::rand() % size--;
      int temp = array [index];
      array [index] = array [size];
      array [size] = temp;
    }
}

// This shouldn't be done in a header!  But, we don't have any other
// need for a .cpp file to link into all tests.
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_Test_Output, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_Test_Output, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_TEST_CONFIG_H */
