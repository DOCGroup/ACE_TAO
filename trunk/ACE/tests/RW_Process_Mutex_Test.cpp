// $Id$
// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    RW_Process_Mutex_Test.cpp
//
// = DESCRIPTION
//    Tests an <ACE_RW_Process_Mutex> shared between multiple child processes.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Process.h"
#include "ace/RW_Process_Mutex.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"



// The parent process is number -1. Writer is 0; Readers are 1-3.
static int child_nr = -1;
static u_short reporting_port = 0;
static const int Nr_Processes = 4;
static ACE_TString mutex_name;
static ACE_TCHAR mutex_check[MAXPATHLEN+1];

// The child processes spawned will report times that they hold the lock.
// The Child class gets records of the timestamps when the lock is acquired
// and released. When the children are done, the time ranges are checked to
// be sure that the writer and readers aren't overlapping and that multiple
// readers can acquire the lock simultaneously.
class Time_Range
{
public:
  Time_Range () : start_ (0), stop_ (0) {}

  void set (const Time_Range &range);
  void set (const ACE_Time_Value &start, const ACE_Time_Value &stop);
  bool overlaps (const Time_Range &other) const;

private:
  ACE_Time_Value start_;
  ACE_Time_Value stop_;
};

// Children send range reports to the waiting parent using Range_Report.
struct Range_Report
{
  int child_;
  Time_Range range_;
};

void
Time_Range::set (const Time_Range &range)
{
  this->start_ = range.start_;
  this->stop_ = range.stop_;
}

void
Time_Range::set (const ACE_Time_Value &start, const ACE_Time_Value &stop)
{
  this->start_ = start;
  this->stop_ = stop;
}

bool
Time_Range::overlaps (const Time_Range &other) const
{
  // Be careful because timestamps can appear to be the same when a
  // process unlocks and a waiter immediately locks.
  if ((this->start_ >= other.start_ && this->start_ < other.stop_) ||
      (this->stop_ > other.start_ && this->stop_ < other.stop_))
    return true;
  return false;
}

class Child : public ACE_Process
{
public:
  Child () : range_count_ (0) {}
  void add_range (const Time_Range &range);
  bool any_overlaps (const Child &other) const;

private:
  enum { Max_Ranges = 5 };
  int range_count_;
  Time_Range ranges_[Max_Ranges];
};

void
Child::add_range (const Time_Range &range)
{
  if (this->range_count_ == Max_Ranges)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Child process %d adds too many ranges\n"),
                  (int)(this->getpid ())));
      return;
    }
  this->ranges_[this->range_count_].set (range);
  ++this->range_count_;
}

bool
Child::any_overlaps (const Child &other) const
{
  bool overlap = false;
  for (int i = 0; i < this->range_count_ && !overlap; ++i)
    {
      for (int j = 0; j < other.range_count_ && !overlap; ++j)
        {
          if (this->ranges_[i].overlaps (other.ranges_[j]))
            overlap = true;
        }
    }
  return overlap;
}


// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("usage: %n [-c n (child number) -p n (port number)] [-n mutex name]\n")));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.
static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("c:n:p:"));

  mutex_name.set (ACE_TEXT ("RW_Process_Mutex_Test.lock"));    // Default name
  int c;
  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'c':
      child_nr = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'n':
      mutex_name.set (get_opt.opt_arg ());
      break;
    case 'p':
      reporting_port = (u_short)ACE_OS::atoi (get_opt.opt_arg ());
      break;
    default:
      print_usage_and_die ();
      break;
  }

  // Now that the mutex name is known, set up the checker file name.
  ACE_OS::strncpy (mutex_check, mutex_name.c_str (), MAXPATHLEN);
  ACE_OS::strncat (mutex_check, ACE_TEXT ("_checker"), MAXPATHLEN);
}

