// $Id$

ACE_INLINE
TAO_EC_Filter_Worker::TAO_EC_Filter_Worker (RtecEventComm::EventSet &event,
                                            const TAO_EC_QOS_Info &event_info)
  :  event_ (event),
     event_info_ (event_info)
{
}
