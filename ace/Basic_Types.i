/* -*- C++ -*- */
// $Id$

#if defined (ACE_LACKS_LONGLONG_T)

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

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator<< (const u_int n) const
{
  const ACE_UINT32 carry_mask = lo_ >> (32 - n);
  ACE_U_LongLong ret (n < 32  ?  lo_ << n  :  0,
                      n < 32  ?  (hi_ << n) | carry_mask  :  carry_mask);

  return ret;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator<<= (const u_int n)
{
  const ACE_UINT32 carry_mask = lo_ >> (32 - n);
  hi_ = n < 32  ?  (hi_ << n) | carry_mask  :  carry_mask;

  // g++ 2.7.2.3/Solaris 2.5.1 doesn't modify lo_ if shifted by 32.
  lo_ = n < 32  ?  lo_ << n  :  0;

  return *this;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator>> (const u_int n) const
{
  const ACE_UINT32 carry_mask = hi_ << (32 - n);
  ACE_U_LongLong ret (n < 32  ?  (lo_ >> n) | carry_mask  :  0,
                      n < 32  ?  hi_ >> n  :  0);

  return ret;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator>>= (const u_int n)
{
  const ACE_UINT32 carry_mask = hi_ << (32 - n);
  lo_ = n < 32  ?  (lo_ >> n) | carry_mask  :  carry_mask;
  hi_ = n < 32  ?  hi_ >> n  :  0;

  return *this;
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
ACE_U_LongLong::operator*= (const ACE_UINT32 n)
{
  // Save the current lo_.
  const ACE_UINT32 lo = lo_;

  lo_ = 0;
  if (n == 0)
    hi_ = 0;

  // Repeatedly add in the lo_ value that was saved above.
  for (ACE_UINT32 i = 0; i < n; ++i)
    operator+= (lo);

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

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator|= (const ACE_U_LongLong n)
{
  lo_ |= n.lo_;
  hi_ |= n.hi_;

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator&= (const ACE_U_LongLong n)
{
  lo_ &= n.lo_;
  hi_ &= n.hi_;

  return *this;
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

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator| (const ACE_INT32 n) const
{
  return lo_ | n;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator& (const ACE_INT32 n) const
{
  return lo_ & n;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator*= (const ACE_INT32 n)
{
  return operator*= ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const ACE_INT32 n) const
{
  return operator/ ((ACE_UINT32) n);
}

#if ACE_SIZEOF_INT == 4
ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const u_long n) const
{
  return operator/ ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const long n) const
{
  return operator/ ((ACE_UINT32) n);
}

#else  /* ACE_SIZEOF_INT != 4 */
ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const u_int n) const
{
  return operator/ ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const int n) const
{
  return operator/ ((ACE_UINT32) n);
}
#endif /* ACE_SIZEOF_INT != 4 */

#endif /* ACE_LACKS_LONGLONG_T */
