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

  /// Init method.
  virtual void init_ps (ACE_ENV_SINGLE_ARG_DECL);

  /// Connect
  void connect (TAO_NS_Consumer* consumer ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosEventChannelAdmin::AlreadyConnected
                     ));
  /// Disconnect
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Dispatch Event to consumer
  virtual void push (TAO_NS_Event_var &event);

  /// Dispatch Event to consumer, no filtering
  virtual void push_no_filtering (TAO_NS_Event_var &event);

  /// Override TAO_NS_Container_T::shutdown  method
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Access our Peer.
  virtual TAO_NS_Peer* peer (void);

  /// Access the Consumer
  TAO_NS_Consumer* consumer (void);

protected:
  /// Return 1 if connected
  int is_connected (void);

  /// The Consumer that we're connect to.
  TAO_NS_Consumer* consumer_;
};

#if defined (__ACE_INLINE__)
#include "ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROXYSUPPLIER_H */
