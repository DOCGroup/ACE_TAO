// This may look like C, but it's really -*- C++ -*-
//
// @(#) $Id$
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// CDR:		 Common Data Representation (CDR) marshaling streams.
//
// This implementation assumes that the native numeric representation
// is two's complement for integers, IEEE single/double for floats.  Also
// that characters are in ISO Latin/1.
//
// Note that CDR itself makes no such assumptions, but this implementation
// makes such assumptions for reasons of efficiency.  Careful enhancements
// could preserve that efficiency where the assumptions are true, yet
// still allow the code to work when they aren't true.
//
// The implementation expects that buffers are aligned according to the
// strongest CDR alignment restriction.
//
// NOTE: this does everything "CDR 1.1" does ... that is, it supports
// the five extended OMG-IDL data types in UNO Appendix A, which provide
// richer arithmetic types (64 bit integers, "quad precision" FP) and
// UNICODE-based characters and strings.  Those types are not standard
// parts of OMG-IDL at this time.
//
// THREADING NOTE:  CDR data structures must be protected against
// concurrent access by their owning thread.

#if !defined (TAO_CDR_H)
#define	TAO_CDR_H

#include	<assert.h>

#if defined(__IIOP_BUILD)
#  include "orb.h"
#else
#  include <corba/orb.h>
#endif

#include	<align.h>


//
// Identify byte order ... this is basically dependent on processor, but some
// processors support different byte orders (e.g. MIPS, UltraSPARC, PowerPC)
// as required for different software environments.
//
// We currently get this information through the "configure" script, which
// must (!!) be run before source code can be correctly compiled; too bad
// we have no way to ensure that the machine on which "configure" ran is
// the same as the one on which "make" will be run.

#undef	LITTLE_ENDIAN
#undef	BIG_ENDIAN
#undef	MY_BYTE_SEX

// PC compilers normally don't assume they'll be given code that runs
// on other architectures; cope with that limitation once here, so that
// only truly compiler-specific code needs to use their predefines.
//
// NOTE:  should really be using "__i386" to be strictly ANSI compliant.
//
#if defined (__BORLANDC__) || defined (_M_IX86)
#	if	!defined (i386)
#		define	i386
#		undef	WORDS_BIGENDIAN
#	endif
#endif

//
// Yes, we assume no bizarre mixed-endian "nuxi" machine, just as we
// assume only IEEE arithmetic.
//
#if defined (WORDS_BIGENDIAN)
#	define	BIG_ENDIAN
#	define	MY_BYTE_SEX	0
#else
#	define	LITTLE_ENDIAN
#	define	MY_BYTE_SEX	1
#endif

