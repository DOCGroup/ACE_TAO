// $Id$

ACE_INLINE
TAO_ECG_CDR_Message_Receiver::Requests::Requests (void)
  : fragmented_requests_ (0)
  , size_ (0)
  , id_range_low_ (0)
  , id_range_high_ (0)
  , min_purge_count_ (0)
{
}

// ****************************************************************

ACE_INLINE
TAO_ECG_CDR_Message_Receiver::TAO_ECG_CDR_Message_Receiver (void)
  : ignore_from_ ()
  , request_map_ ()
  /* , lock_ (0) */
  , max_requests_ (ECG_DEFAULT_MAX_FRAGMENTED_REQUESTS)
  , min_purge_count_ (ECG_DEFAULT_FRAGMENTED_REQUESTS_MIN_PURGE_COUNT)
{
//    ACE_NEW (this->lock_,
//             ACE_Lock_Adapter<ACE_Null_Mutex>);
}

ACE_INLINE
TAO_ECG_CDR_Message_Receiver::~TAO_ECG_CDR_Message_Receiver (void)
{
  this->shutdown ();
}

ACE_INLINE void
TAO_ECG_CDR_Message_Receiver::init (TAO_ECG_Refcounted_Endpoint ignore_from
                                    /*, ACE_Lock *lock */)
{
  this->ignore_from_ = ignore_from;

//    if (lock)
//      {
//        delete this->lock_;
//        this->lock_ = lock;
//      }
}

ACE_INLINE void
TAO_ECG_CDR_Message_Receiver::shutdown (void)
{
  // ACE_GUARD (ACE_Lock, guard, *this->lock_);

  Request_Map::iterator end = this->request_map_.end ();
  for (Request_Map::iterator i =  this->request_map_.begin ();
       i != end;
       ++i)
    {
      delete (*i).int_id_;
      (*i).int_id_ = 0;
    }

  TAO_ECG_Refcounted_Endpoint empty_endpoint;
  this->ignore_from_ = empty_endpoint;
}





