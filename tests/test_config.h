/* -*- C++ -*- */
// $Id$

// ============================================================================
// = FILENAME
//    test_config.h
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu> and Tim Harrison <harrison@cs.wustl.edu>
// 
// ============================================================================

#if !defined (ACE_TEST_CONFIG_H)
#define ACE_TEST_CONFIG_H

#include <iostream.h>
#include <fstream.h>

#if defined (ACE_WIN32)
#define ACE_DEFAULT_TEST_FILE "C:\\temp\\ace_test_file"
#define ACE_TEMP_FILE_NAME "C:\\temp\\ace_temp_file"
#define ACE_LOG_DIRECTORY "C:\\temp\\log\\"
#define MAKE_PIPE_NAME(X) "\\\\.\\pipe\\"#X
#else
#define ACE_DEFAULT_TEST_FILE "/tmp/ace_test_file"
#define ACE_TEMP_FILE_NAME "/tmp/ace_temp_file"
#define ACE_LOG_DIRECTORY "log/"
#define MAKE_PIPE_NAME(X) X
#endif /* ACE_WIN32 */

#define ACE_START_TEST(NAME) \
  const char *program = argv ? argv[0] : NAME; \
  ACE_LOG_MSG->open (program, ACE_Log_Msg::OSTREAM); \
  if (ace_file_stream.set_output (program) != 0) \
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "set_output failed"), -1); \
  ACE_DEBUG ((LM_DEBUG, "starting %s test at %T\n", program));

#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, "Ending %s test at %T\n", program)); \
  ace_file_stream.flush ();

#define ACE_NEW_THREAD \
do {\
  ACE_Log_Msg::instance()->msg_ostream (ace_file_stream.output_file ()); \
  ACE_Log_Msg::instance()->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER ); \
  ACE_Log_Msg::instance()->set_flags (ACE_Log_Msg::OSTREAM); \
} while (0)

const int ACE_NS_MAX_ENTRIES = 2000;
const int ACE_MAX_TIMERS = 4;
const int ACE_MAX_THREADS = 4;
const int ACE_MAX_DELAY = 10;
const int ACE_MAX_INTERVAL = 0;
const int ACE_MAX_ITERATIONS = 10;

class ACE_Test_Output
{
public:
  ACE_Test_Output (void);
  ~ACE_Test_Output (void);
  int set_output (const char *filename);
  ofstream *output_file (void);
  void flush (void);

private:
  ofstream *output_file_;
};

static ACE_Test_Output ace_file_stream;

ACE_Test_Output::ACE_Test_Output (void)
  : output_file_ (0) 
{ 
}

ACE_Test_Output::~ACE_Test_Output (void) 
{ 
  delete this->output_file_; 
}

int 
ACE_Test_Output::set_output (const char *filename)
{
  char temp[BUFSIZ];
  // Ignore the error value since the directory may already exist.
  ACE_OS::mkdir (ACE_LOG_DIRECTORY);
  ACE_OS::sprintf (temp, "%s%s%s", 
		   ACE_LOG_DIRECTORY, 
		   ACE::basename (filename, ACE_DIRECTORY_SEPARATOR_CHAR),
		   ".log");

  ACE_NEW_RETURN (this->output_file_, ofstream (temp), -1);
  ACE_NEW_THREAD;
  return 0;
}

ofstream *
ACE_Test_Output::output_file (void) 
{ 
  return this->output_file_; 
}
  
void 
ACE_Test_Output::flush (void) 
{ 
  this->output_file_->flush (); 
}
#endif /* ACE_TEST_CONFIG_H */