struct _EXPCLASS CDR
// = TITLE
// The core marshaling primitive: a memory buffer, into which all the
// basic OMG-IDL datatypes can be placed ... or from which they can be
// retreived.
//
// = DESCRIPTION
// A particularly useful static member function for this buffer is an
// interpretive encoding routine, usable as a typecode interpreter
// callback.  Ditto for decoding.  These are used to support all
// OMG-IDL datatypes, even those not supported directly by put/get
// primitives.
//
// Struct members are intentionally exposed; the functionality of this
// class, and hence the appropriate abstactions for them, hasn't quite
// settled down enough to settle on fast abstractions that let data be
// hidden without pointlessly sacrificing speed.
{
  // Define these constants as enums to ensure they get inlined
  // and to avoid pointless static memory allocations.
  enum 
  {
    // = Constants defined by the CDR protocol.  

    // Note that some of these get reused as part of the standard
    // binary format: unsigned is the same size as its signed cousin,
    // float is CDR_LONG_SIZE, and double is CDR_LONGLONG_SIZE.

    SHORT_SIZE = 2,
    LONG_SIZE = 4,
    LONGLONG_SIZE = 8,
    LONGDOUBLE_SIZE = 16,

    MAX_ALIGNMENT = 16,
    // maximal CDR 1.1 alignment:   "quad precision" FP
    // (i.e. "long double", size as above)

    DEFAULT_BUFSIZE = 1430		// Ethernet MTU, less headers
    // Default buffer size for request/response messages.  These are
    // normally stack-allocated, and tuning may cause you to want to
    // change this value.  The best value depends on your particular
    // application mix; you can also change how buffers grow().  Most
    // remote invocations (statistically) are "small", and the default
    // used here is perhaps larger than most such messages.
    // 
    // If this size is "too small" you need to heap-allocate buffers too
    // often.  "Too large" is mostly a waste of stackspace, but stack
    // frames as large as the system page size (often 4Kb) can easily
    // overrun the "redzone" at the bottom of most VM-based stacks.
  };

  // = ENCODING SUPPORT 

  // Adjust pointers as needed, then store in the native byte order.
  //
  // There exist only routines to put byte, halfword (2 bytes), word
  // (4 bytes), doubleword (8 bytes) and quadword (16 byte) entities,
  // plus the interpretive encoder.

  CORBA_Boolean		put_byte (char c);
  CORBA_Boolean		put_short (CORBA_Short s);
  CORBA_Boolean		put_long (CORBA_Long l);
  CORBA_Boolean		put_longlong (const CORBA_LongLong &ll);

  inline CORBA_Boolean	put_char (CORBA_Char c)
    { return put_byte ((char) c); }

  inline CORBA_Boolean	put_wchar (CORBA_WChar wc)
    {
      //
      // "wchar_t" isn't always 2 bytes, such
      // systems might need further conversion
      // (e.g. hosts with multibyte characters
      // native, rather than UNICODE)
      //
      return put_short ((short)wc);
    }
    
  inline CORBA_Boolean	put_boolean (CORBA_Boolean b)
    { return put_byte ((char)
		       (b != CORBA_B_FALSE)); }

  inline CORBA_Boolean	put_octet (CORBA_Octet o)
    { return put_byte ((char) o); }

  inline CORBA_Boolean	put_ushort (CORBA_UShort s)
    { return put_short ((CORBA_Short) s); }

  inline CORBA_Boolean	put_ulong (CORBA_ULong l)
    { return put_long ((CORBA_Long) l); }

  inline CORBA_Boolean	put_ulonglong (const CORBA_ULongLong &ll)
    { return put_longlong ((CORBA_LongLong &) ll); }
				    
  inline CORBA_Boolean	put_float (float f)
    { return put_long (*(CORBA_Long *) &f); }

  inline CORBA_Boolean	put_double (const double &d)
    { return
	put_longlong (*(CORBA_LongLong *) &d); }

  CORBA_Boolean		put_longdouble (CORBA_LongDouble &ld);

  static CORBA_TypeCode::traverse_status
    encoder (CORBA_TypeCode_ptr	tc,
	     const void		*data,
	     const void     		*,
	     void			*context,
	     CORBA_Environment	&env);
  // Marshaling interpreter ... 'context' really points to a CDR.

  // = DECODING SUPPORT 

  // Same assumptions are made as above, but a flag is tested to
  // determine whether decode should byteswap or not.  It's cheaper to
  // do it that way than to use virtual functions.

  CORBA_Boolean		get_byte (char &c);
  CORBA_Boolean		get_short (CORBA_Short &s);
  CORBA_Boolean		get_long (CORBA_Long &l);
  CORBA_Boolean		get_longlong (CORBA_LongLong &ll);

  inline CORBA_Boolean	get_char (CORBA_Char &o)
    { return get_byte ((char &) o); }

  inline CORBA_Boolean	get_wchar (CORBA_WChar &wc)
    {
      short s;

      //
      // wchar_t isn't always "short"
      //
      CORBA_Boolean retval = get_short (s);
      wc = s;
      return retval;
    }

  inline CORBA_Boolean	get_boolean (CORBA_Boolean &b)
    {
      CORBA_Char c;

      //
      // CORBA_Boolean is rarely 'char'
      //
      CORBA_Boolean retval = get_char (c);
      b = (c == 1);
      return retval;
    }

  inline CORBA_Boolean	get_octet (CORBA_Octet &o)
    { return get_byte ((char &) o); }

  inline CORBA_Boolean	get_ushort (CORBA_UShort &s)
    { return get_short ((short&) s); }

  inline CORBA_Boolean	get_ulong (CORBA_ULong &l)
    { return get_long ((CORBA_Long &) l); }

  inline CORBA_Boolean	get_ulonglong (const CORBA_ULongLong &ull)
    { return get_longlong ((CORBA_LongLong &) ull); }

  inline CORBA_Boolean	get_float (float &f)
    { return get_long ((CORBA_Long &) f); }

  inline CORBA_Boolean	get_double (double &d)
    { return get_longlong ((CORBA_LongLong &) d); }

  CORBA_Boolean get_longdouble (CORBA_LongDouble &ld);

  static CORBA_TypeCode::traverse_status
    decoder (CORBA_TypeCode_ptr	tc,
	     const void		*data,
	     const void		*,
	     void			*context,
	     CORBA_Environment	&env);
  // Unmarshaling interpreter ... 'context' really points to a CDR.

  CDR (u_char *buf = 0,
       unsigned len = 0,
       int byte_order = MY_BYTE_SEX,
       int consume_buf = 0) 
  // Constructor ... buffer must be aligned for the strictest CDR
  // alignment requirement, since the algorithms used here only
  // maintain alignment with respect to &buffer [0].
  //
  // Yes, that complicates the grow() primitive.
    : real_buffer (buf),
      do_free (consume_buf),
      do_byteswap (byte_order != MY_BYTE_SEX)
    {
      ptr_arith_t temp = (ptr_arith_t) buf;

      temp += MAX_ALIGNMENT - 1;
      temp &= ~((ptr_arith_t) MAX_ALIGNMENT - 1);
      buffer = next = (u_char *) temp;

      length = remaining
	= len - (unsigned) (buffer - real_buffer);
    }

  ~CDR (void)
    { if (do_free) delete real_buffer; }

  void *operator new (size_t, void *_FAR p)
    { return p; }
  void *operator new (size_t s)
    { return ::operator new (s); }
  void operator delete (void *p)
    { ::operator delete (p); }

  // = Used mostly when interpreting typecodes.

  // These may change the state of a CDR buffer even when errors are
  // reported.

  CORBA_Boolean		skip_string ();
  CORBA_Boolean		skip_bytes (u_int nbytes)
    {
      if (remaining < nbytes)
	return CORBA_B_FALSE;
      remaining -= nbytes;
      next += nbytes;
      return CORBA_B_TRUE;
    }
    
  void setup_encapsulation (u_char *buf,
			    u_int len)
  // Also used when interpreting typecodes, but more generally when
  // getting ready to read from encapsulations.  In such cases the
  // buffer alignment guarantees must be provided by the caller, this
  // code doesn't verify them.  These streams are "read only".
    {
      next = buf + 1;
      remaining = len - 1;
      do_byteswap = (*buf != MY_BYTE_SEX);
      do_free = 0;
    }

  CORBA_Boolean grow (size_t newlength);
  // Grow the buffer to the identified size ... if it's zero, just
  // grow it by a standard quantum (e.g. when encoding we can't know
  // in advance how big it will need to become).

  size_t bytes_remaining () { return remaining; }
  // Some code needs to know how much is left on encode or decode

  // private:
  //  DATA MEMBERS ...

  u_char *next;		
  // next data goes here

  size_t remaining;	
  // space left

  u_char *real_buffer;	
  // maybe not aligned

  int do_free;

  u_char *buffer;
  size_t length;

  int do_byteswap;	
  // decode ONLY
};

#endif	/* TAO_CDR_H */
