// $Id$

#ifndef LONGUPCALLS_CONTROLLER_H
#define LONGUPCALLS_CONTROLLER_H

#include "TestS.h"

class Controller
  : public virtual POA_Test::Controller
{
public:
  Controller (void);
  // Constructor

  void dump_results (void);
  // Print out the results and any errors

  // = The skeleton methods
  virtual void worker_started (void)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void worker_finished (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  TAO_SYNCH_MUTEX mutex_;
  CORBA::ULong start_count_;
  CORBA::ULong finish_count_;
};

#endif /* LONGUPCALLS_CONTROLLER_H */
