// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Base<T>::TAO_Intrusive_Ref_Count_Base (void)
  : refcount_(1)
{}


template <typename T>
ACE_INLINE
void
TAO_Intrusive_Ref_Count_Base<T>::_add_ref (void)
{
  ++this->refcount_;
}


template <typename T>
ACE_INLINE
void
TAO_Intrusive_Ref_Count_Base<T>::_remove_ref (void)
{
  CORBA::ULong const new_count = --this->refcount_;

  if (new_count != 0)
    {
      return;
    }

  delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