/*
 * The set of readers and the writer will operate in a staggered sequence
 * of acquiring and releasing the lock. The sequence is designed to exercise
 * waiting behavior of both readers and writer, as well as allowing multiple
 * readers in, without getting tripped up by any differences in ordering
 * on different platforms which may favor writers, or vice-versa.
 * In this timeline, time on seconds is on the left, time holding the lock
 * is solid, time waiting is dots, acquire/release point is a dash, and
 * time without the lock is blank.
 *
 *   TIME        WRITER    READER1    READER2    READER3
 *     0            |
 *                  |
 *     1            |         .
 *                  |         .
 *     2            -         -          -
 *                            |          |
 *     3                      |          |          -
 *                            |          |          |
 *     4                      -          |          |
 *                                       |          |
 *     5                                 -          |
 *                                                  |
 *     6            -                               -
 *                  |
 *     7            |         .          .          .
 *                  |         .          .          .
 *     8            -         -          -          -
 *                            |          |          |
 *     9                      |          |          |
 *
 * A file is used to test the sequencing. When the writer first gets the
 * lock, it will ensure the file is not present. At the end of its time
 * holding the lock the first time, it will write a "writer 1" string to
 * the file. When it gets the lock the second time, it will write a
 * different string to the file, and just before releasing the second time
 * write a "writer 2" string to the file. The readers all check to be sure
 * that the file is present and says "writer 1" at the start and end of
 * their periods of holding the reader lock and, similarly, check for
 * "writer 2" the second time they hold the lock.
 */
static void
reader (int num)
{
  // Let the writer get there first.
  ACE_OS::sleep (1);

  ACE_SOCK_Dgram sock;
  ACE_INET_Addr parent;
  parent.set (reporting_port, ACE_LOCALHOST, 1, AF_INET);
  ACE_TCHAR me_str[80];
  parent.addr_to_string (me_str, 80);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sending reports to %s\n"), me_str));

  if (sock.open (ACE_Addr::sap_any, PF_INET) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("UDP open")));

  Range_Report report;
  report.child_ = num;
  ACE_Time_Value start (ACE_Time_Value::zero), stop (ACE_Time_Value::zero);

  ACE_RW_Process_Mutex mutex (mutex_name.c_str ());

  // Make sure the constructor succeeded
  if (ACE_LOG_MSG->op_status () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Reader %d, mutex %s %p\n"),
                  num,
                  mutex_name.c_str (),
                  ACE_TEXT ("ctor")));
      return;
    }

  ACE_OS::sleep (num);
  // Grab the lock
  if (-1 == mutex.acquire_read ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Reader %d %p\n"),
                num,
                ACE_TEXT ("first acquire_read")));
  else
    {
      start = ACE_OS::gettimeofday ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Reader %d acquired first time\n"),
                  num));
    }

  // Wait a bit, then release and report the range held.
  ACE_OS::sleep (num);

  // Release the lock then wait; in the interim, the writer should change
  // the file.
  stop = ACE_OS::gettimeofday ();
  if (-1 == mutex.release ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Reader %d %p\n"),
                num,
                ACE_TEXT ("first release")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Reader %d released first time\n"), num));
  report.range_.set (start, stop);
  ssize_t bytes = sock.send (&report, sizeof (report), parent);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reader %d sent %b byte report\n"),
              num,
              bytes));

  ACE_OS::sleep (4 - num);
  start = stop = ACE_Time_Value::zero;
  if (-1 == mutex.acquire_read ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Reader %d %p\n"),
                num,
                ACE_TEXT ("second acquire_read")));
  else
    {
      start = ACE_OS::gettimeofday ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Reader %d acquired second time\n"),
                  num));
    }

  // Done; small delay, release, report, and return.
  ACE_OS::sleep (1);
  stop = ACE_OS::gettimeofday ();
  if (-1 == mutex.release ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Reader %d %p\n"),
                num,
                ACE_TEXT ("second release")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Reader %d released second time; done\n"),
                num));
  report.range_.set (start, stop);
  bytes = sock.send (&report, sizeof (report), parent);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reader %d sent %b byte report\n"),
              num,
              bytes));
  sock.close ();
  return;
}

