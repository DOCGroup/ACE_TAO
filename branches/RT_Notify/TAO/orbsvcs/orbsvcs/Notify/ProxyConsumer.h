/* -*- C++ -*- */
/**
 *  @file ProxyConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PROXYCONSUMER_H
#define TAO_NS_PROXYCONSUMER_H
#include "ace/pre.h"

#include "notify_export.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "Event.h"
#include "Supplier.h"
#include "Proxy.h"
#include "orbsvcs/CosEventChannelAdminC.h"

/**
 * @class TAO_NS_ProxyConsumer
 *
 * @brief Base class for all types of ProxyConsumer implementations.
 *
 */
class TAO_Notify_Export TAO_NS_ProxyConsumer : public TAO_NS_Proxy
{
public:
  /// Constuctor
  TAO_NS_ProxyConsumer (void);

  /// Destructor
  ~TAO_NS_ProxyConsumer (); 

  /// Connect
  void connect (TAO_NS_Supplier* supplier ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosEventChannelAdmin::AlreadyConnected
                     ));

  /// Disconnect
  void disconnect (void);

  /// Shutdown  (TAO_NS_Container_T method)
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  virtual void push (TAO_NS_Event_var &event);

protected:
  /// Return 1 if connected
  int is_connected (void);

  /// The Supplier that we're connect to.
  TAO_NS_Supplier* supplier_;
};

#if defined (__ACE_INLINE__)
#include "ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROXYCONSUMER_H */
