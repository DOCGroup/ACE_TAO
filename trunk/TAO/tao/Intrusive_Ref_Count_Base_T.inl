// $Id$

template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Base<T>::TAO_Intrusive_Ref_Count_Base ()
  : ref_count_(1)
{}


template <typename T>
ACE_INLINE
void
TAO_Intrusive_Ref_Count_Base<T>::_add_ref()
{
  ++this->ref_count_;
}


template <typename T>
ACE_INLINE
void
TAO_Intrusive_Ref_Count_Base<T>::_remove_ref()
{
  long new_count = --this->ref_count_;

  if (new_count != 0)
    {
      return;
    }

  delete this;
}
