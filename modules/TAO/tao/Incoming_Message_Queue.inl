// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Incoming_Message_Queue::TAO_Incoming_Message_Queue (TAO_ORB_Core *orb_core)
  : last_added_ (0),
    size_ (0),
    orb_core_ (orb_core)
{
}

ACE_INLINE CORBA::ULong
TAO_Incoming_Message_Queue::queue_length (void) const
{
  return this->size_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
