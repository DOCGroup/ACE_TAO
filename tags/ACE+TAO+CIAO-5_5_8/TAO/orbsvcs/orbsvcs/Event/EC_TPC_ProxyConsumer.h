// -*- C++ -*-

/**
 *  @file   EC_TPC_ProxyConsumer.h
 *
 *  $Id$
 *
 *  @author Chris Cleeland <cleeland at ociweb.com >
 *
 */

#ifndef TAO_EC_TPC_PROXYCONSUMER_H
#define TAO_EC_TPC_PROXYCONSUMER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Default_ProxyConsumer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Dispatching;
class TAO_EC_TPC_Dispatching;

/**
 * @class TAO_EC_TPC_ProxyPushConsumer
 *
 */
class TAO_RTEvent_Serv_Export TAO_EC_TPC_ProxyPushConsumer :
  public TAO_EC_Default_ProxyPushConsumer
{
public:
  TAO_EC_TPC_ProxyPushConsumer (TAO_EC_Event_Channel_Base* ec);
  virtual ~TAO_EC_TPC_ProxyPushConsumer (void);

  virtual void disconnect_push_consumer (void);

private:
  typedef TAO_EC_Default_ProxyPushConsumer BASECLASS;
  TAO_EC_TPC_Dispatching* tpc_dispatching ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif
