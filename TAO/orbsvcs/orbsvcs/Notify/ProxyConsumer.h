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
#include "Proxy.h"
#include "orbsvcs/CosEventChannelAdminC.h"

class TAO_NS_SupplierAdmin;
class TAO_NS_Supplier;

/**
 * @class TAO_NS_ProxyConsumer
 *
 * @brief Base class for all types of ProxyConsumer implementations.
 *
 */
class TAO_Notify_Export TAO_NS_ProxyConsumer : public virtual TAO_NS_Proxy
{
public:
  /// Constuctor
  TAO_NS_ProxyConsumer (void);

  /// Destructor
  ~TAO_NS_ProxyConsumer ();

  /// Init
  void init (TAO_NS_SupplierAdmin* supplier_admin ACE_ENV_ARG_DECL);

  /// Connect
  void connect (TAO_NS_Supplier* supplier ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosEventChannelAdmin::AlreadyConnected
                     ));

  /// Disconnect
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Shutdown  (TAO_NS_Container_T method)
  virtual int shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

  /// Start event propagation.
  virtual void push (TAO_NS_Event_var &event);

  /// Access our Peer.
  virtual TAO_NS_Peer* peer (void);

  /// Access the Supplier
  TAO_NS_Supplier* supplier (void);

  /// Return 1 if connected
  int is_connected (void);

  /// The SA parent.
  TAO_NS_SupplierAdmin* supplier_admin (void);

protected:
  ///= Data Members.
  /// The SA parent.
  TAO_NS_SupplierAdmin* supplier_admin_;

  /// The Supplier that we're connect to.
  TAO_NS_Supplier* supplier_;
};

#if defined (__ACE_INLINE__)
#include "ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROXYCONSUMER_H */
