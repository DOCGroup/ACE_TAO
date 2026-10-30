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
  Checkpoint_i () = default;
  Checkpoint_i (Checkpoint_i const&) = delete;
  Checkpoint_i (Checkpoint_i&&) = delete;
  Checkpoint_i& operator= (Checkpoint_i const&) = delete;
  Checkpoint_i& operator= (Checkpoint_i&&) = delete;

  /// Destructor.
  ~Checkpoint_i () override = default;

  void put_event (Event *event) override;

  Event_List *get_critical_events () override;

  /// Shutdown the server.
  void shutdown () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;
};

#endif /* ECHO_I_H */

