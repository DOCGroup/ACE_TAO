// -*- C++ -*-
//
// $Id$

#include "ace/CDR_Base.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "ace/CDR_Base.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, CDR_Base, "$Id$")


//
// See comments in CDR_Base.i about optimization cases for swap_XX_array.
//

void
ACE_CDR::swap_2_array (const char* orig, char* target, size_t n)
{
  // ACE_ASSERT(n > 0); The caller checks that n > 0

  // Later, we try to read in 32 or 64 bit chunks,
  // so make sure we don't do that for unaligned addresses.
#if ACE_SIZEOF_LONG == 8
  const char* const o8 = ACE_ptr_align_binary(orig, 8);
  while (orig < o8 && n > 0)
    {
      ACE_CDR::swap_2 (orig, target);
      orig += 2;
      target += 2;
      --n;
    }
#else
  const char* const o4 = ACE_ptr_align_binary(orig, 4);
  // this is an _if_, not a _while_. The mistmatch can only be by 2.
  if (orig != o4)
    {
      ACE_CDR::swap_2 (orig, target);
      orig += 2;
      target += 2;
      --n;
    }
#endif
  if (n == 0)
    return;

  //
  // Loop unrolling. Here be dragons.
  //

  // (n & (~3)) is the greatest multiple of 4 not bigger than n.
  // In the while loop ahead, orig will move over the array by 8 byte
  // increments (4 elements of 2 bytes).
  // end marks our barrier for not falling outside.
  const char* const end = orig + 2*(n & (~3));

  // See if we're aligned for writting in 64 or 32 bit chunks...
#if ACE_SIZEOF_LONG == 8
  if (target == ACE_ptr_align_binary(target, 8))
#else
  if (target == ACE_ptr_align_binary(target, 4))
#endif
    {
      while (orig < end)
        {
#if defined(ACE_HAS_PENTIUM) && defined(__GNUG__)
          unsigned int a =
            * ACE_reinterpret_cast(const unsigned int*, orig);
          unsigned int b =
            * ACE_reinterpret_cast(const unsigned int*, orig + 4);
          asm( "bswap %1"      : "=r" (a) : "0" (a) );
          asm( "bswap %1"      : "=r" (b) : "0" (b) );
          asm( "rol $16, %1"   : "=r" (a) : "0" (a) );
          asm( "rol $16, %1"   : "=r" (b) : "0" (b) );
          * ACE_reinterpret_cast(unsigned int*, target) = a;
          * ACE_reinterpret_cast(unsigned int*, target + 4) = b;
#elif defined(ACE_HAS_PENTIUM) \
      && (defined(_MSC_VER) || defined(__BORLANDC__)) \
      && !defined(ACE_LACKS_INLINE_ASSEMBLY)
          __asm mov ecx, orig;
          __asm mov edx, target;
          __asm mov eax, [ecx];
          __asm mov ebx, 4[ecx];
          __asm bswap eax;
          __asm bswap ebx;
          __asm rol eax, 16;
          __asm rol ebx, 16;
          __asm mov [edx], eax;
          __asm mov 4[edx], ebx;
#elif ACE_SIZEOF_LONG == 8
          // 64 bit architecture.
          register unsigned long a =
            * ACE_reinterpret_cast(const unsigned long*, orig);

          register unsigned long a1 = (a & 0x00ff00ff00ff00ffUL) << 8;
          register unsigned long a2 = (a & 0xff00ff00ff00ff00UL) >> 8;

          a = (a1 | a2);

          * ACE_reinterpret_cast(unsigned long*, target) = a;
#else
          register ACE_UINT32 a =
            * ACE_reinterpret_cast(const ACE_UINT32*, orig);
          register ACE_UINT32 b =
            * ACE_reinterpret_cast(const ACE_UINT32*, orig + 4);

          register ACE_UINT32 a1 = (a & 0x00ff00ffU) << 8;
          register ACE_UINT32 b1 = (b & 0x00ff00ffU) << 8;
          register ACE_UINT32 a2 = (a & 0xff00ff00U) >> 8;
          register ACE_UINT32 b2 = (b & 0xff00ff00U) >> 8;

          a = (a1 | a2);
          b = (b1 | b2);

          * ACE_reinterpret_cast(ACE_UINT32*, target) = a;
          * ACE_reinterpret_cast(ACE_UINT32*, target + 4) = b;
#endif
          orig += 8;
          target += 8;
        }
    }
  else
    {
      // We're out of luck. We have to write in 2 byte chunks.
      while (orig < end)
        {
#if defined(ACE_HAS_PENTIUM) && defined(__GNUG__)
          unsigned int a =
            * ACE_reinterpret_cast(const unsigned int*, orig);
          unsigned int b =
            * ACE_reinterpret_cast(const unsigned int*, orig + 4);
          asm( "bswap %1" : "=r" (a) : "0" (a) );
          asm( "bswap %1" : "=r" (b) : "0" (b) );
          // We're little endian.
          * ACE_reinterpret_cast(unsigned short*, target + 2)
              = (unsigned short) (a & 0xffff);
          * ACE_reinterpret_cast(unsigned short*, target + 6)
              = (unsigned short) (b & 0xffff);
          asm( "shrl $16, %1" : "=r" (a) : "0" (a) );
          asm( "shrl $16, %1" : "=r" (b) : "0" (b) );
          * ACE_reinterpret_cast(unsigned short*, target + 0)
              = (unsigned short) (a & 0xffff);
          * ACE_reinterpret_cast(unsigned short*, target + 4)
              = (unsigned short) (b & 0xffff);
#elif defined(ACE_HAS_PENTIUM) \
      && (defined(_MSC_VER) || defined(__BORLANDC__)) \
      && !defined(ACE_LACKS_INLINE_ASSEMBLY)
          __asm mov ecx, orig;
          __asm mov edx, target;
          __asm mov eax, [ecx];
          __asm mov ebx, 4[ecx];
          __asm bswap eax;
          __asm bswap ebx;
          // We're little endian.
          __asm mov 2[edx], ax;
          __asm mov 6[edx], bx;
          __asm shr eax, 16;
          __asm shr ebx, 16;
          __asm mov 0[edx], ax;
          __asm mov 4[edx], bx;
#elif ACE_SIZEOF_LONG == 8
          // 64 bit architecture.
          register unsigned long a =
            * ACE_reinterpret_cast(const unsigned long*, orig);

          register unsigned long a1 = (a & 0x00ff00ff00ff00ffUL) << 8;
          register unsigned long a2 = (a & 0xff00ff00ff00ff00UL) >> 8;

          a = (a1 | a2);

          ACE_UINT16 b1 = ACE_static_cast(ACE_UINT16, (a >> 48));
          ACE_UINT16 b2 = ACE_static_cast(ACE_UINT16, ((a >> 32) & 0xffff));
          ACE_UINT16 b3 = ACE_static_cast(ACE_UINT16, ((a >> 16) & 0xffff));
          ACE_UINT16 b4 = ACE_static_cast(ACE_UINT16, (a & 0xffff));

#if defined(ACE_LITTLE_ENDIAN)
          * ACE_reinterpret_cast(ACE_UINT16*, target) = b4;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 2) = b3;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 4) = b2;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 6) = b1;
