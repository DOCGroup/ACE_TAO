// $Id$

#include "ace/CDR_Stream.h"

#if !defined (__ACE_INLINE__)
# include "ace/CDR_Stream.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, CDR_Stream, "$Id$")

//
// See comments in CDR_Stream.i about optimization cases for swap_XX_array.
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
  db->size (newsize);

  ACE_Message_Block tmp (db);
  ACE_CDR::mb_align (&tmp);

  tmp.copy (mb->rd_ptr (), mb->length());
  mb->data_block (tmp.data_block ()->duplicate ());
  mb->rd_ptr (tmp.rd_ptr ());
  mb->wr_ptr (tmp.wr_ptr ());

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

// ****************************************************************

ACE_OutputCDR::ACE_OutputCDR (size_t size,
                              int byte_order,
                              ACE_Allocator *buffer_allocator,
                              ACE_Allocator *data_block_allocator,
                              size_t memcpy_tradeoff)
  :  start_ (size ? size : ACE_CDR::DEFAULT_BUFSIZE + ACE_CDR::MAX_ALIGNMENT,
             ACE_Message_Block::MB_DATA,
             0,
             0,
             buffer_allocator,
             0,
             0,
             ACE_Time_Value::zero,
             ACE_Time_Value::max_time,
             data_block_allocator),
     current_is_writable_ (1),
     current_alignment_ (0),
     do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff),
     char_translator_ (0),
     wchar_translator_ (0)
{
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

ACE_OutputCDR::ACE_OutputCDR (char *data, size_t size,
                              int byte_order,
                              ACE_Allocator *buffer_allocator,
                              ACE_Allocator *data_block_allocator,
                              size_t memcpy_tradeoff)
  :  start_ (size,
             ACE_Message_Block::MB_DATA,
             0,
             data,
             buffer_allocator,
             0,
             0,
             ACE_Time_Value::zero,
             ACE_Time_Value::max_time,
             data_block_allocator),
     current_is_writable_ (1),
     current_alignment_ (0),
     do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff),
     char_translator_ (0),
     wchar_translator_ (0)
{
  // We cannot trust the buffer to be properly aligned
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

ACE_OutputCDR::ACE_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              size_t memcpy_tradeoff)
  :  start_ (data->data_block ()->duplicate ()),
     current_is_writable_ (1),
     current_alignment_ (0),
     do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
     good_bit_ (1),
     memcpy_tradeoff_ (memcpy_tradeoff),
     char_translator_ (0),
     wchar_translator_ (0)
{
  // We cannot trust the buffer to be properly aligned
  ACE_CDR::mb_align (&this->start_);
  this->current_ = &this->start_;
}

int
ACE_OutputCDR::grow_and_adjust (size_t size,
                                size_t align,
                                char*& buf)
{
  if (!this->current_is_writable_
      || this->current_->cont () == 0
      || this->current_->cont ()->size () < size + ACE_CDR::MAX_ALIGNMENT)
    {
      // Calculate the new buffer's length; if growing for encode, we
      // don't grow in "small" chunks because of the cost.
      size_t cursize = this->current_->size ();
      if (this->current_->cont () != 0)
        cursize = this->current_->cont ()->size ();

      size_t minsize = size + ACE_CDR::MAX_ALIGNMENT;
      // Make sure that there is enough room for <minsize> bytes, but
      // also make it bigger than whatever our current size is.
      if (minsize < cursize)
        {
          minsize = cursize;
        }

      size_t newsize =
        ACE_CDR::next_size (minsize);

      this->good_bit_ = 0;
      ACE_Message_Block* tmp;
      ACE_NEW_RETURN (tmp,
                      ACE_Message_Block (newsize,
                                         ACE_Message_Block::MB_DATA,
                                         0,
                                         0,
                                         this->current_->data_block ()->allocator_strategy (),
                                         0,
                                         0,
                                         ACE_Time_Value::zero,
                                         ACE_Time_Value::max_time,
                                         this->current_->data_block ()->data_block_allocator ()),
                      -1);
      this->good_bit_ = 1;

      // The new block must start with the same alignment as the
      // previous block finished.
      ptr_arith_t tmpalign =
        ptr_arith_t(tmp->rd_ptr ()) % ACE_CDR::MAX_ALIGNMENT;
      ptr_arith_t curalign =
        ptr_arith_t(this->current_alignment_) % ACE_CDR::MAX_ALIGNMENT;
      int offset = curalign - tmpalign;
      if (offset < 0)
        offset += ACE_CDR::MAX_ALIGNMENT;
      tmp->rd_ptr (offset);
      tmp->wr_ptr (tmp->rd_ptr ());

      // grow the chain and set the current block.
      tmp->cont (this->current_->cont ());
      this->current_->cont (tmp);
    }
  this->current_ = this->current_->cont ();
  this->current_is_writable_ = 1;

  return this->adjust (size, align, buf);
}

