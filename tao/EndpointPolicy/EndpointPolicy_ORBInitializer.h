// -*- C++ -*-

//=============================================================================
/**
 *  @file EndpointPolicy_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================


#ifndef TAO_ENDPOINTPOLICY_ORB_INITIALIZER_H
#define TAO_ENDPOINTPOLICY_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/EndpointPolicy/EndpointPolicy_Export.h"

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

/// EndpointPolicy ORB initializer.
class TAO_EndpointPolicy_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /**
   * @name PortableInterceptor::ORBInitializer Methods
   *
   * The following methods are required by the
   * PortableInterceptor::ORBInitializer interface.
   */
  //@{

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         );

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          );

  //@}

private:

  /// Register Endpoint policy factories.
  void register_policy_factories (
    PortableInterceptor::ORBInitInfo_ptr info
   );

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_ENDPOINTPOLICY_ORB_INITIALIZER_H */
