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

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EventType.h"
#include "ace/Unbounded_Set.h"

/**
 * @class TAO_Notify_EventTypeSeq
 *
 * @brief Allows operations using the CosNotification::EventTypeSeq type.
 *
 */
class TAO_Notify_Export TAO_Notify_EventTypeSeq : public ACE_Unbounded_Set <TAO_Notify_EventType>
{
 typedef ACE_Unbounded_Set <TAO_Notify_EventType> inherited;

public:
  /// Constructor
  TAO_Notify_EventTypeSeq (void);
  TAO_Notify_EventTypeSeq (const CosNotification::EventTypeSeq& event_type_seq);

  /// Preprocess the types added and removed.
  void init (TAO_Notify_EventTypeSeq& added, TAO_Notify_EventTypeSeq& removed);

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
};

#if defined (__ACE_INLINE__)
#include "EventTypeSeq.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_EVENTTYPESEQ_H */
