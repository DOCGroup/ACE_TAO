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

#include "Event_TypesS.h"


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
        TAO_ENV_ARG_DECL
  )
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Event_List * get_critical_events (
        TAO_ENV_SINGLE_ARG_DECL
  )
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.
};

#endif /* ECHO_I_H */

