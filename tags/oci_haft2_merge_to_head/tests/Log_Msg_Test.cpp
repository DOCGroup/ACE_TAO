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
#include "ace/Log_Record.h"
#include "test_config.h"

ACE_RCSID(tests, Log_Msg_Test, "$Id$")

static void
cleanup (void)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("cleanup hook (%P)!\n")));
}

static void
cause_error (void)
{
  errno = EWOULDBLOCK;
  ACE_ERROR ((LM_DEBUG,
              ACE_TEXT ("would block\n")));
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
                    ACE_TEXT ("Logger::log->%s\n"),
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
                        ACE_TEXT ("Logger::log->%s\n"),
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
              ACE_TEXT ("(%t) first message\n")));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);

  // This message should not show up anywhere.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) second message\n")));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);

  // This message should not show up anywhere since no callback object
  // has been specified.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) third message\n")));

  // Create a callback object and make it "verbose".
  Logger logger;
  logger.verbose (1);

  // Set the callback object.
  ACE_LOG_MSG->msg_callback (&logger);

  // This message should show up via the Logger callback.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) forth message\n")));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);

  // This message should show up via the Logger callback (somewhat
  // verbosely).
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) fifth message\n")));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE);

  // This message should show up via the Logger callback (really
  // verbosely).
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) sixth message\n")));

  logger.verbose (0);

  // This message should show up via the Logger callback (not
  // verbosely).
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) seventh message\n")));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  // This message should show up in stderr and the Logger callback.
  // The one from the Logger callback will not be verbose, but the one
  // from stderr should be verbose.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) eighth message\n")));
  ACE_LOG_MSG->msg_callback (0);
}

static void
test_log_msg_features (const ACE_TCHAR *program)
{
  // Note that the default behavior is to log to STDERR...

  int counter = 1 ;

  if (ACE_LOG_MSG->open (program) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("cannot open logger!!!\n")));

  cause_error ();

  // Check to see what happened.
  if (ACE_LOG_MSG->op_status () == -1
      && ACE_LOG_MSG->errnum () == EWOULDBLOCK)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("op_status and errnum work!\n")));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("op_status and errnum failed!\n")));

  const char *badname = "badname";

  // We use the DEBUG messages instead of error messages. This is to
  // help the scripts. If we print out error messages the scripts
  // start catching them as errors.
  if (ACE_OS::open (badname,
                    O_RDONLY) == ACE_INVALID_HANDLE)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%n: (%x), can't open %s%r\n"),
                10000,
                badname,
                cleanup));

  // Try a log operation that would overflow the logging buffer if not
  // properly guarded.
  ACE_TCHAR big[ACE_Log_Record::MAXLOGMSGLEN + 1];
  size_t i = 0;
  const ACE_TCHAR alphabet[] = ACE_TEXT ("abcdefghijklmnopqrstuvwxyz");
  size_t j = ACE_OS::strlen (alphabet);
  while (i < ACE_Log_Record::MAXLOGMSGLEN)
    big[i++] = alphabet[i % j];
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("This is too big: %s\n"), big));

  // Exercise many different combinations of OSTREAM.

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              3.1416 * counter++,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("hello"),
              10000));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
  ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              3.1416 * counter,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("world"),
              10000 * counter++));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);

  // The next two messages shouldn't print.
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              3.1416 * counter,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("world"),
              10000 * counter++));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              3.1416 * counter,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("world"),
              10000 * counter++));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              3.1416 * counter,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("world"),
              10000 * counter++));

  static int array[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

  // Print out the binary bytes of the array in hex form.
  ACE_LOG_MSG->log_hexdump (LM_DEBUG,
                            (char *) array,
                            sizeof array);

  // Disable the LM_DEBUG and LM_INFO messages.
  u_long priority_mask =
    ACE_LOG_MSG->priority_mask (ACE_Log_Msg::PROCESS);
  ACE_CLR_BITS (priority_mask,
                LM_DEBUG | LM_INFO);
  ACE_LOG_MSG->priority_mask (priority_mask,
                              ACE_Log_Msg::PROCESS);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("This LM_INFO message should not print!\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This LM_DEBUG message should not print!\n")));

  ACE_SET_BITS (priority_mask,
                LM_INFO);
  ACE_LOG_MSG->priority_mask (priority_mask,
                              ACE_Log_Msg::PROCESS);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("This LM_INFO message should print!\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This LM_DEBUG message should not print!\n")));

  ACE_CLR_BITS (priority_mask, LM_INFO);
  ACE_LOG_MSG->priority_mask (priority_mask,
                              ACE_Log_Msg::PROCESS);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("This LM_INFO message should not print!\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This LM_DEBUG message should not print!\n")));
}

