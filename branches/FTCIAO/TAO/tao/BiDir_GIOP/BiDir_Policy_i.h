// -*- C++ -*-

// ===================================================================
/**
 *  @file BiDir_Policy_i.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_BIDIR_POLICY_I_H
#define TAO_BIDIR_POLICY_I_H
#include /**/ "ace/pre.h"
#include "tao/BiDir_GIOP/bidirgiop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_BidirectionalPolicy
 *
 * @brief  Implementation of the BiDirPolicy::BidirectionalPolicy
 *
 * This policy controls the whether the connections established  by the
 * clients can be made bi-directional or not. Further, this policy
 * also needs to be set by the server to use the connections
 * established by the clients to send requests.
 */
class TAO_BidirectionalPolicy
  : public virtual BiDirPolicy::BidirectionalPolicy
  , public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  TAO_BidirectionalPolicy (const BiDirPolicy::BidirectionalPolicyValue val);

  /// Copy constructor.
  TAO_BidirectionalPolicy (const TAO_BidirectionalPolicy &rhs);

  /// Returns a copy of <this>.
  virtual TAO_BidirectionalPolicy *clone (void) const;

  /// = The BiDir::BidirectionalPolicy methods
  virtual BiDirPolicy::BidirectionalPolicyValue value (void);

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:

  /// The attribute
  BiDirPolicy::BidirectionalPolicyValue value_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_BIDIR_POLICY_I_H */
