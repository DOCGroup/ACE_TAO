// -*- C++ -*-

#ifndef TAO_TWO_OBJECTS_WORKER_H
#define TAO_TWO_OBJECTS_WORKER_H

#include "tao/ORB.h"
#include "ace/Task.h"

//   Use the ACE_Task_Base class to run server threads
class Worker : public ACE_Task_Base
{
public:
  // Ctor
  explicit Worker (CORBA::ORB_ptr orb);

  // The thread entry point.
  int svc () override;

  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker & operator= (Worker const &) = delete;
  Worker & operator= (Worker &&) = delete;

private:
  // The orb
  CORBA::ORB_var orb_;
};

#endif  /* TAO_TWO_OBJECTS_WORKER_H */
