#include "ace/Task_T.h"
#include "ace/Service_Config.h"

#include "TestC.h"

#include <atomic>

/// Provides a thread pool abstraction based on `ACE_Task`.
class Thread_Pool : public ACE_Task<ACE_SYNCH>
{
public:
  /// Activates `n_threads` worker threads in the pool.
  Thread_Pool (CORBA::ORB_ptr orb,
               ACE_Thread_Manager *thr_mgr,
               int n_threads);
  Thread_Pool (Thread_Pool const &) = delete;
  Thread_Pool &operator= (Thread_Pool const &) = delete;
  Thread_Pool (Thread_Pool &&) = delete;
  Thread_Pool &operator= (Thread_Pool &&) = delete;

  ~Thread_Pool () override = default;
  int shutdown ();

  int svc () override;

  virtual int put (Test::Echo_ptr echoptr);

  /// Enqueues a message block for the worker threads.
  int put (ACE_Message_Block *mb,
           ACE_Time_Value *tv = nullptr) override;

private:
  /// Close hook.
  int close (u_long) override;

  CORBA::ORB_var const orb_;

  /// Number of threads in the pool.
  std::atomic<int> nt_;
};
