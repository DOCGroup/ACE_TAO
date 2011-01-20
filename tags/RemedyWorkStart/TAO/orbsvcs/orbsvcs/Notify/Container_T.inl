// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class TYPE> ACE_INLINE typename TAO_Notify_Container_T<TYPE>::COLLECTION*
TAO_Notify_Container_T<TYPE>:: collection (void)
{
  return this->collection_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
