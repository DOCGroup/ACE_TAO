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

#define ACE_HIGHBIT (~(~0UL >> 1))

ACE_INLINE ACE_UINT32
ACE_U_LongLong::ul_shift (ACE_UINT32 a, ACE_UINT32 c_in, ACE_UINT32 *c_out)
{
  const ACE_UINT32 b = (a << 1) | c_in;
  *c_out = (*c_out << 1) + ((a & ACE_HIGHBIT) > 0);

  return b;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_shift (ACE_U_LongLong a, ACE_UINT32 c_in,
                           ACE_UINT32 *c_out)
{
  ACE_U_LongLong b;

  b.lo_ = (a.lo_ << 1) | c_in;
  c_in = ((a.lo_ & ACE_HIGHBIT) > 0);
  b.hi_ = (a.hi_ << 1) | c_in;
  *c_out = (*c_out << 1) + ((a.hi_ & ACE_HIGHBIT) > 0);

  return b;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_add (ACE_U_LongLong a, ACE_U_LongLong b, ACE_UINT32 *carry)
{
  ACE_U_LongLong r (0, 0);
  ACE_UINT32 c1, c2, c3, c4;

  c1 = a.lo_ % 2;
  c2 = b.lo_ % 2;
  c3 = 0;

  r.lo_ = a.lo_/2 +  b.lo_/2 + (c1+c2)/2;
  r.lo_ = ul_shift (r.lo_, (c1+c2)%2, &c3);

  c1 = a.hi_ % 2;
  c2 = b.hi_ % 2;
  c4 = 0;

  r.hi_ = a.hi_/2 + b.hi_/2 + (c1+c2+c3)/2;
  r.hi_ = ul_shift (r.hi_, (c1+c2+c3)%2, &c4);

  *carry = c4;

  return r;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_mult (ACE_U_LongLong a, ACE_UINT32 b, ACE_UINT32 *carry)
{
  register ACE_UINT32 mask = ACE_HIGHBIT;
  const ACE_U_LongLong zero (0, 0);
  ACE_U_LongLong accum (0, 0);
  ACE_UINT32 c;

  *carry = 0;
  if (b > 0)
    do
      {
        accum = ull_shift (accum, 0U, carry);
        if (b & mask)
          accum = ull_add (accum, a, &c);
        else
          accum = ull_add (accum, zero, &c);
        *carry += c;
        mask >>= 1;
      }
    while (mask > 0);

  return accum;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator* (const ACE_UINT32 n)
{
  ACE_UINT32 carry;  // will throw the carry away

  return ull_mult (*this, n, &carry);
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator*= (const ACE_UINT32 n)
{
  ACE_UINT32 carry;  // will throw the carry away

  return *this = ull_mult (*this, n, &carry);
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
  // bits.  Replace 0x100000000 with 0xffffffff + 1 because the former
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
  // because the former has 33 bits.
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

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator* (const ACE_INT32 n)
{
  return operator* ((ACE_UINT32) n);
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
