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
#include "ace/pre.h"
#define TAO_MESSAGING_POLICY_I_H

#include "tao/orbconf.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/MessagingS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Export TAO_RelativeRoundtripTimeoutPolicy : public TAO_RefCountServantBase,
                                                      public POA_Messaging::RelativeRoundtripTimeoutPolicy
{
  // = TITLE
  //   Messaging::RelativeRoundtripTimeoutPolicy implementation
  //
  // = DESCRIPTION
  //   This policy controls the total (round-trip) timeout time for a
  //   request.
  //
public:
  TAO_RelativeRoundtripTimeoutPolicy (PortableServer::POA_ptr poa,
                                      const TimeBase::TimeT& relative_expiry);
  // Constructor.

  TAO_RelativeRoundtripTimeoutPolicy (const TAO_RelativeRoundtripTimeoutPolicy &rhs);
  // Copy constructor.

  static CORBA::Policy_ptr create (PortableServer::POA_ptr poa,
                                   const CORBA::Any& val,
                                   CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Helper method for the implementation of
  // CORBA::ORB::create_policy.

  virtual TAO_RelativeRoundtripTimeoutPolicy *clone (void) const;
  // Returns a copy of <this>.

  // = The Messaging::RelativeRoundtripTimeoutPolicy methods
  virtual TimeBase::TimeT relative_expiry (CORBA::Environment &ACE_TRY_ENV);
  virtual TimeBase::TimeT relative_expiry (void);

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  void set_time_value (ACE_Time_Value &time_value);
  // Change the CORBA representation to the ACE representation.

private:
  PortableServer::POA_var poa_;
  // The default poa

  TimeBase::TimeT relative_expiry_;
  // The attribute
};

////////////////////////////////////////////////////////////////////////////////

class TAO_Export TAO_Sync_Scope_Policy : public TAO_RefCountServantBase,
                                         public POA_Messaging::SyncScopePolicy

{
  // = TITLE
  //   Messaging::SyncScopePolicy implementation.
  //
  // = DESCRIPTION
  //   This policy controls the sync strategy used by the ORB to
  //   transport requests.
  //
public:
  TAO_Sync_Scope_Policy (Messaging::SyncScope synchronization,
                         PortableServer::POA_ptr poa);
  // Constructor.

  TAO_Sync_Scope_Policy (const TAO_Sync_Scope_Policy &rhs);
  // Copy constructor.

  static CORBA::Policy_ptr create (PortableServer::POA_ptr poa,
                                   const CORBA::Any& val,
                                   CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Helper method for the implementation of
  // CORBA::ORB::create_policy.

  virtual TAO_Sync_Scope_Policy *clone (void) const;
  // Returns a copy of <this>.

  // = The Messaging::SyncScopePolicy methods.

  virtual Messaging::SyncScope synchronization (CORBA::Environment &ACE_TRY_ENV);
  virtual Messaging::SyncScope synchronization (void);

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

private:

  Messaging::SyncScope synchronization_;
  // The attribute

  PortableServer::POA_var poa_;
  // The default poa.
};

#if defined (__ACE_INLINE__)
#include "tao/Messaging_Policy_i.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#include "ace/post.h"
#endif /* TAO_MESSAGING_POLICY_I_H */
