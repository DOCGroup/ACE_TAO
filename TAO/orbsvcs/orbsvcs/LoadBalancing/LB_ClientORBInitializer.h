// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_ClientORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_CLIENT_ORB_INITIALIZER_H
#define TAO_LB_CLIENT_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LB_ORBInitializer
 *
 * @brief ORBInitializer for the LB_ClientComponent Service_Object.
 *
 * This ORBInitializer simply creates and registers with the ORB the
 * ClientRequestInterceptor that handles injection of the
 * CosLoadBalancing::LOAD_MANAGED ServiceContext into each request's
 * ServiceContextList.
 */
class TAO_LB_ClientORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /**
   * @name PortableInterceptor::ORBInitializer Methods
   *
   * Methods required by the PortableInterceptor::ORBInitializer
   * interface.
   */
  //@{
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
  //@}

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_LB_CLIENT_ORB_INITIALIZER_H */
