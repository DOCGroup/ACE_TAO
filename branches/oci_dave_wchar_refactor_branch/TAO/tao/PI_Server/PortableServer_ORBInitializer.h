// -*- C++ -*-

//=============================================================================
/**
 *  @file PortableServer_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali <irfan@oomworks.com>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_ORB_INITIALIZER_H
#define TAO_PORTABLESERVER_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// PortableServer ORB initializer.
class TAO_PortableServer_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /**
   * @name PortableInterceptor::ORBInitializer methods
   *
   * The following methods are required by the
   * PortableInterceptor::ORBInitializer interface.
   */
  //@{

  void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void post_init (PortableInterceptor::ORBInitInfo_ptr info
                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

private:

  /// Register PortableServer policy factories.
  void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info
                                  ACE_ENV_ARG_DECL);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_ORB_INITIALIZER_H */
