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

  bool parse_args (int & argc, ACE_TCHAR * argv[]);

  ///Implement ThreeTier methods
  virtual void tick ();
  virtual void tock ();
  virtual ::CORBA::ULong tickCount ();
  virtual ::CORBA::ULong tockCount ();
  virtual void shutdown ();

private:
  CORBA::ORB_var orb_;

  unsigned long tickCount_;
  unsigned long tockCount_;
};

#endif /* SINK_I_H */
