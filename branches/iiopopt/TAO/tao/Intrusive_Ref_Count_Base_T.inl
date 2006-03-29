// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Base<T>::TAO_Intrusive_Ref_Count_Base (void)
  : ref_count_(1)
{}


template <typename T>
ACE_INLINE
void
TAO_Intrusive_Ref_Count_Base<T>::_add_ref (void)
{
  ++this->ref_count_;
}


template <typename T>
ACE_INLINE
void
TAO_Intrusive_Ref_Count_Base<T>::_remove_ref (void)
{
  long const new_count = --this->ref_count_;

  if (new_count != 0)
    {
      return;
    }

  delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
