// Perform a torture test of multiple ACE_Reactors and ACE_Tasks in
// the same process...  Thanks to Detlef Becker for contributing this.

#include "ace/Reactor.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"

#include <atomic>
#include <memory>

#if defined (ACE_HAS_THREADS)

#include "ace/Recursive_Thread_Mutex.h"

static constexpr int NUM_INVOCATIONS = 10;
static constexpr int MAX_TASKS = 20;

class Test_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Test_Task ();
  ~Test_Task () override;

  Test_Task (Test_Task const &) = delete;
  Test_Task (Test_Task &&) = delete;
  Test_Task &operator= (Test_Task const &) = delete;
  Test_Task &operator= (Test_Task &&) = delete;

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void *args = nullptr) override;
  int close (u_long flags = 0) override;
  //FUZZ: enable check_for_lack_ACE_OS

  int svc () override;

  int handle_input (ACE_HANDLE handle) override;
  int handle_close (ACE_HANDLE fd,
                            ACE_Reactor_Mask close_mask) override;

private:
  int handled_ {};

  static int task_count_;
};

int Test_Task::task_count_ = 0;

static std::atomic<int> done_count {MAX_TASKS * 2};

static ACE_Recursive_Thread_Mutex reclock_;

Test_Task::Test_Task ()
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, reclock_);

  Test_Task::task_count_++;
  ACE_DEBUG ((LM_DEBUG,
              "(%t) TT+ Test_Task::task_count_ = %d\n",
              Test_Task::task_count_));
}

Test_Task::~Test_Task ()
{
  ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, reclock_);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) TT- Test_Task::task_count_ = %d\n",
              Test_Task::task_count_));
}

int
Test_Task::open (void *args)
{
  this->reactor (static_cast<ACE_Reactor *> (args));
  return this->activate (THR_NEW_LWP);
}

int
Test_Task::close (u_long)
{
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, reclock_, -1);

  Test_Task::task_count_--;
  ACE_DEBUG ((LM_DEBUG,
              "(%t) close Test_Task::task_count_ = %d\n",
              Test_Task::task_count_));
  return 0;
}

int
Test_Task::svc ()
{
  for (int i = 0; i < NUM_INVOCATIONS; i++)
    {
      ACE_OS::thr_yield ();

      // ACE_DEBUG ((LM_DEBUG, "(%t) calling notify %d\n", i));

      if (this->reactor ()->notify (this, ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "notify"), -1);

      // ACE_DEBUG ((LM_DEBUG, "(%t) leaving notify %d\n", i));
    }

  return 0;
}

int
Test_Task::handle_close (ACE_HANDLE,
                         ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_close\n"));
  return 0;
}

int
Test_Task::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_input\n"));

  this->handled_++;

  if (this->handled_ == NUM_INVOCATIONS)
    {
      done_count--;
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) handle_input, handled_ = %d, done_count = %d\n",
                  this->handled_, done_count.load ()));
    }

  ACE_OS::thr_yield ();
  return -1;
}

static void *
worker (void *args)
{
  ACE_Reactor * const reactor = static_cast<ACE_Reactor *> (args);

  reactor->owner (ACE_Thread::self ());

  ACE_Time_Value timeout (4);

  for (;;)
    {
      //ACE_DEBUG ((LM_DEBUG, "(%t) calling handle_events\n"));

      switch (reactor->handle_events (timeout))
        {
        case -1:
          ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "reactor"), 0);
          /* NOTREACHED */
        case 0:
          ACE_ERROR_RETURN ((LM_ERROR, "(%t) timeout\n"), 0);
          /* NOTREACHED */
        }

      // ACE_DEBUG ((LM_DEBUG, "(%t) done with handle_events\n"));
    }

  ACE_NOTREACHED(return nullptr);
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Reactor * const react1 = ACE_Reactor::instance ();
  std::unique_ptr<ACE_Reactor> const react2 = std::make_unique<ACE_Reactor> ();
  Test_Task tt1[MAX_TASKS] {};
  Test_Task tt2[MAX_TASKS] {};

  for (int i = 0; i < MAX_TASKS; i++)
    {
      tt1[i].open (react1);
      tt2[i].open (react2.get ());
    }

  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (worker), static_cast<void *> (react1), THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), -1);

  else if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (worker), static_cast<void *> (react2.get ()), THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), -1);

  ACE_Thread_Manager::instance ()->wait ();
  ACE_DEBUG ((LM_DEBUG, "(%t) done\n"));

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
