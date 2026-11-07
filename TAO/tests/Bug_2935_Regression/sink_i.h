// -*- C++ -*-

//=============================================================================
/**
 *  @file    sink_i.h
 *
 *  This class implements the ThreeTier IDL interface.
 *  It acts as the "end of the line" for tick and tock
 *  requests.
 *
 *  @author Dale Wilson <wilsond@ociweb.com>
 */
//=============================================================================


#ifndef SINK_I_H
#define SINK_I_H

#include "ThreeTierS.h"

class Sink_i : public POA_ThreeTier
{
public:
  /// Construct
  /// @param an initialized ORB
  explicit Sink_i (CORBA::ORB_ptr orb);
  Sink_i (Sink_i const &) = delete;
  Sink_i &operator= (Sink_i const &) = delete;
  Sink_i (Sink_i &&) = delete;
  Sink_i &operator= (Sink_i &&) = delete;

  ///Implement ThreeTier methods
  void tick () override;
  void tock () override;
  ::CORBA::ULong tickCount () override;
  ::CORBA::ULong tockCount () override;
  void shutdown () override;

private:
  CORBA::ORB_var const orb_;

  CORBA::ULong tickCount_ {};
  CORBA::ULong tockCount_ {};
};

#endif /* SINK_I_H */