static void
writer (void)
{
  ACE_RW_Process_Mutex mutex (mutex_name.c_str ());

  // Make sure the constructor succeeded
  if (ACE_LOG_MSG->op_status () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Writer, mutex %s %p\n"),
                  mutex_name.c_str (),
                  ACE_TEXT ("ctor")));
      return;
    }

  ACE_SOCK_Dgram sock;
  ACE_INET_Addr parent;
  parent.set (reporting_port, ACE_LOCALHOST, 1, AF_INET);
  ACE_TCHAR me_str[80];
  parent.addr_to_string (me_str, 80);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sending reports to %s\n"), me_str));
  if (sock.open (ACE_Addr::sap_any, PF_INET) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("UDP open")));

  Range_Report report;
  report.child_ = 0;   // We're the writer
  ACE_Time_Value start (ACE_Time_Value::zero), stop (ACE_Time_Value::zero);

  // Grab the lock
  if (-1 == mutex.acquire_write ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Writer first %p\n"),
                ACE_TEXT ("acquire_write")));
  else
    {
      start = ACE_OS::gettimeofday ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Writer acquired first time\n")));
    }

  // Now sleep, making the readers wait for the lock. Then release the lock,
  // sleep, and reacquire the lock.
  ACE_OS::sleep (2);
  stop = ACE_OS::gettimeofday ();
  if (-1 == mutex.release ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Writer %p\n"),
                ACE_TEXT ("first release")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Writer released first time\n")));

  report.range_.set (start, stop);
  ssize_t bytes = sock.send (&report, sizeof (report), parent);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Writer sent %b byte report\n"), bytes));

  ACE_OS::sleep (1);   // Ensure we don't immediately grab the lock back

  start = stop = ACE_Time_Value::zero;

  if (-1 == mutex.acquire_write ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Writer second %p\n"),
                ACE_TEXT ("acquire_write")));
  else
    {
      start = ACE_OS::gettimeofday ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Writer acquired second time\n")));
    }

  ACE_OS::sleep (2);
  stop = ACE_OS::gettimeofday ();
  if (-1 == mutex.release ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Writer %p\n"),
                ACE_TEXT ("second release")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Writer released second time\n")));
  report.range_.set (start, stop);
  bytes = sock.send (&report, sizeof (report), parent);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Writer sent %b byte report\n"), bytes));
  sock.close ();
  return;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  // Child process code.
  if (child_nr >= 0)
    {
      ACE_TCHAR lognm[MAXPATHLEN];
      int mypid (ACE_OS::getpid ());
      ACE_OS::sprintf(lognm,
                      ACE_TEXT ("RW_Process_Mutex_Test-child-%d"),
                      (int)mypid);
      ACE_START_TEST (lognm);
      if (child_nr == 0)
        writer ();
      else
        reader (child_nr);
      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ACE_TEXT ("RW_Process_Mutex_Test"));
      // Although it should be safe for each process to construct and
      // destruct the rw lock, this can disturb other process still
      // using the lock. This is not really correct, and should be
      // looked at, but it gets things moving.
      // Also see Process_Mutex_Test.cpp for similar issue.
      ACE_RW_Process_Mutex mutex (mutex_name.c_str ());
      // Make sure the constructor succeeded
      if (ACE_LOG_MSG->op_status () != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Parent, mutex %s %p\n"),
                      mutex_name.c_str (),
                      ACE_TEXT ("ctor")));
        }
#if !defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
      static const ACE_TCHAR* format = ACE_TEXT ("%ls -c %d -p %u -n %ls");
#else
      static const ACE_TCHAR* format = ACE_TEXT ("%s -c %d -p %u -n %s");
