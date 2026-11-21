#ifndef LONGUPCALLS_AMI_MANAGER_H
#define LONGUPCALLS_AMI_MANAGER_H

#include "TestS.h"
#include "ace/Task.h"

class AMI_Manager
  : public virtual POA_Test::Manager
{
public:
  explicit AMI_Manager (CORBA::ORB_ptr orb);

  /// @name Skeleton methods
  /// @{
  void start_workers (CORBA::Short worker_count,
                      CORBA::Long milliseconds,
                      Test::Controller_ptr controller) override;
  void shutdown () override;
  /// @}

  AMI_Manager (AMI_Manager const &) = delete;
  AMI_Manager (AMI_Manager &&) = delete;
  AMI_Manager &operator= (AMI_Manager const &) = delete;
  AMI_Manager &operator= (AMI_Manager &&) = delete;

private:
  /// The ORB used to shut down the application.
  CORBA::ORB_var orb_;
};

class Worker : public ACE_Task_Base
{
public:
  Worker (ACE_Thread_Manager *thr_mgr,
          Test::Controller_ptr controller,
          CORBA::Long milliseconds,
          CORBA::ORB_ptr orb);

  /// Run the worker thread.
  int svc () override;

  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

private:
  /// Controller to report worker progress.
  Test::Controller_var controller_;

  /// Number of milliseconds to run.
  CORBA::Long milliseconds_;

  /// ORB used for asynchronous calls.
  CORBA::ORB_var orb_;
};

class Controller_Handler
  : public virtual POA_Test::AMI_ControllerHandler
{
public:
  Controller_Handler (TAO_SYNCH_MUTEX *mutex,
                      int *pending_replies);

  void worker_started () override;
  void worker_started_excep (::Messaging::ExceptionHolder *holder) override;
  void worker_finished () override;
  void worker_finished_excep (::Messaging::ExceptionHolder *holder) override;

  Controller_Handler (Controller_Handler const &) = delete;
  Controller_Handler (Controller_Handler &&) = delete;
  Controller_Handler &operator= (Controller_Handler const &) = delete;
  Controller_Handler &operator= (Controller_Handler &&) = delete;

private:
  /// Non-owning pointer to the mutex owned by the worker.
  TAO_SYNCH_MUTEX * const mutex_;

  /// Non-owning pointer to the worker's pending-reply counter.
  /// The worker decrements it after each reply and uses the mutex to
  /// protect access to the counter.
  int * const pending_replies_;
};

#endif /* LONGUPCALLS_AMI_MANAGER_H */
