// -*- C++ -*-
//
// $Id$

//
// The ACE_CDR::swap_X and ACE_CDR::swap_X_array routines are broken
// in 4 cases for optimization:
//
// * x86 Pentium CPU + gnu g++
//   (ACE_HAS_PENTIUM && __GNUG__)
//   => gcc x86 inline assembly.
//
// * x86 Pentium CPU and (_MSC_VER) or BORLAND C++)
//   (ACE_HAS_PENTIUM && ( _MSC_VER || __BORLANDC__ )
//   => MSC x86 inline assembly.
//
// * 64 bit architecture
//   (ACE_SIZEOF_LONG == 8)
//   => shift/masks using 64bit words.
//
// * default
//   (none of the above)
//   => shift/masks using 32bit words.
//
//
// Some things you could find useful to know if you intend to mess
// with this optimizations for swaps:
//
//      * MSVC++ don't assume register values are conserved between
//        statements. So you can clobber any register you want,
//        whenever you want (well not *anyone* really, see manual).
//        The MSVC++ optimizer will try to pick different registers
//        for the C++ statements sorrounding your asm block, and if
//        it's not possible will use the stack.
//
//      * If you clobber registers with asm statements in gcc, you
//        better do it in an asm-only function, or save/restore them
//        before/after in the stack. If not, sorrounding C statements
//        could end using the same registers and big-badda-bum (been
//        there, done that...). The big-badda-bum could happen *even
//        if you specify the clobbered register in your asm's*.
//        Even better, use gcc asm syntax for detecting the register
//        asigned to a certain variable so you don't have to clobber any
//        register directly.
//

ACE_INLINE void
ACE_CDR::swap_2 (const char *orig, char* target)
{
#if defined(ACE_HAS_PENTIUM)
# if defined(__GNUG__)
  unsigned short a =
    *ACE_reinterpret_cast(const unsigned short*, orig);
  asm( "rolw $8, %0" : "=r" (a) : "0" (a) );
  *ACE_reinterpret_cast(unsigned short*, target) = a;
# elif (defined(_MSC_VER) || defined(__BORLANDC__)) \
       && !defined(ACE_LACKS_INLINE_ASSEMBLY)
  __asm mov ebx, orig;
  __asm mov ecx, target;
  __asm mov ax, [ebx];
  __asm rol ax, 8;
  __asm mov [ecx], ax;
# else
  // For CISC Platforms this is faster than shift/masks.
  target[1] = orig[0];
  target[0] = orig[1];
# endif
#else
  register ACE_UINT16 usrc = * ACE_reinterpret_cast(const ACE_UINT16*, orig);
  register ACE_UINT16* udst = ACE_reinterpret_cast(ACE_UINT16*, target);
  *udst = (usrc << 8) | (usrc >> 8);
#endif /* ACE_HAS_PENTIUM */
}

