// -*- C++ -*-

//=============================================================================
/**
 * @file Foo_i.h
 *
 * $Id$
 *
 * Implementation header for the Secure_Invocation test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef SECURE_INVOCATION_FOO_I_H
#define SECURE_INVOCATION_FOO_I_H

#include "FooS.h"
#include "orbsvcs/SecurityLevel2C.h"

class Foo_i : public virtual POA_Foo::Bar
{
public:

  /// Constructor.
  Foo_i (CORBA::ORB_ptr,
         SecurityLevel2::Current_ptr current);

  /// Test method.
  virtual void baz (CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Reference to the ORB.
  CORBA::ORB_var orb_;

  /// Reference to the "SecurityCurrent" object.
  SecurityLevel2::Current_var current_;

};

#endif  /* SECURE_INVOCATION_FOO_I_H */
