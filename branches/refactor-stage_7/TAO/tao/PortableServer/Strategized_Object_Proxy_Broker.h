// -*- C++ -*-

//=============================================================================
/**
 *  @file    Strategized_Object_Proxy_Broker.h
 *
 *  $Id$
 *
 *  This files contains the strategized proxy broker implementation.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_STRATEGIZED_OBJECT_PROXY_BROKER_H_
#define TAO_STRATEGIZED_OBJECT_PROXY_BROKER_H_

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "tao/Object_Proxy_Broker.h"

#include "ace/Thread_Mutex.h"

class TAO_PortableServer_Export TAO_Strategized_Object_Proxy_Broker
: public TAO_Object_Proxy_Broker
{
public:

  /// Constructor.
  TAO_Strategized_Object_Proxy_Broker (void);

  /// Destructor.
  ~TAO_Strategized_Object_Proxy_Broker (void);

  virtual TAO_Object_Proxy_Impl & select_proxy (CORBA::Object_ptr object
                                                ACE_ENV_ARG_DECL);

private:

  void create_proxy (int collocation_strategy
                     ACE_ENV_ARG_DECL);

private:

  TAO_Object_Proxy_Impl * proxy_cache_[TAO_Collocation_Strategies::CS_LAST];

  TAO_SYNCH_MUTEX mutex_;
};

/// This function is used to access the unique instance of strategized
/// proxy broker. All the CORBA::Object share the proxy broker.
TAO_Strategized_Object_Proxy_Broker *the_tao_strategized_object_proxy_broker (void);


#include /**/ "ace/post.h"

#endif /* TAO_STRATEGIZED_OBJECT_PROXY_BROKER_H_ */
