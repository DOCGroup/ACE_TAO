/* -*- C++ -*- */
// @(#)test_config.h	1.1	10/18/96

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

const int ACE_NS_MAX_ENTRIES = 2000;
const int ACE_MAX_TIMERS = 4;
const int ACE_MAX_THREADS = 4;
const int ACE_MAX_DELAY = 10;
const int ACE_MAX_INTERVAL = 0;
const int ACE_MAX_ITERATIONS = 10;

class ACE_Test_Output
{
public:
  ACE_Test_Output (void): output_file_ (0)
    {
    }

  ~ACE_Test_Output (void)
    {
      delete this->output_file_;
    }

  void set_output (char *filename)
    {
      char temp[BUFSIZ];
      ACE_OS::sprintf (temp, "%s%s%s", 
		       ACE_LOG_DIRECTORY, 
		       ACE::basename (filename, ACE_DIRECTORY_SEPARATOR_CHAR),
		       ".log");
      this->output_file_ = new ofstream (temp);
      
      ACE_Log_Msg::instance()->msg_ostream (this->output_file_);
      ACE_Log_Msg::instance()->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER );
      ACE_Log_Msg::instance()->set_flags (ACE_Log_Msg::OSTREAM);
    }

  ofstream *output_file (void)
    {
      return this->output_file_;
    }
  
  void flush (void)
    {
      this->output_file_->flush ();
    }

private:
  ofstream *output_file_;
};

static ACE_Test_Output ace_file_stream;

#define ACE_START_TEST \
  ace_file_stream.set_output (argv[0]); \
  ACE_DEBUG ((LM_DEBUG, "starting %s test at %T\n", argv[0]));

#define ACE_END_TEST \
  ACE_DEBUG ((LM_DEBUG, "Ending %s test at %T\n", argv[0])); \
  ace_file_stream.flush ();

#define ACE_NEW_THREAD \
  ACE_Log_Msg::instance()->msg_ostream (ace_file_stream.output_file ()); \
  ACE_Log_Msg::instance()->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER ); \
  ACE_Log_Msg::instance()->set_flags (ACE_Log_Msg::OSTREAM);

#endif /* ACE_TEST_CONFIG_H */
