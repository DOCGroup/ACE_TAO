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

#ifndef TAO_Notify_PROXYCONSUMER_H
#define TAO_Notify_PROXYCONSUMER_H
#include /**/ "ace/pre.h"

#include "notify_export.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventChannelAdminC.h"
#include "Refcountable_Guard_T.h"
#include "Event.h"
#include "Proxy.h"


class TAO_Notify_SupplierAdmin;
class TAO_Notify_Supplier;

/**
 * @class TAO_Notify_ProxyConsumer
 *
 * @brief Base class for all types of ProxyConsumer implementations.
 *
 */
class TAO_Notify_Export TAO_Notify_ProxyConsumer : public virtual TAO_Notify_Proxy
{
public:
  /// Constuctor
  TAO_Notify_ProxyConsumer (void);

  /// Destructor
  ~TAO_Notify_ProxyConsumer ();

  /// Init
  void init (TAO_Notify_SupplierAdmin* supplier_admin ACE_ENV_ARG_DECL);

  /// Connect
  void connect (TAO_Notify_Supplier* supplier ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosEventChannelAdmin::AlreadyConnected
                     ));

  /// Disconnect
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Shutdown  (TAO_Notify_Container_T method)
  virtual int shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

  /// Access our Peer.
  virtual TAO_Notify_Peer* peer (void);

  /// Access the Supplier
  TAO_Notify_Supplier* supplier (void);

  /// Return 1 if connected
  int is_connected (void);

  /// The SA parent.
  TAO_Notify_SupplierAdmin* supplier_admin (void);

protected:
  ///= Data Members.
  /// The SA parent.
  TAO_Notify_SupplierAdmin* supplier_admin_;

  /// The Supplier that we're connect to.
  TAO_Notify_Supplier* supplier_;
};

typedef TAO_Notify_Refcountable_Guard_T<TAO_Notify_ProxyConsumer> TAO_Notify_ProxyConsumer_Guard;

#if defined (__ACE_INLINE__)
#include "ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXYCONSUMER_H */
