/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Basic_Types.h
//
// = AUTHORS
//    David L. Levine
//
// = DESCRIPTION
//    #defines the following preprocessor macros:
//
//    Sizes of built-in types:
//      ACE_SIZEOF_CHAR
//      ACE_SIZEOF_WCHAR
//      ACE_SIZEOF_SHORT
//      ACE_SIZEOF_INT
//      ACE_SIZEOF_LONG
//      ACE_SIZEOF_LONG_LONG
//      ACE_SIZEOF_VOID_P
//      ACE_SIZEOF_FLOAT
//      ACE_SIZEOF_DOUBLE
//      ACE_SIZEOF_LONG_DOUBLE
//
//    Wrappers for built-in types of specific sizes:
//      ACE_INT16
//      ACE_UINT16
//      ACE_INT32
//      ACE_UINT32
//      ACE_UINT64
//    (Note: ACE_INT64 is not defined, because there is no ACE_LongLong for
//     platforms that don't have a native 8-byte integer type.)
//
//    Byte-order (endian-ness) determination:
//      ACE_BYTE_ORDER, to either ACE_BIG_ENDIAN or ACE_LITTLE_ENDIAN
//
// ============================================================================

#if !defined (ACE_BASIC_TYPES_H)
#define ACE_BASIC_TYPES_H

// A char always has 1 byte, by definition.
#define ACE_SIZEOF_CHAR 1

// Unfortunately, there isn't a portable way to determine the size of a wchar.
#if defined (VXWORKS) && defined (ghs)
# define ACE_SIZEOF_WCHAR 1
#else
# define ACE_SIZEOF_WCHAR sizeof (wchar_t)
#endif /* VXWORKS && ghs */

// The number of bytes in a short.
#if (USHRT_MAX) == 255U
# define ACE_SIZEOF_SHORT 1
#elif (USHRT_MAX) == 65535U
# define ACE_SIZEOF_SHORT 2
  typedef short ACE_INT16;
  typedef unsigned short ACE_UINT16;
#elif (USHRT_MAX) == 4294967295U
# define ACE_SIZEOF_SHORT 4
  typedef short ACE_INT32;
  typedef unsigned short ACE_UINT32;
#elif (USHRT_MAX) == 18446744073709551615U
# define ACE_SIZEOF_SHORT 8
#else
# error: unsupported short size, must be updated for this platform!
#endif /* USHRT_MAX */

// The number of bytes in an int.
#if (UINT_MAX) == 65535U
# define ACE_SIZEOF_INT 2
# if ACE_SIZEOF_SHORT != 2
    typedef int ACE_INT16;
    typedef unsigned int ACE_UINT16;
# endif /* ACE_SIZEOF_SHORT != 2 */
#elif (UINT_MAX) == 4294967295U
# define ACE_SIZEOF_INT 4
# if ACE_SIZEOF_SHORT != 4
    typedef int ACE_INT32;
    typedef unsigned int ACE_UINT32;
# endif /* ACE_SIZEOF_SHORT != 4 */
#elif (UINT_MAX) == 18446744073709551615U
# define ACE_SIZEOF_INT 8
  typedef unsigned int ACE_UINT64;
#else
# error: unsupported int size, must be updated for this platform!
#endif /* UINT_MAX */

// The number of bytes in a long.
#if (ULONG_MAX) == 65535UL
# define ACE_SIZEOF_LONG 2
#elif (ULONG_MAX) == 4294967295UL
# define ACE_SIZEOF_LONG 4
# if ACE_SIZEOF_SHORT != 4 && ACE_SIZEOF_INT != 4
    typedef long ACE_INT32;
    typedef unsigned long ACE_UINT32;
# endif /* ACE_SIZEOF_SHORT != 4 && ACE_SIZEOF_INT != 4 */
#elif (ULONG_MAX) == 18446744073709551615UL
# define ACE_SIZEOF_LONG 8
# if ACE_SIZEOF_INT != 8
    typedef unsigned long ACE_UINT64;
# endif /* ACE_SIZEOF_INT != 8 */
#else
# error: unsupported long size, must be updated for this platform!
#endif /* ULONG_MAX */

// The number of bytes in a long long.
#if defined (ACE_WIN32)
#   define ACE_SIZEOF_LONG_LONG 8
    typedef unsigned __int64 ACE_UINT64;
#elif defined (ULLONG_MAX) && ! defined (__GNUG__)
# if (ULLONG_MAX) == 18446744073709551615ULL
#   define ACE_SIZEOF_LONG_LONG 8
#   if ACE_SIZEOF_INT != 8 && ACE_SIZEOF_LONG != 8
      typedef unsigned long long ACE_UINT64;
