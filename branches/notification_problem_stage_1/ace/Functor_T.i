/* -*- C++ -*- */
// $Id$

template <class TYPE> ACE_INLINE u_long
ACE_Hash<TYPE>::operator () (const TYPE &t) const
{
  return t.hash ();
}

template <class TYPE> ACE_INLINE u_long
ACE_Pointer_Hash<TYPE>::operator () (TYPE t) const
{
  return ACE_reinterpret_cast (u_long, t);
}

template <class TYPE> ACE_INLINE int
ACE_Equal_To<TYPE>::operator () (const TYPE &lhs,
                                 const TYPE &rhs) const
{
  return lhs == rhs;
}

template <class TYPE> ACE_INLINE int
ACE_Less_Than<TYPE>::operator () (const TYPE &lhs,
                                  const TYPE &rhs) const
{
  return lhs < rhs ? 1 : 0;
}
