// $Id$

#include "RT_Class.h"
#include "Client_Options.h"

#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Barrier.h"
#include "ace/OS_NS_unistd.h"
#include "tao/orbconf.h"

ACE_TCHAR const *hi_endpoint = ACE_TEXT ("localhost:12345");
ACE_TCHAR const *lo_endpoint = ACE_TEXT ("localhost:23456");

int
parse_args (int argc, ACE_TCHAR *argv[]);

class Scavenger_Task : public ACE_Task_Base
{
public:
  Scavenger_Task (ACE_TCHAR const * endpoint,
                  ACE_Barrier * the_barrier,
                  int period_in_usecs);

  void stop(void);

  virtual int svc ();

private:
  ACE_TCHAR const * endpoint_;
  ACE_Barrier * the_barrier_;
  int period_in_usecs_;
  TAO_SYNCH_MUTEX mutex_;
  int stopped_;
};

class Measuring_Task : public ACE_Task_Base
{
public:
  Measuring_Task (ACE_TCHAR const * endpoint,
                  ACE_Barrier *the_barrier,
                  int iterations,
                  int period_in_usecs);

  virtual int svc();

  ACE_Sample_History sample_history;

private:
  ACE_TCHAR const * endpoint_;
  ACE_Barrier * the_barrier_;
  int iterations_;
  int period_in_usecs_;
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  RT_Class rt_class;

  Client_Options options (argc, argv);

  if (parse_args (argc, argv) != 0)
    return 1;

  ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
  ACE_High_Res_Timer::calibrate ();

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

  int thread_count = 1 + options.nthreads;
  ACE_Barrier the_barrier (thread_count);

  int per_thread_period = options.low_priority_period;
  if (options.global_low_priority_rate)
    per_thread_period = options.low_priority_period * options.nthreads;

  Scavenger_Task lo_task (lo_endpoint, &the_barrier,
                          per_thread_period);
  lo_task.activate (rt_class.thr_sched_class () | THR_NEW_LWP | THR_JOINABLE,
                    options.nthreads, 1,
                    rt_class.priority_low ());

  Measuring_Task hi_task (hi_endpoint, &the_barrier,
                          options.iterations,
                          options.high_priority_period);
  hi_task.activate (rt_class.thr_sched_class () | THR_NEW_LWP | THR_JOINABLE,
                    1, 1,
                    rt_class.priority_low ());

  hi_task.wait ();
  lo_task.stop ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - high prio task joined\n"));

  ACE_Sample_History &history = hi_task.sample_history;
  if (options.dump_history)
    {
      history.dump_samples (ACE_TEXT("HISTORY"), gsf);
    }

  ACE_Basic_Stats high_priority_stats;
  history.collect_basic_stats (high_priority_stats);
  high_priority_stats.dump_results (ACE_TEXT("High Priority"), gsf);

  lo_task.wait ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - all task(s) joined\n"));

  return 0;
}

// ****************************************************************

Scavenger_Task::Scavenger_Task(ACE_TCHAR const * endpoint,
                               ACE_Barrier * the_barrier,
                               int period_in_usecs)
  : endpoint_ (endpoint)
  , the_barrier_ (the_barrier)
  , period_in_usecs_ (period_in_usecs)
  , mutex_ ()
  , stopped_ (0)
{
}

void
Scavenger_Task::stop(void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->stopped_ = 1;
}

int
Scavenger_Task::svc(void)
{
  this->the_barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting scavenger thread\n"));

  ACE_SOCK_Stream stream;
  {
    ACE_INET_Addr remote_sap (this->endpoint_);
    ACE_SOCK_Connector connector;

    if (connector.connect(stream, remote_sap) == -1)
      {
        ACE_ERROR((LM_ERROR, "Cannot connect to <%s>\n", endpoint_));
        return -1;
      }
  }

  for (;;)
    {
      ACE_Time_Value period (0, this->period_in_usecs_);
      ACE_OS::sleep (period);

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
        if (this->stopped_)
          break;
      }
      ACE_hrtime_t start = ACE_OS::gethrtime ();
      ssize_t n = stream.send_n(&start, sizeof(start));
      if (n == 0 || n == -1)
        break;

      ACE_hrtime_t end;
      n = stream.recv(&end, sizeof(end));
      if (n == 0 || n == -1)
        break;

      if (start != end)
      {
        ACE_ERROR((LM_ERROR,
                   "Mismatched response from <%s>\n", endpoint_));
        break;
      }

    }
  stream.close();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Finishing scavenger thread\n"));
  return 0;
}

// ****************************************************************

Measuring_Task::Measuring_Task (ACE_TCHAR const * endpoint,
                                ACE_Barrier * the_barrier,
                                int iterations,
                                int period_in_usecs)
  : sample_history (iterations)
  , endpoint_ (endpoint)
  , the_barrier_ (the_barrier)
  , iterations_  (iterations)
  , period_in_usecs_ (period_in_usecs)
{
}

int
Measuring_Task::svc ()
{
  this->the_barrier_->wait ();

  ACE_SOCK_Stream stream;
  {
    ACE_INET_Addr remote_sap (this->endpoint_);
    ACE_SOCK_Connector connector;

    if (connector.connect(stream, remote_sap) == -1)
      {
        ACE_ERROR((LM_ERROR, "Cannot connect to <%s>\n", endpoint_));
        return -1;
      }
  }

  for (int i = 0; i != this->iterations_; ++i)
    {
      ACE_Time_Value period (0, this->period_in_usecs_);
      ACE_OS::sleep (period);

      ACE_hrtime_t start = ACE_OS::gethrtime ();
      ssize_t n = stream.send_n(&start, sizeof(start));
      if (n == 0) {
        ACE_ERROR((LM_ERROR,
                   "Connection closed while writing data to server <%s>\n",
                   endpoint_));
        break;
      } else if (n == -1) {
        ACE_ERROR((LM_ERROR,
                   "Error writing data to server <%s> %p\n",
                   endpoint_));
        break;
      }
      if (n == 0 || n == -1)
      {
        ACE_ERROR((LM_ERROR,
                   "Error sending data to server <%s>\n", endpoint_));
        break;
      }

      ACE_hrtime_t end;
      n = stream.recv_n(&end, sizeof(end));
      if (n == 0) {
        ACE_ERROR((LM_ERROR,
                   "Connection closed while reading data from server <%s>\n",
                   endpoint_));
        break;
      } else if (n == -1) {
        ACE_ERROR((LM_ERROR,
                   "Error reading data from server <%s> %p\n",
                   endpoint_));
        break;
      }

      if (start != end)
      {
        ACE_ERROR((LM_ERROR,
                   "Mismatched response from <%s>\n", endpoint_));
        return -1;
      }
      ACE_hrtime_t elapsed = ACE_OS::gethrtime () - start;

      this->sample_history.sample (elapsed);
    }

  stream.close();

  return 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("H:L:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'H':
        hi_endpoint = get_opts.opt_arg ();
        break;

      case 'L':
        lo_endpoint = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-i iterations (iterations) "
                           "-h high_priority_period (usecs) "
                           "-l low_priority_period (usecs) "
                           "-w high_priority_workload (usecs) "
                           "-v low_priority_workload (usecs) "
                           "-r (enable RT-CORBA) "
                           "-n nthreads (low priority thread) "
                           "-d (dump history) "
                           "-z (disable low priority) "
                           "-H hi_endpoint "
                           "-L lo_endpoint "
                           "\n",
                         argv [0]),
                        1);
      }
  // Indicates successful parsing of the command line
  return 0;
}
