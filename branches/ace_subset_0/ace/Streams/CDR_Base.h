// -*- C++ -*-

//=============================================================================
/**
 *  @file   CDR_Base.h
 *
 *  $Id$
 *
 * ACE Common Data Representation (CDR)	basic types.
 *
 * The current implementation assumes that the host has	1-byte,
 * 2-byte and 4-byte integral types, and that it has single
 * precision and double	precision IEEE floats.
 * Those assumptions are pretty	good these days, with Crays being
 * the only known exception.
 *
 *
 *  @author TAO	version	by
 *  @author Aniruddha Gokhale <gokhale@cs.wustl.edu>
 *  @author Carlos O'Ryan<coryan@cs.wustl.edu>
 *  @author ACE	version	by
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 *  @author Istvan Buki	<istvan.buki@euronet.be>
 */
//=============================================================================


#ifndef	ACE_CDR_BASE_H
#define	ACE_CDR_BASE_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE	*/

#include "ace/OS/Basic_Types.h"
#include "ace/Utils/Message_Block.h"

/**
 * @class ACE_CDR
 *
 * @brief Keep constants and some routines common to both Output and
 * Input CDR streams.
 */
class ACE_Export ACE_CDR
{
public:
  // = Constants defined by the	CDR protocol.
  // By	defining as many of these constants as possible	as enums we
  // ensure they get inlined and avoid pointless static	memory
  // allocations.

  enum
  {
    // Note that some of these get reused as part of the standard
    // binary format: unsigned is the same size	as its signed cousin,
    // float is	LONG_SIZE, and double is LONGLONG_SIZE.

    OCTET_SIZE = 1,
    SHORT_SIZE = 2,
    LONG_SIZE =	4,
    LONGLONG_SIZE = 8,
    LONGDOUBLE_SIZE = 16,

    OCTET_ALIGN	= 1,
    SHORT_ALIGN	= 2,
    LONG_ALIGN = 4,
    LONGLONG_ALIGN = 8,
    ///	@note the CORBA	LongDouble alignment requirements do not
    ///	match its size...
    LONGDOUBLE_ALIGN = 8,

    ///	Maximal	CDR 1.1	alignment: "quad precision" FP (i.e. "CDR::Long
    ///	double", size as above).
    MAX_ALIGNMENT = 8,

    ///	The default buffer size.
    /**
     * @todo We	want to	add options to control this
     *	 default value,	so this	constant should	be read	as the default
     *	 default value ;-)
     */
    DEFAULT_BUFSIZE = ACE_DEFAULT_CDR_BUFSIZE,

    ///	The buffer size	grows exponentially until it reaches this size;
    ///	afterwards it grows linearly using the next constant
    EXP_GROWTH_MAX = ACE_DEFAULT_CDR_EXP_GROWTH_MAX,

    ///	Once exponential growth	is ruled out the buffer	size increases
    ///	in chunks of this size,	note that this constants have the same
    ///	value right now, but it	does not need to be so.
    LINEAR_GROWTH_CHUNK	= ACE_DEFAULT_CDR_LINEAR_GROWTH_CHUNK
  };

  /**
   * Do	byte swapping for each basic IDL type size.  There exist only
   * routines to put byte, halfword (2 bytes), word (4 bytes),
   * doubleword	(8 bytes) and quadword (16 byte); because those	are
   * the IDL basic type	sizes.
   */
  static void swap_2 (const char *orig,	char *target);
  static void swap_4 (const char *orig,	char *target);
  static void swap_8 (const char *orig,	char *target);
  static void swap_16 (const char *orig, char *target);
  static void swap_2_array (const char *orig,
			    char *target,
			    size_t length);
  static void swap_4_array (const char *orig,
			    char *target,
			    size_t length);
  static void swap_8_array (const char *orig,
			    char *target,
			    size_t length);
  static void swap_16_array (const char	*orig,
			     char *target,
			     size_t length);

  /// Align the	message	block to ACE_CDR::MAX_ALIGNMENT,
  /// set by the CORBA spec at 8 bytes.
  static void mb_align (ACE_Message_Block *mb);

  /**
   * Compute the size of the smallest buffer that can contain at least
   * <minsize> bytes.
   * To	understand how a "best fit" is computed	look at	the
   * algorithm in the code.
   * Basically the buffers grow	exponentially, up to a certain point,
   * then the buffer size grows	linearly.
   * The advantage of this algorithm is	that is	rapidly	grows to a
   * large value, but does not explode at the end.
   */
  static size_t	first_size (size_t minsize);

  /// Compute not the smallest,	but the	second smallest	buffer that
  /// will fir <minsize> bytes.
  static size_t	next_size (size_t minsize);

  /**
   * Increase the capacity of mb to contain at least <minsize> bytes.
   * If	<minsize> is zero the size is increased	by an amount at	least
   * large enough to contain any of the	basic IDL types.  Return -1 on
   * failure, 0	on success.
   */
  static int grow (ACE_Message_Block *mb, size_t minsize);

  /// Copy a message block chain into a	single message block,
  /// preserving the alignment of the original stream.
  static void consolidate (ACE_Message_Block *dst,
			  const	ACE_Message_Block *src);

  static size_t	total_length (const ACE_Message_Block *begin,
			      const ACE_Message_Block *end);

