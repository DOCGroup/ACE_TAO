TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_EC_Filter_Worker::TAO_EC_Filter_Worker (RtecEventComm::EventSet const &event,
                                            TAO_EC_QOS_Info const &event_info)
  :  event_ (event),
     event_info_ (event_info)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
