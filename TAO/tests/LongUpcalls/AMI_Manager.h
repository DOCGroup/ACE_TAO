#ifndef LONGUPCALLS_AMI_MANAGER_H
#define LONGUPCALLS_AMI_MANAGER_H

#include "TestS.h"
#include "ace/Task.h"

class AMI_Manager
  : public virtual POA_Test::Manager
{
public:
  explicit AMI_Manager (CORBA::ORB_ptr orb);
  // Constructor

  // = The skeleton methods
  void start_workers (CORBA::Short worker_count,
                      CORBA::Long milliseconds,
                      Test::Controller_ptr controller) override;
  void shutdown () override;

  AMI_Manager (AMI_Manager const &) = delete;
  AMI_Manager (AMI_Manager &&) = delete;
  AMI_Manager &operator= (AMI_Manager const &) = delete;
  AMI_Manager &operator= (AMI_Manager &&) = delete;

private:
  CORBA::ORB_var orb_;
  // The ORB, to shutdown
};

// ****************************************************************

class Worker : public ACE_Task_Base
{
public:
  Worker (ACE_Thread_Manager *thr_mgr,
          Test::Controller_ptr controller,
          CORBA::Long milliseconds,
          CORBA::ORB_ptr orb);

  int svc () override;

  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;
  // The service method

private:
  Test::Controller_var controller_;
  // The controller pointer

  CORBA::Long milliseconds_;
  // The number of milliseconds.

  CORBA::ORB_var orb_;
  // The orb
};

// ****************************************************************

class Controller_Handler
  : public virtual POA_Test::AMI_ControllerHandler
{
public:
  Controller_Handler (TAO_SYNCH_MUTEX *mutex,
                      int *pending_replies);
  // Constructor

  void worker_started () override;
  void worker_started_excep (::Messaging::ExceptionHolder *holder) override;
  void worker_finished () override;
  void worker_finished_excep (::Messaging::ExceptionHolder *holder) override;

  Controller_Handler (Controller_Handler const &) = delete;
  Controller_Handler (Controller_Handler &&) = delete;
  Controller_Handler &operator= (Controller_Handler const &) = delete;
  Controller_Handler &operator= (Controller_Handler &&) = delete;

private:
  TAO_SYNCH_MUTEX * const mutex_; // Non-owning; the worker owns the mutex.
  int * const pending_replies_; // Non-owning; the worker owns the counter.
  // The pending replies is decremented each time a reply is
  // received.
  // The mutex is used to protect the access to the flag.
};

#endif /* LONGUPCALLS_AMI_MANAGER_H */
