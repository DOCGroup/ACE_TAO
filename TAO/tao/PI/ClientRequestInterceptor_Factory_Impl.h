/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    PolicyFactory_Loader.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_IMPL_H
#define TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_IMPL_H

#include /**/ "ace/pre.h"

#include "pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

#include "tao/ClientRequestInterceptor_Adapter_Factory.h"

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

ACE_STATIC_SVC_DECLARE (TAO_ClientRequestInterceptor_Adapter_Factory_Impl)
ACE_FACTORY_DECLARE (TAO_PI, TAO_ClientRequestInterceptor_Adapter_Factory_Impl)

#include /**/ "ace/post.h"

#endif /* TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_IMPL_H */
