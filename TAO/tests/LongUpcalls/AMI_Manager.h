// $Id$

#ifndef LONGUPCALLS_AMI_MANAGER_H
#define LONGUPCALLS_AMI_MANAGER_H

#include "TestS.h"
#include "ace/Task.h"

class AMI_Manager
  : public virtual POA_Test::Manager
{
public:
  AMI_Manager (CORBA::ORB_ptr orb);
  // Constructor

  // = The skeleton methods
  virtual void start_workers (CORBA::Short worker_count,
                              CORBA::Long milliseconds,
                              Test::Controller_ptr controller)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

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

  virtual int svc (void);
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
  :  public virtual POA_Test::AMI_ControllerHandler
{
public:
  Controller_Handler (TAO_SYNCH_MUTEX *mutex,
                      int *pending_replies);
  // Constructor

  virtual void worker_started (void)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void worker_started_excep (::Messaging::ExceptionHolder*)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void worker_finished (void)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void worker_finished_excep (::Messaging::ExceptionHolder*)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  TAO_SYNCH_MUTEX *mutex_;
  int *pending_replies_;
  // The pending replies is decremented each time a reply is
  // received.
  // The mutex is used to protect the access to the flag.
};

#endif /* LONGUPCALLS_AMI_MANAGER_H */
