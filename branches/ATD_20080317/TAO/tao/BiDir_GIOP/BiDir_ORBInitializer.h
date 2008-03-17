// -*- C++ -*-
//
// ===================================================================
/**
 *  @file BiDir_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================
#ifndef TAO_BIDIR_ORB_INITIALIZER_H
#define TAO_BIDIR_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/BiDir_GIOP/bidirgiop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Messaging ORB initializer.
class TAO_BiDir_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual TAO_Local_RefCounted_Object
{
public:

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:

  /// Register BiDir policy factories.
  void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_BIDIR_ORB_INITIALIZER_H */
