// -*- C++ -*-

//=============================================================================
/**
 *  @file FTRT_ClientORB_Initializer.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================


#ifndef FTRT_CLIENTORB_INITIALIZER_H
#define FTRT_CLIENTORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// RTCORBA ORB initializer.
class FTRT_ClientORB_Initializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual ::CORBA::LocalObject
{
public:
  /// Priority mapping types
  FTRT_ClientORB_Initializer (int transaction_depth);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
private:
  PortableInterceptor::ClientRequestInterceptor_var client_interceptor_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */


#include /**/ "ace/post.h"

#endif /* FTRT_CLIENTORB_INITIALIZER_H */
