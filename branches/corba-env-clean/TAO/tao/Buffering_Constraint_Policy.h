/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Buffering_Constraint_Policy.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali (irfan@cs.wustl.edu)
 */
//=============================================================================


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

class TAO_Export TAO_Buffering_Constraint_Policy
  : public TAO::BufferingConstraintPolicy,
    public TAO_Local_RefCounted_Object
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
  /// Constructor.
  TAO_Buffering_Constraint_Policy (const TAO::BufferingConstraint &buffering_constraint);

  /// Copy constructor.
  TAO_Buffering_Constraint_Policy (const TAO_Buffering_Constraint_Policy &rhs);

  /// Helper method for the implementation of CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any& val
                                   TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Returns a copy of <this>.
  virtual TAO_Buffering_Constraint_Policy *clone (void) const;

  // = The TAO::Buffering_Constraint_Policy methods

  virtual TAO::BufferingConstraint buffering_constraint (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void get_buffering_constraint (TAO::BufferingConstraint &) const;

  virtual CORBA::PolicyType policy_type (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:
  /// The attribute
  TAO::BufferingConstraint buffering_constraint_;
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
