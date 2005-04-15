// -*- C++ -*-

//=============================================================================
/**
 *  @file CodecFactory_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_CODEC_FACTORY_ORB_INITIALIZER_H
#define TAO_CODEC_FACTORY_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "ORBInitializerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/**
 * @class TAO_CodecFactory_ORBInitializer
 *
 * @brief ORBInitializer for the CodecFactory support.
 *
 * This class simply registers the CodecFactory object with the ORB
 * resolve_initial_references() mechanism.
 */
class TAO_Export TAO_CodecFactory_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
public:

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_CODEC_FACTORY_ORB_INITIALIZER_H */
