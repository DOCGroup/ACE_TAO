// $Id$

template<ACE_SYNCH_DECL> ACE_INLINE
TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE>::
       TAO_EC_ConsumerAdmin_T (TAO_EC_Event_Channel *event_channel)
 :  TAO_EC_ConsumerAdmin (event_channel),
    busy_cond_ (lock_),
    busy_count_ (0),
    reached_hwm_ (0)
{
}

template<ACE_SYNCH_DECL> ACE_INLINE int
TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE>::busy_count (void) const
{
  return this->busy_count_;
}

template<ACE_SYNCH_DECL> ACE_INLINE
TAO_EC_ConsumerAdmin_Immediate<ACE_SYNCH_USE>::
       TAO_EC_ConsumerAdmin_Immediate (TAO_EC_Event_Channel *event_channel)
 :  TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE> (event_channel)
{
}

template<ACE_SYNCH_DECL> ACE_INLINE
TAO_EC_ConsumerAdmin_Delayed<ACE_SYNCH_USE>::
       TAO_EC_ConsumerAdmin_Delayed (TAO_EC_Event_Channel *event_channel)
 :  TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE> (event_channel)
{
}
