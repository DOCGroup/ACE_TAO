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

#ifndef TAO_NS_EVENTTYPESEQ_H
#define TAO_NS_EVENTTYPESEQ_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EventType.h"

template<class T> class ACE_Unbounded_Set;

/**
 * @class TAO_NS_EventTypeSeq
 *
 * @brief Allows operations using the CosNotification::EventTypeSeq type.
 *
 */
class TAO_Notify_Export TAO_NS_EventTypeSeq : public ACE_Unbounded_Set <TAO_NS_EventType>
{
 typedef ACE_Unbounded_Set <TAO_NS_EventType> inherited;

public:
  /// Constructor
  TAO_NS_EventTypeSeq (void);
  TAO_NS_EventTypeSeq (const CosNotification::EventTypeSeq& event_type_seq);

  /// Preprocess the types added and removed.
  void init (TAO_NS_EventTypeSeq& added, TAO_NS_EventTypeSeq& removed);

  /// insert_seq the contents of <event_type_seq> into this object.
  void insert_seq (const CosNotification::EventTypeSeq& event_type_seq);

  /// remove_seq the contents of <event_type_seq> from this object.
  void remove_seq (const CosNotification::EventTypeSeq& event_type_seq);

  /// insert_seq the contents of <event_type_seq> into this object.
  void insert_seq (const TAO_NS_EventTypeSeq& event_type_seq);

  /// remove_seq the contents of <event_type_seq> from this object.
  void remove_seq (const TAO_NS_EventTypeSeq& event_type_seq);

  /// Populate <event_type_seq> with the contents of this object.
  void populate (CosNotification::EventTypeSeq& event_type) const;
};

#if defined (__ACE_INLINE__)
#include "EventTypeSeq.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENTTYPESEQ_H */
