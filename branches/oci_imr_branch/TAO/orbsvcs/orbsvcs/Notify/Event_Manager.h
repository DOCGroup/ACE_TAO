/* -*- C++ -*- */
/**
 *  @file Event_Manager.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_EVENT_MANAGER_H
#define TAO_Notify_EVENT_MANAGER_H

#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/ESF/ESF_Worker.h"

#include "tao/orbconf.h"

#include "ace/CORBA_macros.h"

class TAO_Notify_ProxySupplier;
class TAO_Notify_ProxyConsumer;
class TAO_Notify_EventTypeSeq;

template <class PROXY, class ACE_LOCK> 
class TAO_Notify_Event_Map_T;

typedef TAO_Notify_Event_Map_T<TAO_Notify_ProxySupplier, 
                               TAO_SYNCH_RW_MUTEX> 
  TAO_Notify_Consumer_Map;

typedef TAO_Notify_Event_Map_T<TAO_Notify_ProxyConsumer, 
                               TAO_SYNCH_RW_MUTEX> 
  TAO_Notify_Supplier_Map;

/**
 * @class TAO_Notify_Event_Manager
 *
 * @brief A class that manages the Consumer and Supplier maps.
 *
 */
class TAO_Notify_Export TAO_Notify_Event_Manager
{
public:
  /// Constuctor
  TAO_Notify_Event_Manager (void);

  /// Destructor
  ~TAO_Notify_Event_Manager ();

  /// Init
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Init
  void shutdown (void);

  /// Connect ProxySupplier
  void connect (TAO_Notify_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL);

  /// Disconnect ProxySupplier
  void disconnect (TAO_Notify_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL);

  /// Connect ProxyConsumer
  void connect (TAO_Notify_ProxyConsumer* proxy_consumer ACE_ENV_ARG_DECL);

  /// Disconnect ProxyConsumer
  void disconnect (TAO_Notify_ProxyConsumer* proxy_consumer ACE_ENV_ARG_DECL);

  /// Map accessors.
  TAO_Notify_Consumer_Map* consumer_map (void);
  TAO_Notify_Supplier_Map* supplier_map (void);

  /// Offer change received on <proxy_consumer>.
  void offer_change (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed ACE_ENV_ARG_DECL);

  /// Subscription change received on <proxy_supplier>.
  void subscription_change (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed ACE_ENV_ARG_DECL);

  /// What are the types being offered.
  const TAO_Notify_EventTypeSeq& offered_types (void);

  /// What are the types being subscribed.
  const TAO_Notify_EventTypeSeq& subscription_types (void);

protected:
  /// Subscribe <proxy_supplier> to the event type sequence list <seq>.
  void subscribe (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& new_seq ACE_ENV_ARG_DECL);

  /// Unsubscribe <proxy_supplier> to the event type sequence list <seq>.
  void un_subscribe (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& last_seq ACE_ENV_ARG_DECL);

  /// Subscribe <proxy_consumer> to the event type sequence list <seq>.
  void publish (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& new_seq ACE_ENV_ARG_DECL);

  /// Subscribe <proxy_consumer> to the event type sequence list <seq>.
  void un_publish (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& last_seq ACE_ENV_ARG_DECL);

  /// Consumer Map
  TAO_Notify_Consumer_Map* consumer_map_;

  /// Supplier Map
  TAO_Notify_Supplier_Map* supplier_map_;
};

/********************************************************************************/

/**
 * @class TAO_Notify_ProxyConsumer_Update_Worker
 *
 * @brief Inform ProxyConsumer of updates.
 *
 */
class TAO_Notify_Export TAO_Notify_ProxyConsumer_Update_Worker : public TAO_ESF_Worker<TAO_Notify_ProxyConsumer>
{
public:
  TAO_Notify_ProxyConsumer_Update_Worker (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed);

protected:
  ///= TAO_ESF_Worker method
  void work (TAO_Notify_ProxyConsumer* proxy ACE_ENV_ARG_DECL);

  const TAO_Notify_EventTypeSeq& added_;
  const TAO_Notify_EventTypeSeq& removed_;
};

/********************************************************************************/

/**
 * @class TAO_Notify_ProxySupplier_Update_Worker
 *
 * @brief Inform ProxySupplier of updates.
 *
 */
class TAO_Notify_Export TAO_Notify_ProxySupplier_Update_Worker : public TAO_ESF_Worker<TAO_Notify_ProxySupplier>
{
public:
  TAO_Notify_ProxySupplier_Update_Worker (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed);

protected:
  ///= TAO_ESF_Worker method
  void work (TAO_Notify_ProxySupplier* proxy ACE_ENV_ARG_DECL);

  const TAO_Notify_EventTypeSeq& added_;
  const TAO_Notify_EventTypeSeq& removed_;
};

/********************************************************************************/

#if defined (__ACE_INLINE__)
#include "Event_Manager.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENT_MANAGER_H */
