#ifndef LONGUPCALLS_CONTROLLER_H
#define LONGUPCALLS_CONTROLLER_H

#include "TestS.h"

class Controller
  : public virtual POA_Test::Controller
{
public:
  Controller () = default;
  // Constructor

  void dump_results ();
  // Print out the results and any errors

  // = The skeleton methods
  void worker_started () override;
  void worker_finished () override;

  Controller (Controller const &) = delete;
  Controller (Controller &&) = delete;
  Controller & operator= (Controller const &) = delete;
  Controller & operator= (Controller &&) = delete;

private:
  TAO_SYNCH_MUTEX mutex_;
  CORBA::ULong start_count_ {};
  CORBA::ULong finish_count_ {};
};

#endif /* LONGUPCALLS_CONTROLLER_H */
