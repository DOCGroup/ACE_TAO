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

#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "rtscheduler_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"


// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_RTScheduler_Current;

/// RTCORBA ORB initializer.
class TAO_RTScheduler_Export TAO_RTScheduler_ORB_Initializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
{
public:

  

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
private:
	TAO_RTScheduler_Current* current_;
  
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_RTSCHEDULER__INITIALIZER_H */
