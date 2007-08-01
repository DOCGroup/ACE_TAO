// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/ThreeTier
//
// = FILENAME
//    Sink_i.h
//
// = DESCRIPTION
//    This class implements the ThreeTier IDL interface.
//    It acts as the "end of the line" for tick and tock
//    requests.
//
// = AUTHOR
//    Dale Wilson <wilsond@ociweb.com>
//
// ============================================================================

#ifndef SINK_I_H
#define SINK_I_H

#include "ThreeTierS.h"

class Sink_i: public POA_ThreeTier
{
public:
  /// Construct
  /// @param an initialized ORB
  Sink_i (CORBA::ORB_ptr orb);

  ~Sink_i ();
  // Destructor

  bool parse_args (int & argc, char * argv[]);

  ///Implement ThreeTier methods
  virtual void tick ()
    ACE_THROW_SPEC ((::CORBA::SystemException));
  virtual void tock ()
    ACE_THROW_SPEC ((::CORBA::SystemException));
  virtual ::CORBA::ULong tickCount ()
    ACE_THROW_SPEC ((::CORBA::SystemException));
  virtual ::CORBA::ULong tockCount ()
    ACE_THROW_SPEC ((::CORBA::SystemException));
  virtual void shutdown ()
    ACE_THROW_SPEC ((::CORBA::SystemException));

private:
  CORBA::ORB_var orb_;

  unsigned long tickCount_;
  unsigned long tockCount_;
};

#endif /* SINK_I_H */
