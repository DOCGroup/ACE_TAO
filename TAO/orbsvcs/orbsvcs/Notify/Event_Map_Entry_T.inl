// $Id$

template <class PROXY> ACE_INLINE TAO_NS_Event_Map_Entry_T<PROXY>::COLLECTION*
TAO_NS_Event_Map_Entry_T<PROXY>::collection (void)
{
  return collection_;
}

template <class PROXY> ACE_INLINE int
TAO_NS_Event_Map_Entry_T<PROXY>::count (void)
{
  return this->count_;
}
