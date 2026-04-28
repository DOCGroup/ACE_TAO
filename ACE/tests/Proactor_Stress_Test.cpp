// ============================================================================
/**
 *  @file Proactor_Stress_Test.cpp
 *
 *  Stress coverage for recursive timer-driven dispatch patterns across
 *  selectable ACE Proactor backends.
 */
// ============================================================================

#include "test_config.h"

#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS))

#include "ace/Condition_Thread_Mutex.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Proactor.h"
#include "ace/Task.h"
#include "ace/Thread_Mutex.h"

#include "Proactor_Test_Backend.h"

namespace
{
  double
  seconds_between (const ACE_Time_Value &start,
                   const ACE_Time_Value &end)
  {
    const ACE_Time_Value delta = end - start;
    return static_cast<double> (delta.sec ()) +
      (static_cast<double> (delta.usec ()) / 1000000.0);
  }

  class Proactor_Task : public ACE_Task<ACE_MT_SYNCH>
  {
  public:
    Proactor_Task ()
      : proactor_ (0)
    {
    }

    ~Proactor_Task ()
    {
      this->stop ();
    }

    int start (int thread_count,
               Proactor_Test_Backend::Type backend,
               size_t max_aio_operations)
    {
      if (this->proactor_ != 0)
        return -1;

      if (Proactor_Test_Backend::create_proactor (backend,
                                                  max_aio_operations,
                                                  this->proactor_,
                                                  true) != 0)
        return -1;

      return this->activate (THR_NEW_LWP | THR_JOINABLE, thread_count);
    }

    int stop ()
    {
      ACE_Proactor *proactor = this->proactor_;
      if (proactor == 0)
        return 0;

      proactor->proactor_end_event_loop ();
      this->wait ();
#if defined (ACE_WIN32)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Skipping ACE_Proactor::close_singleton() on Windows test shutdown\n")));
#else
      ACE_Proactor::close_singleton ();
#endif
      this->proactor_ = 0;
      return 0;
    }

    virtual int svc (void)
    {
      return ACE_Proactor::instance ()->proactor_run_event_loop ();
    }

  private:
    ACE_Proactor *proactor_;
  };

  class Recursive_Dispatch_Handler : public ACE_Handler
  {
  public:
    Recursive_Dispatch_Handler (ACE_Proactor &proactor,
                                size_t dispatch_scale,
                                size_t target_count)
      : proactor_ (proactor)
      , lock_ ()
      , target_reached_ (lock_)
      , idle_ (lock_)
      , dispatch_scale_ (dispatch_scale)
      , target_count_ (target_count)
      , call_count_ (0)
      , pending_ (0)
      , schedule_failures_ (0)
      , target_seen_ (false)
    {
    }

    int start (size_t initial_events)
    {
      for (size_t index = 0; index < initial_events; ++index)
        {
          if (this->schedule_one () != 0)
            return -1;
        }
      return 0;
    }

    void stop_dispatching ()
    {
      ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
      this->dispatch_scale_ = 0;
      if (this->pending_ == 0)
        this->idle_.broadcast ();
    }

    bool wait_for_target (const ACE_Time_Value &max_wait)
    {
      const ACE_Time_Value deadline = ACE_OS::gettimeofday () + max_wait;
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->lock_, false);
      while (!this->target_seen_)
        {
          if (this->target_reached_.wait (&deadline) == -1)
            return this->target_seen_;
        }
      return true;
    }

