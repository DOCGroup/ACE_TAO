/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO Real-tiem Event Services
//
// ============================================================================

#include "orbsvcs/Event/Event_Manip.h"

#if !defined (__ACE_INLINE__)
#include "Event_Manip.i"
#endif /* __ACE_INLINE__ */

TAO_EC_Event_Set::~TAO_EC_Event_Set (void)
{
  RtecEventComm::EventSet::freebuf (this->buffer_);
  this->buffer_ = 0;
  this->length_ = 0;
}

TAO_EC_Event_Set*
TAO_EC_Event_Set::_create (const RtecEventComm::Event& event)
{
  RtecEventComm::Event* buffer =
    RtecEventComm::EventSet::allocbuf (1);
  buffer[0] = event;
  return new TAO_EC_Event_Set (1, buffer);
}

TAO_EC_Event_Set*
TAO_EC_Event_Set::_create (RtecEventComm::EventSet& event_set)
{
  // Orphan the buffer....
  CORBA::ULong length = event_set.length ();
  return new TAO_EC_Event_Set (length,
                               event_set.get_buffer (1));
}

CORBA::ULong
TAO_EC_Event_Set::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
  return this->refcnt_++;
}

CORBA::ULong
TAO_EC_Event_Set::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
    this->refcnt_--;
    if (this->refcnt_ != 0)
      return this->refcnt_;
  }

  delete this;
  return 0;
}

// ****************************************************************

TAO_EC_Event::TAO_EC_Event (void)
  :  event_set_ (0),
     index_ (0)
{
}

TAO_EC_Event::TAO_EC_Event (TAO_EC_Event_Set *event_set,
                            CORBA::ULong index)
  :  event_set_ (TAO_EC_Event_Set::_duplicate (event_set)),
     index_ (index)
{
}

TAO_EC_Event::~TAO_EC_Event (void)
{
  TAO_EC_Event_Set::_release (this->event_set_);
  this->event_set_ = 0;
}

TAO_EC_Event::TAO_EC_Event (const TAO_EC_Event& rhs)
  :  event_set_ (TAO_EC_Event_Set::_duplicate (rhs.event_set_)),
     index_ (rhs.index_)
{
}

TAO_EC_Event&
TAO_EC_Event::operator= (const TAO_EC_Event& rhs)
{
  if (this == &rhs)
    return *this;

  // Be careful, increase the reference count and then release our
  // event set, just in case they happen to be the same.
  TAO_EC_Event_Set* tmp = 
    TAO_EC_Event_Set::_duplicate (rhs.event_set_);
  
  TAO_EC_Event_Set::_release (this->event_set_);
  this->event_set_ = tmp;
  this->index_ = rhs.index_;

  return *this;
}





