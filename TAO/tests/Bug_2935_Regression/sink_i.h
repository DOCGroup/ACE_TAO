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
  Sink_i (CORBA::ORB_ptr orb);

  /// Destructor
  ~Sink_i () override = default;

  bool parse_args (int &argc, ACE_TCHAR *argv[]);

  ///Implement ThreeTier methods
  void tick () override;
  void tock () override;
  ::CORBA::ULong tickCount () override;
  ::CORBA::ULong tockCount () override;
  void shutdown () override;

private:
  CORBA::ORB_var orb_;

  unsigned long tickCount_;
  unsigned long tockCount_;
};

#endif /* SINK_I_H */
