/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO
//
// = DESCRIPTION
//   Implementation classes for the Messaging related policies
//
// = FILENAME
//   Messaging_Policy_i.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_MESSAGING_POLICY_I_H
#define TAO_MESSAGING_POLICY_I_H

#include "MessagingS.h"

#if defined (TAO_HAS_CORBA_MESSAGING)

#if defined(_MSC_VER)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Export TAO_RelativeRoundtripTimeoutPolicy_i : public POA_Messaging::RelativeRoundtripTimeoutPolicy, public TAO_RefCountServantBase
{
  // = TITLE
  //   Messaging::RelativeRoundtripTimeoutPolicy implementation
  //
  // = DESCRIPTION
  //   This policy controls the total (round-trip) timeout time for a
  //   request.
  //
public:
  TAO_RelativeRoundtripTimeoutPolicy_i (PortableServer::POA_ptr poa,
                                      const TimeBase::TimeT& relative_expiry);
  // Constructor

  static CORBA::Policy_ptr create (
      PortableServer::POA_ptr poa,
      const CORBA::Any& val,
      CORBA::Environment &ACE_TRY_ENV =
      CORBA::default_environment ()
    );
  // Helper method for the implementation of CORBA::ORB::create_policy

  // = The Messaging::RelativeRoundtripTimeoutPolicy methods
  virtual TimeBase::TimeT relative_expiry (
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::Environment::default_environment ()
    );
  virtual CORBA::PolicyType policy_type (
      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ()
    );
  virtual CORBA::Policy_ptr copy (
      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ()
    );
  virtual void destroy (
      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ()
    );
  virtual PortableServer::POA_ptr _default_POA (
      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ()
    );

private:
  PortableServer::POA_var poa_;
  // The default poa

  TimeBase::TimeT relative_expiry_;
  // The attribute
};

#if defined (__ACE_INLINE__)
#include "tao/Messaging_Policy_i.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER)
#pragma warning(default:4250)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING */

#endif /* TAO_MESSAGING_POLICY_I_H */
