/* -*- C++ -*- */
/**
 *  @file EventType.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_EVENTTYPE_H
#define TAO_NS_EVENTTYPE_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"

/**
 * @class TAO_NS_EventType
 *
 * @brief
 *
 * This type is used to compare different event types.
 * It is used by the Event Manager as a key to find subscription lists.
 *
 */
class TAO_Notify_Export TAO_NS_EventType
{
public:
  /// Constuctor
  TAO_NS_EventType (void);
  TAO_NS_EventType (const char* domain_name, const char* type_name);
  TAO_NS_EventType (const CosNotification::EventType& event_type);
  // Constuctor

  /// Destructor
  ~TAO_NS_EventType ();

  /// hash value
  u_long hash (void) const;

  /// Assignment from CosNotification::EventType
  TAO_NS_EventType& operator=(const CosNotification::EventType& event_type);

  /// Assignment operator.
  TAO_NS_EventType& operator=(const TAO_NS_EventType& event_type);

  /// == operator
  int operator==(const TAO_NS_EventType& notify_event_type) const;

  /// != operator
  int operator!=(const TAO_NS_EventType& notify_event_type) const;

  static TAO_NS_EventType special (void);
  // Return the special event type.

  CORBA::Boolean is_special (void) const;
  // Is this the special event (accept everything).

  const CosNotification::EventType& native (void) const;
  // Get the type underneath us.

  /// Helper to print contents.
  void dump (void) const;

protected:
  /// Init this object.
  void init_i (const char* domain_name, const char* type_name);

  void recompute_hash (void);
  // Recompute the hash value.

  // = Data Members
  CosNotification::EventType event_type_;
  // The event_type that we're decorating.

  u_long hash_value_;
  // The hash value computed.
};

#if defined (__ACE_INLINE__)
#include "EventType.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENTTYPE_H */
