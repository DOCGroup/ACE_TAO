/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO Real-time Event Services
//
// = FILENAME
//   Event_Manip
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wust.edu)
//
// = DESCRIPTION
//   The Event Channel uses some of this classes to simplify its event
//   manipulation.
//   For instance it keeps a reference counted RtecEventComm::EventSet
//   and classes to automatically manage this reference count.
//
// ============================================================================

#ifndef TAO_EC_EVENT_MANIP_H
#define TAO_EC_EVENT_MANIP_H

#include "orbsvcs/RtecEventCommC.h"

class TAO_ORBSVCS_Export TAO_EC_Event_Set
{
  // = TITLE
  //   A reference counted RtecEventComm::EventSet
  //
  // = DESCRIPTION
  //   The Event_Channel receives an event set from its suppliers, but
  //   it has to pass it to (potentially) multiple consumers, running
  //   at different priorities.
  //   Thus is is difficult to know in advance the life time of this
  //   EventSet.
  //
public:
  TAO_EC_Event_Set (CORBA::ULong length,
                    RtecEventComm::Event* buffer);
  // Constructor...

  ~TAO_EC_Event_Set (void);
  // destructor

  CORBA::ULong length (void) const;
  // The length.

  RtecEventComm::Event& operator[] (CORBA::ULong i) const;
  // Access a particular element.

  static TAO_EC_Event_Set* _create (const RtecEventComm::Event&);
  static TAO_EC_Event_Set* _create (RtecEventComm::EventSet&);
  // Create from an event set.

  static TAO_EC_Event_Set* _duplicate (TAO_EC_Event_Set*);
  static void _release (TAO_EC_Event_Set*);
  // Reference counting.

private:
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Manipulate the reference count.

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Event_Set (const TAO_EC_Event_Set&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Event_Set& operator= (const TAO_EC_Event_Set&))

private:
  CORBA::ULong length_;
  // The length of the buffer.

  RtecEventComm::Event* buffer_;
  // The buffer.

  ACE_SYNCH_MUTEX lock_;
  // Synchronize access to the class internals.

  CORBA::ULong refcnt_;
  // The reference count.
};

class TAO_ORBSVCS_Export TAO_EC_Event
{
  // = TITLE
  //   A smart event class.
  //
  // = DESCRIPTION
  //   Inside the Event Channl a set of events is broken into the
  //   events that compose it and processed independently.
  //   To minimize data copying and memory allocations the Event is
  //   represented as a smart reference to one element of the
  //   reference counted Event Set (see TAO_EC_Event_Set above).
  //   Events can be chained together using a cont() field, much like
  //   ACE_Message_Block.
  //
public:
  TAO_EC_Event (void);
  // Default constructor.

  TAO_EC_Event (TAO_EC_Event_Set *event_set,
                CORBA::ULong index);
  // Constructor, we assume ownership (i.e. duplicate and release the
  // set)

  ~TAO_EC_Event (void);
  // Destructor

  TAO_EC_Event (const TAO_EC_Event& event);
  TAO_EC_Event& operator= (const TAO_EC_Event& event);
  // "Deep" copy, i.e. increases the reference count.

  int empty (void) const;
  // Return true if there is no event inside this object.

  void clear (void);
  // Release the event...

  RtecEventComm::Event& event (void);
  const RtecEventComm::Event& event (void) const;
  // The event..

  // = Shortcuts
  RtecEventComm::EventHeader& header (void);
  RtecEventComm::EventData& data (void);
  const RtecEventComm::EventHeader& header (void) const;
  const RtecEventComm::EventData& data (void) const;

  RtecEventComm::EventSourceID source (void) const;
  RtecEventComm::EventType type (void) const;

private:
  TAO_EC_Event_Set *event_set_;
  // The event set.

  CORBA::ULong index_;
  // The position of the event in the set
};

#if defined (__ACE_INLINE__)
#include "Event_Manip.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_EVENT_MANIP_H */
