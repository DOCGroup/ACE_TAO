#ifndef LONGUPCALLS_CONTROLLER_H
#define LONGUPCALLS_CONTROLLER_H

#include "TestS.h"

class Controller
  : public virtual POA_Test::Controller
{
public:
  Controller ();
  // Constructor

  void dump_results ();
  // Print out the results and any errors

  // = The skeleton methods
  virtual void worker_started ();
  virtual void worker_finished ();

private:
  TAO_SYNCH_MUTEX mutex_;
  CORBA::ULong start_count_;
  CORBA::ULong finish_count_;
};

#endif /* LONGUPCALLS_CONTROLLER_H */
