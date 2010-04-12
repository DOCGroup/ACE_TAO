// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerRequestInterceptor_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_SERVERREQUESTINTERCEPTOR_ADAPTER_FACTORY_H
#define TAO_SERVERREQUESTINTERCEPTOR_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class ServerRequestInterceptor_Adapter;
}

/**
 * @class TAO_ServerRequestInterceptor_Adapter_Factory
 *
 * @brief TAO_ServerRequestInterceptor_Adapter_Factory.
 */
class TAO_Export TAO_ServerRequestInterceptor_Adapter_Factory
  : public ACE_Service_Object
{
public:
  virtual ~TAO_ServerRequestInterceptor_Adapter_Factory (void);

  virtual TAO::ServerRequestInterceptor_Adapter * create (void) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_SERVERREQUESTINTERCEPTOR_ADAPTER_FACTORY_H */
