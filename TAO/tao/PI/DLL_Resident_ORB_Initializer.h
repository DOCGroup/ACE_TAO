// -*- C++ -*-

//=============================================================================
/**
 *  @file DLL_Resident_ORB_Initializer.h
 *
 *  $Id$
 *
 *  @author Dale Wilson <wilsond@ociweb.com>
 */
//=============================================================================


#ifndef PI_DLL_RESIDENT_ORB_INITIALIZER_H
#define PI_DLL_RESIDENT_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/DLL.h"
#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
//#include "tao/PI/ORBInitializerC.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableInterceptor
{

  /**
   * @class DLL_Resident_ORB_Initializer
   *
   * Decorator for an ORBInitializer implemented in a DLL
   *
   * Ensures that the DLL stays loaded while the ORBInitializer exists.
   */
  class TAO_PI_Export DLL_Resident_ORB_Initializer
  : public virtual PortableInterceptor::ORBInitializer
  {
    public:
      DLL_Resident_ORB_Initializer (
        PortableInterceptor::ORBInitializer_ptr initializer,
        const ACE_TCHAR * dll_name);
      virtual ~DLL_Resident_ORB_Initializer ();

      virtual void pre_init (
        ::PortableInterceptor::ORBInitInfo_ptr info
      )
      throw (
        ::CORBA::SystemException
      );

    virtual void post_init (
        ::PortableInterceptor::ORBInitInfo_ptr info
      )
      throw (
        ::CORBA::SystemException
      );

  private:
    PortableInterceptor::ORBInitializer_var initializer_;
#if 1 // to re-inject the CSD vs SSLIOP bug, make this a zero
    ACE_DLL dll_;
#else // CSD vs SSLIOP
    const ACE_TCHAR * dll_;
#endif // CSD vs SSLIOP
};
}
TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* PI_DLL_RESIDENT_ORB_INITIALIZER_H */




