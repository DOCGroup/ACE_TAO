/* -*- C++ -*- */
// $Id$

#if !defined (ACE_WIN32) && !defined (ACE_HAS_LONGLONG_T)
ACE_INLINE
ACE_U_LongLong::ACE_U_LongLong (const ACE_UINT32 lo, const ACE_UINT32 hi)
  : hi_ (hi), lo_ (lo)
{
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::hi (void) const
{
  return hi_;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::lo (void) const
{
  return lo_;
}

ACE_INLINE void
ACE_U_LongLong::hi (const ACE_UINT32 hi)
{
  hi_ = hi;
}

ACE_INLINE void
ACE_U_LongLong::lo (const ACE_UINT32 lo)
{
  lo_ = lo;
}

ACE_INLINE
ACE_U_LongLong::~ACE_U_LongLong (void)
{
}

ACE_INLINE int
ACE_U_LongLong::operator== (const ACE_U_LongLong &ll) const
{
  return hi_ == ll.hi_  &&  lo_ == ll.lo_;
}

ACE_INLINE int
ACE_U_LongLong::operator!= (const ACE_U_LongLong &ll) const
{
  return ! (*this == ll);
}

ACE_INLINE int
ACE_U_LongLong::operator< (const ACE_U_LongLong &ll) const
{
  return hi_ < ll.hi_ ? 1
                      : hi_ > ll.hi_ ? 0
                                     : lo_ < ll.lo_;
}

ACE_INLINE int
ACE_U_LongLong::operator<= (const ACE_U_LongLong &ll) const
{
  return hi_ < ll.hi_ ? 1
                      : hi_ > ll.hi_ ? 0
                                     : lo_ <= ll.lo_;
}

ACE_INLINE int
ACE_U_LongLong::operator> (const ACE_U_LongLong &ll) const
{
  return hi_ > ll.hi_ ? 1
                      : hi_ < ll.hi_ ? 0
                                     : lo_ > ll.lo_;
}

ACE_INLINE int
ACE_U_LongLong::operator>= (const ACE_U_LongLong &ll) const
{
  return hi_ > ll.hi_ ? 1
                      : hi_ < ll.hi_ ? 0
                                     : lo_ >= ll.lo_;
}

ACE_INLINE
ACE_U_LongLong::ACE_U_LongLong (const ACE_U_LongLong &ll)
  : hi_ (ll.hi_),
    lo_ (ll.lo_)
{
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator= (const ACE_U_LongLong &ll)
{
  hi_ = ll.hi_;
  lo_ = ll.lo_;
  return *this;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator+ (const ACE_U_LongLong &ll) const
{
  ACE_U_LongLong ret (lo_ + ll.lo_, hi_ + ll.hi_);
  if (ret.lo_ < ll.lo_) /* carry */ ++ret.hi_;
  return ret;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator- (const ACE_U_LongLong &ll) const
{
  ACE_U_LongLong ret (lo_ - ll.lo_, hi_ - ll.hi_);
  if (lo_ < ll.lo_) /* borrow */ --ret.hi_;
  return ret;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const ACE_UINT32 ul) const
{
  return hi_ / ul * ULONG_MAX + lo_ / ul;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator+= (const ACE_U_LongLong &ll)
{
  hi_ += ll.hi_;
  lo_ += ll.lo_;
  if (lo_ < ll.lo_) /* carry */ ++hi_;
  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-= (const ACE_U_LongLong &ll)
{
  hi_ -= ll.hi_;
  if (lo_ < ll.lo_) /* borrow */ --hi_;
  lo_ -= ll.lo_;
  return *this;
}

#endif /* ! ACE_WIN32 && ! ACE_HAS_LONGLONG_T */