#else
          * ACE_reinterpret_cast(ACE_UINT16*, target) = b1;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 2) = b2;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 4) = b3;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 6) = b4;
#endif
#else
          register ACE_UINT32 a =
            * ACE_reinterpret_cast(const ACE_UINT32*, orig);
          register ACE_UINT32 b =
            * ACE_reinterpret_cast(const ACE_UINT32*, orig + 4);

          register ACE_UINT32 a1 = (a & 0x00ff00ff) << 8;
          register ACE_UINT32 b1 = (b & 0x00ff00ff) << 8;
          register ACE_UINT32 a2 = (a & 0xff00ff00) >> 8;
          register ACE_UINT32 b2 = (b & 0xff00ff00) >> 8;

          a = (a1 | a2);
          b = (b1 | b2);

          ACE_UINT32 c1 = ACE_static_cast(ACE_UINT16, (a >> 16));
          ACE_UINT32 c2 = ACE_static_cast(ACE_UINT16, (a & 0xffff));
          ACE_UINT32 c3 = ACE_static_cast(ACE_UINT16, (b >> 16));
          ACE_UINT32 c4 = ACE_static_cast(ACE_UINT16, (b & 0xffff));

#if defined(ACE_LITTLE_ENDIAN)
          * ACE_reinterpret_cast(ACE_UINT16*, target) = c2;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 2) = c1;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 4) = c4;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 6) = c3;
#else
          * ACE_reinterpret_cast(ACE_UINT16*, target) = c1;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 2) = c2;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 4) = c3;
          * ACE_reinterpret_cast(ACE_UINT16*, target + 6) = c4;
