
//=============================================================================
/**
 *  @file    Log_Msg_Test.cpp
 *
 *  $Id$
 *
 *   This program tests the <ACE_Log_Msg> class in various ways and
 *   also illustrates many of the features of the <ACE_Log_Msg> For
 *   instance, this program tests the <ACE_Log_Msg> abstraction wrt
 *   writing to stderr and to a file.  It also tests writing to user
 *   defined callback objects.
 *
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/FILE_Connector.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"
#include "ace/Time_Value.h"
#include "ace/Thread.h"

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
  /// Constructor sets whether we're testing "recursive" callback
  /// logging!
  Logger (bool be_recursive = true);

  /// Logging callback
  void log (ACE_Log_Record &log_record);

  void verbose (bool be_verbose);

private:
  /// Flag for testing verbose logging.
  bool verbose_logging_;

  /// Flag for testing recursive callback logging.
  bool recursive_;
};

void
Logger::verbose (bool be_verbose)
{
  this->verbose_logging_ = be_verbose;
}

Logger::Logger (bool be_recursive)
  : recursive_ (be_recursive)
{
}

void
Logger::log (ACE_Log_Record &log_record)
{
  bool use_log_msg = false;
  if (this->recursive_)
    {
      this->recursive_ = false;
      use_log_msg = true;
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
    this->recursive_ = true;
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
                ACE_TEXT ("%n: (%x), can't open %C%r\n"),
                10000,
                badname,
                cleanup));

  // Try a log operation that would overflow the logging buffer if not
  // properly guarded.
  ACE_TCHAR big[ACE_Log_Record::MAXLOGMSGLEN + 1];
  size_t i = 0;
  static const ACE_TCHAR alphabet[] = ACE_TEXT ("abcdefghijklmnopqrstuvwxyz");
  size_t j = ACE_OS::strlen (alphabet);
  while (i < ACE_Log_Record::MAXLOGMSGLEN)
    {
      size_t const index = i++;
      big[index] = alphabet[i % j];
    }
  big[ACE_Log_Record::MAXLOGMSGLEN] = ACE_TEXT ('\0');
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("This is too big: %s\n"), big));

  ACE_HEX_DUMP((LM_INFO, (const char*)big, ACE_Log_Record::MAXLOGMSGLEN ));

  // Exercise many different combinations of OSTREAM.

  double f = 3.1416 * counter++;
  int n = 10000;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              f,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("hello"),
              n));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
  ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());

  f = 3.1416 * counter;
  n = 10000 * counter++;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              f,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("world"),
              n));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);

  // The next two messages shouldn't print.

  f = 3.1416 * counter;
  n = 10000 * counter++;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              f,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("world"),
              n));

  f = 3.1416 * counter;
  n = 10000 * counter++;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              f,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("world"),
              n));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  f = 3.1416 * counter;
  n = 10000 * counter++;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%10f, %*s%s = %d\n"),
              f,
              8,
              ACE_TEXT (""),
              ACE_TEXT ("world"),
              n));

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

// For testing how many log records has been output
class Log_Count : public ACE_Log_Msg_Callback
{
  int count_;
public:
  /// logging!
  Log_Count () : count_(0)
  {
  }

  /// Logging callback
  void log (ACE_Log_Record &)
  {
    ++count_;
  }

  int count() const
  {
    return count_;
  }
};

static int
test_acelib_category()
{
  int failed = 0;

  Log_Count  counter;

  ACE_LOG_MSG->msg_callback (&counter);

  // Disable the LM_DEBUG and LM_INFO messages.
  u_long priority_mask =
    ACE_LOG_MSG->priority_mask (ACE_Log_Msg::PROCESS);
  ACE_CLR_BITS (priority_mask,
                LM_DEBUG | LM_INFO);
  ACE_Log_Category::ace_lib().priority_mask (priority_mask);

  ACELIB_DEBUG ((LM_INFO,
              ACE_TEXT ("This LM_INFO message should not print!\n")));
  ACELIB_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This LM_DEBUG message should not print!\n")));

  if (counter.count() != 0)
  {
    ++failed;
  }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("This LM_INFO message should print!\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This LM_DEBUG message should print!\n")));

  if (counter.count() != 2)
  {
    ++failed;
  }

  ACE_SET_BITS (priority_mask,
                LM_INFO);
  ACE_Log_Category::ace_lib().priority_mask (priority_mask);

  ACELIB_DEBUG ((LM_INFO,
              ACE_TEXT ("This LM_INFO message should print!\n")));

  if (counter.count() != 3)
  {
    ++failed;
  }

  ACELIB_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This LM_DEBUG message should not print!\n")));

  if (counter.count() != 3)
  {
    ++failed;
  }

  ACE_CLR_BITS (priority_mask, LM_INFO);
  ACE_Log_Category::ace_lib().priority_mask (priority_mask);

  ACELIB_DEBUG ((LM_INFO,
              ACE_TEXT ("This LM_INFO message should not print!\n")));
  ACELIB_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This LM_DEBUG message should not print!\n")));

  if (counter.count() != 3)
  {
    ++failed;
  }


  if (failed == 0) {
    ACE_DEBUG((LM_DEBUG, "All ace lib category log passed\n"));
  }
  else {
    ACE_ERROR((LM_ERROR, "Some ace lib category log failed\n"));
  }
  ACE_LOG_MSG->msg_callback (0);
  return failed;
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

  OFSTREAM *old_stream = ace_file_stream::instance ()->output_file ();
  // Set the ostream.
  ACE_LOG_MSG->msg_ostream (&myostream);

  // This message should show up in the ostream.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("fourth message\n")));
  // Set the ostream back to the test's log file.
  ACE_LOG_MSG->msg_ostream (old_stream);
  // Now close the ostream file and check its contents.
  myostream.close ();

  ACE_FILE_Connector connector;
  ACE_FILE_IO file;
  ACE_FILE_Addr file_addr (filename);

  // Open up the file.
  if (connector.connect (file, file_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("connect failed for %p\n"),
                         filename),
                        1);
    }

#if !defined (ACE_VXWORKS) && !defined (ACE_HAS_PHARLAP) || (defined(ACE_VXWORKS) && (ACE_VXWORKS > 0x680))
# define TEST_CAN_UNLINK_IN_ADVANCE
#endif

#if defined (TEST_CAN_UNLINK_IN_ADVANCE)
  // Unlink this file right away so that it is automatically removed
  // when the process exits.Ignore error returns in case this operation
  // is not supported.
  ACE_OS::unlink(filename);
#endif

  ACE_FILE_Info info;
  if (file.get_info (info) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("get_info failed on %p\n"),
                         filename),
                        -1);
    }

  // Allocate the input buffer
  char *buffer = 0;
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
              ACE_TEXT ("%C"),
              buffer));

#if !defined (TEST_CAN_UNLINK_IN_ADVANCE)
  file.close ();
  if (file.unlink () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("unlink failed for %p\n"),
                       file_addr.get_path_name ()),
                       1);
#endif

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
  Log_Spec_Verify (bool be_recursive = true) : fail_ (0), tests_ (0), recursive_ (be_recursive) {};

  /// Logging callback
  void log (ACE_Log_Record &log_record);

  int  result ();

private:
  /// Count how many tests failed.
  int fail_;

  /// Count how many tests we run
  int tests_;

  bool recursive_;
};

void
Log_Spec_Verify::log (ACE_Log_Record &log_record)
{
  bool use_log_msg = false;
  if (this->recursive_)
    {
      this->recursive_ = false;
      use_log_msg = true;
    }

  if (!use_log_msg)
    {
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
      *ace_file_stream::instance ()->output_file ()
        << "Logger callback = "
        << log_record.msg_data ()
        << endl;
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
    }
  else
    {
      const ACE_TCHAR *b = log_record.msg_data ();
      const ACE_TCHAR *expect = 0;

      ++this->tests_;

      if (ACE_OS::strncmp (b, ACE_TEXT ("l1:"), 3) == 0)
        {
          expect = ACE_TEXT ("42");
          b += 3;
        }
      else if (ACE_OS::strncmp (b, ACE_TEXT ("l2:"), 3) == 0)
        {
          expect = ACE_TEXT ("   42");
          b += 3;
        }
      else if (ACE_OS::strncmp (b, ACE_TEXT ("l3N1:"), 4) == 0)
        {
          expect = ACE_TEXT ("0042,Log_Msg");
          b += 5;
        }
      else if (ACE_OS::strncmp (b, ACE_TEXT ("l4:"), 3) == 0)
        {
          b += 3;
          // Check if we have a string, exact length could vary
          if (b != log_record.msg_data () && ACE_OS::strlen (b) < 15)
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("Test %s failed; expected %d\n"),
                          log_record.msg_data (), ACE_OS::strlen (b)));
              ++this->fail_;
            }
        }
        else if (ACE_OS::strncmp (b, ACE_TEXT ("l5:"), 3) == 0)
        {
          b += 3;
          switch (log_record.type())
          {
            case (LM_SHUTDOWN): expect = ACE_TEXT("S"); break;
            case (LM_TRACE): expect = ACE_TEXT("T"); break;
            case (LM_DEBUG): expect = ACE_TEXT("D"); break;
            case (LM_INFO): expect = ACE_TEXT("I"); break;
            case (LM_NOTICE): expect = ACE_TEXT("N"); break;
            case (LM_WARNING): expect = ACE_TEXT("W"); break;
            case (LM_STARTUP): expect = ACE_TEXT("U"); break;
            case (LM_ERROR): expect = ACE_TEXT("E"); break;
            case (LM_CRITICAL): expect = ACE_TEXT("C"); break;
            case (LM_ALERT): expect = ACE_TEXT("A"); break;
            case (LM_EMERGENCY): expect = ACE_TEXT("!"); break;
            default: expect = ACE_TEXT("?"); break;
          }

        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Log_Spec_Verify, unrecognized test: %s\n"),
                      b));
          ++this->fail_;
        }

      if (b != log_record.msg_data () && expect && ACE_OS::strcmp (b, expect) != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Test %s failed; expected %s\n"),
                      log_record.msg_data (), expect));
          ++this->fail_;
        }
    }

  // Cleanup on the way out.
  if (use_log_msg)
    this->recursive_ = true;
}

int
Log_Spec_Verify::result (void)
{
  if (this->fail_ == 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("All logging specifier tests passed.\n")));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%d logging specifier tests failed!\n"),
                this->fail_));

  if (this->tests_ != 15)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected number of tests run is %d, not 15!\n"),
                this->tests_));
    ++this->fail_;
  }
  return this->fail_;
}

static int
test_format_specs (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("l1:%l\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("l2:%5l\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("l3N1:%0*l,%.7N\n"), 4));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%*ISTART INDENTING %{\n"), 4));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IONE%{\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%ITWO%{\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%ITHREE\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%}%ITWO\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%}%IONE\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%}%IENDINDENTING\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%W\n"), ACE_TEXT_WIDE ("My string test\n")));
  ACE_TCHAR* nill_string = 0;
  char* char_nill_string = 0;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%W\n"), nill_string));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s\n"), nill_string));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%C\n"), char_nill_string));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%m %p\n"), nill_string));
  errno = ENOENT;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%m %p\n"), ACE_TEXT("perror")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%S\n"), SIGINT));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%S\n"), ACE_NSIG));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D\n")));
  ACE_Time_Value tv = ACE_OS::gettimeofday ();
  tv += ACE_Time_Value (25*60*60); // + 25 hours
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%#D\n"), &tv));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%T\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%#T\n"), &tv));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("thread id %t\n")));

  Log_Spec_Verify  verifier;

  ACE_LOG_MSG->msg_callback (&verifier);
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE);
  ACE_LOG_MSG->linenum (42);
  ACE_LOG_MSG->file ("Log_Msg_Test.cpp");

  ACE_LOG_MSG->log (LM_DEBUG, ACE_TEXT ("l1:%l"));
  ACE_LOG_MSG->log (LM_DEBUG, ACE_TEXT ("l2:%5l"));
  ACE_LOG_MSG->log (LM_DEBUG, ACE_TEXT ("l3N1:%0*l,%.7N"), 4);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("l4:%T")));

  ACE_LOG_MSG->priority_mask (LM_SHUTDOWN |
                              LM_TRACE |
                              LM_DEBUG |
                              LM_INFO |
                              LM_NOTICE |
                              LM_WARNING |
                              LM_STARTUP |
                              LM_ERROR |
                              LM_CRITICAL |
                              LM_ALERT |
                              LM_EMERGENCY,
                              ACE_Log_Msg::PROCESS);
  ACE_DEBUG ((LM_SHUTDOWN, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_TRACE, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_NOTICE, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_WARNING, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_STARTUP, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_CRITICAL, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_ALERT, ACE_TEXT ("l5:%.1M")));
  ACE_DEBUG ((LM_EMERGENCY, ACE_TEXT ("l5:%.1M")));

  ACE_LOG_MSG->msg_ostream (ace_file_stream::instance ()->output_file ());
  ACE_LOG_MSG->msg_callback (0);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);

  return verifier.result ();
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
  ACE_LOG_MSG->priority_mask (ACE_LOG_MSG->priority_mask () | LM_DEBUG | LM_ERROR,
                              ACE_Log_Msg::PROCESS);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("**** running format specifiers test\n")));

  if (status += test_format_specs ())
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("logging specifier tests failed!\n")));
      status = 1;
    }

  status += test_acelib_category();

  ACE_END_TEST;
  return status;
}

