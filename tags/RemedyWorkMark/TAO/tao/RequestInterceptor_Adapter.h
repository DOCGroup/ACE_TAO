// -*- C++ -*-

//=============================================================================
/**
 *  @file    RequestInterceptor_Adapter.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef TAO_REQUEST_INTERCEPTOR_ADAPTER_H
#define TAO_REQUEST_INTERCEPTOR_ADAPTER_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

namespace TAO
{
  /**
   * @class RequestInterceptor_Adapter
   *
   * @brief A base helper class to invoke registered request
   *        interceptors
   */
  class TAO_Export RequestInterceptor_Adapter
  {
  public:
    virtual ~RequestInterceptor_Adapter (void);

    virtual void destroy_interceptors (void) = 0;

    virtual void popTSC (TAO_ORB_Core *orb_core_) = 0;
    virtual void pushTSC (TAO_ORB_Core *orb_core_) = 0;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_REQUEST_INTERCEPTOR_ADAPTER_H */
