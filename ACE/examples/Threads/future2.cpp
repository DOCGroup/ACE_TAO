
//=============================================================================
/**
 *  @file    future2.cpp
 *
 *  This example tests the ACE Future.
 *
 *  @author Andres Kruse <Andres.Kruse@cern.ch> and Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *          and "Method_Request_work".
 *          - make the methods "work_i" and "name_i" private
 *          - make the tests more modular.
 */
//=============================================================================


#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Message_Queue.h"
#include "ace/Future.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"
#include <atomic>
#include <memory>
#include <string>
#include <utility>

#if defined (ACE_HAS_THREADS)

using ATOMIC_INT = std::atomic<int>;

// a counter for the tasks..
static ATOMIC_INT scheduler_open_count (0);

// forward declarations
class Method_Request_work;
class Method_Request_name;

/**
 * @class Scheduler
 *
 * @brief Active Object Scheduler.
 */
class Scheduler : public ACE_Task_Base
{
  // Every method object has to be able to access the private methods.

  friend class Method_Request_work;
  friend class Method_Request_name;
  friend class Method_Request_end;
public:
  explicit Scheduler (std::string name, Scheduler *scheduler = nullptr);
  ~Scheduler () override;

  Scheduler (Scheduler const &) = delete;
  Scheduler (Scheduler &&) = delete;
  Scheduler &operator= (Scheduler const &) = delete;
  Scheduler &operator= (Scheduler &&) = delete;

  //FUZZ: disable check_for_lack_ACE_OS
  /// The method that is used to start the active object.
  ///FUZZ: enable check_for_lack_ACE_OS
  int open (void *args = nullptr) override;

  // = Here are the methods exported by the class. They return an
  // <ACE_Future>.
  ACE_Future<u_long> work (u_long param, int count = 1);
  ACE_Future<std::string> name ();
  void end ();

private:
  //FUZZ: disable check_for_lack_ACE_OS
  /// Should not be accessible from outside...  (use end () instead).
  ///FUZZ: enable check_for_lack_ACE_OS
  int close (u_long flags = 0) override;

  /// Here the actual servicing of all requests is happening..
  int svc () override;

  // = Implementation methods.
  u_long work_i (u_long, int) const;
  std::string name_i () const;
  int enqueue (std::unique_ptr<ACE_Method_Request> request);

  std::string const name_;
  ACE_Activation_Queue activation_queue_;
  Scheduler * const scheduler_ {}; // Non-owning delegate; it must outlive this scheduler.
};

/**
 * @class Method_Request_work
 *
 * @brief Reification of the <work> method.
 */
class Method_Request_work : public ACE_Method_Request
{
public:
  Method_Request_work (Scheduler *, u_long, int, ACE_Future<u_long> &);
  ~Method_Request_work () override;
  int call () override;

  Method_Request_work (Method_Request_work const &) = delete;
  Method_Request_work (Method_Request_work &&) = delete;
  Method_Request_work &operator= (Method_Request_work const &) = delete;
  Method_Request_work &operator= (Method_Request_work &&) = delete;

private:
  Scheduler * const scheduler_ {}; // Non-owning; its owner outlives the queued request.
  u_long const param_ {};
  int const count_ {};
  ACE_Future<u_long> future_result_;
};

Method_Request_work::Method_Request_work (Scheduler *new_Scheduler,
                                        u_long new_param,
                                        int new_count,
                                        ACE_Future<u_long> &new_result)
  : scheduler_ (new_Scheduler),
    param_ (new_param),
    count_ (new_count),
    future_result_ (new_result)
{
}

Method_Request_work::~Method_Request_work () = default;

int
Method_Request_work::call ()
{
  return this->future_result_.set (this->scheduler_->work_i (this->param_, this->count_));
}

/**
 * @class Method_Request_name
 *
 * @brief Reification of the <name> method.
 */
class Method_Request_name : public ACE_Method_Request
{
public:
  Method_Request_name (Scheduler *, ACE_Future<std::string> &);
  ~Method_Request_name () override;
  int call () override;

  Method_Request_name (Method_Request_name const &) = delete;
  Method_Request_name (Method_Request_name &&) = delete;
  Method_Request_name &operator= (Method_Request_name const &) = delete;
  Method_Request_name &operator= (Method_Request_name &&) = delete;

private:
  Scheduler * const scheduler_ {}; // Non-owning; its owner outlives the queued request.
  ACE_Future<std::string> future_result_;
};


Method_Request_name::Method_Request_name (Scheduler *new_scheduler,
                                         ACE_Future<std::string> &new_result)
  : scheduler_ (new_scheduler),
    future_result_ (new_result)
{
  ACE_DEBUG ((LM_DEBUG,
              " (%t) Method_Request_name created\n"));
}

Method_Request_name::~Method_Request_name ()
{
  ACE_DEBUG ((LM_DEBUG,
              " (%t) Method_Request_name will be deleted.\n"));
}

int
Method_Request_name::call ()
{
  return future_result_.set (scheduler_->name_i ());
}

/**
 * @class Method_Request_end
 *
 * @brief Reification of the <end> method.
 */
