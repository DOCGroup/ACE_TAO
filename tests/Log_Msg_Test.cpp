// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Log_Msg_Test.cpp
//
// = DESCRIPTION
//     This program tests the <ACE_Log_Msg> class in various ways and
//     also illustrates many of the features of the <ACE_Log_Msg> For
//     instance, this program tests the <ACE_Log_Msg> abstraction wrt
//     writing to stderr and to a file.  It also tests writing to user
//     defined callback objects.
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "ace/streams.h"
#include "ace/FILE_Connector.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg_Callback.h"
#include "test_config.h"

ACE_RCSID(tests, Log_Msg_Test, "$Id$")

static void
cleanup (void)
{
  ACE_DEBUG ((LM_INFO,
              "cleanup hook (%P)!\n"));
}

static void
cause_error (void)
{
  errno = EWOULDBLOCK;
  ACE_ERROR ((LM_DEBUG,
              "would block\n"));
}

class Logger : public ACE_Log_Msg_Callback
{
public:
  Logger (int be_recursive = 1);
  // Constructor sets whether we're testing "recursive" callback
  // logging!

  void log (ACE_Log_Record &log_record);
  // Logging callback

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
                    "Logger::log->%s\n",
                    log_record.msg_data ()));
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
      else
        *ace_file_stream::instance ()->output_file ()
          << "Recursive Logger callback = "
          << log_record.msg_data ()
          << endl;
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
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
                        "Logger::log->%s\n",
                        verbose_msg));
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
          else
            *ace_file_stream::instance ()->output_file ()
              << "Recursive Logger callback = "
              << log_record.msg_data ()
              << endl;
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
        }
    }

  // Cleanup on the way out.
  if (use_log_msg)
    this->recursive_ = 1;
}

static void
test_callbacks (void)
{
  // This message should show up in stderr.
  ACE_DEBUG ((LM_DEBUG,
              "(%t) first message\n"));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);

  // This message should not show up anywhere.
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
              "(%t) forth message\n"));

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

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  // This message should show up in stderr and the Logger callback.
  // The one from the Logger callback will not be verbose, but the one
  // from stderr should be verbose.
  ACE_DEBUG ((LM_DEBUG,
              "(%t) eighth message\n"));
  ACE_LOG_MSG->msg_callback (0);
}

static void
test_log_msg_features (const ACE_TCHAR *program)
{
  // Note that the default behavior is to log to STDERR...

  int counter = 1 ;

  if (ACE_LOG_MSG->open (program) == -1)
    ACE_ERROR ((LM_ERROR,
                "cannot open logger!!!\n"));

  cause_error ();

  // Check to see what happened.
  if (ACE_LOG_MSG->op_status () == -1
      && ACE_LOG_MSG->errnum () == EWOULDBLOCK)
    ACE_DEBUG ((LM_DEBUG,
                "op_status and errnum work!\n"));
  else
    ACE_ERROR ((LM_ERROR,
                "op_status and errnum failed!\n"));

  const char *badname = "badname";

  if (ACE_OS::open (badname,
                    O_RDONLY) == ACE_INVALID_HANDLE)
    ACE_ERROR ((LM_ERROR,
                "%n: (%x), can't open %s%r\n",
                10000,
                badname,
                cleanup));

  // Exercise many different combinations of OSTREAM.

  ACE_DEBUG ((LM_INFO,
              "%10f, %*s%s = %d\n",
              3.1416 * counter++,
              8,
              "",
              "hello",
              10000));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
  ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());

  ACE_DEBUG ((LM_INFO,
              "%10f, %*s%s = %d\n",
              3.1416 * counter,
              8,
              "",
              "world",
              10000 * counter++));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);

  // The next two messages shouldn't print.
  ACE_DEBUG ((LM_INFO,
              "%10f, %*s%s = %d\n",
              3.1416 * counter,
              8,
              "",
              "world",
              10000 * counter++));

  ACE_DEBUG ((LM_INFO,
              "%10f, %*s%s = %d\n",
              3.1416 * counter,
              8,
              "",
              "world",
              10000 * counter++));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
  ACE_DEBUG ((LM_INFO,
              "%10f, %*s%s = %d\n",
              3.1416 * counter,
              8,
              "",
              "world",
              10000 * counter++));

  static int array[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

  // Print out the binary bytes of the array in hex form.
  ACE_LOG_MSG->log_hexdump (LM_DEBUG,
                            (char *) array,
                            sizeof array);

  // Disable the LM_DEBUG and LM_INFO messages.
  u_long priority_mask = ACE_LOG_MSG->priority_mask ();
  ACE_CLR_BITS (priority_mask,
                LM_DEBUG | LM_INFO);
  ACE_LOG_MSG->priority_mask (priority_mask);

  ACE_DEBUG ((LM_INFO,
              "This LM_INFO message should not print!\n"));
  ACE_DEBUG ((LM_DEBUG,
              "This LM_DEBUG message should not print!\n"));

  ACE_SET_BITS (priority_mask,
                LM_INFO);
  ACE_LOG_MSG->priority_mask (priority_mask,
                              ACE_Log_Msg::PROCESS);

  ACE_DEBUG ((LM_INFO,
              "This LM_INFO message should print!\n"));
  ACE_DEBUG ((LM_DEBUG,
              "This LM_DEBUG message should not print!\n"));

  ACE_CLR_BITS (priority_mask, LM_INFO);
  ACE_LOG_MSG->priority_mask (priority_mask,
                              ACE_Log_Msg::PROCESS);

  ACE_DEBUG ((LM_INFO,
              "This LM_INFO message should not print!\n"));
  ACE_DEBUG ((LM_DEBUG,
              "This LM_DEBUG message should not print!\n"));
}

