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
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

#include "tao/TAOC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Export TAO_Buffering_Constraint_Policy : public TAO::BufferingConstraintPolicy
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
  TAO_Buffering_Constraint_Policy (const TAO::BufferingConstraint &buffering_constraint);
  // Constructor.

  TAO_Buffering_Constraint_Policy (const TAO_Buffering_Constraint_Policy &rhs);
  // Copy constructor.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  static CORBA::Policy_ptr create (const CORBA::Any& val,
                                   CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Helper method for the implementation of CORBA::ORB::create_policy.

  virtual TAO_Buffering_Constraint_Policy *clone (void) const;
  // Returns a copy of <this>.

  // = The TAO::Buffering_Constraint_Policy methods

  virtual TAO::BufferingConstraint buffering_constraint (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC (());
  void get_buffering_constraint (TAO::BufferingConstraint &) const;

  virtual CORBA::PolicyType policy_type (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

private:
  TAO::BufferingConstraint buffering_constraint_;
  // The attribute
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "tao/Buffering_Constraint_Policy.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#include "ace/post.h"
#endif /* TAO_BUFFERING_CONSTRAINT_POLICY_H */
