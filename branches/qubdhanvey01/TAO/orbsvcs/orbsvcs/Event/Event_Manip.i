// $Id$

ACE_INLINE
TAO_EC_Event_Set::TAO_EC_Event_Set (CORBA::ULong length,
                                    RtecEventComm::Event* buffer)
  :  length_ (length),
     buffer_ (buffer),
     refcnt_ (1)
{
}

ACE_INLINE CORBA::ULong
TAO_EC_Event_Set::length (void) const
{
  return this->length_;
}

ACE_INLINE RtecEventComm::Event&
TAO_EC_Event_Set::operator[] (CORBA::ULong i) const
{
  return this->buffer_[i];
}

ACE_INLINE TAO_EC_Event_Set*
TAO_EC_Event_Set::_duplicate (TAO_EC_Event_Set* x)
{
  if (x == 0)
    return 0;
  x->_incr_refcnt ();
  return x;
}

ACE_INLINE void
TAO_EC_Event_Set::_release (TAO_EC_Event_Set* x)
{
  if (x == 0)
    return;
  x->_decr_refcnt ();
}


// ****************************************************************

ACE_INLINE int
TAO_EC_Event::empty (void) const
{
  return this->event_set_ == 0;
}

ACE_INLINE void
TAO_EC_Event::clear (void)
{
  TAO_EC_Event_Set::_release (this->event_set_);
  this->event_set_ = 0;
  this->index_ = 0;
}

ACE_INLINE RtecEventComm::Event&
TAO_EC_Event::event (void)
{
  return (*this->event_set_)[this->index_];
}

ACE_INLINE const RtecEventComm::Event&
TAO_EC_Event::event (void) const
{
  return (*this->event_set_)[this->index_];
}

ACE_INLINE RtecEventComm::EventHeader&
TAO_EC_Event::header (void)
{
  return this->event ().header;
}

ACE_INLINE const RtecEventComm::EventHeader&
TAO_EC_Event::header (void) const
{
  return this->event ().header;
}

ACE_INLINE RtecEventComm::EventData&
TAO_EC_Event::data (void)
{
  return this->event ().data;
}

ACE_INLINE const RtecEventComm::EventData&
TAO_EC_Event::data (void) const
{
  return this->event ().data;
}

ACE_INLINE RtecEventComm::EventSourceID
TAO_EC_Event::source (void) const
{
  return this->event ().header.source;
}

ACE_INLINE RtecEventComm::EventType
TAO_EC_Event::type (void) const
{
  return this->event ().header.type;
}
