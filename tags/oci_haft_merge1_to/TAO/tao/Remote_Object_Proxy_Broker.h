// -*- C++ -*-

//=============================================================================
/**
 *  @file    Remote_Object_Proxy_Broker.h
 *
 *  $Id$
 *
 *  This files contains the Remote Proxy Broker implementation
 *  for the CORBA Object.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_REMOTE_OBJECT_PROXY_BROKER_H
#define TAO_REMOTE_OBJECT_PROXY_BROKER_H

#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/Object_Proxy_Broker.h"
#include "tao/Remote_Object_Proxy_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_Remote_Object_Proxy_Broker
  : public TAO_Object_Proxy_Broker
{
public:

  virtual TAO_Object_Proxy_Impl &select_proxy (CORBA::Object_ptr object
                                               ACE_ENV_ARG_DECL);

private:

  TAO_Remote_Object_Proxy_Impl remote_proxy_impl_;

};

// -----------------------------------------------------

TAO_Remote_Object_Proxy_Broker *the_tao_remote_object_proxy_broker (void);


#include "ace/post.h"

#endif /* TAO_REMOTE_OBJECT_PROXY_BROKER_H */