class Method_Request_end : public ACE_Method_Request
{
public:
  Method_Request_end () = default;
  ~Method_Request_end () override = default;
  int call () override { return -1; }

  Method_Request_end (Method_Request_end const &) = delete;
  Method_Request_end (Method_Request_end &&) = delete;
  Method_Request_end &operator= (Method_Request_end const &) = delete;
  Method_Request_end &operator= (Method_Request_end &&) = delete;
};

// constructor
Scheduler::Scheduler (std::string name, Scheduler *scheduler)
  : name_ (std::move (name)),
    scheduler_ (scheduler)
{
  ACE_DEBUG ((LM_DEBUG, " (%t) Scheduler %s created\n", this->name_.c_str ()));
}

// Destructor
Scheduler::~Scheduler ()
{
  ACE_DEBUG ((LM_DEBUG, " (%t) Scheduler %s will be destroyed\n", this->name_.c_str ()));
}

int
Scheduler::open (void *)
{
  scheduler_open_count.fetch_add (1, std::memory_order_relaxed);
  ACE_DEBUG ((LM_DEBUG, " (%t) Scheduler %s open\n", this->name_.c_str ()));
  return this->activate (THR_BOUND);
}

int
Scheduler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, " (%t) Scheduler %s close\n", this->name_.c_str ()));
  scheduler_open_count.fetch_sub (1, std::memory_order_relaxed);
  return 0;
}

int
Scheduler::svc ()
{
  // Main event loop for this active object.
  for (;;)
    {
      // Dequeue the next method object (we use an unique pointer in
      // case an exception is thrown in the <call>).
      std::unique_ptr<ACE_Method_Request> mo (this->activation_queue_.dequeue ());

      if (!mo)
        break;

      ACE_DEBUG ((LM_DEBUG, " (%t) calling method object\n"));
      // Call it.
      if (mo->call () == -1)
        break;
      // Smart pointer destructor automatically deletes mo.
    }

  /* NOTREACHED */
  return 0;
}

void
Scheduler::end ()
{
  if (this->enqueue (std::make_unique<Method_Request_end> ()) == -1)
    ACE_ERROR ((LM_ERROR, " (%t) failed to enqueue end request\n"));
}

// Here's where the Work takes place.
u_long
Scheduler::work_i (u_long param,
                   int count) const
{
  ACE_UNUSED_ARG (count);

  return ACE::is_prime (param, 2, param / 2);
}

std::string
Scheduler::name_i () const
{
  return this->name_;
}

int
Scheduler::enqueue (std::unique_ptr<ACE_Method_Request> request)
{
  int const result = this->activation_queue_.enqueue (request.get ());
  if (result != -1)
    request.release ();
  return result;
}

ACE_Future<std::string>
Scheduler::name ()
{
  if (this->scheduler_)
    // Delegate to the other scheduler
    return this->scheduler_->name ();
  else
    {
      ACE_Future<std::string> new_future;

      if (this->thr_count () == 0)
        {
          // This scheduler is inactive... so we execute the user
          // request right away...
          std::unique_ptr<ACE_Method_Request> mo =
            std::make_unique<Method_Request_name> (this, new_future);

          mo->call ();
          // Smart pointer destructor automatically deletes mo.
        }
      else
        {
          if (this->enqueue (std::make_unique<Method_Request_name> (this, new_future)) == -1)
            ACE_ERROR ((LM_ERROR, " (%t) failed to enqueue name request\n"));
        }

      return new_future;
    }
}

ACE_Future<u_long>
Scheduler::work (u_long newparam, int newcount)
{
  if (this->scheduler_)
    return this->scheduler_->work (newparam, newcount);
  else
    {
      ACE_Future<u_long> new_future;

      if (this->thr_count () == 0)
        {
          std::unique_ptr<ACE_Method_Request> mo =
            std::make_unique<Method_Request_work> (this, newparam, newcount, new_future);
          mo->call ();
          // Smart pointer destructor automatically deletes it.
        }
      else
        {
          if (this->enqueue (std::make_unique<Method_Request_work> (
                this, newparam, newcount, new_future)) == -1)
            ACE_ERROR ((LM_ERROR, " (%t) failed to enqueue work request\n"));
        }

      return new_future;
    }
}

static int
determine_iterations ()
{
  int n_iterations;

  ACE_DEBUG ((LM_DEBUG," (%t) determining the number of iterations...\n"));
  Scheduler worker_a ("worker A");

  ACE_Time_Value tstart (ACE_OS::gettimeofday ());
  ACE_Time_Value tend (ACE_OS::gettimeofday ());

  // Determine the number of iterations... we want so many that the
  // work () takes about 1 second...

  for (n_iterations = 1;
       (tend.sec () - tstart.sec ()) < 1;
       n_iterations *= 2)
    {
      tstart = ACE_OS::gettimeofday ();

      worker_a.work (9013, n_iterations);

      tend = ACE_OS::gettimeofday ();
    }

  ACE_DEBUG ((LM_DEBUG," (%t) n_iterations %d\n",
              (int) n_iterations));

  return n_iterations;
}