#endif
#endif

          orig += 8;
          target += 8;
        }
    }

  // (n & 3) == (n % 4).
  switch (n&3) {
  case 3:
    ACE_CDR::swap_2 (orig, target);
    orig += 2;
    target += 2;
  case 2:
    ACE_CDR::swap_2 (orig, target);
    orig += 2;
    target += 2;
  case 1:
    ACE_CDR::swap_2 (orig, target);
  }
}

void
ACE_CDR::swap_4_array (const char* orig, char* target, size_t n)
{
  // ACE_ASSERT(n > 0); The caller checks that n > 0

#if ACE_LONG_SIZE == 8
  // Later, we read from *orig in 64 bit chunks,
  // so make sure we don't generate unaligned readings.
  const char* const o8 = ACE_ptr_align_binary(orig, 8);
  // The mistmatch can only be by 4.
  if (orig != o8)
    {
      ACE_CDR::swap_4 (orig, target);
      orig += 4;
      target += 4;
      --n;
    }
#endif
  if (n == 0)
    return;

  //
  // Loop unrolling. Here be dragons.
  //

  // (n & (~3)) is the greatest multiple of 4 not bigger than n.
  // In the while loop, orig will move over the array by 16 byte
  // increments (4 elements of 4 bytes).
  // ends marks our barrier for not falling outside.
  const char* const end = orig + 4*(n & (~3));

#if ACE_LONG_SIZE == 8
  // 64 bits architecture.
  // See if we can write in 8 byte chunks.
  if (target == ACE_ptr_align_binary(target, 8))
    {
      while (orig < end)
        {
          register unsigned long a =
            * ACE_reinterpret_cast(const long*, orig);
          register unsigned long b =
            * ACE_reinterpret_cast(const long*, orig + 8);

          register unsigned long a84 = (a & 0x000000ff000000ffL) << 24;
          register unsigned long b84 = (b & 0x000000ff000000ffL) << 24;
          register unsigned long a73 = (a & 0x0000ff000000ff00L) << 8;
          register unsigned long b73 = (b & 0x0000ff000000ff00L) << 8;
          register unsigned long a62 = (a & 0x00ff000000ff0000L) >> 8;
          register unsigned long b62 = (b & 0x00ff000000ff0000L) >> 8;
          register unsigned long a51 = (a & 0xff000000ff000000L) >> 24;
          register unsigned long b51 = (b & 0xff000000ff000000L) >> 24;

          a = (a84 | a73 | a62 | a51);
          b = (b84 | b73 | b62 | b51);

          * ACE_reinterpret_cast(long*, target) = a;
          * ACE_reinterpret_cast(long*, target + 8) = b;

          orig += 16;
          target += 16;
        }
    }
  else
    {
      // We are out of luck, we have to write in 4 byte chunks.
      while (orig < end)
        {
          register unsigned long a =
            * ACE_reinterpret_cast(const long*, orig);
          register unsigned long b =
            * ACE_reinterpret_cast(const long*, orig + 8);

          register unsigned long a84 = (a & 0x000000ff000000ffL) << 24;
          register unsigned long b84 = (b & 0x000000ff000000ffL) << 24;
          register unsigned long a73 = (a & 0x0000ff000000ff00L) << 8;
          register unsigned long b73 = (b & 0x0000ff000000ff00L) << 8;
          register unsigned long a62 = (a & 0x00ff000000ff0000L) >> 8;
          register unsigned long b62 = (b & 0x00ff000000ff0000L) >> 8;
          register unsigned long a51 = (a & 0xff000000ff000000L) >> 24;
          register unsigned long b51 = (b & 0xff000000ff000000L) >> 24;

          a = (a84 | a73 | a62 | a51);
          b = (b84 | b73 | b62 | b51);

          ACE_UINT32 c1 = ACE_static_cast(ACE_UINT32, (a >> 32));
          ACE_UINT32 c2 = ACE_static_cast(ACE_UINT32, (a & 0xffffffff));
          ACE_UINT32 c3 = ACE_static_cast(ACE_UINT32, (b >> 32));
          ACE_UINT32 c4 = ACE_static_cast(ACE_UINT32, (b & 0xffffffff));

#if defined(ACE_LITTLE_ENDIAN)
          * ACE_reinterpret_cast(ACE_UINT32*, target + 0) = c2;
          * ACE_reinterpret_cast(ACE_UINT32*, target + 4) = c1;
          * ACE_reinterpret_cast(ACE_UINT32*, target + 8) = c4;
          * ACE_reinterpret_cast(ACE_UINT32*, target + 12) = c3;
#else
          * ACE_reinterpret_cast(ACE_UINT32*, target + 0) = c1;
          * ACE_reinterpret_cast(ACE_UINT32*, target + 4) = c2;
          * ACE_reinterpret_cast(ACE_UINT32*, target + 8) = c3;
          * ACE_reinterpret_cast(ACE_UINT32*, target + 12) = c4;
#endif
          orig += 16;
          target += 16;
        }
    }

#else /* ACE_LONG_SIZE != 8 */

  while (orig < end)
    {
#if defined(ACE_HAS_PENTIUM) && defined(__GNUG__)
      register unsigned int a =
        *ACE_reinterpret_cast(const unsigned int*, orig);
      register unsigned int b =
        *ACE_reinterpret_cast(const unsigned int*, orig + 4);
      register unsigned int c =
        *ACE_reinterpret_cast(const unsigned int*, orig + 8);
      register unsigned int d =
        *ACE_reinterpret_cast(const unsigned int*, orig + 12);

      asm ("bswap %1" : "=r" (a) : "0" (a));
      asm ("bswap %1" : "=r" (b) : "0" (b));
      asm ("bswap %1" : "=r" (c) : "0" (c));
      asm ("bswap %1" : "=r" (d) : "0" (d));

      *ACE_reinterpret_cast(unsigned int*, target) = a;
      *ACE_reinterpret_cast(unsigned int*, target + 4) = b;
      *ACE_reinterpret_cast(unsigned int*, target + 8) = c;
      *ACE_reinterpret_cast(unsigned int*, target + 12) = d;
#elif defined(ACE_HAS_PENTIUM) \
      && (defined(_MSC_VER) || defined(__BORLANDC__)) \
      && !defined(ACE_LACKS_INLINE_ASSEMBLY)
      __asm mov eax, orig
      __asm mov esi, target
      __asm mov edx, [eax]
      __asm mov ecx, 4[eax]
      __asm mov ebx, 8[eax]
      __asm mov eax, 12[eax]
      __asm bswap edx
      __asm bswap ecx
      __asm bswap ebx
      __asm bswap eax
      __asm mov [esi], edx
      __asm mov 4[esi], ecx
      __asm mov 8[esi], ebx
      __asm mov 12[esi], eax
#else
      register ACE_UINT32 a =
        * ACE_reinterpret_cast(const ACE_UINT32*, orig);
      register ACE_UINT32 b =
        * ACE_reinterpret_cast(const ACE_UINT32*, orig + 4);
      register ACE_UINT32 c =
        * ACE_reinterpret_cast(const ACE_UINT32*, orig + 8);
      register ACE_UINT32 d =
        * ACE_reinterpret_cast(const ACE_UINT32*, orig + 12);

      // Expect the optimizer reordering this A LOT.
      // We leave it this way for clarity.
      a = (a << 24) | ((a & 0xff00) << 8) | ((a & 0xff0000) >> 8) | (a >> 24);
      b = (b << 24) | ((b & 0xff00) << 8) | ((b & 0xff0000) >> 8) | (b >> 24);
      c = (c << 24) | ((c & 0xff00) << 8) | ((c & 0xff0000) >> 8) | (c >> 24);
      d = (d << 24) | ((d & 0xff00) << 8) | ((d & 0xff0000) >> 8) | (d >> 24);

      * ACE_reinterpret_cast(ACE_UINT32*, target) = a;
      * ACE_reinterpret_cast(ACE_UINT32*, target + 4) = b;
      * ACE_reinterpret_cast(ACE_UINT32*, target + 8) = c;
      * ACE_reinterpret_cast(ACE_UINT32*, target + 12) = d;
#endif

      orig += 16;
      target += 16;
    }

#endif /* ACE_LONG_SIZE == 8 */

  // (n & 3) == (n % 4).
  switch (n&3) {
  case 3:
    ACE_CDR::swap_4 (orig, target);
    orig += 4;
    target += 4;
  case 2:
    ACE_CDR::swap_4 (orig, target);
    orig += 4;
    target += 4;
  case 1:
    ACE_CDR::swap_4 (orig, target);
  }
}

