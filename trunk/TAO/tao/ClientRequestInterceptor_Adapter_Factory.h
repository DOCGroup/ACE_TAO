// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ClientRequestInterceptor_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_H
#define TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"


namespace TAO
{
  class ClientRequestInterceptor_Adapter;
}

/**
 * @class TAO_ClientRequestInterceptor_Adapter_Factory
 *
 * @brief TAO_ClientRequestInterceptor_Adapter_Factory.
 */
class TAO_Export TAO_ClientRequestInterceptor_Adapter_Factory
  : public ACE_Service_Object
{
public:
  virtual TAO::ClientRequestInterceptor_Adapter *create (void) = 0;
};

#include /**/ "ace/post.h"

#endif /* TAO_CLIENTREQUESTINTERCEPTOR_ADAPTER_FACTORY_H */