    bool wait_for_idle (const ACE_Time_Value &max_wait)
    {
      const ACE_Time_Value deadline = ACE_OS::gettimeofday () + max_wait;
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->lock_, false);
      while (this->pending_ != 0)
        {
          if (this->idle_.wait (&deadline) == -1)
            return this->pending_ == 0;
        }
      return true;
    }

    size_t call_count () const
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, const_cast<ACE_Thread_Mutex &> (this->lock_), 0);
      return this->call_count_;
    }

    size_t schedule_failures () const
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, const_cast<ACE_Thread_Mutex &> (this->lock_), 0);
      return this->schedule_failures_;
    }

    virtual void handle_time_out (const ACE_Time_Value &, const void *)
    {
      size_t schedule_count = 0;
      {
        ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
        if (this->pending_ > 0)
          --this->pending_;

        ++this->call_count_;
        if (!this->target_seen_ && this->call_count_ >= this->target_count_)
          {
            this->target_seen_ = true;
            this->target_reached_.broadcast ();
          }

        schedule_count = this->dispatch_scale_;
        if (schedule_count == 0 && this->pending_ == 0)
          this->idle_.broadcast ();
      }

      for (size_t index = 0; index < schedule_count; ++index)
        {
          if (this->schedule_one () != 0)
            break;
        }
    }

  private:
    int schedule_one ()
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->lock_, -1);
      ++this->pending_;

      long timer_id = this->proactor_.schedule_timer (*this,
                                                      0,
                                                      ACE_Time_Value::zero);
      if (timer_id == -1)
        {
          --this->pending_;
          ++this->schedule_failures_;
          if (this->dispatch_scale_ == 0 && this->pending_ == 0)
            this->idle_.broadcast ();
          return -1;
        }

      return 0;
    }

    ACE_Proactor &proactor_;
    mutable ACE_Thread_Mutex lock_;
    ACE_Condition_Thread_Mutex target_reached_;
    ACE_Condition_Thread_Mutex idle_;
    size_t dispatch_scale_;
    size_t target_count_;
    size_t call_count_;
    size_t pending_;
    size_t schedule_failures_;
    bool target_seen_;
  };

  int print_usage (ACE_TCHAR *argv0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Usage: %s [-t <backend>]\n"),
                argv0));
    return Proactor_Test_Backend::print_type_usage (argv0);
  }

  int run_recursive_test (Proactor_Test_Backend::Type backend,
                          bool immediate_shutdown)
  {
    const ACE_Time_Value start_time = ACE_OS::gettimeofday ();
    Proactor_Task task;
    if (task.start (8, backend, 512) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to start proactor task for backend %s\n"),
                           Proactor_Test_Backend::name (backend)),
                          -1);
      }

    Recursive_Dispatch_Handler handler (*ACE_Proactor::instance (), 2, 100000);
    if (handler.start (2) != 0)
      {
        task.stop ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to seed recursive dispatch for backend %s\n"),
                           Proactor_Test_Backend::name (backend)),
                          -1);
      }

    if (!handler.wait_for_target (ACE_Time_Value (30)))
      {
        task.stop ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Timed out waiting for target count on backend %s\n"),
                           Proactor_Test_Backend::name (backend)),
                          -1);
      }

    handler.stop_dispatching ();

    if (!immediate_shutdown && !handler.wait_for_idle (ACE_Time_Value (10)))
      {
        task.stop ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Timed out waiting for idle state on backend %s\n"),
                           Proactor_Test_Backend::name (backend)),
                          -1);
      }

    task.stop ();

    if (handler.call_count () < 100000)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Expected at least 100000 callbacks for backend %s; got %u\n"),
                           Proactor_Test_Backend::name (backend),
                           static_cast<unsigned> (handler.call_count ())),
                          -1);
      }

    if (!immediate_shutdown && handler.schedule_failures () != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Unexpected timer scheduling failures for backend %s: %u\n"),
                           Proactor_Test_Backend::name (backend),
                           static_cast<unsigned> (handler.schedule_failures ())),
                          -1);
      }

    const ACE_Time_Value end_time = ACE_OS::gettimeofday ();
    const double elapsed = seconds_between (start_time, end_time);
    const double safe_elapsed = elapsed > 0.0 ? elapsed : 0.000001;
    const double callbacks_per_sec =
      static_cast<double> (handler.call_count ()) / safe_elapsed;

    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("PERF_RESULT benchmark=stress backend=%s mode=%s ")
                ACE_TEXT ("elapsed_sec=%.6f callbacks=%u callbacks_per_sec=%.3f ")
                ACE_TEXT ("schedule_failures=%u\n"),
                Proactor_Test_Backend::name (backend),
                immediate_shutdown ? ACE_TEXT ("immediate") : ACE_TEXT ("graceful"),
                elapsed,
                static_cast<unsigned> (handler.call_count ()),
                callbacks_per_sec,
                static_cast<unsigned> (handler.schedule_failures ())));

    return 0;
  }
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  Proactor_Test_Backend::Type backend = Proactor_Test_Backend::BACKEND_DEFAULT;
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("t:u"));
  int c = 0;
  int parse_failed = 0;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 't':
          if (Proactor_Test_Backend::parse_type (get_opt.opt_arg (), backend) == 0)
            {
              if (Proactor_Test_Backend::is_available (backend) != 0)
                break;

              Proactor_Test_Backend::unsupported (backend);
            }
          parse_failed = 1;
          break;
        case 'u':
        default:
          parse_failed = 1;
          break;
        }

      if (parse_failed)
        break;
    }

  ACE_START_TEST (ACE_TEXT ("Proactor_Stress_Test"));

  if (parse_failed)
    {
      print_usage (argv[0]);
      ACE_END_TEST;
      return -1;
    }

  int status = 0;
  if (run_recursive_test (backend, false) != 0)
    status = -1;
  else if (run_recursive_test (backend, true) != 0)
    status = -1;

  ACE_END_TEST;
  return status;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Stress_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Asynchronous IO is unsupported.\n")
              ACE_TEXT ("Proactor_Stress_Test will not be run.\n")));

  ACE_END_TEST;
  return 0;
}

#endif /* ACE_HAS_THREADS && async io support */