//
// We don't benefit from unrolling in swap_8_array and swap_16_array
// (swap_8 and swap_16 are big enough).
//
void
ACE_CDR::swap_8_array (const char* orig, char* target, size_t n)
{
  // ACE_ASSERT(n > 0); The caller checks that n > 0

  const char* const end = orig + 8*n;
  while (orig < end)
    {
      swap_8(orig, target);
      orig += 8;
      target += 8;
    }
}

void
ACE_CDR::swap_16_array (const char* orig, char* target, size_t n)
{
  // ACE_ASSERT(n > 0); The caller checks that n > 0

  const char* const end = orig + 16*n;
  while (orig < end)
    {
      swap_16(orig, target);
      orig += 16;
      target += 16;
    }
}

int
ACE_CDR::grow (ACE_Message_Block *mb, size_t minsize)
{
  size_t newsize =
    ACE_CDR::first_size (minsize + ACE_CDR::MAX_ALIGNMENT);

  if (newsize <= mb->size ())
    return 0;

  ACE_Data_Block *db =
    mb->data_block ()->clone_nocopy ();

  if (db->size (newsize) == -1)
    return -1;

  ACE_Message_Block tmp (db);
  ACE_CDR::mb_align (&tmp);

  tmp.copy (mb->rd_ptr (), mb->length());
  mb->data_block (tmp.data_block ()->duplicate ());
  mb->rd_ptr (tmp.rd_ptr ());
  mb->wr_ptr (tmp.wr_ptr ());

  // Remove the DONT_DELETE flags from mb
  mb->clr_self_flags (ACE_Message_Block::DONT_DELETE);

  return 0;
}

