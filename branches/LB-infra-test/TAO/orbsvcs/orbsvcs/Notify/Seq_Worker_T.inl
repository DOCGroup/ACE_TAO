// $Id$

template<class TYPE> ACE_INLINE void
TAO_NS_Seq_Worker_T<TYPE>::work (TYPE* type ACE_ENV_ARG_DECL_NOT_USED)
{
  this->seq_->length (this->seq_->length () + 1);

  this->seq_[this->seq_->length () - 1] = type->id ();
}
