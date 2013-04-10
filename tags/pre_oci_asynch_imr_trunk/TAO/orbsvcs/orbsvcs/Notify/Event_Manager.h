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
#include "ace/Auto_Ptr.h"

#include "orbsvcs/Notify/Refcountable.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

namespace CORBA
{
  class Environment;
}

/**
 * @class TAO_Notify_Event_Manager
 *
 * @brief A class that manages the Consumer and Supplier maps.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Event_Manager : public TAO_Notify_Refcountable
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_Event_Manager > Ptr;
  /// Constructor
  TAO_Notify_Event_Manager (void);

  /// Destructor
  virtual ~TAO_Notify_Event_Manager ();

  void release();

  /// Init
  void init (void);

  /// Init
  void shutdown (void);

  /// Connect ProxySupplier
  void connect (TAO_Notify_ProxySupplier* proxy_supplier);

  /// Disconnect ProxySupplier
  void disconnect (TAO_Notify_ProxySupplier* proxy_supplier);

  /// Connect ProxyConsumer
  void connect (TAO_Notify_ProxyConsumer* proxy_consumer);

  /// Disconnect ProxyConsumer
  void disconnect (TAO_Notify_ProxyConsumer* proxy_consumer);

  /// Map accessors.
  TAO_Notify_Consumer_Map& consumer_map (void);
  TAO_Notify_Supplier_Map& supplier_map (void);

  /// Offer change received on <proxy_consumer>.
  void offer_change (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed);

  /// Subscription change received on <proxy_supplier>.
  void subscription_change (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed);

  /// What are the types being offered.
  const TAO_Notify_EventTypeSeq& offered_types (void) const;

  /// What are the types being subscribed.
  const TAO_Notify_EventTypeSeq& subscription_types (void) const;

protected:
  /// Subscribe <proxy_supplier> to the event type sequence list <seq>.
  void subscribe (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& new_seq);

  /// Unsubscribe <proxy_supplier> to the event type sequence list <seq>.
  void un_subscribe (TAO_Notify_ProxySupplier* proxy_supplier, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& last_seq);

  /// Subscribe <proxy_consumer> to the event type sequence list <seq>.
  void publish (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& new_seq);

  /// Subscribe <proxy_consumer> to the event type sequence list <seq>.
  void un_publish (TAO_Notify_ProxyConsumer* proxy_consumer, const TAO_Notify_EventTypeSeq& seq, TAO_Notify_EventTypeSeq& last_seq);

private:
  // Disallow copying and assignment; we don't need them
  // and if we let the compiler generate them it needs the
  // full declaration of the template arguments for ACE_Auto_Ptr<>
  // below.
  TAO_Notify_Event_Manager (TAO_Notify_Event_Manager&);
  TAO_Notify_Event_Manager& operator= (TAO_Notify_Event_Manager&);

  /// Consumer Map
  ACE_Auto_Ptr< TAO_Notify_Consumer_Map > consumer_map_;

  /// Supplier Map
  ACE_Auto_Ptr< TAO_Notify_Supplier_Map > supplier_map_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENT_MANAGER_H */
