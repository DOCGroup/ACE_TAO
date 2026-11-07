#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/ORB.h"
#include "ace/Task.h"
#include <memory>

class Thread_Task : public ACE_Task <ACE_SYNCH>
{
 public:
  explicit Thread_Task (CORBA::ORB_ptr orb);
  Thread_Task (Thread_Task const &) = delete;
  Thread_Task (Thread_Task &&) = delete;
  Thread_Task & operator= (Thread_Task const &) = delete;
  Thread_Task & operator= (Thread_Task &&) = delete;

  int activate_task (int thr_count);

  ~Thread_Task () override = default;

 protected:
  /// task svc
  int svc () override;

 private:
  CORBA::ORB_var orb_;
  RTScheduling::Current_var current_;
  int active_thread_count_ {};
  std::unique_ptr<TAO_SYNCH_MUTEX> shutdown_lock_;
  std::unique_ptr<TAO_SYNCH_MUTEX> lock_;
};

#endif /* THREAD_TASK_H */
