/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO
//
// = DESCRIPTION
//   Implementation class for TAO-specific Client Priority Policy.
//
// = FILENAME
//   Client_Priority_Policy.h
//
// = AUTHOR
//   Marina Spivak (marina@cs.wustl.edu)
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//   Ossama Othman (othman@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_CLIENT_PRIORITY_POLICY_H
#define TAO_CLIENT_PRIORITY_POLICY_H

#include "TAOS.h"

#if defined(_MSC_VER)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Export TAO_Client_Priority_Policy : public virtual PortableServer::RefCountServantBase,
                                              public virtual POA_TAO::ClientPriorityPolicy
{
  // = TITLE
  //
  //
  // = DESCRIPTION
  //
  //
public:
  TAO_Client_Priority_Policy (PortableServer::POA_ptr poa,
                              const TAO::PrioritySpecification &priority_spec);
  // Constructor

  static CORBA::Policy_ptr create (
      PortableServer::POA_ptr poa,
      const CORBA::Any& val,
      CORBA::Environment &ACE_TRY_ENV =
      TAO_default_environment ()
    );
  // Helper method for the implementation of CORBA::ORB::create_policy

  // = The TAO::Client_Priority_Policy methods
  virtual TAO::PrioritySpecification priority_specification (
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::Environment::default_environment ()
    );
  virtual CORBA::PolicyType policy_type (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );
  virtual CORBA::Policy_ptr copy (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );
  virtual void destroy (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );
  virtual PortableServer::POA_ptr _default_POA (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

private:
  PortableServer::POA_var poa_;
  // The default poa

  TAO::PrioritySpecification priority_spec_;
  // The attribute
};

#if defined (__ACE_INLINE__)
#include "tao/Client_Priority_Policy.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER)
#pragma warning(default:4250)
#endif /* _MSC_VER */

#endif /* TAO_CLIENT_PRIORITY_POLICY_H */
