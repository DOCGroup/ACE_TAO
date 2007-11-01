// -*- C++ -*-

//=============================================================================
/**
 *  @file PI_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_PI_ORB_INITIALIZER_H
#define TAO_PI_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "tao/PI/PI_includeC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward Declarations
class TAO_PI_PolicyFactory;

/// PortableServer ORB initializer.
class TAO_PI_Export TAO_PI_ORBInitializer
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
  void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  void post_init (PortableInterceptor::ORBInitInfo_ptr info);

  //@}

private:

  /// Register PortableInterceptor policy factories.
  void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info);

  /// Instance of the PI policy factory.
  /**
   * The PI policy factory is stateless and reentrant, so share
   * a single instance between all ORBs.
   */
  PortableInterceptor::PolicyFactory_var policy_factory_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_ORB_INITIALIZER_H */