  // Definitions of the	IDL basic types, for use in the	CDR
  // classes. The cleanest way to avoid	complaints from	all compilers
  // is	to define them all.
  #if defined (CHORUS) && defined (ghs)	&& !defined (__STANDARD_CXX)
    // This is non-compliant, but a nasty bout with
    // Green Hills C++68000 1.8.8 forces us into it.
    typedef unsigned long Boolean;
  #else	 /* ! (CHORUS && ghs 1.8.8) */
    typedef u_char Boolean;
  #endif /* ! (CHORUS && ghs 1.8.8) */

  typedef u_char Octet;
  typedef char Char;
  typedef ACE_OS::WChar	WChar;
  typedef ACE_INT16 Short;
  typedef ACE_UINT16 UShort;
  typedef ACE_INT32 Long;
  typedef ACE_UINT32 ULong;
  typedef ACE_UINT64 ULongLong;

  # if	  (defined (_MSC_VER) && (_MSC_VER >= 900)) \
	  || (defined (__BORLANDC__) &&	(__BORLANDC__ >= 0x530))
      typedef __int64 LongLong;
  # elif ACE_SIZEOF_LONG == 8 && !defined(_CRAYMPP)
      typedef long LongLong;
  # elif ACE_SIZEOF_LONG_LONG == 8 && !defined (ACE_LACKS_LONGLONG_T)
  #   if defined (sun) && !defined (ACE_LACKS_U_LONGLONG_T) && !defined	(__KCC)
	      // sun #defines	u_longlong_t, maybe other platforms do also.
	      // Use it, at least with g++, so that its -pedantic doesn't
	      // complain about no ANSI C++ long long.
	      typedef	longlong_t LongLong;
  #   else
	      // LynxOS 2.5.0	and Linux don't	have u_longlong_t.
	      typedef	long long LongLong;
  #   endif /* sun */
  # else  /* no	native 64 bit integer type */
  #   define NONNATIVE_LONGLONG
  #   if defined (ACE_BIG_ENDIAN)
	      struct ACE_Export LongLong 
        { 
          ACE_CDR::Long	h;
          ACE_CDR::Long l; 
          int operator== (const LongLong &rhs) const;
          int operator!= (const LongLong &rhs) const;
        };
  #   else
	      struct ACE_Export LongLong 
        { 
          ACE_CDR::Long	l;
          ACE_CDR::Long h; 
          int operator== (const LongLong &rhs) const;
          int operator!= (const LongLong &rhs) const;
        };
  #   endif /* ! ACE_BIG_ENDIAN	*/
  # endif /* no	native 64 bit integer type */

  # if defined (NONNATIVE_LONGLONG)
  #   define ACE_CDR_LONGLONG_INITIALIZER {0,0}
  # else
  #   define ACE_CDR_LONGLONG_INITIALIZER 0
  # endif /* NONNATIVE_LONGLONG */

  # if ACE_SIZEOF_FLOAT	== 4
      typedef float Float;
  # else  /* ACE_SIZEOF_FLOAT != 4 */
      struct Float
      {
  #	if ACE_SIZEOF_INT == 4
	  // Use u_int to get word alignment.
	  u_int	f;
  #	else  /* ACE_SIZEOF_INT	!= 4 */
	  // Applications will probably	have trouble with this.
	  char f[4];
  #	  if defined(_UNICOS) && !defined(_CRAYMPP)
	    Float (void);
	    Float (const float &init);
	    Float & operator= (const float &rhs);
	    int	operator!= (const Float	&rhs) const;
  #	  endif	/* _UNICOS */
  #	endif /* ACE_SIZEOF_INT	!= 4 */
      };
  # endif /* ACE_SIZEOF_FLOAT != 4 */

  # if ACE_SIZEOF_DOUBLE == 8
      typedef double Double;
  # else  /* ACE_SIZEOF_DOUBLE != 8 */
      struct Double
      {
  #	if ACE_SIZEOF_LONG == 8
	  // Use u_long	to get word alignment.
	  u_long f;
  #	else  /* ACE_SIZEOF_INT	!= 8 */
	  // Applications will probably	have trouble with this.
	  char f[8];
  #	endif /* ACE_SIZEOF_INT	!= 8 */
      };
  # endif /* ACE_SIZEOF_DOUBLE != 8 */

    // 94-9-32 Appendix	A defines a 128	bit floating point "long
    // double" data type, with greatly extended	precision and four
    // more bits of exponent (compared to "double").  This is an IDL
    // extension, not yet standard.

  #  if	  ACE_SIZEOF_LONG_DOUBLE == 16
       typedef long double	LongDouble;
       #define	ACE_CDR_LONG_DOUBLE_INITIALIZER	0
  #  else
  #    define NONNATIVE_LONGDOUBLE
  #    define	ACE_CDR_LONG_DOUBLE_INITIALIZER	{{0}}
       struct ACE_Export LongDouble
       {
         char ld[16];
         int operator== (const LongDouble &rhs) const;
         int operator!= (const LongDouble &rhs) const;
         // @@ also need other comparison operators.
       };
  #  endif /* ACE_SIZEOF_LONG_DOUBLE !=	16 */

#if !defined (ACE_CDR_GIOP_MAJOR_VERSION)
#   define ACE_CDR_GIOP_MAJOR_VERSION 1
#endif /*ACE_CDR_GIOP_MAJOR_VERSION */

#if !defined (ACE_CDR_GIOP_MINOR_VERSION)
#   define ACE_CDR_GIOP_MINOR_VERSION 2
#endif /* ACE_CDR_GIOP_MINOR_VERSION */
};

#if defined (__ACE_INLINE__)
# include "ace/Streams/CDR_Base.inl"
#endif	/* __ACE_INLINE__ */


#include "ace/post.h"

#endif /* ACE_CDR_BASE_H */
