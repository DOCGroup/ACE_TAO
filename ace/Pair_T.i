// $Id$

template <class T1, class T2> ACE_INLINE
ACE_Pair<T1, T2>::ACE_Pair (const T1 &t1,
                            const T2 &t2)
  : first_ (t1),
    second_ (t2)
{
}

template <class T1, class T2> ACE_INLINE
ACE_Pair<T1, T2>::ACE_Pair (void)
{
}

template <class T1, class T2> ACE_INLINE T1 &
ACE_Pair<T1, T2>::first (void)
{
  return this->first_;
}

template <class T1, class T2> ACE_INLINE const T1 &
ACE_Pair<T1, T2>::first (void) const
{
  return this->first_;
}

template <class T1, class T2> ACE_INLINE void
ACE_Pair<T1, T2>::first (const T1 &t1)
{
  this->first_ = t1;
}

template <class T1, class T2> ACE_INLINE T2 &
ACE_Pair<T1, T2>::second (void)
{
  return this->second_;
}

template <class T1, class T2> ACE_INLINE const T2 &
ACE_Pair<T1, T2>::second (void) const
{
  return this->second_;
}

template <class T1, class T2> ACE_INLINE void
ACE_Pair<T1, T2>::second (const T2 &t2)
{
  this->second_ = t2;
}

template <class T1, class T2> ACE_INLINE
ACE_Reference_Pair<T1, T2>::ACE_Reference_Pair (T1 &t1,
                                                T2 &t2)
  : first_ (t1),
    second_ (t2)
{
}

template <class T1, class T2> ACE_INLINE T1 &
ACE_Reference_Pair<T1, T2>::first (void) const
{
  return this->first_;
}

template <class T1, class T2> ACE_INLINE T2 &
ACE_Reference_Pair<T1, T2>::second (void) const
{
  return this->second_;
}

////////////////////////////////////////////////////////////////////////////////
