// -*- C++ -*-
#ifndef TAO_ECHO_CLIENT_ORB_INITIALIZER_H
#define TAO_ECHO_CLIENT_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/PI/ProcessingModePolicyC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Echo client ORB initializer.
class Echo_Client_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:
  explicit Echo_Client_ORBInitializer (
      PortableInterceptor::ProcessingMode proc_mode);
  Echo_Client_ORBInitializer (Echo_Client_ORBInitializer const &) = delete;
  Echo_Client_ORBInitializer (Echo_Client_ORBInitializer &&) = delete;
  Echo_Client_ORBInitializer &operator= (Echo_Client_ORBInitializer const &) = delete;
  Echo_Client_ORBInitializer &operator= (Echo_Client_ORBInitializer &&) = delete;

  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;

private:
  PortableInterceptor::ProcessingMode const processing_mode_;
};

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_ECHO_CLIENT_ORB_INITIALIZER_H */
