// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class TYPE> ACE_INLINE void
TAO_Notify_Seq_Worker_T<TYPE>::work (TYPE* type)
{
  this->seq_->length (this->seq_->length () + 1);

  this->seq_[this->seq_->length () - 1] = type->id ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
