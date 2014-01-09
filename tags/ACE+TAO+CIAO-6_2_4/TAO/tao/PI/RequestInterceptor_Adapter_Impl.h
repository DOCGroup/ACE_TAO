// -*- C++ -*-

//=============================================================================
/**
 *  @file    RequestInterceptor_Adapter_Impl.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef TAO_REQUEST_INTERCEPTOR_ADAPTER_IMPL_H
#define TAO_REQUEST_INTERCEPTOR_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/pi_export.h"
#include "tao/PI/Interceptor_List_T.h"
#include "tao/PI/ClientRequestInterceptorC.h"
#include "tao/PI/ClientRequestDetails.h"
#include "tao/RequestInterceptor_Adapter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class Invocation_Base;

/**
 * @class TAO_RequestInterceptor_Adapter_Impl
 *
 * @brief A convenient helper class to invoke registered client request
 *        interceptors
 *
 * This class invokes all registered interceptors at interception
 * point, and enforces flow rules dictated by the Portable Interceptor
 * specification/chapter.
 */
class TAO_PI_Export TAO_RequestInterceptor_Adapter_Impl
  : public TAO::RequestInterceptor_Adapter
{
public:
  ~TAO_RequestInterceptor_Adapter_Impl (void);

  void popTSC (TAO_ORB_Core *orb_core);
  void pushTSC (TAO_ORB_Core *orb_core);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_REQUEST_INTERCEPTOR_ADAPTER_IMPL_H */
