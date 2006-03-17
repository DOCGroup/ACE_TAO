// -*- C++ -*-

//=============================================================================
/**
 *  @file    ClientRequestInterceptor_Factory_Impl.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_IMPL_H
#define TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/ClientRequestInterceptor_Adapter_Factory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class ClientRequestInterceptor_Adapter;
}

class TAO_PI_Export TAO_ClientRequestInterceptor_Adapter_Factory_Impl
  : public TAO_ClientRequestInterceptor_Adapter_Factory
{
public:
  virtual TAO::ClientRequestInterceptor_Adapter *create (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_ClientRequestInterceptor_Adapter_Factory_Impl)
ACE_FACTORY_DECLARE (TAO_PI, TAO_ClientRequestInterceptor_Adapter_Factory_Impl)

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_IMPL_H */
