/* -*- C++ -*- */
// $Id$

#if defined (ACE_USES_ACE_U_LONGLONG)

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
ACE_U_LongLong::operator== (const ACE_U_LongLong &n) const
{
  return hi_ == n.hi_  &&  lo_ == n.lo_;
}

ACE_INLINE int
ACE_U_LongLong::operator!= (const ACE_U_LongLong &n) const
{
  return ! (*this == n);
}

ACE_INLINE int
ACE_U_LongLong::operator< (const ACE_U_LongLong &n) const
{
  return hi_ < n.hi_ ? 1
                     : hi_ > n.hi_ ? 0
                                   : lo_ < n.lo_;
}

ACE_INLINE int
ACE_U_LongLong::operator<= (const ACE_U_LongLong &n) const
{
  return hi_ < n.hi_ ? 1
                     : hi_ > n.hi_ ? 0
                                   : lo_ <= n.lo_;
}

ACE_INLINE int
ACE_U_LongLong::operator> (const ACE_U_LongLong &n) const
{
  return hi_ > n.hi_ ? 1
                     : hi_ < n.hi_ ? 0
                                   : lo_ > n.lo_;
}

ACE_INLINE int
ACE_U_LongLong::operator>= (const ACE_U_LongLong &n) const
{
  return hi_ > n.hi_ ? 1
                     : hi_ < n.hi_ ? 0
                                   : lo_ >= n.lo_;
}

ACE_INLINE
ACE_U_LongLong::ACE_U_LongLong (const ACE_U_LongLong &n)
  : hi_ (n.hi_),
    lo_ (n.lo_)
{
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator= (const ACE_U_LongLong &n)
{
  hi_ = n.hi_;
  lo_ = n.lo_;
  return *this;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator+ (const ACE_U_LongLong &n) const
{
  ACE_U_LongLong ret (lo_ + n.lo_, hi_ + n.hi_);
  if (ret.lo_ < n.lo_) /* carry */ ++ret.hi_;
  return ret;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator- (const ACE_U_LongLong &n) const
{
  ACE_U_LongLong ret (lo_ - n.lo_, hi_ - n.hi_);
  if (lo_ < n.lo_) /* borrow */ --ret.hi_;
  return ret;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const ACE_UINT32 n) const
{
  // This takes advantage of the fact that the return type has only 32
  // bits.  Replace 0x100000000 with 0xffffffff + 1 because the latter
  // has 33 bits.
  // Quotient = (0x100000000u * hi_ + lo_) / n
  //          = ((0x100000000u - n + n) * hi_ + lo_) / n
  //          = ((0x100000000u - n) / n * hi_  +  hi_ * n / n  +  lo_ / n
  //          = (0x100000000u - n) / n * hi_ +  hi_  +  lo_ / n
  //          = (0xffffffffu - n + 1) / n * hi_ +  hi_  +  lo_ / n

  return (0xffffffffu - n + 1) / n * hi_  +  hi_  +  lo_ / n;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator% (const ACE_UINT32 n) const
{
  // Because the argument is an ACE_UINT32, the result can never be
  // bigger than 32 bits.  Replace 0x100000000 with 0xffffffff + 1
  // because the latter has 33 bits.
  // Mod = (0x100000000u * hi_ + lo_) % n
  //     = (0x100000000u % n * hi_  +  lo_ % n) % n
  //     = ((0x100000000u - n) % n * hi_  +  lo_ % n) % n
  //     = ((0xffffffffu - n + 1) % n * hi_  +  lo_ % n) % n

  return ((0xffffffff - n + 1)  % n * hi_  +  lo_ % n) % n;
}

ACE_INLINE double
ACE_U_LongLong::operator/ (const double n) const
{
  // See the derivation above in operator/ (const ACE_UINT32).

  return ((double) 0xffffffffu - n + 1.0) / n * hi_  +
         (double) hi_  +  (double) lo_ / n;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator+= (const ACE_U_LongLong &n)
{
  hi_ += n.hi_;
  lo_ += n.lo_;
  if (lo_ < n.lo_) /* carry */ ++hi_;
  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-= (const ACE_U_LongLong &n)
{
  hi_ -= n.hi_;
  if (lo_ < n.lo_) /* borrow */ --hi_;
  lo_ -= n.lo_;
  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator++ ()
{
  ++lo_;
  if (lo_ == 0) /* carry */ ++hi_;
  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-- ()
{
  if (lo_ == 0) /* borrow */ --hi_;
  --lo_;
  return *this;
}

#endif /* ACE_USES_ACE_U_LONGLONG */
