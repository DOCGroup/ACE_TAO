// -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_Types.h
 *
 *  $Id$
 *
 *  @author David L. Levine
 *
 *  #defines the list of preprocessor macros below.  The config.h file can
 *  pre-define any of these to short-cut the definitions.  This is usually
 *  only necessary if the preprocessor does all of its math using integers.
 *
 *  Sizes of built-in types:
 *    - ACE_SIZEOF_CHAR
 *    - ACE_SIZEOF_WCHAR
 *    - ACE_SIZEOF_SHORT
 *    - ACE_SIZEOF_INT
 *    - ACE_SIZEOF_LONG
 *    - ACE_SIZEOF_LONG_LONG
 *    - ACE_SIZEOF_VOID_P
 *    - ACE_SIZEOF_FLOAT
 *    - ACE_SIZEOF_DOUBLE
 *    - ACE_SIZEOF_LONG_DOUBLE
 *
 *  Wrappers for built-in types of specific sizes:
 *    - ACE_USHORT16 (For backward compatibility.  Use ACE_UINT16 instead.)
 *    - ACE_INT16
 *    - ACE_UINT16
 *    - ACE_INT32
 *    - ACE_UINT32
 *    - ACE_UINT64
 *  (Note: ACE_INT64 is not defined, because there is no ACE_LongLong for
 *   platforms that don't have a native 8-byte integer type.)
 *
 *  Byte-order (endian-ness) determination:
 *    ACE_BYTE_ORDER, to either ACE_BIG_ENDIAN or ACE_LITTLE_ENDIAN
 *
 *
 */
//=============================================================================


#ifndef ACE_BASIC_TYPES_H
# define ACE_BASIC_TYPES_H

# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/ACE_export.h"
# include "ace/os_include/limits.h"

// Pull in definitions
//# include /**/ <limits.h>   // Integer limits
# include /**/ <float.h>    // Floating point limits
//# include /**/ <stdlib.h>   // Other types
# include "ace/os_include/stdlib.h"

# if defined(ACE_LACKS_LONGLONG_T)
#   include "ace/os_include/stdio.h"
# endif  /* ACE_LACKS_LONGLONG_T */



#if 0
# include "ace/os_include/sys/types.h"
# include "ace/os_include/sys/param.h"
#endif /* 0 */



// A char always has 1 byte, by definition.
# define ACE_SIZEOF_CHAR 1

// Unfortunately, there isn't a portable way to determine the size of a wchar.
// So we just define them on a platform basis.
# if defined (ACE_HAS_WCHAR)
#   if !defined (ACE_SIZEOF_WCHAR)
#     if defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
#       define ACE_SIZEOF_WCHAR sizeof (wchar_t)
#     else
// 0 so the Basic_Types test will catch this.
#       define ACE_SIZEOF_WCHAR 0
#     endif /* ACE_HAS_XPG4_MULTIBYTE_CHAR */
#   endif /* !ACE_SIZEOF_WCHAR */
# endif /* ACE_HAS_WCHAR */

// The number of bytes in a short.
# if !defined (ACE_SIZEOF_SHORT)
#   if (USHRT_MAX) == 255U
#     define ACE_SIZEOF_SHORT 1
#   elif (USHRT_MAX) == 65535U
#     define ACE_SIZEOF_SHORT 2
#   elif (USHRT_MAX) == 4294967295U
#     define ACE_SIZEOF_SHORT 4
#   elif (USHRT_MAX) == 18446744073709551615U
#     define ACE_SIZEOF_SHORT 8
#   else
#     error: unsupported short size, must be updated for this platform!
#   endif /* USHRT_MAX */
# endif /* !defined (ACE_SIZEOF_SHORT) */

// The number of bytes in an int.
# if !defined (ACE_SIZEOF_INT)
#   if (UINT_MAX) == 65535U
#     define ACE_SIZEOF_INT 2
#   elif (UINT_MAX) == 4294967295U
#     define ACE_SIZEOF_INT 4
#   elif (UINT_MAX) == 18446744073709551615U
#     define ACE_SIZEOF_INT 8
#   else
#     error: unsupported int size, must be updated for this platform!
#   endif /* UINT_MAX */
# endif /* !defined (ACE_SIZEOF_INT) */

// The number of bytes in a long.
// NOTE - since preprocessors only need to do integer math, this is a likely
// place for a preprocessor to not properly support being able to figure out
// the proper size.  HP aC++ and GNU gcc have this difficulty so watch out.
# if !defined (ACE_SIZEOF_LONG)
#   if (ULONG_MAX) == 65535UL
#     define ACE_SIZEOF_LONG 2
#   elif ((ULONG_MAX) == 4294967295UL)
#     define ACE_SIZEOF_LONG 4
#   elif ((ULONG_MAX) == 18446744073709551615UL)
#     define ACE_SIZEOF_LONG 8
#   else
#     error: unsupported long size, must be updated for this platform!
#   endif /* ULONG_MAX */
# endif /* !defined (ACE_SIZEOF_LONG) */

// The number of bytes in a long long.
# if !defined (ACE_SIZEOF_LONG_LONG)
#   if defined (ACE_LACKS_LONGLONG_T)
#     define ACE_SIZEOF_LONG_LONG 8
#   else  /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
#     if ACE_SIZEOF_LONG == 8
#       define ACE_SIZEOF_LONG_LONG 8
       typedef unsigned long ACE_UINT64;
#     elif defined (ULLONG_MAX) && !defined (__GNUG__)
       // Some compilers use ULLONG_MAX and others, e.g. Irix, use
       // ULONGLONG_MAX.
#       if (ULLONG_MAX) == 18446744073709551615ULL
#         define ACE_SIZEOF_LONG_LONG 8
#       elif (ULLONG_MAX) == 4294967295ULL
#         define ACE_SIZEOF_LONG_LONG 4
#       else
#         error Unsupported long long size needs to be updated for this platform
#       endif
       typedef unsigned long long ACE_UINT64;
#     elif defined (ULONGLONG_MAX) && !defined (__GNUG__)
       // Irix 6.x, for example.
#       if (ULONGLONG_MAX) == 18446744073709551615ULL
#         define ACE_SIZEOF_LONG_LONG 8
#       elif (ULONGLONG_MAX) == 4294967295ULL
#         define ACE_SIZEOF_LONG_LONG 4
#       else
#         error Unsupported long long size needs to be updated for this platform
#       endif
       typedef unsigned long long ACE_UINT64;
#     else
       // ACE_SIZEOF_LONG_LONG is not yet known, but the platform doesn't
       // claim ACE_LACKS_LONGLONG_T, so assume it has 8-byte long longs.
#       define ACE_SIZEOF_LONG_LONG 8
#       if defined (sun) && !defined (ACE_LACKS_U_LONGLONG_T)
         // sun #defines u_longlong_t, maybe other platforms do also.
         // Use it, at least with g++, so that its -pedantic doesn't
         // complain about no ANSI C++ long long.
         typedef u_longlong_t ACE_UINT64;
#       else
         // LynxOS 2.5.0 and Linux don't have u_longlong_t.
         typedef unsigned long long ACE_UINT64;
#       endif /* sun */
#     endif /* ULLONG_MAX && !__GNUG__ */
#   endif /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
# endif /* !defined (ACE_SIZEOF_LONG_LONG) */

// Moved from OS.h dah
# if defined (ACE_PSOS_TM)
typedef long long longlong_t;
typedef long      id_t;
# endif /* ACE_PSOS_TM */

// The sizes of the commonly implemented types are now known.  Set up
// typedefs for whatever we can.  Some of these are needed for certain cases
// of ACE_UINT64, so do them before the 64-bit stuff.

# if ACE_SIZEOF_SHORT == 2
  typedef short ACE_INT16;
  typedef unsigned short ACE_UINT16;
# elif ACE_SIZEOF_INT == 2
  typedef int ACE_INT16;
  typedef unsigned int ACE_UINT16;
# elif (ACE_SIZEOF_SHORT) == 4 && defined(_CRAYMPP)
  // mpp cray - uses Alpha processors
  //   Use the real 32-bit quantity for ACE_INT32's, and use a "long"
  //   for ACE_INT16's.  This gets around conflicts with size_t in some ACE
  //   method signatures, among other things.
  typedef long ACE_INT16;
  typedef unsigned long ACE_UINT16;
  typedef short ACE_INT32;
  typedef unsigned short ACE_UINT32;
# elif (ACE_SIZEOF_SHORT) == 8 && defined(_UNICOS)
  // vector cray - hard 64-bit, all 64 bit types
  typedef short ACE_INT16;
  typedef unsigned short ACE_UINT16;
# else
#   error Have to add to the ACE_UINT16 type setting
# endif

typedef ACE_UINT16 ACE_USHORT16;

// Define a pseudo wide character type when wchar is not supported so we
// can support basic wide character string operations.

# if defined (ACE_HAS_WCHAR)
#   define ACE_WINT_T wint_t
#   define ACE_WCHAR_T wchar_t
# else
#   define ACE_WINT_T ACE_USHORT16
#   define ACE_WCHAR_T ACE_USHORT16
# endif /* ACE_HAS_WCHAR */

# if ACE_SIZEOF_INT == 4
  typedef int ACE_INT32;
  typedef unsigned int ACE_UINT32;
#   if defined (__KCC) && !defined (ACE_LACKS_LONGLONG_T)
  typedef unsigned long long ACE_UINT64;
#   endif /* __KCC */
# elif ACE_SIZEOF_LONG == 4
  typedef long ACE_INT32;
  typedef unsigned long ACE_UINT32;
# elif (ACE_SIZEOF_INT) == 8 && defined(_UNICOS)
  // vector cray - hard 64-bit, all 64 bit types
#   if !defined(_CRAYMPP)
      typedef int ACE_INT32;
      typedef unsigned int ACE_UINT32;
#   endif
  typedef unsigned long long ACE_UINT64;
# else
#   error Have to add to the ACE_UINT32 type setting
# endif

// The number of bytes in a void *.
# ifndef ACE_SIZEOF_VOID_P
#   define ACE_SIZEOF_VOID_P ACE_SIZEOF_LONG
# endif /* ACE_SIZEOF_VOID_P */

// Type for doing arithmetic on pointers ... as elsewhere, we assume
// that unsigned versions of a type are the same size as the signed
// version of the same type.
#if ACE_SIZEOF_VOID_P == ACE_SIZEOF_INT
# if defined (__SUNPRO_CC)
    // For unknown reasons, Sun CC 5.0 won't allow a reintepret cast
    // of a 64-bit pointer to a 64-bit int.
    typedef u_long ptr_arith_t;
# else  /* ! __SUNPRO_CC */
    typedef u_int ptr_arith_t;
# endif /* ! __SUNPRO_CC */
#elif ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG
  typedef u_long ptr_arith_t;
#elif ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG_LONG
  typedef u_long long ptr_arith_t;
#else
# error "Can't find a suitable type for doing pointer arithmetic."
#endif /* ACE_SIZEOF_VOID_P */

#if defined (ACE_LACKS_LONGLONG_T)
  // This throws away the high 32 bits.  It's very unlikely that a
  // pointer will be more than 32 bits wide if the platform does not
  // support 64-bit integers.
# define ACE_LONGLONG_TO_PTR(PTR_TYPE, L) \
  ACE_reinterpret_cast (PTR_TYPE, L.lo ())
#else  /* ! ACE_LACKS_LONGLONG_T */
# define ACE_LONGLONG_TO_PTR(PTR_TYPE, L) \
  ACE_reinterpret_cast (PTR_TYPE, ACE_static_cast (ptr_arith_t, L))
#endif /* ! ACE_LACKS_LONGLONG_T */

// If the platform lacks a long long, define one.
# if defined (ACE_LACKS_LONGLONG_T)
#   if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
// Forward declaration for streams
#     include "ace/iosfwd.h"
#   else /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */
// Else they will get all the stream header files
#     include "ace/streams.h"
#   endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

/**
 * @class ACE_U_LongLong
 *
 * @brief Unsigned long long for platforms that don't have one.
 *
 * Provide our own unsigned long long.  This is intended to be
 * use with ACE_High_Res_Timer, so the division operator assumes
 * that the quotient fits into a u_long.
 * Please note that the constructor takes (optionally) two values.
 * The high one contributes 0x100000000 times its value.  So,
 * for example, (0, 2) is _not_ 20000000000, but instead
 * 0x200000000.  To emphasize this, the default values are expressed
 * in hex, and output () dumps the value in hex.
 */
  class ACE_Export ACE_U_LongLong
  {
  public:
    // = Initialization and termination methods.
    ACE_U_LongLong (const ACE_UINT32 lo = 0x0, const ACE_UINT32 hi = 0x0);
    ACE_U_LongLong (const ACE_U_LongLong &);
    ACE_U_LongLong &operator= (const ACE_U_LongLong &);
    ~ACE_U_LongLong (void);

    // = Overloaded relation operators.
    int operator== (const ACE_U_LongLong &) const;
    int operator== (const ACE_UINT32) const;
    int operator!= (const ACE_U_LongLong &) const;
    int operator!= (const ACE_UINT32) const;
    int operator< (const ACE_U_LongLong &) const;
    int operator< (const ACE_UINT32) const;
    int operator<= (const ACE_U_LongLong &) const;
    int operator<= (const ACE_UINT32) const;
    int operator> (const ACE_U_LongLong &) const;
    int operator> (const ACE_UINT32) const;
    int operator>= (const ACE_U_LongLong &) const;
    int operator>= (const ACE_UINT32) const;

    ACE_U_LongLong operator+ (const ACE_U_LongLong &) const;
    ACE_U_LongLong operator+ (const ACE_UINT32) const;
    ACE_U_LongLong operator- (const ACE_U_LongLong &) const;
    ACE_U_LongLong operator- (const ACE_UINT32) const;
    ACE_U_LongLong operator* (const ACE_UINT32) const;
    ACE_U_LongLong &operator*= (const ACE_UINT32);

    ACE_U_LongLong operator<< (const u_int) const;
    ACE_U_LongLong &operator<<= (const u_int);
    ACE_U_LongLong operator>> (const u_int) const;
    ACE_U_LongLong &operator>>= (const u_int);

    double operator/ (const double) const;

    ACE_U_LongLong &operator+= (const ACE_U_LongLong &);
    ACE_U_LongLong &operator+= (const ACE_UINT32);
    ACE_U_LongLong &operator-= (const ACE_U_LongLong &);
    ACE_U_LongLong &operator-= (const ACE_UINT32);
    ACE_U_LongLong &operator++ ();
    ACE_U_LongLong &operator-- ();
    ACE_U_LongLong &operator|= (const ACE_U_LongLong);
    ACE_U_LongLong &operator|= (const ACE_UINT32);
    ACE_U_LongLong &operator&= (const ACE_U_LongLong);
    ACE_U_LongLong &operator&= (const ACE_UINT32);

    // Note that the following take ACE_UINT32 arguments.  These are
    // typical use cases, and easy to implement.  But, they limit the
    // return values to 32 bits as well.  There are no checks for
    // overflow.
    ACE_UINT32 operator/ (const ACE_UINT32) const;
    ACE_UINT32 operator% (const ACE_UINT32) const;

    // The following only operate on the lower 32 bits (they take only
    // 32 bit arguments).
    ACE_UINT32 operator| (const ACE_INT32) const;
    ACE_UINT32 operator& (const ACE_INT32) const;

    // The following operators convert their arguments to
    // ACE_UINT32.  So, there may be information loss if they are
    // used.
    ACE_U_LongLong operator* (const ACE_INT32) const;
    ACE_U_LongLong &operator*= (const ACE_INT32);
    ACE_UINT32 operator/ (const ACE_INT32) const;
#   if ACE_SIZEOF_INT == 4
    ACE_UINT32 operator/ (const u_long) const;
    ACE_UINT32 operator/ (const long) const;
#   else  /* ACE_SIZEOF_INT != 4 */
    ACE_UINT32 operator/ (const u_int) const;
    ACE_UINT32 operator/ (const int) const;
#   endif /* ACE_SIZEOF_INT != 4 */

    // = Helper methods.
    /// Outputs the value to the FILE, in hex.
    void output (FILE * = stdout) const;

    ACE_TCHAR *as_string (ACE_TCHAR *string,
                          u_int base = 10,
                          u_int uppercase = 0) const;

    ACE_UINT32 hi (void) const;
    ACE_UINT32 lo (void) const;

    void hi (const ACE_UINT32 hi);
    void lo (const ACE_UINT32 lo);

  private:
    union
      {
        struct
          {
            ACE_UINT32 hi_;
            // High 32 bits.

            ACE_UINT32 lo_;
            // Low 32 bits.
          } data_;
        // To ensure alignment on 8-byte boundary.

        // double isn't usually usable with ACE_LACKS_FLOATING_POINT,
        // but this seems OK.
        double for_alignment_;
        // To ensure alignment on 8-byte boundary.
      };

    // NOTE:  the following four accessors are inlined here in
    // order to minimize the extent of the data_ struct.  It's
    // only used here; the .i and .cpp files use the accessors.

    /// Internal utility function to hide access through struct.
    const ACE_UINT32 &h_ () const { return data_.hi_; }

    /// Internal utility function to hide access through struct.
    ACE_UINT32 &h_ () { return data_.hi_; }

    /// Internal utility function to hide access through struct.
    const ACE_UINT32 &l_ () const { return data_.lo_; }

    /// Internal utility function to hide access through struct.
    ACE_UINT32 &l_ () { return data_.lo_; }

    // NOTE:  the above four accessors are inlined here in
    // order to minimize the extent of the data_ struct.  It's
    // only used here; the .i and .cpp files use the accessors.

    /// These functions are used to implement multiplication.
    ACE_UINT32 ul_shift (ACE_UINT32 a,
                         ACE_UINT32 c_in,
                         ACE_UINT32 *c_out) const;
    ACE_U_LongLong ull_shift (ACE_U_LongLong a,
                              ACE_UINT32 c_in,
                              ACE_UINT32 *c_out) const;
    ACE_U_LongLong ull_add (ACE_U_LongLong a,
                            ACE_U_LongLong b,
                            ACE_UINT32 *carry) const;
    ACE_U_LongLong ull_mult (ACE_U_LongLong a,
                             ACE_UINT32 b,
                             ACE_UINT32 *carry) const;
  };

  typedef ACE_U_LongLong ACE_UINT64;

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  ostream &operator<< (ostream &, const ACE_U_LongLong &);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

# endif /* ACE_LACKS_LONGLONG_T */

// Conversions from ACE_UINT64 to ACE_UINT32.  ACE_CU64_TO_CU32 should
// be used on const ACE_UINT64's.
# if defined (ACE_LACKS_LONGLONG_T)
#   define ACE_U64_TO_U32(n) ((n).lo ())
#   define ACE_CU64_TO_CU32(n) ((n).lo ())
# else  /* ! ACE_LACKS_LONGLONG_T */
#   define ACE_U64_TO_U32(n) (ACE_static_cast (ACE_UINT32, (n)))
#   define ACE_CU64_TO_CU32(n) \
     (ACE_static_cast (ACE_CAST_CONST ACE_UINT32, (n)))
# endif /* ! ACE_LACKS_LONGLONG_T */

// 64-bit literals require special marking on some platforms.
# if defined (ACE_LACKS_LONGLONG_T)
    // Can only specify 32-bit arguments.
#   define ACE_UINT64_LITERAL(n) (ACE_U_LongLong (n))
      // This one won't really work, but it'll keep
      // some compilers happy until we have better support
#   define ACE_INT64_LITERAL(n) (ACE_U_LongLong (n))
# elif defined (ACE_WIN32)
#  if defined (__IBMCPP__) && (__IBMCPP__ >= 400)
#   define ACE_UINT64_LITERAL(n) n ## LL
#   define ACE_INT64_LITERAL(n) n ## LL
#  elif defined (__MINGW32__)
#   define ACE_UINT64_LITERAL(n) n ## ull
#   define ACE_INT64_LITERAL(n) n ## ll
#  else
#   define ACE_UINT64_LITERAL(n) n ## ui64
#   define ACE_INT64_LITERAL(n) n ## i64
#  endif /* defined (__IBMCPP__) && (__IBMCPP__ >= 400) */
# else  /* ! ACE_WIN32  &&  ! ACE_LACKS_LONGLONG_T */
#   define ACE_UINT64_LITERAL(n) n ## ull
#   define ACE_INT64_LITERAL(n) n ## ll
# endif /* ! ACE_WIN32  &&  ! ACE_LACKS_LONGLONG_T */

#if !defined (ACE_UINT64_FORMAT_SPECIFIER)
# define ACE_UINT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%llu")
#endif /* ACE_UINT64_FORMAT_SPECIFIER */

#if !defined (ACE_INT64_FORMAT_SPECIFIER)
# define ACE_INT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%lld")
#endif /* ACE_INT64_FORMAT_SPECIFIER */

#if !defined (ACE_SSIZE_T_FORMAT_SPECIFIER)
# define ACE_SSIZE_T_FORMAT_SPECIFIER ACE_LIB_TEXT ("%d")
#endif /* ACE_SSIZE_T_FORMAT_SPECIFIER */

#if !defined (ACE_SIZE_T_FORMAT_SPECIFIER)
# define ACE_SIZE_T_FORMAT_SPECIFIER ACE_LIB_TEXT ("%u")
#endif /* ACE_SIZE_T_FORMAT_SPECIFIER */

// Cast from UINT64 to a double requires an intermediate cast to INT64
// on some platforms.
# if defined (ACE_LACKS_LONGLONG_T)
   // Only use the low 32 bits.
#   define ACE_UINT64_DBLCAST_ADAPTER(n) ACE_U64_TO_U32 (n)
# elif defined (ACE_WIN32)
#   define ACE_UINT64_DBLCAST_ADAPTER(n) ACE_static_cast (__int64, n)
# else  /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
#   define ACE_UINT64_DBLCAST_ADAPTER(n) (n)
# endif /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */


// The number of bytes in a float.
# ifndef ACE_SIZEOF_FLOAT
#   if FLT_MAX_EXP == 128
#     define ACE_SIZEOF_FLOAT 4
#   elif FLT_MAX_EXP == 1024
#     define ACE_SIZEOF_FLOAT 8
#   else
#     error: unsupported float size, must be updated for this platform!
#   endif /* FLT_MAX_EXP */
# endif /* ACE_SIZEOF_FLOAT */

// The number of bytes in a double.
# ifndef ACE_SIZEOF_DOUBLE
#   if DBL_MAX_EXP == 128
#     define ACE_SIZEOF_DOUBLE 4
#   elif DBL_MAX_EXP == 1024
#     define ACE_SIZEOF_DOUBLE 8
#   else
#     error: unsupported double size, must be updated for this platform!
#   endif /* DBL_MAX_EXP */
# endif /* ACE_SIZEOF_DOUBLE */

// The number of bytes in a long double.
# ifndef ACE_SIZEOF_LONG_DOUBLE
#   if LDBL_MAX_EXP == 128
#     define ACE_SIZEOF_LONG_DOUBLE 4
#   elif LDBL_MAX_EXP == 1024
#     define ACE_SIZEOF_LONG_DOUBLE 8
#   elif LDBL_MAX_EXP == 16384
#     if defined (LDBL_DIG)  &&  LDBL_DIG == 18
#       if defined (__ia64)
#         define ACE_SIZEOF_LONG_DOUBLE 16
#       else /* ! __ia64 */
#       define ACE_SIZEOF_LONG_DOUBLE 12
#       endif /* __ia64 */
#     else  /* ! LDBL_DIG  ||  LDBL_DIG != 18 */
#       define ACE_SIZEOF_LONG_DOUBLE 16
#     endif /* ! LDBL_DIG  ||  LDBL_DIG != 18 */
#   else
#     error: unsupported double size, must be updated for this platform!
#   endif /* LDBL_MAX_EXP */
# endif /* ACE_SIZEOF_LONG_DOUBLE */

// Max and min sizes for the ACE integer types.
#define ACE_CHAR_MAX 0x7F
#define ACE_CHAR_MIN -(ACE_CHAR_MAX)-1
#define ACE_OCTET_MAX 0xFF
#define ACE_INT16_MAX 0x7FFF
#define ACE_INT16_MIN -(ACE_INT16_MAX)-1
#define ACE_UINT16_MAX 0xFFFF
#define ACE_WCHAR_MAX ACE_UINT16_MAX
#define ACE_INT32_MAX 0x7FFFFFFF
#define ACE_INT32_MIN -(ACE_INT32_MAX)-1
#define ACE_UINT32_MAX 0xFFFFFFFF
#define ACE_INT64_MAX ACE_INT64_LITERAL(0x7FFFFFFFFFFFFFFF)
#define ACE_INT64_MIN -(ACE_INT64_MAX)-1
#define ACE_UINT64_MAX ACE_UINT64_LITERAL(0xFFFFFFFFFFFFFFFF)
// These use ANSI/IEEE format.
#define ACE_FLT_MAX 3.402823466e+38F
#define ACE_DBL_MAX 1.7976931348623158e+308







// Type-safe, and unsigned.
static const ACE_UINT32 ACE_U_ONE_SECOND_IN_MSECS = 1000U;
static const ACE_UINT32 ACE_U_ONE_SECOND_IN_USECS = 1000000U;
static const ACE_UINT32 ACE_U_ONE_SECOND_IN_NSECS = 1000000000U;




/* This should work for linux, solaris 5.6 and above, IRIX, OSF */
# if defined (ACE_HAS_LLSEEK) || defined (ACE_HAS_LSEEK64)
#   if ACE_SIZEOF_LONG == 8
      typedef off_t ACE_LOFF_T;
#   elif defined (__sgi) || defined (AIX) || defined (HPUX) \
    || defined (__QNX__)
      typedef off64_t ACE_LOFF_T;
#   elif defined (__sun)
      typedef offset_t ACE_LOFF_T;
#   elif defined (WIN32) //Add by Nick Lin -- for win32 llseek
      typedef __int64  ACE_LOFF_T;  //Add by Nick Lin -- for win32 llseek
#   else
      typedef loff_t ACE_LOFF_T;
#   endif
# endif /* ACE_HAS_LLSEEK || ACE_HAS_LSEEK64 */

// Should this be in sys/types.h?
# if defined (ACE_HAS_HI_RES_TIMER) &&  !defined (ACE_LACKS_LONGLONG_T)
  /* hrtime_t is defined on systems (Suns) with ACE_HAS_HI_RES_TIMER */
  typedef hrtime_t ACE_hrtime_t;
# else  /* ! ACE_HAS_HI_RES_TIMER  ||  ACE_LACKS_LONGLONG_T */
  typedef ACE_UINT64 ACE_hrtime_t;
# endif /* ! ACE_HAS_HI_RES_TIMER  ||  ACE_LACKS_LONGLONG_T */

# if defined (CHORUS) && !defined (CHORUS_4)
#   define ACE_HRTIMER_START_VAL K_BSTART
#   define ACE_HRTIMER_INCR_VAL K_BPOINT
#   define ACE_HRTIMER_STOP_VAL K_BSTOP
# else  /* ! CHORUS */
#   define ACE_HRTIMER_START_VAL 0x0
#   define ACE_HRTIMER_INCR_VAL 0x1
#   define ACE_HRTIMER_STOP_VAL 0X3
# endif /* CHORUS && !CHORUS_4 */

# if !defined (ACE_HRTIMER_START)
#   define ACE_HRTIMER_START ACE_HRTIMER_START_VAL
# endif /* !ACE_HRTIMER_START */

# if !defined (ACE_HRTIMER_INCR)
#   define ACE_HRTIMER_INCR ACE_HRTIMER_INCR_VAL
# endif /* !ACE_HRTIMER_INCR */

# if !defined (ACE_HRTIMER_STOP)
#   define ACE_HRTIMER_STOP ACE_HRTIMER_STOP_VAL
# endif /* !ACE_HRTIMER_STOP */

# if !defined (ACE_HRTIMER_GETTIME)
#   define ACE_HRTIMER_GETTIME 0xFFFF
# endif /* !ACE_HRTIMER_GETTIME */



#if 0
// These aren't used anywhere..
# if defined (ACE_HAS_AIO_CALLS)
  // = Giving unique ACE scoped names for some important
  // RTSignal-Related constants. Becuase sometimes, different
  // platforms use different names for these constants.

  // Number of realtime signals provided in the system.
  // _POSIX_RTSIG_MAX is the upper limit on the number of real time
  // signals supported in a posix-4 compliant system.
#   if defined (_POSIX_RTSIG_MAX)
#     define ACE_RTSIG_MAX _POSIX_RTSIG_MAX
#   else /* not _POSIX_RTSIG_MAX */
  // POSIX-4 compilant system has to provide atleast 8 RT signals.
  // @@ Make sure the platform does *not* define this constant with
  // some other name. If yes, use that instead of 8.
#     define ACE_RTSIG_MAX 8
#   endif /* _POSIX_RTSIG_MAX */
# endif /* ACE_HAS_AIO_CALLS */
#endif /* 0 */

// this doesn't belong here, but until I get an ace_aio.h file, here will be fine.
// Defining POSIX4 real-time signal range.
# if defined ACE_HAS_AIO_CALLS
#   define ACE_SIGRTMIN SIGRTMIN
#   define ACE_SIGRTMAX SIGRTMAX
# else /* !ACE_HAS_AIO_CALLS */
#   define ACE_SIGRTMIN 0
#   define ACE_SIGRTMAX 0
# endif /* ACE_HAS_AIO_CALLS */

# if defined (ACE_WIN32) && ! defined (ACE_HAS_WINCE) \
                         && ! defined (ACE_HAS_PHARLAP)
typedef TRANSMIT_FILE_BUFFERS ACE_TRANSMIT_FILE_BUFFERS;
typedef LPTRANSMIT_FILE_BUFFERS ACE_LPTRANSMIT_FILE_BUFFERS;
typedef PTRANSMIT_FILE_BUFFERS ACE_PTRANSMIT_FILE_BUFFERS;

#   define ACE_INFINITE INFINITE
#   define ACE_STATUS_TIMEOUT STATUS_TIMEOUT
#   define ACE_WAIT_FAILED WAIT_FAILED
#   define ACE_WAIT_TIMEOUT WAIT_TIMEOUT
# else /* ACE_WIN32 && !ACE_HAS_WINCE && !ACE_HAS_PHARLAP */

struct ACE_TRANSMIT_FILE_BUFFERS
{
  void *Head;
  size_t HeadLength;
  void *Tail;
  size_t TailLength;
};
typedef ACE_TRANSMIT_FILE_BUFFERS* ACE_PTRANSMIT_FILE_BUFFERS;
typedef ACE_TRANSMIT_FILE_BUFFERS* ACE_LPTRANSMIT_FILE_BUFFERS;

#   if !defined (ACE_INFINITE)
#     define ACE_INFINITE LONG_MAX
#   endif /* ACE_INFINITE */
#   define ACE_STATUS_TIMEOUT LONG_MAX
#   define ACE_WAIT_FAILED LONG_MAX
#   define ACE_WAIT_TIMEOUT LONG_MAX
# endif /* ACE_WIN32 */




// probably doesn't belong here...
//@{
/**
 * @name Efficiently compute aligned pointers to powers of 2 boundaries.
 */

/**
 * Efficiently align "value" up to "alignment", knowing that all such
 * boundaries are binary powers and that we're using two's complement
 * arithmetic.
 *
 * Since the alignment is a power of two its binary representation is:
 *
 * alignment      = 0...010...0
 *
 * hence
 *
 * alignment - 1  = 0...001...1 = T1
 *
 * so the complement is:
 *
 * ~(alignment - 1) = 1...110...0 = T2
 *
 * Notice that there is a multiple of <alignment> in the range
 * [<value>,<value> + T1], also notice that if
 *
 * X = ( <value> + T1 ) & T2
 *
 * then
 *
 * <value> <= X <= <value> + T1
 *
 * because the & operator only changes the last bits, and since X is a
 * multiple of <alignment> (its last bits are zero) we have found the
 * multiple we wanted.
 */
/// Return the next integer aligned to a required boundary
/**
 * @param ptr the base pointer
 * @param alignment the required alignment
 */
# define ACE_align_binary(ptr, alignment) \
    ((ptr + ((ptr_arith_t)((alignment)-1))) & (~((ptr_arith_t)((alignment)-1))))

/// Return the next address aligned to a required boundary
# define ACE_ptr_align_binary(ptr, alignment) \
        ((char *) ACE_align_binary (((ptr_arith_t) (ptr)), (alignment)))
//@}

// this is only used in Log_Msg.cpp and Profile_Timer.h for right now.
// maybe Time_Value.h?
# if defined (ACE_LACKS_FLOATING_POINT)
typedef ACE_UINT32 ACE_timer_t;
# else
typedef double ACE_timer_t;
# endif /* ACE_LACKS_FLOATING_POINT */


# if defined (__ACE_INLINE__)
#   include "ace/Basic_Types.i"
# endif /* __ACE_INLINE__ */

# include "ace/post.h"
#endif /* ACE_BASIC_TYPES_H */