#   endif /* ACE_SIZEOF_INT != 8 && ACE_SIZEOF_LONG != 8 */
# elif (ULLONG_MAX) == 4294967295ULL
    // For some reason, this branch is taken if it appears first.
    // g++ can't deal with the ULL's at all.  What's going on here?
#   define ACE_SIZEOF_LONG_LONG 4
#   if ACE_SIZEOF_SHORT != 4 && ACE_SIZEOF_INT != 4  && ACE_SIZEOF_LONG != 4
      typedef long long ACE_INT32;
      typedef unsigned long long ACE_UINT32;
#   endif /* ACE_SIZEOF_{SHORT,INT,LONG} != 4 */
# else
#   error: unsupported long long size, must be updated for this platform!
# endif /* ULLONG_MAX */
#elif defined (ULONGLONG_MAX)
  // Irix 6.x, for example.
# if (ULONGLONG_MAX) == 18446744073709551615ULL
#   define ACE_SIZEOF_LONG_LONG 8
#   if ACE_SIZEOF_INT != 8 && ACE_SIZEOF_LONG != 8
      typedef unsigned long long ACE_UINT64;
#   endif /* ACE_SIZEOF_INT != 8 && ACE_SIZEOF_LONG != 8 */
# else
#   error: unsupported long long size, must be updated for this platform!
# endif /* ULLONG_MAX */
#else /* ! ACE_WIN32 && ! ULLONG_MAX && ! ULONGLONG_MAX */
# if defined (ACE_HAS_LONGLONG_T)
  // Assume 8-byte long longs.  It would be better to #define
  // ACE_HAS_LONGLONG_T to the number of bytes in a long,
  // but we'd have to do that consistently in all config files.
#   define ACE_SIZEOF_LONG_LONG 8
#   if ACE_SIZEOF_INT != 8 && ACE_SIZEOF_LONG != 8
#     if defined (sun)
        // sun #defines u_longlong_t, maybe other platforms do also.
        // Use it, at least with g++, so that its -pedantic doesn't
        // complain about no ANSI C++ long long.
        typedef u_longlong_t ACE_UINT64;
#     else
        // LynxOS 2.5.0 and Linux don't have u_longlong_t.
        typedef unsigned long long ACE_UINT64;
#     endif /* __GNUG__ && sun */
#   endif /* ACE_SIZEOF_INT != 8 && ACE_SIZEOF_LONG != 8 */
# else /* ! ACE_HAS_LONGLONG_T */

    class ACE_Export ACE_U_LongLong
      // = TITLE
      //     Unsigned long long for platforms that don't have one.
      //
      // = DESCRIPTION
      //     Provide our own unsigned long long.  This is intended to be
      //     use with ACE_High_Res_Timer, so the division operator assumes
      //     that the quotient fits into a u_long.
      //     Please note that the constructor takes (optionally) two values.
      //     The high one contributes 0x100000000 times its value.  So,
      //     for example, (0, 2) is _not_ 20000000000, but instead
      //     0x200000000.  To emphasize this, the default values are expressed
      //     in hex, and dump () outputs the value in hex.
    {
    public:
      // = Initialization and termination methods.
      ACE_U_LongLong (const ACE_UINT32 lo = 0x0, const ACE_UINT32 hi = 0x0);
      ACE_U_LongLong (const ACE_U_LongLong &);
      ACE_U_LongLong &operator= (const ACE_U_LongLong &);
      ~ACE_U_LongLong (void);

      // = Overloaded relation operators.
      int operator== (const ACE_U_LongLong &) const;
      int operator!= (const ACE_U_LongLong &) const;
      int operator< (const ACE_U_LongLong &) const;
      int operator<= (const ACE_U_LongLong &) const;
      int operator> (const ACE_U_LongLong &) const;
      int operator>= (const ACE_U_LongLong &) const;

      ACE_U_LongLong operator+ (const ACE_U_LongLong &) const;
      ACE_U_LongLong operator- (const ACE_U_LongLong &) const;

      // Note that the following take ACE_UINT32 arguments.  These are
      // typical use cases, and easy to implement.  But, they're very
      // limited in the ranges of values that they can handle.
      ACE_UINT32 operator/ (const ACE_UINT32) const;
      // ACE_UINT32 operator% (const ACE_UINT32) const;

      ACE_U_LongLong &operator+= (const ACE_U_LongLong &);
      ACE_U_LongLong &operator++ ();
      ACE_U_LongLong &operator-= (const ACE_U_LongLong &);

      // = Helper methods.
      void output (FILE * = stdout) const;
      // Outputs the value to the FILE, in hex.

      ACE_UINT32 hi (void) const;
      ACE_UINT32 lo (void) const;

      void hi (const ACE_UINT32 hi);
      void lo (const ACE_UINT32 lo);

      ACE_ALLOC_HOOK_DECLARE;

    private:
      ACE_UINT32 hi_;
      ACE_UINT32 lo_;
    };

