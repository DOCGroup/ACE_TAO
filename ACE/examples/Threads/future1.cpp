
//=============================================================================
/**
 *  @file    future1.cpp
 *
 *  This example tests the ACE Future.
 *
 *  @author Andres Kruse <Andres.Kruse@cern.ch> and Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Thread_Mutex.h"
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
static ATOMIC_INT task_count (0);

// a counter for the futures..
static ATOMIC_INT future_count (0);
static ATOMIC_INT future_no (0);

// a counter for the capsules..
static ATOMIC_INT capsule_count (0);
static ATOMIC_INT capsule_no (0);

// a counter for the method objects...
static ATOMIC_INT methodobject_count (0);
static ATOMIC_INT methodobject_no (0);

/**
 * @class Scheduler
 *
 * @brief Active Object Scheduler.
 */
class Scheduler : public ACE_Task_Base
{
  friend class Method_Request_work;
  friend class Method_Request_name;
public:
  explicit Scheduler (std::string name, Scheduler *scheduler = nullptr);
  ~Scheduler () override;

  Scheduler (Scheduler const &) = delete;
  Scheduler (Scheduler &&) = delete;
  Scheduler &operator= (Scheduler const &) = delete;
  Scheduler &operator= (Scheduler &&) = delete;

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  int open (void *args = nullptr) override;
  int close (u_long flags = 0) override;

  int svc () override;

  ACE_Future<u_long> work (u_long param, int count = 1);
  ACE_Future<std::string> name ();
  void end ();

private:
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

Method_Request_work::Method_Request_work (Scheduler* new_Scheduler,
                                        u_long new_param,
                                        int new_count,
                                        ACE_Future<u_long> &new_result)
  :   scheduler_ (new_Scheduler),
      param_ (new_param),
      count_ (new_count),
      future_result_ (new_result)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Method_Request_work created\n"));
}

Method_Request_work::~Method_Request_work ()
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Method_Request_work will be deleted.\n"));
}


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
              "(%t) Method_Request_name created\n"));
}

Method_Request_name::~Method_Request_name ()
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Method_Request_name will be deleted.\n"));
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

// Constructor.
Scheduler::Scheduler (std::string name,
                      Scheduler *scheduler)
  : name_ (std::move (name)),
    scheduler_ (scheduler)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Scheduler %s created\n", this->name_.c_str ()));
}

// Destructor
Scheduler::~Scheduler ()
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Scheduler %s will be destroyed\n", this->name_.c_str ()));
}

// open
int
Scheduler::open (void *)
{
  task_count.fetch_add (1, std::memory_order_relaxed);
  ACE_DEBUG ((LM_DEBUG, "(%t) Scheduler %s open\n", this->name_.c_str ()));
  return this->activate (THR_BOUND);
}

// close
int
Scheduler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Scheduler %s close\n", this->name_.c_str ()));
  task_count.fetch_sub (1, std::memory_order_relaxed);
  return 0;
}

// service..
int
Scheduler::svc ()
{
  for (;;)
    {
      // Dequeue the next method object (we use an unique pointer in
      // case an exception is thrown in the <call>).
      std::unique_ptr<ACE_Method_Request> mo (this->activation_queue_.dequeue ());

      if (!mo)
        break;

      ACE_DEBUG ((LM_DEBUG, "(%t) calling method object\n"));
      // Call it.
      if (mo->call () == -1)
        break;
      // Destructor automatically deletes it.
    }

  /* NOTREACHED */
  return 0;
}

void
Scheduler::end ()
{
  if (this->enqueue (std::make_unique<Method_Request_end> ()) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) failed to enqueue end request\n"));
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
    // Delegate to the Scheduler.
    return this->scheduler_->name ();
  else
    {
      ACE_Future<std::string> new_future;
      if (this->enqueue (std::make_unique<Method_Request_name> (this, new_future)) == -1)
        ACE_ERROR ((LM_ERROR, "(%t) failed to enqueue name request\n"));

      return new_future;
    }
}

