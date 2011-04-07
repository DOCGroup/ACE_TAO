// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename OBJ, typename ACE_LOCK>
ACE_INLINE
TAO_Intrusive_Ref_Count_Object<OBJ,ACE_LOCK>::TAO_Intrusive_Ref_Count_Object (void)
{}

template <typename OBJ, typename ACE_LOCK>
ACE_INLINE
TAO_Intrusive_Ref_Count_Object<OBJ,ACE_LOCK>::TAO_Intrusive_Ref_Count_Object (OBJ* obj)
: obj_ (obj)
{}


template <typename OBJ, typename ACE_LOCK>
ACE_INLINE OBJ*
TAO_Intrusive_Ref_Count_Object<OBJ,ACE_LOCK>::get () const
{
  return this->obj_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
