// -*- C++ -*-

//=============================================================================
/**
 *  @file PortableServer_ORBInitializer.h
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
    public virtual ::CORBA::LocalObject
{
public:
  TAO_PortableServer_ORBInitializer () = default;

  /**
   * @name PortableInterceptor::ORBInitializer methods
   *
   * The following methods are required by the
   * PortableInterceptor::ORBInitializer interface.
   */
  //@{
  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;
  //@}

private:
  TAO_PortableServer_ORBInitializer (TAO_PortableServer_ORBInitializer const &) = delete;
  TAO_PortableServer_ORBInitializer &operator= (TAO_PortableServer_ORBInitializer const &) = delete;
  TAO_PortableServer_ORBInitializer (TAO_PortableServer_ORBInitializer &&) = delete;
  TAO_PortableServer_ORBInitializer &operator= (TAO_PortableServer_ORBInitializer &&) = delete;

  /// Register PortableServer policy factories.
  void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_ORB_INITIALIZER_H */
