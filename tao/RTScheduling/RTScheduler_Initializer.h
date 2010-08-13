// -*- C++ -*-

//=============================================================================
/**
 *  @file RTScheduler_Initializer.h
 *
 *  $Id$
 *
 *  @author Yamuna Krishnamurthy <yamuna@oomworks.com>
 */
//=============================================================================


#ifndef TAO_RTSCHEDULER_INITIALIZER_H
#define TAO_RTSCHEDULER_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"
#include "tao/RTScheduling/rtscheduler_export.h"
#include "tao/RTScheduling/Current.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI_Server/PI_Server.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// RTCORBA ORB initializer.
class TAO_RTScheduler_ORB_Initializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual ::CORBA::LocalObject
{
public:
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:
  TAO_RTScheduler_Current_var current_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_RTSCHEDULER_INITIALIZER_H */