ACE_CDR::Boolean
ACE_OutputCDR::write_string (ACE_CDR::ULong len,
                             const char *x)
{
  // @@ This is a slight violation of "Optimize for the common case",
  // i.e. normally the translator will be 0, but OTOH the code is
  // smaller and should be better for the cache ;-) ;-)
  if (this->char_translator_ != 0)
    return this->char_translator_->write_string (*this, len, x);

  if (len != 0)
    {
      if (this->write_ulong (len + 1))
        {
          return this->write_char_array (x, len + 1);
        }
    }
  else
    {
      // Be nice to programmers: treat nulls as empty strings not
      // errors. (OMG-IDL supports languages that don't use the C/C++
      // notion of null v. empty strings; nulls aren't part of the OMG-IDL
      // string model.)
      if (this->write_ulong (1))
        {
          return this->write_char (0);
        }
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_wstring (ACE_CDR::ULong len,
                              const ACE_CDR::WChar *x)
{
  // @@ This is a slight violation of "Optimize for the common case",
  // i.e. normally the translator will be 0, but OTOH the code is
  // smaller and should be better for the cache ;-) ;-)
  if (this->wchar_translator_ != 0)
    return this->wchar_translator_->write_wstring (*this, len, x);

  if (x != 0)
    {
      if (this->write_ulong (len + 1))
        {
          return this->write_wchar_array (x, len + 1);
        }
    }
  else
    {
      if (this->write_ulong (1))
        {
          return this->write_wchar (0);
        }
    }
  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_octet_array_mb (const ACE_Message_Block* mb)
{
  // If the buffer is small and it fits in the current message
  // block it is be cheaper just to copy the buffer.
  for (const ACE_Message_Block* i = mb;
       i != 0;
       i = i->cont ())
    {
      size_t length = i->length ();

      // If the mb does not own its data we are forced to make a copy.
      if (ACE_BIT_ENABLED (i->flags (),
                           ACE_Message_Block::DONT_DELETE))
        {
          if (! this->write_array (i->rd_ptr (),
                                   ACE_CDR::OCTET_SIZE,
                                   ACE_CDR::OCTET_ALIGN,
                                   length))
            {
              return 0;
            }
          continue;
        }

      if (length < this->memcpy_tradeoff_
          && this->current_->wr_ptr () + length < this->current_->end ())
        {
          if (! this->write_array (i->rd_ptr (),
                                   ACE_CDR::OCTET_SIZE,
                                   ACE_CDR::OCTET_ALIGN,
                                   length))
            {
              return 0;
            }
          continue;
        }

      ACE_Message_Block* cont;
      this->good_bit_ = 0;
      ACE_NEW_RETURN (cont,
                      ACE_Message_Block (i->data_block ()->duplicate ()),
                      0);
      this->good_bit_ = 1;

      if (cont != 0)
        {
          if (this->current_->cont () != 0)
            ACE_Message_Block::release (this->current_->cont ());
          cont->rd_ptr (i->rd_ptr ());
          cont->wr_ptr (i->wr_ptr ());

          this->current_->cont (cont);
          this->current_ = cont;
          this->current_is_writable_ = 0;
          this->current_alignment_ =
            (this->current_alignment_ + cont->length ()) % ACE_CDR::MAX_ALIGNMENT;
        }
      else
        {
          this->good_bit_ = 0;
          return 0;
        }
    }
  return 1;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_1 (const ACE_CDR::Octet *x)
{
  char *buf;
  if (this->adjust (1, buf) == 0)
    {
      *ACE_reinterpret_cast(ACE_CDR::Octet*, buf) = *x;
      return 1;
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_2 (const ACE_CDR::UShort *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::SHORT_SIZE, buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(ACE_CDR::UShort*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (ACE_CDR::UShort *, buf) = *x;
          return 1;
        }
      else
        {
          ACE_CDR::swap_2 (ACE_reinterpret_cast (const char*, x), buf);
          return 1;
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_4 (const ACE_CDR::ULong *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONG_SIZE, buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(ACE_CDR::ULong*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (ACE_CDR::ULong *, buf) = *x;
          return 1;
        }
      else
        {
          ACE_CDR::swap_4 (ACE_reinterpret_cast (const char*, x), buf);
          return 1;
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_8 (const ACE_CDR::ULongLong *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONGLONG_SIZE, buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(ACE_CDR::ULongLong*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (ACE_CDR::ULongLong *, buf) = *x;
          return 1;
        }
      else
        {
          ACE_CDR::swap_8 (ACE_reinterpret_cast (const char*, x), buf);
          return 1;
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_16 (const ACE_CDR::LongDouble *x)
{
  char* buf;
  if (this->adjust (ACE_CDR::LONGDOUBLE_SIZE,
                    ACE_CDR::LONGDOUBLE_ALIGN,
                    buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(ACE_CDR::LongDouble*,buf) = *x;
      return 1;
#else
      if (!this->do_byte_swap_)
        {
          *ACE_reinterpret_cast (ACE_CDR::LongDouble *, buf) = *x;
          return 1;
        }
      else
        {
          ACE_CDR::swap_16 (ACE_reinterpret_cast (const char*, x), buf);
          return 1;
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }

  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_array (const void *x,
                            size_t size,
                            size_t align,
                            ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;
  char *buf;
  if (this->adjust (size * length, align, buf) == 0)
    {
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
      ACE_OS::memcpy (buf, x, size*length);
      return 1;
#else
      if (!this->do_byte_swap_ || size == 1)
        {
          ACE_OS::memcpy (buf, x, size*length);
          return 1;
        }
      else
        {
          const char *source = ACE_reinterpret_cast (const char *, x);
          switch (size)
            {
            case 2:
	      ACE_CDR::swap_2_array (source, buf, length);
	      return 1;
            case 4:
	      ACE_CDR::swap_4_array (source, buf, length);
	      return 1;
            case 8:
	      ACE_CDR::swap_8_array (source, buf, length);
	      return 1;
            case 16:
	      ACE_CDR::swap_16_array (source, buf, length);
	      return 1;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return 0;
            }
        }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
    }
  this->good_bit_ = 0;
  return 0;
}

ACE_CDR::Boolean
ACE_OutputCDR::write_boolean_array (const ACE_CDR::Boolean* x,
                                    ACE_CDR::ULong length)
{
  // It is hard to optimize this, the spec requires that on the wire
  // booleans be represented as a byte with value 0 or 1, but in
  // memoery it is possible (though very unlikely) that a boolean has
  // a non-zero value (different from 1).
  // We resort to a simple loop.
  const ACE_CDR::Boolean* end = x + length;
  for (const ACE_CDR::Boolean* i = x; i != end && this->good_bit (); ++i)
    {
      this->write_boolean (*i);
    }
  return this->good_bit ();
}

// ****************************************************************

ACE_InputCDR::ACE_InputCDR (const char *buf,
                            size_t bufsiz,
                            int byte_order)
  : start_ (buf, bufsiz),
    do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0)
{
  this->start_.wr_ptr (bufsiz);
}

ACE_InputCDR::ACE_InputCDR (size_t bufsiz,
                            int byte_order)
  : start_ (bufsiz),
    do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0)
{
}

ACE_InputCDR::ACE_InputCDR (const ACE_Message_Block *data,
                            int byte_order)
  : start_ (),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0)
{
  this->reset (data, byte_order);
}

ACE_InputCDR::ACE_InputCDR (ACE_Data_Block *data,
                            int byte_order)
  : start_ (data),
    do_byte_swap_ (byte_order != ACE_CDR_BYTE_ORDER),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0)
{
}

ACE_InputCDR::ACE_InputCDR (const ACE_InputCDR& rhs,
                            size_t size,
                            ACE_CDR::Long offset)
  : start_ (rhs.start_.data_block ()->duplicate ()),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0)
{
  char* newpos = rhs.start_.rd_ptr() + offset;
  if (this->start_.base () <= newpos
      && newpos <= this->start_.end ()
      && newpos + size <= this->start_.end ())
    {
      this->start_.rd_ptr (newpos);
      this->start_.wr_ptr (newpos + size);
    }
  else
    {
      this->good_bit_ = 0;
    }
}

ACE_InputCDR::ACE_InputCDR (const ACE_InputCDR& rhs,
                            size_t size)
  : start_ (rhs.start_.data_block ()->duplicate ()),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0)
{
  char* newpos = rhs.start_.rd_ptr();
  if (this->start_.base () <= newpos
      && newpos <= this->start_.end ()
      && newpos + size <= this->start_.end ())
    {
      // Notice that ACE_Message_Block::duplicate may leave the
      // wr_ptr() with a higher value that what we actually want.
      this->start_.rd_ptr (newpos);
      this->start_.wr_ptr (newpos + size);

      ACE_CDR::Octet byte_order;
      this->read_octet (byte_order);
      this->do_byte_swap_ = (byte_order != ACE_CDR_BYTE_ORDER);
    }
  else
    {
      this->good_bit_ = 0;
    }
}

ACE_InputCDR::ACE_InputCDR (const ACE_InputCDR& rhs)
  : start_ (rhs.start_.data_block ()->duplicate ()),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (rhs.char_translator_),
    wchar_translator_ (rhs.wchar_translator_)
{
  this->start_.rd_ptr (rhs.start_.rd_ptr ());
  this->start_.wr_ptr (rhs.start_.wr_ptr ());
}

ACE_InputCDR::ACE_InputCDR (ACE_InputCDR::Transfer_Contents x)
  : start_ (x.rhs_.start_.data_block ()),
    do_byte_swap_ (x.rhs_.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (x.rhs_.char_translator_),
    wchar_translator_ (x.rhs_.wchar_translator_)
{
  this->start_.rd_ptr (x.rhs_.start_.rd_ptr ());
  this->start_.wr_ptr (x.rhs_.start_.wr_ptr ());

  ACE_Data_Block* db = this->start_.data_block ()->clone_nocopy ();
  (void) x.rhs_.start_.replace_data_block (db);
}

ACE_InputCDR&
ACE_InputCDR::operator= (const ACE_InputCDR& rhs)
{
  if (this != &rhs)
    {
      this->start_.data_block (rhs.start_.data_block ()->duplicate ());
      this->start_.rd_ptr (rhs.start_.rd_ptr ());
      this->start_.wr_ptr (rhs.start_.wr_ptr ());
      this->do_byte_swap_ = rhs.do_byte_swap_;
      this->good_bit_ = 1;
    }
  return *this;
}

ACE_InputCDR::ACE_InputCDR (const ACE_OutputCDR& rhs,
                            ACE_Allocator* buffer_allocator,
                            ACE_Allocator* data_block_allocator)
  : start_ (rhs.total_length () + ACE_CDR::MAX_ALIGNMENT,
            ACE_Message_Block::MB_DATA,
            0,
            0,
            buffer_allocator,
            0,
            0,
            ACE_Time_Value::zero,
            ACE_Time_Value::max_time,
            data_block_allocator),
    do_byte_swap_ (rhs.do_byte_swap_),
    good_bit_ (1),
    char_translator_ (0),
    wchar_translator_ (0)
{
  ACE_CDR::mb_align (&this->start_);
  for (const ACE_Message_Block *i = rhs.begin ();
       i != rhs.end ();
       i = i->cont ())
    this->start_.copy (i->rd_ptr (), i->length ());
}

ACE_CDR::Boolean
ACE_InputCDR::read_string (char *&x)
{
  // @@ This is a slight violation of "Optimize for the common case",
  // i.e. normally the translator will be 0, but OTOH the code is
  // smaller and should be better for the cache ;-) ;-)
  if (this->char_translator_ != 0)
    return this->char_translator_->read_string (*this, x);

  ACE_CDR::ULong len;

  this->read_ulong (len);
  if (len > 0)
    {
      ACE_NEW_RETURN (x,
                      ACE_CDR::Char[len],
                      0);
      if (this->read_char_array (x, len))
        return 1;
      delete [] x;
    }

  x = 0;
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_string (ACE_CString &x)
{
  ACE_CDR::Char *data;
  if (this->read_string (data))
    {
      x = data;
      delete [] data;
      return 1;
    }

  x = "";
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_wstring (ACE_CDR::WChar*& x)
{
  // @@ This is a slight violation of "Optimize for the common case",
  // i.e. normally the translator will be 0, but OTOH the code is
  // smaller and should be better for the cache ;-) ;-)
  if (this->wchar_translator_ != 0)
    return this->wchar_translator_->read_wstring (*this, x);

  ACE_CDR::ULong len;
  this->read_ulong (len);
  if (this->good_bit())
    {
      ACE_NEW_RETURN (x,
                      ACE_CDR::WChar[len],
                      0);
      if (this->read_wchar_array (x, len))
        return 1;

      delete [] x;
    }
  x = 0;
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_array (void* x,
                          size_t size,
                          size_t align,
                          ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;
  char* buf;
  if (this->adjust (size * length, align, buf) == 0)
    {
#if defined (ACE_DISABLE_SWAP_ON_READ)
      ACE_OS::memcpy (x, buf, size*length);
#else
      if (!this->do_byte_swap_ || size == 1)
        {
          ACE_OS::memcpy (x, buf, size*length);
        }
      else
        {
          char *target = ACE_reinterpret_cast (char*, x);
          switch (size)
            {
            case 2:
 	      ACE_CDR::swap_2_array (buf, target, length);
	      break;
            case 4:
	      ACE_CDR::swap_4_array (buf, target, length);
              break;
            case 8:
	      ACE_CDR::swap_8_array (buf, target, length);
              break;
            case 16:
	      ACE_CDR::swap_16_array (buf, target, length);
              break;
            default:
              // TODO: print something?
              this->good_bit_ = 0;
              return 0;
            }
        }
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return this->good_bit_;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_boolean_array (ACE_CDR::Boolean *x,
                                  ACE_CDR::ULong length)
{
  // It is hard to optimize this, the spec requires that on the wire
  // booleans be represented as a byte with value 0 or 1, but in
  // memoery it is possible (though very unlikely) that a boolean has
  // a non-zero value (different from 1).
  // We resort to a simple loop.
  for (ACE_CDR::ULong i = 0; i != length && this->good_bit_; ++i)
    {
      this->read_boolean (x[i]);
    }
  return this->good_bit_;
}

ACE_CDR::Boolean
ACE_InputCDR::read_1 (ACE_CDR::Octet *x)
{
  if (this->rd_ptr () < this->end ())
    {
      *x = *ACE_reinterpret_cast (ACE_CDR::Octet*,this->rd_ptr());
      this->start_.rd_ptr (1);
      return 1;
    }

  this->good_bit_ = 0;
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_2 (ACE_CDR::UShort *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::SHORT_SIZE, buf) == 0)
    {
#if !defined (ACE_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast (ACE_CDR::UShort*, buf);
        }
      else
        {
          ACE_CDR::swap_2 (buf, ACE_reinterpret_cast (char*, x));
        }
#else
      *x = *ACE_reinterpret_cast(ACE_CDR::UShort*,buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_4 (ACE_CDR::ULong *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONG_SIZE, buf) == 0)
    {
#if !defined (ACE_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast (ACE_CDR::ULong*, buf);
        }
      else
        {
          ACE_CDR::swap_4 (buf, ACE_reinterpret_cast (char*, x));
        }
#else
      *x = *ACE_reinterpret_cast(ACE_CDR::ULong*,buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_8 (ACE_CDR::ULongLong *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONGLONG_SIZE, buf) == 0)
    {
#if !defined (ACE_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast (ACE_CDR::ULongLong *, buf);
        }
      else
        {
          ACE_CDR::swap_8 (buf, ACE_reinterpret_cast (char*, x));
        }
#else
      *x = *ACE_reinterpret_cast(ACE_CDR::ULongLong*,buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::read_16 (ACE_CDR::LongDouble *x)
{
  char *buf;
  if (this->adjust (ACE_CDR::LONGLONG_SIZE,
                    ACE_CDR::LONGLONG_ALIGN,
                    buf) == 0)
    {
#if !defined (ACE_DISABLE_SWAP_ON_READ)
      if (!this->do_byte_swap_)
        {
          *x = *ACE_reinterpret_cast (ACE_CDR::LongDouble *, buf);
        }
      else
        {
          ACE_CDR::swap_16 (buf, ACE_reinterpret_cast (char*, x));
        }
#else
      *x = *ACE_reinterpret_cast(ACE_CDR::LongDouble*,buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::skip_string (void)
{
  ACE_CDR::ULong len;
  if (this->read_ulong (len))
    {
      if (this->rd_ptr () + len <= this->end ())
        {
          this->rd_ptr (len);
          return 1;
        }
      this->good_bit_ = 0;
    }

  return 0;
}

ACE_CDR::Boolean
ACE_InputCDR::skip_bytes (size_t len)
{
  if (this->rd_ptr () + len <= this->end ())
    {
      this->rd_ptr (len);
      return 1;
    }
  this->good_bit_ = 0;
  return 0;
}

int
ACE_InputCDR::grow (size_t newsize)
{
  if (ACE_CDR::grow (&this->start_, newsize) == -1)
    return -1;

  ACE_CDR::mb_align (&this->start_);
  this->start_.wr_ptr (newsize);
  return 0;
}

void
ACE_InputCDR::reset (const ACE_Message_Block* data,
                     int byte_order)
{
  this->reset_byte_order (byte_order);
  ACE_CDR::consolidate (&this->start_, data);
}

void
ACE_InputCDR::steal_from (ACE_InputCDR &cdr)
{
  this->do_byte_swap_ = cdr.do_byte_swap_;
  this->start_.data_block (cdr.start_.data_block ()->duplicate ());
  this->start_.rd_ptr (cdr.start_.rd_ptr ());
  this->start_.wr_ptr (cdr.start_.wr_ptr ());

  cdr.reset_contents ();
}

ACE_Message_Block*
ACE_InputCDR::steal_contents (void)
{
  ACE_Message_Block* block =
    this->start_.clone ();
  this->start_.data_block (block->data_block ()->clone ());
  ACE_CDR::mb_align (&this->start_);

  return block;
}

void
ACE_InputCDR::reset_contents (void)
{
  this->start_.data_block (this->start_.data_block ()->clone_nocopy ());
}
