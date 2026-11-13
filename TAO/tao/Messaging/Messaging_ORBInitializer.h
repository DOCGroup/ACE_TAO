// -*- C++ -*-

/**
 *  @file     Messaging_ORBInitializer.h
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */

#ifndef TAO_MESSAGING_ORB_INITIALIZER_H
#define TAO_MESSAGING_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/Messaging/messaging_export.h"

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
class TAO_Messaging_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual ::CORBA::LocalObject
{
public:
  TAO_Messaging_ORBInitializer () = default;

  TAO_Messaging_ORBInitializer (TAO_Messaging_ORBInitializer const &) = delete;
  TAO_Messaging_ORBInitializer &operator= (TAO_Messaging_ORBInitializer const &) = delete;
  TAO_Messaging_ORBInitializer (TAO_Messaging_ORBInitializer &&) = delete;
  TAO_Messaging_ORBInitializer &operator= (TAO_Messaging_ORBInitializer &&) = delete;

  /**
   * @name PortableInterceptor::ORBInitializer Methods
   *
   * The following methods are required by the
   * PortableInterceptor::ORBInitializer interface.
   */
  //@{
  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  //@}

private:
  /// Register Messaging policy factories.
  void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info);

  /// Register ExceptionHolder value factory
  void register_value_factory (PortableInterceptor::ORBInitInfo_ptr info);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_MESSAGING_ORB_INITIALIZER_H */
