// $Id$
// ==========================================================================
//
// = LIBRARY
//   Orbsvcs
//
// = FILENAME
//   Notify_Types.h
//
// = DESCRIPTION
//   Internal types used by Notify
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_TYPES_H
#define TAO_NOTIFY_TYPES_H
#include "ace/pre.h"

#include "orbsvcs/CosNotifyFilterC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "ace/Containers_T.h"

class TAO_Notify_Event_Listener;
class TAO_Notify_Update_Listener;

class TAO_Notify_EventType
{
  // = TITLE
  //   TAO_Notify_EventType
  //
  // = DESCRIPTION
  //   This type is used to compare different event types.
  //   It is used by the Event Manager as a key to find subscription lists.
  //
public:
  // = Initialization and termination
  TAO_Notify_EventType (void);
  TAO_Notify_EventType (const char* domain_name, const char* type_name);
  TAO_Notify_EventType (const CosNotification::EventType& event_type);
  // Constuctor

  ~TAO_Notify_EventType ();
  // Destructor

  u_long hash (void) const;
  // hash value

  void operator=(const CosNotification::EventType& event_type);
  // Assignment from CosNotification::EventType

  int operator==(const TAO_Notify_EventType& notify_event_type) const;
  // == operator

  static TAO_Notify_EventType& special_event_type (void);
  // Return the special event type.

  CORBA::Boolean is_special (void) const;
  // Is this the special event (accept everything).

  const CosNotification::EventType& get_native (void) const;
  // Get the type underneath us.

protected:
  void recompute_hash (void);
  // Recompute the hash value.

  // = Data Members
  CosNotification::EventType event_type_;
  // The event_type that we're decorating.

  u_long hash_value_;
  // The hash value computed.

  static TAO_Notify_EventType special_event_type_;
  // A special event type
};

class TAO_Notify_Event
{
  // = TITLE
  //   TAO_Notify_Event
  //
  // = DESCRIPTION
  //   Abstraction for an event
  //
public:
  virtual CORBA::Boolean is_special_event_type (void) const = 0;
  // Is this the "special" event type.

  virtual const TAO_Notify_EventType& event_type (void) const = 0;
  // Get the event type.

  virtual TAO_Notify_Event* clone (void) = 0;
  // We may need to make a copy of the underlying data if it is not owned
  // by us.
  // Note this behaviour: If this object owns the data, then we *transfer*
  // ownership of the data to the new object otherwise we copy the data
  // for the new object.

  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter, CORBA::Environment &ACE_TRY_ENV) const = 0;
  // Returns true if the filter matches.

  virtual void do_push (CosEventComm::PushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const = 0;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const = 0;
  // Push self to <consumer>
protected:
  CORBA::Boolean is_owner_;
  // Do we own the data.
};

class TAO_Notify_Any : public TAO_Notify_Event
{
public:
  TAO_Notify_Any (void);
  TAO_Notify_Any (const CORBA::Any & data);
  virtual ~TAO_Notify_Any ();

  virtual TAO_Notify_Event* clone (void);

  void operator=(const TAO_Notify_Any& notify_any);

  virtual CORBA::Boolean is_special_event_type (void) const;
  virtual const TAO_Notify_EventType& event_type (void) const;
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter, CORBA::Environment &ACE_TRY_ENV) const;
  virtual void do_push (CosEventComm::PushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const;

protected:
  CORBA::Any* data_;
  // The data
};

class TAO_Notify_StructuredEvent : public TAO_Notify_Event
{
public:
  TAO_Notify_StructuredEvent (void);
  TAO_Notify_StructuredEvent (const CosNotification::StructuredEvent & notification);
  virtual ~TAO_Notify_StructuredEvent ();

  virtual TAO_Notify_Event* clone (void);
  void operator=(const TAO_Notify_StructuredEvent &structured_event);

  virtual CORBA::Boolean is_special_event_type (void) const;
  virtual const TAO_Notify_EventType& event_type (void) const;
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter, CORBA::Environment &ACE_TRY_ENV) const;
  virtual void do_push (CosEventComm::PushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const;

protected:
  CosNotification::StructuredEvent* data_;
  // The data

  TAO_Notify_EventType event_type_;
  // The event type of <data_>
};

class EVENTTYPE_LIST : public ACE_Unbounded_Set <TAO_Notify_EventType>
{
  // = TITLE
  //   EVENTTYPE_LIST
  //
  // = DESCRIPTION
  //   Allows operations using the CosNotification::EventTypeSeq type.
  //

  typedef ACE_Unbounded_Set <TAO_Notify_EventType> inherited;

public:
  void populate (CosNotification::EventTypeSeq& event_type_seq);
  // Populate <event_type_seq> with the contents of this object.

  void insert_seq (const CosNotification::EventTypeSeq& event_type_seq);
  // insert the contents of <event_type_seq> into this object.

  void remove_seq (const CosNotification::EventTypeSeq& event_type_seq);
  // remove the contents of <event_type_seq> from this object.
};

// = typedefs
typedef ACE_Unbounded_Set<TAO_Notify_Event_Listener*> EVENT_LISTENER_LIST;
// A list of event listeners that are looking for the same event type.

typedef ACE_Unbounded_Set<TAO_Notify_Update_Listener*> UPDATE_LISTENER_LIST;
// list of update listeners.

#include "ace/post.h"
#endif /* TAO_NOTIFY_TYPES_H */
