//=============================================================================
/**
 *  @file   Notify_Collection.h
 *
 *  $Id$
 *
 * Collection types used by Notify
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_COLLECTION_H
#define TAO_NOTIFY_COLLECTION_H

#include "ace/pre.h"
#include "orbsvcs/CosNotifyCommC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "notify_export.h"
#include "Notify_Event.h"

template<class PROXY> class TAO_ESF_Proxy_Collection;
template<class T> class ACE_Unbounded_Set;

class TAO_Notify_EventListener;
class TAO_Notify_UpdateListener;
class TAO_Notify_EventType;

/// A list of event listeners that are looking for the same event type.
typedef TAO_ESF_Proxy_Collection<TAO_Notify_EventListener> TAO_Notify_EventListener_List;

/// A list of update listeners who want to be notified about publish/subscribe changes.
typedef TAO_ESF_Proxy_Collection<TAO_Notify_UpdateListener> TAO_Notify_UpdateListener_List;

/**
 * @class TAO_Notify_EventType_List
 *
 * @brief TAO_Notify_EventType_List
 *
 * Allows operations using the CosNotification::EventTypeSeq type.
 */
class TAO_Notify_Export TAO_Notify_EventType_List : public ACE_Unbounded_Set <TAO_Notify_EventType>
{

  typedef ACE_Unbounded_Set <TAO_Notify_EventType> inherited;

public:
  /// Preprocess lists for subscriptions
  static void preprocess (TAO_Notify_EventType_List& current, TAO_Notify_EventType_List& seq_added, TAO_Notify_EventType_List& seq_removed);

  /// Populate <event_type_seq> with the contents of this object.
  void populate (CosNotification::EventTypeSeq& event_type_seq);

  /// insert the contents of <event_type_seq> into this object.
  void insert_seq (const CosNotification::EventTypeSeq& event_type_seq);

  /// remove the contents of <event_type_seq> from this object.
  void remove_seq (const CosNotification::EventTypeSeq& event_type_seq);

  /// insert the contents of <event_type_seq> into this object.
  void insert_seq (const TAO_Notify_EventType_List& event_type_seq);

  /// remove the contents of <event_type_seq> from this object.
  void remove_seq (const TAO_Notify_EventType_List& event_type_seq);
};

// ****************************************************************

// = Collection Iterators.

 /**
  * @class TAO_Notify_Shutdown_Worker
  *
  * @brief TAO_Notify_Shutdown_Worker
  *
  * Shutdown each listener
  */
class TAO_Notify_Export TAO_Notify_Shutdown_Worker : public TAO_ESF_Worker<TAO_Notify_EventListener>
{
 public:
  TAO_Notify_Shutdown_Worker (void);

  // = TAO_ESF_Worker method
  void work (TAO_Notify_EventListener* listener ACE_ENV_ARG_DECL);
};

// ****************************************************************
#include "ace/post.h"
#endif /* TAO_NOTIFY_COLLECTION_H */
