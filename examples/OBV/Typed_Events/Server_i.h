// -*- C++ -*-

//=============================================================================
/**
 *  @file    Server_i.h
 *
 *  $Id$
 *
 *  This class implements the Event_Types IDL interface.
 *
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
  // = Initialization and termination methods.
  /// Constructor.
  Checkpoint_i (void);

  /// Destructor.
  ~Checkpoint_i (void);

  virtual void put_event (
        Event * e
  );

  virtual Event_List * get_critical_events (
  );

  /// Shutdown the server.
  virtual void shutdown (void);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;
};

#endif /* ECHO_I_H */

