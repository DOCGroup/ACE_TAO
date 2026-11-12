// -*- C++ -*-

/**
 *  @file   Buffering_Constraint_Policy.h
 *
 *  @author Irfan Pyarali (irfan@cs.wustl.edu)
 */

#ifndef TAO_BUFFERING_CONSTRAINT_POLICY_H
#define TAO_BUFFERING_CONSTRAINT_POLICY_H

#include /**/ "ace/pre.h"

#include "tao/Messaging/TAO_ExtC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Buffering_Constraint_Policy
 *
 * @brief Implementation class for TAO-specific Buffering Constraint Policy.
 *
 * This policy is used to control the buffering of requests by the ORB.
 */
class TAO_Buffering_Constraint_Policy
  : public TAO::BufferingConstraintPolicy,
    public ::CORBA::LocalObject
{
public:
  explicit TAO_Buffering_Constraint_Policy (TAO::BufferingConstraint const &buffering_constraint);

  TAO_Buffering_Constraint_Policy (TAO_Buffering_Constraint_Policy const &rhs);
  TAO_Buffering_Constraint_Policy &operator= (TAO_Buffering_Constraint_Policy const &) = delete;
  TAO_Buffering_Constraint_Policy (TAO_Buffering_Constraint_Policy &&) = delete;
  TAO_Buffering_Constraint_Policy &operator= (TAO_Buffering_Constraint_Policy &&) = delete;

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (CORBA::Any const &val);

  /// Returns a copy of @c this.
  virtual TAO_Buffering_Constraint_Policy *clone () const;

  TAO::BufferingConstraint buffering_constraint () override;

  void get_buffering_constraint (TAO::BufferingConstraint &) const;

  CORBA::PolicyType policy_type () override;

  CORBA::Policy_ptr copy () override;

  void destroy () override;

  /// Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type () const override;

private:
  /// The attribute
  TAO::BufferingConstraint buffering_constraint_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "tao/Messaging/Buffering_Constraint_Policy.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#include /**/ "ace/post.h"
#endif /* TAO_BUFFERING_CONSTRAINT_POLICY_H */
