/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO
//
// = DESCRIPTION
//   Implementation class for TAO-specific Buffering Constraint Policy.
//
// = FILENAME
//   Buffering_Constraint_Policy.h
//
// = AUTHOR
//   Irfan Pyarali (irfan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_BUFFERING_CONSTRAINT_POLICY_H
#define TAO_BUFFERING_CONSTRAINT_POLICY_H

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING)

#include "tao/TAOS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Export TAO_Buffering_Constraint_Policy : public TAO_RefCountServantBase,
                                                   public POA_TAO::BufferingConstraintPolicy
{
  // = TITLE
  //     Implementation class for TAO-specific Buffering Constraint
  //     Policy.
  //
  // = DESCRIPTION
  //     This policy is used to control the buffering of requests by
  //     the ORB.
  //
public:
  TAO_Buffering_Constraint_Policy (const TAO::BufferingConstraint &buffering_constraint,
                                   PortableServer::POA_ptr poa);
  // Constructor.

  TAO_Buffering_Constraint_Policy (const TAO_Buffering_Constraint_Policy &rhs);
  // Copy constructor.

  static CORBA::Policy_ptr create (PortableServer::POA_ptr poa,
                                   const CORBA::Any& val,
                                   CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Helper method for the implementation of CORBA::ORB::create_policy.

  virtual TAO_Buffering_Constraint_Policy *clone (void) const;
  // Returns a copy of <this>.

  // = The TAO::Buffering_Constraint_Policy methods

  virtual TAO::BufferingConstraint buffering_constraint (CORBA::Environment &ACE_TRY_ENV);
  virtual TAO::BufferingConstraint buffering_constraint (void);

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

private:
  TAO::BufferingConstraint buffering_constraint_;
  // The attribute

  PortableServer::POA_var poa_;
  // The default poa
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING */

#if defined (__ACE_INLINE__)
#include "tao/Buffering_Constraint_Policy.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BUFFERING_CONSTRAINT_POLICY_H */