static int
test_ostream (void)
{
  // This message should show up in the log file.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("first message\n")));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);

  // This message should not show up anywhere.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("second message\n")));

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
              ACE_TEXT ("fourth message\n")));
  // Set the ostream back to the test's log file.
  ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());
  // Now close the ostream file and check its contents.
  myostream.close ();

  ACE_FILE_Connector connector;
  ACE_FILE_IO file;

  // Open up the file.
  if (connector.connect (file,
                         ACE_FILE_Addr (filename)) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("connect failed for %p\n"),
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
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("get_info failed on %p\n"),
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
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Read %d bytes, rather than expected %d bytes\n"),
                         size,
                         info.size_),
                        -1);
    }
  // Make sure to NUL-terminate this turkey!
  buffer[size] = '\0';


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%s"),
              buffer));

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

  // This message should show up in stderr and the ostream (without
  // ACE_LACKS_IOSTREAM_TOTALLY).
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("fifth message\n")));

  return 0;
}


// For testing the format specifiers, a class is defined as a callback
// mechanism. It will get the formatted messages and check them for
// correctness. The test_format_specs() function will set the first
// few characters to say which test is being run, and the Log_Spec_Verify
// class will use that to decide how to verify the results.

class Log_Spec_Verify : public ACE_Log_Msg_Callback
{
public:
  Log_Spec_Verify () : fail_ (0) {};

  void log (ACE_Log_Record &log_record);
  // Logging callback

  int  result ();

private:
  int fail_;
  // Count how many tests failed.
};

void
Log_Spec_Verify::log (ACE_Log_Record &log_record)
{
  const ACE_TCHAR *b = log_record.msg_data ();
  const ACE_TCHAR *expect = 0;

  if (ACE_OS::strncmp (b, ACE_TEXT ("l1:"), 3) == 0)
    {
      expect = ACE_TEXT ("42");
      b += 4;  //3
    }
  else if (ACE_OS::strncmp (b, ACE_TEXT ("l2:"), 3) == 0)
    {
      expect = ACE_TEXT ("   42");
      b += 3;
    }
  else if (ACE_OS::strncmp (b, ACE_TEXT ("l3N1:"), 4) == 0)
    {
      expect = ACE_TEXT ("0042,Log_Msg");
      b += 4;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Log_Spec_Verify, unrecognized test: %s\n"),
                  b));
      this->fail_++;
    }

  if (b != log_record.msg_data () && ACE_OS::strcmp (b, expect) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Test %s failed; expected %s\n"),
                  log_record.msg_data (), expect));
      this->fail_++;
    }

  return;
}

int
Log_Spec_Verify::result (void)
{
  if (this->fail_ == 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("All logging specifier tests passed.\n")));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%d logging specifier tests failed!\n"),
                this->fail_));
  return this->fail_;
}

static int
test_format_specs (void)
{
#if 0
  Log_Spec_Verify  verifier;
  ACE_Log_Msg      logger;

  if (logger.open (ACE_TEXT ("Log_Msg_Test"), ACE_Log_Msg::MSG_CALLBACK) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("%T: test_format_specs open")),
                      1);
  logger.msg_callback (&verifier);

  logger.linenum (42);
  logger.file (ACE_TEXT ("Log_Msg_Test.cpp"));
  logger.log (LM_DEBUG, ACE_TEXT ("l1:%l"));
  logger.log (LM_DEBUG, ACE_TEXT ("l2:%5l"));
  logger.log (LM_DEBUG, ACE_TEXT ("l3N1:%0*l,%.7N"), 4);
  return verifier.result ();
#else
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("l1:%l\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("l2:%5l\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("l3N1:%0*l,%.7N\n"), 4));
  return 0;
#endif

}

// Main function.

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Log_Msg_Test"));

  int status = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("**** running ostream test\n")));

  // Test the <ACE_Log_Msg> abstraction wrt writing to stderr and to a
  // file.
  test_ostream ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%M **** running callback test\n")));

  // Test the <ACE_Log_Msg> callback mechanism.
  test_callbacks ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("**** running features test\n")));

  // Test various features of the <ACE_Log_Msg>.
  test_log_msg_features ((argc > 0 ? argv[0] : ACE_TEXT ("program")));

  // Test the format specifiers

  // Restore this mask so diags and the shutdown message will print correctly!
  ACE_LOG_MSG->priority_mask (ACE_LOG_MSG->priority_mask () | LM_DEBUG,
                              ACE_Log_Msg::PROCESS);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("**** running format specifiers test\n")));
  if (test_format_specs () != 0)
    status = 1;

  ACE_END_TEST;
  return status;
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