static void
test_active_object (int n_iterations)
{
  ACE_UNUSED_ARG (n_iterations);

  ACE_DEBUG ((LM_DEBUG," (%t) testing active object pattern...\n"));
  // A simple example for the use of the active object pattern and
  // futures to return values from an active object.

  Scheduler worker_a ("worker A");
  Scheduler worker_b ("worker B");
  // Have worker_c delegate his work to worker_a.
  Scheduler worker_c ("worker C", &worker_a);

  // loop 0:
  // test the Schedulers when they are not active.
  // now the method objects will be created but since
  // there is no active thread they will also be
  // immediately executed, in the "main" thread.
  // loop 1:
  // do the same test but with the schedulers
  // activated
  for (int i = 0; i < 2; i++)
    {
      if (i == 1)
        {
          worker_a.open ();
          worker_b.open ();
          worker_c.open ();
        }

      ACE_Future<u_long> fresulta = worker_a.work (9013);
      ACE_Future<u_long> fresultb = worker_b.work (9013);
      ACE_Future<u_long> fresultc = worker_c.work (9013);

      if (i == 0)
        {
          if (!fresulta.ready ())
            ACE_DEBUG ((LM_DEBUG," (%t) ERROR: worker A is should be ready!!!\n"));
          if (!fresultb.ready ())
            ACE_DEBUG ((LM_DEBUG," (%t) ERROR: worker B is should be ready!!!\n"));
          if (!fresultc.ready ())
            ACE_DEBUG ((LM_DEBUG," (%t) ERROR: worker C is should be ready!!!\n"));
        }

      // When the workers are active we will block here until the
      // results are available.

      u_long resulta = fresulta;
      u_long resultb = fresultb;
      u_long resultc = fresultc;

      ACE_Future<std::string> fnamea = worker_a.name ();
      ACE_Future<std::string> fnameb = worker_b.name ();
      ACE_Future<std::string> fnamec = worker_c.name ();

      std::string const namea = fnamea;
      std::string const nameb = fnameb;
      std::string const namec = fnamec;

      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %u\n",
                  namea.c_str (), (u_int) resulta));
      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %u\n",
                  nameb.c_str (), (u_int) resultb));
      ACE_DEBUG ((LM_DEBUG, " (%t) result from %s %u\n",
                  namec.c_str (), (u_int) resultc));
    }

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d before end ()\n",
              scheduler_open_count.load (std::memory_order_relaxed)));

  worker_a.end ();
  worker_b.end ();
  worker_c.end ();
  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d immediately after end ()\n",
              scheduler_open_count.load (std::memory_order_relaxed)));

  ACE_DEBUG ((LM_DEBUG, " (%t) scheduler_open_count %d after waiting\n",
              scheduler_open_count.load (std::memory_order_relaxed)));
}

static void
test_cancellation (int n_iterations)
{
  ACE_DEBUG ((LM_DEBUG," (%t) testing cancellation of a future...\n"));

  // Now test the cancelling a future.

  Scheduler worker_a ("worker A");
  worker_a.open ();

  ACE_Future<u_long> fresulta = worker_a.work (9013, n_iterations);

  // save the result by copying the future
  ACE_Future<u_long> fresultb = fresulta;

  // now we cancel the first future.. but the
  // calculation will still go on...
  fresulta.cancel (10);

  if (!fresulta.ready ())
    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: future A is should be ready!!!\n"));

  u_long resulta = fresulta;

  ACE_DEBUG ((LM_DEBUG, " (%t) cancelled result %u\n", (u_int) resulta));

  if (resulta != 10)
    ACE_DEBUG ((LM_DEBUG, " (%t) cancelled result should be 10!!\n", resulta));

  resulta = fresultb;

  ACE_DEBUG ((LM_DEBUG, " (%t) true result %u\n", (u_int) resulta));

  worker_a.end ();
  ACE_Thread_Manager::instance ()->wait ();
}

static void
test_timeout (int n_iterations)
{
  ACE_DEBUG ((LM_DEBUG," (%t) testing timeout on waiting for the result...\n"));
  Scheduler worker_a ("worker A");
  worker_a.open ();

  ACE_Future<u_long> fresulta = worker_a.work (9013, 2 * n_iterations);

  // Should immediately return... and we should see an error...
  ACE_Time_Value delay (1);

  u_long resulta = 0;
  fresulta.get (resulta, &delay);

  if (fresulta.ready ())
    ACE_DEBUG ((LM_DEBUG," (%t) ERROR: future A is should not be ready!!!\n"));
  else
    ACE_DEBUG ((LM_DEBUG," (%t) timed out on future A\n"));

  // now we wait until we are done...
  fresulta.get (resulta);
  ACE_DEBUG ((LM_DEBUG, " (%t) result %u\n", (u_int) resulta));

  worker_a.end ();
  ACE_Thread_Manager::instance ()->wait ();
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  int n_iterations = determine_iterations ();

  test_active_object (n_iterations);
  test_cancellation (n_iterations);
  test_timeout (n_iterations);

  ACE_DEBUG ((LM_DEBUG," (%t) that's all folks!\n"));

  ACE_OS::sleep (5);
  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
