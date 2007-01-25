// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class PROXY> ACE_INLINE typename
TAO_Notify_Event_Map_Entry_T<PROXY>::COLLECTION*
TAO_Notify_Event_Map_Entry_T<PROXY>::collection (void) {
  return collection_;
}

template <class PROXY> ACE_INLINE int
TAO_Notify_Event_Map_Entry_T<PROXY>::count (void)
{
  return this->count_;
}

template <class PROXY> ACE_INLINE CORBA::ULong
TAO_Notify_Event_Map_Entry_T<PROXY>::_incr_refcnt (void)
{
  return ++this->usage_count_;
}

template <class PROXY> ACE_INLINE CORBA::ULong
TAO_Notify_Event_Map_Entry_T<PROXY>::_decr_refcnt (void)
{
  return --this->usage_count_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
