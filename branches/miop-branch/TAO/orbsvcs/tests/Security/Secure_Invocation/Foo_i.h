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
  virtual void baz (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Foo::Bar::NoSecurityAttributes));

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Reference to the ORB.
  CORBA::ORB_var orb_;

  /// Reference to the "SecurityCurrent" object.
  SecurityLevel2::Current_var current_;

};

#endif  /* SECURE_INVOCATION_FOO_I_H */
