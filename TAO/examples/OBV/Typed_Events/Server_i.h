// -*- C++ -*-

//=============================================================================
/**
 *  @file    Server_i.h
 *
 *  This class implements the Event_Types IDL interface.
 *
 *  @author Torsten Kuepper derived from the Echo example TAO/example/Simple/echo of Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef ECHO_I_H
#define ECHO_I_H

#include "Event_TypesS.h"


class Checkpoint_i : public POA_Checkpoint
{
public:
  /// Constructor.
  Checkpoint_i ();

  /// Destructor.
  ~Checkpoint_i ();

  virtual void put_event (
        Event * e
  );

  virtual Event_List * get_critical_events (
  );

  /// Shutdown the server.
  virtual void shutdown ();

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;
};

#endif /* ECHO_I_H */