ACE_Future<u_long>
Scheduler::work (u_long newparam,
                 int newcount)
{
  if (this->scheduler_)
    return this->scheduler_->work (newparam, newcount);

  ACE_Future<u_long> new_future;
  if (this->enqueue (std::make_unique<Method_Request_work> (
        this, newparam, newcount, new_future)) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) failed to enqueue work request\n"));
  return new_future;
}

// @@ These values should be set by the command line options!

// Total number of loops.
static constexpr size_t n_loops = 100;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Create active objects..
  // @@ Should "open" be subsumed within the constructor of
  // Scheduler()?
  Scheduler andres ("andres");
  Scheduler peter ("peter");
  Scheduler helmut ("helmut");
  Scheduler matias ("matias", &andres);
  if (andres.open () == -1 || peter.open () == -1 || helmut.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "failed to open a scheduler\n"), -1);

  // Matias passes all asynchronous method calls on to Andres...
  if (matias.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "failed to open the delegating scheduler\n"), -1);

  for (size_t i = 0; i < n_loops; i++)
    {
      {
        ACE_Future<u_long> fresulta, fresultb, fresultc, fresultd, fresulte;
        ACE_Future<std::string> fname;

        ACE_DEBUG ((LM_DEBUG, "(%t) going to do a non-blocking call\n"));

        fresulta = andres.work (9013);
        fresultb = peter.work (9013);
        fresultc = helmut.work (9013);
        fresultd = matias.work (9013);
        fname = andres.name ();

        // see if the result is available...
        if (fresulta.ready ())
          ACE_DEBUG ((LM_DEBUG, "(%t) wow.. work is ready.....\n"));

        ACE_DEBUG ((LM_DEBUG, "(%t) non-blocking call done... now blocking...\n"));

        // Save the result of fresulta.

        fresulte = fresulta;

        if (i % 3 == 0)
          {
            // Every 3rd time... disconnect the futures...
            // but "fresulte" should still contain the result...
            fresulta.cancel (10);
            fresultb.cancel (20);
            fresultc.cancel (30);
            fresultd.cancel (40);
          }

        u_long resulta = 0, resultb = 0, resultc = 0, resultd = 0, resulte = 0;

        fresulta.get (resulta);
        fresultb.get (resultb);
        fresultc.get (resultc);
        fresultd.get (resultd);
        fresulte.get (resulte);

        ACE_DEBUG ((LM_DEBUG, "(%t) result a %u\n", (u_int) resulte));
        ACE_DEBUG ((LM_DEBUG, "(%t) result b %u\n", (u_int) resulta));
        ACE_DEBUG ((LM_DEBUG, "(%t) result c %u\n", (u_int) resultb));
        ACE_DEBUG ((LM_DEBUG, "(%t) result d %u\n", (u_int) resultc));
        ACE_DEBUG ((LM_DEBUG, "(%t) result e %u\n", (u_int) resultd));

        std::string name;

        fname.get (name);

        ACE_DEBUG ((LM_DEBUG, "(%t) name %s\n", name.c_str ()));
      }

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) task_count %d future_count %d capsule_count %d methodobject_count %d\n",
                  task_count.load (std::memory_order_relaxed),
                  future_count.load (std::memory_order_relaxed),
                  capsule_count.load (std::memory_order_relaxed),
                  methodobject_count.load (std::memory_order_relaxed)));
    }

  // Close things down.
  andres.end ();
  peter.end ();
  helmut.end ();
  matias.end ();
  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%t) task_count %d future_count %d capsule_count %d methodobject_count %d\n",
              task_count.load (std::memory_order_relaxed),
              future_count.load (std::memory_order_relaxed),
              capsule_count.load (std::memory_order_relaxed),
              methodobject_count.load (std::memory_order_relaxed)));

  ACE_DEBUG ((LM_DEBUG,"(%t) th' that's all folks!\n"));

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
