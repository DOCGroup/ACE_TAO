/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Event_Manip.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wust.edu)
 *
 * The Event Channel uses some of this classes to simplify its event
 * manipulation.
 * For instance it keeps a reference counted RtecEventComm::EventSet
 * and classes to automatically manage this reference count.
 *
 *
 */
//=============================================================================


#ifndef TAO_EC_EVENT_MANIP_H
#define TAO_EC_EVENT_MANIP_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventCommC.h"
#include "old_event_export.h"

/**
 * @class TAO_EC_Event_Set
 *
 * @brief A reference counted RtecEventComm::EventSet
 *
 * The Event_Channel receives an event set from its suppliers, but
 * it has to pass it to (potentially) multiple consumers, running
 * at different priorities.
 * Thus is is difficult to know in advance the life time of this
 * EventSet.
 */
class TAO_RTOLDEvent_Export TAO_EC_Event_Set
{
public:
  /// Constructor...
  TAO_EC_Event_Set (CORBA::ULong length,
                    RtecEventComm::Event* buffer);

  /// destructor
  ~TAO_EC_Event_Set (void);

  /// The length.
  CORBA::ULong length (void) const;

  /// Access a particular element.
  RtecEventComm::Event& operator[] (CORBA::ULong i) const;

  /// Create from an event set.
  static TAO_EC_Event_Set* _create (const RtecEventComm::Event&);
  static TAO_EC_Event_Set* _create (RtecEventComm::EventSet&);

  /// Reference counting.
  static TAO_EC_Event_Set* _duplicate (TAO_EC_Event_Set*);
  static void _release (TAO_EC_Event_Set*);

private:
  /// Manipulate the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Event_Set (const TAO_EC_Event_Set&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Event_Set& operator= (const TAO_EC_Event_Set&))

private:
  /// The length of the buffer.
  CORBA::ULong length_;

  /// The buffer.
  RtecEventComm::Event* buffer_;

  /// Synchronize access to the class internals.
  TAO_SYNCH_MUTEX lock_;

  /// The reference count.
  CORBA::ULong refcnt_;
};

/**
 * @class TAO_EC_Event
 *
 * @brief A smart event class.
 *
 * Inside the Event Channl a set of events is broken into the
 * events that compose it and processed independently.
 * To minimize data copying and memory allocations the Event is
 * represented as a smart reference to one element of the
 * reference counted Event Set (see TAO_EC_Event_Set above).
 * Events can be chained together using a cont() field, much like
 * ACE_Message_Block.
 */
class TAO_RTOLDEvent_Export TAO_EC_Event
{
public:
  /// Default constructor.
  TAO_EC_Event (void);

  /// Constructor, we assume ownership (i.e. duplicate and release the
  /// set)
  TAO_EC_Event (TAO_EC_Event_Set *event_set,
                CORBA::ULong index);

  /// Destructor
  ~TAO_EC_Event (void);

  /// "Deep" copy, i.e. increases the reference count.
  TAO_EC_Event (const TAO_EC_Event& event);
  TAO_EC_Event& operator= (const TAO_EC_Event& event);

  /// Return true if there is no event inside this object.
  int empty (void) const;

  /// Release the event...
  void clear (void);

  /// The event..
  RtecEventComm::Event& event (void);
  const RtecEventComm::Event& event (void) const;

  // = Shortcuts
  RtecEventComm::EventHeader& header (void);
  RtecEventComm::EventData& data (void);
  const RtecEventComm::EventHeader& header (void) const;
  const RtecEventComm::EventData& data (void) const;

  RtecEventComm::EventSourceID source (void) const;
  RtecEventComm::EventType type (void) const;

private:
  /// The event set.
  TAO_EC_Event_Set *event_set_;

  /// The position of the event in the set
  CORBA::ULong index_;
};

#if defined (__ACE_INLINE__)
#include "Event_Manip.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_EVENT_MANIP_H */