static int
test_ostream (void)
{
  // This message should show up in the log file.
  ACE_DEBUG ((LM_DEBUG,
              "first message\n"));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);

  // This message should not show up anywhere.
  ACE_DEBUG ((LM_DEBUG,
              "second message\n"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  // Create a persistent store.
  const ACE_TCHAR *filename = ACE_TEXT ("output");
  ofstream myostream (ACE_TEXT_ALWAYS_CHAR (filename), ios::out | ios::trunc);

  // Check for errors.
  if (myostream.bad ())
    return -1;

  // Set the ostream.
  ACE_LOG_MSG->msg_ostream (&myostream);

  // This message should show up in the ostream.
  ACE_DEBUG ((LM_DEBUG,
              "fourth message\n"));

  ACE_FILE_Connector connector;
  ACE_FILE_IO file;

  // Open up the file.
  if (connector.connect (file,
                         ACE_FILE_Addr (filename)) == -1)
  {
	  // Set the ostream back to NULL to prevent "later functions using myostream".
	  ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());
      ACE_ERROR_RETURN ((LM_ERROR,
                       "connect failed for %p\n",
                       filename),
                      1);
  }

  // Unlink this file right away so that it is automatically removed
  // when the process exits.Ignore error returns in case this operation
  // is not supported.
  ACE_OS::unlink(filename);
 
  ACE_FILE_Info info;
  if (file.get_info (info) == -1)
  {
	  // Set the ostream back to NULL to prevent "later functions using myostream".
	  ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());
	  ACE_ERROR_RETURN ((LM_ERROR,
                       "get_info failed on %p\n",
                       filename),
                      -1);
  }

  // Allocate the input buffer
  char *buffer;
  ACE_NEW_RETURN (buffer,
                  char[info.size_ + 1],
                  -1);
  // Make sure <buffer> is released automagically.
  ACE_Auto_Basic_Array_Ptr<char> b (buffer);

  // Read the file into the buffer.
  ssize_t size = file.recv (buffer,
                            info.size_);
  if (size != info.size_)
  {
	// Set the ostream back to NULL to prevent "later functions using myostream".
	ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Read %d bytes, rather than expected %d bytes\n",
                       size,
                       info.size_),
                      -1);
  }
  // Make sure to NUL-terminate this turkey!
  buffer[size] = '\0';

  // Set the ostream back to NULL to prevent "confusion".
  ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());

  ACE_DEBUG ((LM_DEBUG,
              "%s",
              buffer));

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

  // This message should show up in stderr and the ostream (without
  // ACE_LACKS_IOSTREAM_TOTALLY).
  ACE_DEBUG ((LM_DEBUG,
              "fifth message\n"));

  return 0;
}

// Main function.

int
main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Log_Msg_Test"));
  ACE_DEBUG ((LM_DEBUG,
              "**** running ostream test\n"));

  // Test the <ACE_Log_Msg> abstraction wrt writing to stderr and to a
  // file.
  test_ostream ();

  ACE_DEBUG ((LM_DEBUG,
              "**** running callback test\n"));

  // Test the <ACE_Log_Msg> callback mechanism.
  test_callbacks ();

  ACE_DEBUG ((LM_DEBUG,
              "**** running features test\n"));

  // Test various features of the <ACE_Log_Msg>.
  test_log_msg_features (argv[0]);

  // Restore this mask so the shutdown message will print correctly!
  ACE_LOG_MSG->priority_mask (ACE_LOG_MSG->priority_mask () | LM_DEBUG,
                              ACE_Log_Msg::PROCESS);
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// Not necessary here, because it's instantiated in
// ace/Memory_Pool.cpp.
// template class ACE_Auto_Basic_Array_Ptr<ACE_CDR::Char>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// Not necessary here, because it's instantiated in
// ace/Memory_Pool.cpp.
// #pragma instantiate ACE_Auto_Basic_Array_Ptr<ACE_CDR::Char>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
