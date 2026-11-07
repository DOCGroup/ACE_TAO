
#ifndef LONGUPCALLS_MANAGER_H
#define LONGUPCALLS_MANAGER_H

#include "TestS.h"
#include "ace/Task.h"

class Manager
  : public virtual POA_Test::Manager
{
public:
  explicit Manager (CORBA::ORB_ptr orb);
  // Constructor

  // = The skeleton methods
  void start_workers (CORBA::Short worker_count,
                      CORBA::Long milliseconds,
                      Test::Controller_ptr controller) override;
  void shutdown () override;

  Manager (Manager const &) = delete;
  Manager (Manager &&) = delete;
  Manager &operator= (Manager const &) = delete;
  Manager &operator= (Manager &&) = delete;

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
          CORBA::Long milliseconds);

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
};

#endif /* LONGUPCALLS_MANAGER_H */
