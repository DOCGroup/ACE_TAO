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
#if defined (ACE_WIN64)
  // The cast below is legit... we only want a hash, and need not convert
  // the hash back to a pointer.
#  pragma warning(push)
#  pragma warning(disable : 4311)   /* Truncate pointer to u_long */
#endif /* ACE_WIN64 */
  return ACE_reinterpret_cast (u_long, t);
#if defined (ACE_WIN64)
#  pragma warning(pop)
#endif /* ACE_WIN64 */
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
