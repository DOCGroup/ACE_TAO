/* -*- C++ -*- $Id$ */

template <class ID_TYPE> ACE_INLINE ID_TYPE
ID_Pool<ID_TYPE>::get (void)
{
   return ++id_;
}

template <class ID_TYPE> ACE_INLINE void
ID_Pool<ID_TYPE>::put (ID_TYPE id)
{
  // TBD:
}
