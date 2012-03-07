// -*- C -*-

//=============================================================================
/**
 *  @file ZIOP_ORBInitializer.h
 *
 *  $Id$
 *
*  @author  Johnny Willemsen
 */
//=============================================================================


#ifndef TAO_ZIOP_ORB_INITIALIZER_H
#define TAO_ZIOP_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/ZIOP/ziop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ZIOP_Loader;

/// ZIOP ORB initializer.
class TAO_ZIOP_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual ::CORBA::LocalObject
{
public:
  TAO_ZIOP_ORBInitializer (TAO_ZIOP_Loader* loader);

  /**
   * @name PortableInterceptor::ORBInitializer Methods
   *
   * The following methods are required by the
   * PortableInterceptor::ORBInitializer interface.
   */
  //@{
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
  //@}

private:

  /// Register policy factories.
  void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info);

  TAO_ZIOP_Loader* loader_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif

#include /**/ "ace/post.h"

#endif /* TAO_ZIOP_ORB_INITIALIZER_H */

