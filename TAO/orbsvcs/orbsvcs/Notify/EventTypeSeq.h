/* -*- C++ -*- */
/**
 *  @file EventTypeSeq.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_EVENTTYPESEQ_H
#define TAO_Notify_EVENTTYPESEQ_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/EventType.h"

#include <vector>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_EventTypeSeq
 *
 * @brief Allows operations using the CosNotification::EventTypeSeq type.
 */
class TAO_Notify_Serv_Export TAO_Notify_EventTypeSeq
  : public TAO_Notify::Topology_Object
{
  using Storage = std::vector<TAO_Notify_EventType>;

public:
  using const_iterator = Storage::const_iterator;

  /// Constructor
  TAO_Notify_EventTypeSeq ();
  explicit TAO_Notify_EventTypeSeq (const CosNotification::EventTypeSeq& event_type_seq);
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
  /// Excludes the special event type. This is used to avoid sending * type updates to proxys.
  void populate_no_special (CosNotification::EventTypeSeq& event_type) const;

  const_iterator begin () const;
  const_iterator end () const;
  size_t size () const;
  bool empty () const;
  void clear ();
  bool contains (TAO_Notify_EventType const & event_type) const;
  int add (TAO_Notify_EventType const & event_type);
  int erase (TAO_Notify_EventType const & event_type);

  /// Print the contents.
  void dump () const;

  // TAO_Notify::Topology_Object

  void save_persistent (TAO_Notify::Topology_Saver& saver) override;
  TAO_Notify::Topology_Object* load_child (ACE_CString const & type, CORBA::Long id,
                                           TAO_Notify::NVPList const & attrs) override;

private:
  void release () override;

  Storage event_types_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_EVENTTYPESEQ_H */
