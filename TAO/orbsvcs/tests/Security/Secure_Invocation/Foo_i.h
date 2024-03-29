// -*- C++ -*-

//=============================================================================
/**
 * @file Foo_i.h
 *
 * Implementation header for the Secure_Invocation test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef SECURE_INVOCATION_FOO_I_H
#define SECURE_INVOCATION_FOO_I_H

#include "FooS.h"
#include "orbsvcs/SecurityLevel3C.h"

class Foo_i : public virtual POA_Foo::Bar
{
public:
  /// Constructor.
  Foo_i (CORBA::ORB_ptr,
         SecurityLevel3::SecurityCurrent_ptr current);

  /// Test method.
  virtual void baz ();

  virtual void shutdown ();

private:
  /// Reference to the ORB.
  CORBA::ORB_var orb_;

  /// Reference to the "SecurityCurrent" object.
  SecurityLevel3::SecurityCurrent_var current_;
};

#endif  /* SECURE_INVOCATION_FOO_I_H */
