// $Id$

ACE_INLINE
TAO_EC_Null_ObserverStrategy::TAO_EC_Null_ObserverStrategy (void)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Basic_ObserverStrategy::
      TAO_EC_Basic_ObserverStrategy (TAO_EC_Event_Channel* ec,
                                     ACE_Lock* lock)
  :  event_channel_ (ec),
     lock_ (lock),
     handle_generator_ (1)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Basic_ObserverStrategy::Observer_Entry::Observer_Entry (void)
  :  handle (0)
{
}

ACE_INLINE
TAO_EC_Basic_ObserverStrategy::Observer_Entry::
      Observer_Entry (RtecEventChannelAdmin::Observer_Handle h,
                      RtecEventChannelAdmin::Observer_ptr o)
  :  handle (h),
     observer (o)
{
}

// ****************************************************************

ACE_INLINE int
TAO_EC_Basic_ObserverStrategy::Header_Compare::
    operator () (const RtecEventComm::EventHeader& lhs,
                 const RtecEventComm::EventHeader& rhs) const
{
  if (lhs.source == rhs.source)
    return lhs.type < rhs.type;
  return lhs.source < rhs.source;
}
