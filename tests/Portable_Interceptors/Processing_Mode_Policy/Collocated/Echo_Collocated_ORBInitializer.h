// -*- C++ -*-
//
// $Id$

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
class Echo_Collocated_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
{
public:

  Echo_Collocated_ORBInitializer
    (PortableInterceptor::ProcessingMode server_proc_mode,
     PortableInterceptor::ProcessingMode client_proc_mode);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:

  PortableInterceptor::ProcessingMode server_processing_mode_;
  PortableInterceptor::ProcessingMode client_processing_mode_;
};

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */


#include /**/ "ace/post.h"

#endif /* TAO_ECHO_CLIENT_ORB_INITIALIZER_H */
