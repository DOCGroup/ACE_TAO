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

#ifndef TAO_NS_EVENT_MANAGER_H
#define TAO_NS_EVENT_MANAGER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Types.h"

class TAO_NS_EventTypeSeq;

/**
 * @class TAO_NS_Event_Manager
 *
 * @brief A class that manages the Consumer and Supplier maps. 
 *
 */
class TAO_Notify_Export TAO_NS_Event_Manager
{
public:
  /// Constuctor
  TAO_NS_Event_Manager (void);

  /// Destructor
  ~TAO_NS_Event_Manager ();  

  /// Init
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Subscribe <proxy_supplier> to the event type sequence list <seq>.
  void subscribe (TAO_NS_ProxySupplier* proxy_supplier, const TAO_NS_EventTypeSeq& seq ACE_ENV_ARG_DECL);

  /// Unsubscribe <proxy_supplier> to the event type sequence list <seq>.
  void un_subscribe (TAO_NS_ProxySupplier* proxy_supplier, const TAO_NS_EventTypeSeq& seq ACE_ENV_ARG_DECL);

  /// Subscribe <proxy_consumer> to the event type sequence list <seq>.
  void publish (TAO_NS_ProxyConsumer* proxy_consumer, const TAO_NS_EventTypeSeq& seq ACE_ENV_ARG_DECL);

  /// Subscribe <proxy_consumer> to the event type sequence list <seq>.
  void un_publish (TAO_NS_ProxyConsumer* proxy_consumer, const TAO_NS_EventTypeSeq& seq ACE_ENV_ARG_DECL);

  /// Map accessors.
  TAO_NS_Consumer_Map* consumer_map (void);
  TAO_NS_Supplier_Map* supplier_map (void);

protected:
  /// Consumer Map
  TAO_NS_Consumer_Map* consumer_map_;

  /// Supplier Map
  TAO_NS_Supplier_Map* supplier_map_;

  /// Cache the list that always gets events.
  TAO_NS_Consumer_Collection* broadcast_list_;
};

#if defined (__ACE_INLINE__)
#include "Event_Manager.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENT_MANAGER_H */
