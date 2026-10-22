// -*- C++ -*-

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
bool
TAO::CSD::TP_Queue::is_empty() const
{
  return (this->head_ == nullptr);
}

TAO_END_VERSIONED_NAMESPACE_DECL
