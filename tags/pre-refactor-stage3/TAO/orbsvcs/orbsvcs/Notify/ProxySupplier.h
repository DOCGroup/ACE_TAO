/* -*- C++ -*- */
/**
 *  @file ProxySupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PROXYSUPPLIER_H
#define TAO_NS_PROXYSUPPLIER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Event.h"
#include "Proxy.h"
#include "orbsvcs/CosEventChannelAdminC.h"

class TAO_NS_Consumer;
class TAO_NS_ConsumerAdmin;

/**
 * @class TAO_NS_ProxySupplier
 *
 * @brief Base class for all the ProxySuppliers.
 *
 */
class TAO_Notify_Export TAO_NS_ProxySupplier : public virtual TAO_NS_Proxy
{
  friend class TAO_NS_Consumer;

public:
  /// Constuctor
  TAO_NS_ProxySupplier (void);

  /// Destructor
  virtual ~TAO_NS_ProxySupplier ();

  /// Init
  virtual void init (TAO_NS_ConsumerAdmin* consumer_admin ACE_ENV_ARG_DECL);

  /// Connect
  void connect (TAO_NS_Consumer* consumer ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosEventChannelAdmin::AlreadyConnected
                     ));
  /// Disconnect
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Dispatch Event to consumer
  virtual void push (const TAO_NS_Event_var &event);

  /// Dispatch Event to consumer, no filtering
  virtual void push_no_filtering (const TAO_NS_Event_var &event);

  /// Override TAO_NS_Container_T::shutdown  method
  virtual int shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

  /// Override, TAO_NS_Proxy::qos_changed to apply MaxEventssPerConsumer QoS.
  virtual void qos_changed (const TAO_NS_QoSProperties& qos_properties);

  /// Access our Peer.
  virtual TAO_NS_Peer* peer (void);

  /// Access the Consumer
  TAO_NS_Consumer* consumer (void);

  /// Return 1 if connected
  int is_connected (void);

  /// The CA parent.
  TAO_NS_ConsumerAdmin* consumer_admin (void);

protected:

  ///= Data Members.
  /// The CA parent.
  TAO_NS_ConsumerAdmin* consumer_admin_;

  /// The Consumer that we're connect to.
  TAO_NS_Consumer* consumer_;
};

#if defined (__ACE_INLINE__)
#include "ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROXYSUPPLIER_H */
