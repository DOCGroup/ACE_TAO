// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/OBV/Typed_Events
//
// = FILENAME
//    Server_i.h
//
// = DESCRIPTION
//    This class implements the Event_Types IDL interface.
//
// = AUTHOR
//    Torsten Kuepper
//    derived from the Echo example TAO/example/Simple/echo
//    of Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef ECHO_I_H
#define ECHO_I_H

#include "OBVS.h"


class Checkpoint_i : public POA_Checkpoint
{
public:
  // = Initialization and termination methods.
  Checkpoint_i (void);
  // Constructor.

  ~Checkpoint_i (void);
  // Destructor.

  virtual void put_event (
        Event * e
  );

  virtual void shutdown (
    );

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.
};

#endif /* ECHO_I_H */

