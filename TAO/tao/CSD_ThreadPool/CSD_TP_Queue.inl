// $Id$


ACE_INLINE
TAO::CSD::TP_Queue::TP_Queue()
  : head_(0),
    tail_(0)
{
}


ACE_INLINE
TAO::CSD::TP_Queue::~TP_Queue()
{
}


ACE_INLINE
bool
TAO::CSD::TP_Queue::is_empty() const
{
  return (this->head_ == 0);
}