#   define ACE_USES_ACE_U_LONGLONG
#   define ACE_SIZEOF_LONG_LONG 8
#   if ACE_SIZEOF_INT != 8 && ACE_SIZEOF_LONG != 8
      typedef ACE_U_LongLong ACE_UINT64;
#   endif /* ACE_SIZEOF_INT != 8 && ACE_SIZEOF_LONG != 8 */
# endif /* ! ACE_HAS_LONGLONG_T */
#endif /* ! ACE_WIN32 && ! ULLONG_MAX && ! ULONGLONG_MAX */

// The number of bytes in a void *.
#define ACE_SIZEOF_VOID_P ACE_SIZEOF_LONG

// The number of bytes in a float.
#if FLT_MAX_EXP == 128
# define ACE_SIZEOF_FLOAT 4
#elif FLT_MAX_EXP == 1024
# define ACE_SIZEOF_FLOAT 8
#else
# error: unsupported float size, must be updated for this platform!
#endif /* FLT_MAX */

// The number of bytes in a double.
#if DBL_MAX_EXP == 128
# define ACE_SIZEOF_DOUBLE 4
#elif DBL_MAX_EXP == 1024
# define ACE_SIZEOF_DOUBLE 8
#else
# error: unsupported double size, must be updated for this platform!
#endif /* DBL_MAX */

// The number of bytes in a long double.
#if defined (__sgi)
  // The Irix 6.x float.h doesn't allow us to distinguish between a
  // double and a long double.  Gotta hard code this . . .
# define ACE_SIZEOF_LONG_DOUBLE 16
#elif LDBL_MAX_EXP == 128
# define ACE_SIZEOF_LONG_DOUBLE 4
#elif defined (i386) || defined (__i386__) || defined (_M_IX386) || \
  defined (ACE_NETBSD) || defined (__FreeBSD__)
# define ACE_SIZEOF_LONG_DOUBLE 12
#elif LDBL_MAX_EXP == 1024
# define ACE_SIZEOF_LONG_DOUBLE 8
#elif LDBL_MAX_EXP == 16384
# define ACE_SIZEOF_LONG_DOUBLE 16
#else
# error: unsupported double size, must be updated for this platform!
#endif /* LDBL_MAX_EXP */

// Byte-order (endian-ness) determination.
#if defined (BYTE_ORDER)
# if (BYTE_ORDER == LITTLE_ENDIAN)
#  define ACE_LITTLE_ENDIAN 0123X
#  define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
# elif (BYTE_ORDER == BIG_ENDIAN)
#  define ACE_BIG_ENDIAN 3210X
#  define ACE_BYTE_ORDER ACE_BIG_ENDIAN
# else
#  error: unknown BYTE_ORDER!
# endif /* BYTE_ORDER */
#elif defined (__BYTE_ORDER)
# if (__BYTE_ORDER == __LITTLE_ENDIAN)
#  define ACE_LITTLE_ENDIAN 0123X
#  define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
# elif (__BYTE_ORDER == __BIG_ENDIAN)
#  define ACE_BIG_ENDIAN 3210X
#  define ACE_BYTE_ORDER ACE_BIG_ENDIAN
# else
#  error: unknown __BYTE_ORDER!
# endif /* __BYTE_ORDER */
#else /* ! BYTE_ORDER && ! __BYTE_ORDER */
  // We weren't explicitly told, so we have to figure it out . . .
# if defined (i386) || defined (__i386__) || defined (_M_IX86) || \
     defined (vax) || defined (__alpha)
    // We know these are little endian.
#   define ACE_LITTLE_ENDIAN 0123X
#   define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
# else
    // Otherwise, we'll assume big endian.
#   define ACE_BIG_ENDIAN 3210X
#   define ACE_BYTE_ORDER ACE_BIG_ENDIAN
# endif
#endif /* ! BYTE_ORDER && ! __BYTE_ORDER */

#if defined (__ACE_INLINE__)
# include "ace/Basic_Types.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_BASIC_TYPES_H */
