#include "ace/Get_Opt.h"
#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_time.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://ior_1")};
  int iterations {1000};
  bool shutdown_server {};
};

char const *
sched_policy_name (int sched_policy)
{
  char const *name {};

  switch (sched_policy)
    {
    case ACE_SCHED_OTHER:
      name = "SCHED_OTHER";
      break;
    case ACE_SCHED_RR:
      name = "SCHED_RR";
      break;
    case ACE_SCHED_FIFO:
      name = "SCHED_FIFO";
      break;
    }

  return name;
}

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("xk:i:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'x':
        options.shutdown_server = true;
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-k <ior> "
                                     "-i <iterations> "
                                     "-x [shutdown server] "
                                     "\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

class Task : public ACE_Task_Base
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb,
        Client_Options const &options);
  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  int svc () override;
  bool decreased () const;

private:
  CORBA::ORB_var const orb_;
  Client_Options const options_;
  bool decreased_ {};
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb,
            Client_Options const &options)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb)),
    options_ (options)
{
}

int
Task::svc ()
{
  try
    {
      CORBA::Object_var object = this->orb_->string_to_object (this->options_.ior);

      test_var const test = test::_narrow (object.in ());

      pid_t const pid = ACE_OS::getpid ();
      CORBA::Long thread_count {};

      timespec millis {};
      millis.tv_sec = 0;
      millis.tv_nsec = 50 * 1000000L; // 1e6 nanos per milli

      ACE_hrtime_t const start = ACE_OS::gethrtime ();

      for (int i = 0; i != this->options_.iterations; ++i)
        {
          // ~20 req/sec means to sleep 50 millis between each
          ACE_OS::nanosleep (&millis); // ignore signals

          CORBA::Long current_thread_count {};
          CORBA::Long const result =
            test->method (pid,
                          i,
                          current_thread_count);

          ACE_ASSERT (result == i);
          // Assert disappears with optimizations on.
          ACE_UNUSED_ARG (result);

          if (current_thread_count > thread_count)
            {
              ACE_DEBUG ((LM_DEBUG, "Thread count increased to %d\n", current_thread_count));
              thread_count = current_thread_count;
            }
          else if (current_thread_count < thread_count)
            {
              ACE_DEBUG ((LM_DEBUG, "Thread count decreased to %d\n", current_thread_count));
              this->decreased_ = true;
              thread_count = current_thread_count;
            }
        }

      ACE_hrtime_t const duration = ACE_OS::gethrtime () - start;
      double const throughput = static_cast<double> (this->options_.iterations) /
                                (duration / 1000000) * 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Throughput: %f\n", throughput));

      ACE_OS::sleep (10);

      CORBA::Long final_thread_count {};
      CORBA::Long const result = test->method (pid, 0, final_thread_count);

      ACE_ASSERT (result == 0);
      // Assert disappears with optimizations on.
      ACE_UNUSED_ARG (result);

      if (final_thread_count != 0)
        ACE_ERROR ((LM_ERROR,
                    "Dynamic thread count should be 0, not %d\n",
                    final_thread_count));

      if (this->options_.shutdown_server)
        test->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

bool
Task::decreased () const
{
  return this->decreased_;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      Client_Options options;
      int result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      ACE_Thread_Manager thread_manager;
      Task task (thread_manager, orb.in (), options);

      long const flags = THR_NEW_LWP |
                         THR_JOINABLE |
                         orb->orb_core ()->orb_params ()->thread_creation_flags ();

      result = task.activate (flags);

      if (result == -1)
        {
          if (errno == EPERM)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot create thread with scheduling policy %s\n"
                               "because the user does not have the appropriate privileges, terminating program....\n"
                               "Check svc.conf options and/or run as root\n",
                               sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                              2);
          else
            ACE_ASSERT (0);
        }

      result = thread_manager.wait ();
      ACE_ASSERT (result != -1);

      if (!task.decreased ())
        ACE_ERROR_RETURN ((LM_ERROR, "ERROR: Number of threads didn't decrease\n"), -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
