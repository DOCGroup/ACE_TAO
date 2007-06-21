/* -*- C++ -*- */
/**
 *  @file EventTypeSeq.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_EVENTTYPESEQ_H
#define TAO_Notify_EVENTTYPESEQ_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/EventType.h"
#include "ace/Unbounded_Set.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_EventTypeSeq
 *
 * @brief Allows operations using the CosNotification::EventTypeSeq type.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_EventTypeSeq
  : public ACE_Unbounded_Set <TAO_Notify_EventType>
  , public TAO_Notify::Topology_Object
{
 typedef ACE_Unbounded_Set <TAO_Notify_EventType> inherited;

public:
  /// Constructor
  TAO_Notify_EventTypeSeq (void);
  TAO_Notify_EventTypeSeq (const CosNotification::EventTypeSeq& event_type_seq);
  TAO_Notify_EventTypeSeq (const TAO_Notify_EventTypeSeq & rhs);
  TAO_Notify_EventTypeSeq & operator = (const TAO_Notify_EventTypeSeq & rhs);

  /// Preprocess the types added and removed.
  void add_and_remove (TAO_Notify_EventTypeSeq& added, TAO_Notify_EventTypeSeq& removed);

  /// Populate this sequence with the intersection of rhs and lhs.
  void intersection (const TAO_Notify_EventTypeSeq& rhs, const TAO_Notify_EventTypeSeq& lhs);

  /// insert_seq the contents of <event_type_seq> into this object.
  void insert_seq (const CosNotification::EventTypeSeq& event_type_seq);

  /// remove_seq the contents of <event_type_seq> from this object.
  void remove_seq (const CosNotification::EventTypeSeq& event_type_seq);

  /// insert_seq the contents of <event_type_seq> into this object.
  void insert_seq (const TAO_Notify_EventTypeSeq& event_type_seq);

  /// remove_seq the contents of <event_type_seq> from this object.
  void remove_seq (const TAO_Notify_EventTypeSeq& event_type_seq);

  /// Populate <event_type_seq> with the contents of this object.
  void populate (CosNotification::EventTypeSeq& event_type) const;

  /// Populate <event_type_seq> with the contents of this object.
  // Excludes the special event type. This is used to avoid sending * type updates to proxys.
  void populate_no_special (CosNotification::EventTypeSeq& event_type) const;

  /// Print the contents.
  void dump (void) const;

  // TAO_Notify::Topology_Object

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver);
  virtual TAO_Notify::Topology_Object* load_child (const ACE_CString &type, CORBA::Long id,
    const TAO_Notify::NVPList& attrs);

private:

  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_EVENTTYPESEQ_H */