size_t
ACE_CDR::total_length (const ACE_Message_Block* begin,
                       const ACE_Message_Block* end)
{
  size_t l = 0;
  // Compute the total size.
  for (const ACE_Message_Block *i = begin;
       i != end;
       i = i->cont ())
    l += i->length ();
  return l;
}

void
ACE_CDR::consolidate (ACE_Message_Block *dst,
                      const ACE_Message_Block *src)
{
  if (src == 0)
    return;

  // If we do not have a chain of message blocks, all that is needed
  // is an increment of the reference count.
  if(src->cont () != 0)
    {
      size_t newsize =
        ACE_CDR::first_size (ACE_CDR::total_length (src, 0)
                             + ACE_CDR::MAX_ALIGNMENT);
      dst->size (newsize);

      // We must copy the contents of <src> into the new buffer, but
      // respecting the alignment.
      ptr_arith_t srcalign =
        ptr_arith_t(src->rd_ptr ()) % ACE_CDR::MAX_ALIGNMENT;
      ptr_arith_t dstalign =
        ptr_arith_t(dst->rd_ptr ()) % ACE_CDR::MAX_ALIGNMENT;
      int offset = srcalign - dstalign;
      if (offset < 0)
        offset += ACE_CDR::MAX_ALIGNMENT;
      dst->rd_ptr (offset);
      dst->wr_ptr (dst->rd_ptr ());

      for (const ACE_Message_Block* i = src;
           i != 0;
           i = i->cont ())
        {
          dst->copy (i->rd_ptr (), i->length ());
        }
    }
  else
    {
      // Just increment the refcount of the underlying datablock and
      // assign the data block to the message block
      dst->data_block (src->data_block()->duplicate());

      // Adjust rd/wr pointers
      dst->rd_ptr (src->rd_ptr());
      dst->wr_ptr (src->wr_ptr());
    }
}

#if defined (NONNATIVE_LONGLONG)
int
ACE_CDR::LongLong::operator== (const ACE_CDR::LongLong &rhs) const
{
  return this->h == rhs.h && this->l == rhs.l;
}

int
ACE_CDR::LongLong::operator!= (const ACE_CDR::LongLong &rhs) const
{
  return this->l != rhs.l || this->h != rhs.h;
}
#endif /* NONNATIVE_LONGLONG */

#if defined (NONNATIVE_LONGDOUBLE)
int
ACE_CDR::LongDouble::operator== (const ACE_CDR::LongDouble &rhs) const
{
  return ACE_OS::memcmp (this->ld, rhs.ld, 16) == 0;
}

int
ACE_CDR::LongDouble::operator!= (const ACE_CDR::LongDouble &rhs) const
{
  return ACE_OS::memcmp (this->ld, rhs.ld, 16) != 0;
}
#endif /* NONNATIVE_LONGDOUBLE */

#if defined(_UNICOS) && !defined(_CRAYMPP)
// placeholders to get things compiling
ACE_CDR::Float::Float()
{
}

ACE_CDR::Float::Float(const float & init)
{
}

ACE_CDR::Float &
ACE_CDR::Float::operator= (const float &rhs)
{
  return *this;
}

int
ACE_CDR::Float::operator!= (const ACE_CDR::Float &rhs) const
{
  return 0;
}
#endif /* _UNICOS */
