// -*- C++ -*-

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Queue::TP_Queue()
  : head_(nullptr),
    tail_(nullptr)
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
  return (this->head_ == nullptr);
}

TAO_END_VERSIONED_NAMESPACE_DECL