ACE_INLINE void
ACE_CDR::swap_4 (const char* orig, char* target)
{
#if defined(ACE_HAS_PENTIUM) && defined(__GNUG__)
  // We have ACE_HAS_PENTIUM, so we know the sizeof's.
  register unsigned int j =
    *ACE_reinterpret_cast(const unsigned int*, orig);
  asm ("bswap %1" : "=r" (j) : "0" (j));
  *ACE_reinterpret_cast(unsigned int*, target) = j;
#elif defined(ACE_HAS_PENTIUM) \
      && (defined(_MSC_VER) || defined(__BORLANDC__)) \
      && !defined(ACE_LACKS_INLINE_ASSEMBLY)
  __asm mov ebx, orig;
  __asm mov ecx, target;
  __asm mov eax, [ebx];
  __asm bswap eax;
  __asm mov [ecx], eax;
#else
  register ACE_UINT32 x = * ACE_reinterpret_cast(const ACE_UINT32*, orig);
  x = (x << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | (x >> 24);
  * ACE_reinterpret_cast(ACE_UINT32*, target) = x;
#endif
}

ACE_INLINE void
ACE_CDR::swap_8 (const char* orig, char* target)
{
#if defined(ACE_HAS_PENTIUM) && defined(__GNUG__)
  register unsigned int i =
    *ACE_reinterpret_cast(const unsigned int*, orig);
  register unsigned int j =
    *ACE_reinterpret_cast(const unsigned int*, orig + 4);
  asm ("bswap %1" : "=r" (i) : "0" (i));
  asm ("bswap %1" : "=r" (j) : "0" (j));
  *ACE_reinterpret_cast(unsigned int*, target + 4) = i;
  *ACE_reinterpret_cast(unsigned int*, target) = j;
#elif defined(ACE_HAS_PENTIUM) \
      && (defined(_MSC_VER) || defined(__BORLANDC__)) \
      && !defined(ACE_LACKS_INLINE_ASSEMBLY)
  __asm mov ecx, orig;
  __asm mov edx, target;
  __asm mov eax, [ecx];
  __asm mov ebx, 4[ecx];
  __asm bswap eax;
  __asm bswap ebx;
  __asm mov 4[edx], eax;
  __asm mov [edx], ebx;
#elif ACE_SIZEOF_LONG == 8
  // 64 bit architecture.
  register unsigned long x =
    * ACE_reinterpret_cast(const unsigned long*, orig);
  register unsigned long x84 = (x & 0x000000ff000000ffUL) << 24;
  register unsigned long x73 = (x & 0x0000ff000000ff00UL) << 8;
  register unsigned long x62 = (x & 0x00ff000000ff0000UL) >> 8;
  register unsigned long x51 = (x & 0xff000000ff000000UL) >> 24;
  x = (x84 | x73 | x62 | x51);
  x = (x << 32) | (x >> 32);
  *ACE_reinterpret_cast(unsigned long*, target) = x;
#else
  register ACE_UINT32 x =
    * ACE_reinterpret_cast(const ACE_UINT32*, orig);
  register ACE_UINT32 y =
    * ACE_reinterpret_cast(const ACE_UINT32*, orig + 4);
  x = (x << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | (x >> 24);
  y = (y << 24) | ((y & 0xff00) << 8) | ((y & 0xff0000) >> 8) | (y >> 24);
  * ACE_reinterpret_cast(ACE_UINT32*, target) = y;
  * ACE_reinterpret_cast(ACE_UINT32*, target + 4) = x;
#endif
}

ACE_INLINE void
ACE_CDR::swap_16 (const char* orig, char* target)
{
  swap_8 (orig + 8, target);
  swap_8 (orig, target + 8);
}

ACE_INLINE size_t
ACE_CDR::first_size (size_t minsize)
{
  if (minsize == 0)
    return ACE_CDR::DEFAULT_BUFSIZE;

  size_t newsize = ACE_CDR::DEFAULT_BUFSIZE;
  while (newsize < minsize)
    {
      if (newsize < ACE_CDR::EXP_GROWTH_MAX)
        {
          // We grow exponentially at the beginning, this is fast and
          // reduces the number of allocations.
          newsize *= 2;
        }
      else
        {
          // but continuing with exponential growth can result in over
          // allocations and easily yield an allocation failure.
          // So we grow linearly when the buffer is too big.
          newsize += ACE_CDR::LINEAR_GROWTH_CHUNK;
        }
    }
  return newsize;
}

ACE_INLINE size_t
ACE_CDR::next_size (size_t minsize)
{
  size_t newsize =
    ACE_CDR::first_size (minsize);

  if (newsize == minsize)
    {
      // If necessary increment the size
      if (newsize < ACE_CDR::EXP_GROWTH_MAX)
        newsize *= 2;
      else
        newsize += ACE_CDR::LINEAR_GROWTH_CHUNK;
    }

  return newsize;
}

// ****************************************************************