#endif /* !ACE_WIN32 && ACE_USES_WCHAR */

      // The parent process reads time ranges sent from the children via
      // UDP. Grab an unused UDP port to tell the children to send to.
      ACE_INET_Addr me;
      ACE_SOCK_Dgram sock;
      if (sock.open (ACE_Addr::sap_any, PF_INET) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Socket %p\n"),
                           ACE_TEXT ("open")),
                          -1);
      sock.get_local_addr (me);
      ACE_TCHAR me_str[80];
      me.addr_to_string (me_str, 80);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiving on %s\n"), me_str));

      // Spawn 1 writer and 3 reader processes that will contend for the
      // lock.
      Child writer;
      Child readers[Nr_Processes - 1];
      int i;

      for (i = 0; i < Nr_Processes; i++)
        {
          Child *child = (i == 0 ? &writer : &readers[i-1]);
          ACE_Process_Options options;
          options.command_line (format,
                                argc > 0 ? argv[0] : ACE_TEXT ("RW_Process_Mutex_Test"),
                                i,
                                (unsigned int)me.get_port_number (),
                                mutex_name.c_str ());
          if (child->spawn (options) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("spawn of child %d %p\n"),
                                 i,
                                 ACE_TEXT ("failed")),
                                -1);
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Child process %d has pid = %d.\n"),
                          i,
                          (int)(child->getpid ())));
            }
        }

      // Keep reading time ranges reported from the children until all the
      // children have exited. Alternate between checking for a range and
      // checking for exits.
      int processes = Nr_Processes;
      Child *children[Nr_Processes];
      for (i = 0; i < Nr_Processes; i++)
        children[i] = (i == 0 ? &writer : &readers[i-1]);

      Range_Report report;
      ACE_Time_Value poll (0);
      ACE_INET_Addr from;
      ssize_t bytes;
      while (processes > 0)
        {
          ACE_Time_Value limit (10);
          bytes = sock.recv (&report, sizeof (report), from, 0, &limit);
          if (bytes > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Report from child %d; %b bytes\n"),
                          report.child_, bytes));
              if (report.child_ == 0)
                writer.add_range (report.range_);
              else
                {
                  if (report.child_ >= 1 && report.child_ < Nr_Processes)
                    readers[report.child_ - 1].add_range (report.range_);
                  else
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("Report from out-of-range child #%d\n"),
                                report.child_));
                }
            }
          else
            {
              if (errno == ETIME)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("UDP time out; check child exits\n")));
              else
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("UDP recv")));
            }

          for (i = 0; i < Nr_Processes; i++)
            {
              if (children[i] == 0)
                continue;
              ACE_exitcode child_status;
              // See if the child has exited.
              int wait_result = children[i]->wait (poll, &child_status);
              if (wait_result == -1)
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("Wait for child %d, %p\n"),
                            i, ACE_TEXT ("error")));
              else if (wait_result != 0)
                {
                  if (child_status == 0)
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("Child %d finished ok\n"),
                                (int)(children[i]->getpid ())));
                  else
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("Child %d finished with status %d\n"),
                                (int)(children[i]->getpid ()), child_status));
                  children[i] = 0;
                  --processes;
                }
            }
        }

      sock.close ();

      if (0 != mutex.remove ())
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("mutex remove")));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Comparing time ranges...\n")));
      // The writer should never overlap any readers
      bool writer_overlap = false;
      for (i = 0; i < Nr_Processes - 1; ++i)
        {
          if (writer.any_overlaps (readers[i]))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Writer overlaps reader %d\n"),
                          i+1));
              writer_overlap = true;
            }
        }
      if (!writer_overlap)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Writer does not overlap with readers; Ok\n")));

      // And there should be some overlap between readers.
      bool reader_overlap = false;
      for (i = 0; i < Nr_Processes - 1; ++i)
        {
          // Just compare to those higher, else it compares the same ones,
          // only in reverse.
          for (int j = i + 1; j < Nr_Processes - 1; ++j)
            {
              if (readers[i].any_overlaps (readers[j]))
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("Reader %d overlaps reader %d; Ok\n"),
                              i + 1, j + 1));
                  reader_overlap = true;
                }
            }
        }
      if (!reader_overlap)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("No readers overlapped!\n")));

      ACE_END_TEST;
    }

  return 0;
}
