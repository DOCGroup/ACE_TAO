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

#if !defined (ACE_TEST_CONFIG_H)
#define ACE_TEST_CONFIG_H

// This first #undef protects against command-line definitions.
#undef ACE_NDEBUG
#include "ace/OS.h"
#include "ace/streams.h"

// The second #undef protects against being reset in a config.h file.
#undef ACE_NDEBUG

#if !defined (ACE_HAS_TEMPLATE_SPECIALIZATION)
class KEY
{
  // = TITLE
  //    Define a key for use with the Map_Manager_Test.
  //
  // = DESCRIPTION
  //    This class is put into the test_config.h header file to work
  //    around AIX C++ compiler "features" related to template
  //    instantiation...  It is only used by Map_Manager_Test.cpp
public:
  KEY (size_t v = 0): value_ (v)
  { }

  size_t hash (void) const { return this->value_; }
  operator size_t () const { return this->value_; }

private:
  size_t value_;
};
#else
typedef size_t KEY;
#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */

#if defined (ACE_WIN32)

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

const size_t ACE_NS_MAX_ENTRIES = 1000;
const size_t ACE_DEFAULT_USECS = 1000;
const size_t ACE_MAX_TIMERS = 4;
const size_t ACE_MAX_THREADS = 4;
const size_t ACE_MAX_DELAY = 10;
const size_t ACE_MAX_INTERVAL = 0;
const size_t ACE_MAX_ITERATIONS = 10;
const size_t ACE_MAX_PROCESSES = 10;
const size_t ACE_MAX_CLIENTS = 30;

char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

#if !defined (ACE_HAS_WINCE)
#define ACE_START_TEST(NAME) \
  const char *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE); \
  if (ace_file_stream.set_output (program) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "set_output failed"), -1); \
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting %s test at %D\n", program))

#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Ending %s test at %D\n", program)); \
  ace_file_stream.close ()

#define ACE_APPEND_LOG(NAME) \
  const char *program = NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE); \
  ace_file_stream.close (); \
  if (ace_file_stream.set_output (program, 1) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "set_output failed"), -1); \
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting %s test at %D\n", program));

#define ACE_END_LOG \
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Ending %s test at %D\n\n", program)); \
  ace_file_stream.close ();

#if defined (VXWORKS)
  // This is the only way I could figure out to avoid an error
  // about attempting to unlink a non-existant file.
#define ACE_INIT_LOG(NAME) \
  char temp[MAXPATHLEN]; \
  ACE_OS::sprintf (temp, "%s%s%s", \
                   ACE_LOG_DIRECTORY_A, \
                   ACE::basename (NAME, ACE_DIRECTORY_SEPARATOR_CHAR_A), \
                   ".log"); \
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
                   ".log"); \
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Deleting old log file %s (if any)\n\n", temp)); \
  ACE_OS::unlink (temp);
#endif /* ! VXWORKS */

class ACE_Test_Output
{
public:
  ACE_Test_Output (void);
  ~ACE_Test_Output (void);
  int set_output (const char *filename, int append = 0);
  ofstream *output_file (void);
  void close (void);

private:
  ofstream output_file_;
};

static ACE_Test_Output ace_file_stream;

ACE_Test_Output::ACE_Test_Output (void)
{
}

ACE_Test_Output::~ACE_Test_Output (void)
{
}

int
ACE_Test_Output::set_output (const char *filename, int append)
{
  char temp[MAXPATHLEN];
  // Ignore the error value since the directory may already exist.

  char *test_dir = ACE_OS::getenv ("ACE_TEST_DIR");

  if (test_dir == 0)
    test_dir = "";

  ACE_OS::sprintf (temp,
                   "%s%s%s%s",
                   test_dir,
                   ACE_LOG_DIRECTORY_A,
                   ACE::basename (filename, ACE_DIRECTORY_SEPARATOR_CHAR_A),
                   ".log");

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

  int flags = ios::out;
  if (append)
    flags |= ios::app;
  else
    flags |= ios::trunc;

  this->output_file_.open (temp, flags);
  if (this->output_file_.bad ())
    {
      return -1;
    }

  ACE_LOG_MSG->msg_ostream (ace_file_stream.output_file ());
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER );
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  return 0;
}

ofstream *
ACE_Test_Output::output_file (void)
{
  return &this->output_file_;
}

void
ACE_Test_Output::close (void)
{
  this->output_file_.flush ();
  this->output_file_.close ();
}
#else

#define ACE_START_TEST(NAME) \
  const ASYS_TCHAR *program = NAME; \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) starting %s test at %D\n"), program))

#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) Ending %s test at %D\n"), program)); \

#endif /* ACE_HAS_WINCE */

void
randomize (int array[], size_t size)
{
  size_t i;

  for (i = 0; i < size; i++)
    array [i] = i;

  ACE_OS::srand (ACE_OS::time (0L));

  // Generate an array of random numbers from 0 .. size - 1.

  for (i = 0; i < size; i++)
    {
      int index = ACE_OS::rand() % size--;
      int temp = array [index];
      array [index] = array [size];
      array [size] = temp;
    }
}

#endif /* ACE_TEST_CONFIG_H */
