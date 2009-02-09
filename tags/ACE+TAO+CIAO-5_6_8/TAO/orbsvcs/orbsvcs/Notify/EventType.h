/* -*- C++ -*- */
/**
 *  @file EventType.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_EVENTTYPE_H
#define TAO_Notify_EVENTTYPE_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/Notify/Topology_Object.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_EventType
 *
 * @brief
 *
 * This type is used to compare different event types.
 * It is used by the Event Manager as a key to find subscription lists.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_EventType : public TAO_Notify::Topology_Savable
{
public:
  /// Constructor
  TAO_Notify_EventType (void);
  TAO_Notify_EventType (const char* domain_name, const char* type_name);
  TAO_Notify_EventType (const CosNotification::EventType& event_type);
  // Constructor

  /// Destructor
  virtual ~TAO_Notify_EventType ();

  /// hash value
  u_long hash (void) const;

  /// Assignment from CosNotification::EventType
  TAO_Notify_EventType& operator=(const CosNotification::EventType& event_type);

  /// Assignment operator.
  TAO_Notify_EventType& operator=(const TAO_Notify_EventType& event_type);

  /// == operator
  bool operator==(const TAO_Notify_EventType& notify_event_type) const;

  /// != operator
  bool operator!=(const TAO_Notify_EventType& notify_event_type) const;

  /// Return the special event type.
  static TAO_Notify_EventType special (void);

  /// Is this the special event (accept everything).
  CORBA::Boolean is_special (void) const;

  /// Get the type underneath us.
  const CosNotification::EventType& native (void) const;

  /// Helper to print contents.
  void dump (void) const;

  /// Initialize from an NVPList, return false on failure
  bool init(const TAO_Notify::NVPList& attrs);

  // TAO_Notify::Topology_Object

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver);

protected:
  /// Init this object.
  void init_i (const char* domain_name, const char* type_name);

  /// Recompute the hash value.
  void recompute_hash (void);

  bool domain_is_wildcard (const char* domain) const;
  bool type_is_wildcard (const char* type) const;

  // = Data Members
  /// The event_type that we're decorating.
  CosNotification::EventType event_type_;

  /// The hash value computed.
  u_long hash_value_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/EventType.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENTTYPE_H */
