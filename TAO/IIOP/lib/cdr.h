// This may look like C, but it's really -*- C++ -*-

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    cdr.h
//
// = DESCRIPTION
//     Common Data Representation (CDR) marshaling streams.
//
//     This implementation assumes that the native numeric
//     representation is two's complement for integers, IEEE
//     single/double for floats.  Also that characters are in ISO
//     Latin/1.
//
//     Note that CDR itself makes no such assumptions, but this
//     implementation makes such assumptions for reasons of
//     efficiency.  Careful enhancements could preserve that
//     efficiency where the assumptions are true, yet still allow the
//     code to work when they aren't true.
//
//     The implementation expects that buffers are aligned according
//     to the strongest CDR alignment restriction.
//
//     NOTE: this does everything "CDR 1.1" does ... that is, it
//     supports the five extended OMG-IDL data types in UNO Appendix
//     A, which provide richer arithmetic types (64 bit integers,
//     "quad precision" FP) and UNICODE-based characters and strings.
//     Those types are not standard parts of OMG-IDL at this time.
//
//     THREADING NOTE: CDR data structures must be protected against
//     concurrent access by their owning thread.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
// 
// ============================================================================

#if !defined (TAO_CDR_H)
#define	TAO_CDR_H

#include <assert.h>
#include "ace/OS.h"

#include	"align.h"

#if defined(__IIOP_BUILD)
#  include "orb.h"
#  include "marshal.h"
#else
#  include <corba/orb.h>
#  include <marshal.h>
#endif

// Identify byte order ... this is basically dependent on processor,
// but some processors support different byte orders (e.g. MIPS,
// UltraSPARC, PowerPC) as required for different software
// environments.
//
// We currently get this information through the "configure" script,
// which must (!!) be run before source code can be correctly
// compiled; too bad we have no way to ensure that the machine on
// which "configure" ran is the same as the one on which "make" will
// be run.

#undef	LITTLE_ENDIAN
#undef	BIG_ENDIAN
#undef	MY_BYTE_SEX

// PC compilers normally don't assume they'll be given code that runs
// on other architectures; cope with that limitation once here, so
// that only truly compiler-specific code needs to use their
// predefines.
//
// NOTE:  should really be using "__i386" to be strictly ANSI compliant.
//
#if defined (__BORLANDC__) || defined (_M_IX86)
#	if	!defined (i386)
#		define	i386
#		undef	WORDS_BIGENDIAN
#	endif
#endif

// Yes, we assume no bizarre mixed-endian "nuxi" machine, just as we
// assume only IEEE arithmetic.

#if defined (WORDS_BIGENDIAN)
#	define	BIG_ENDIAN
#	define	MY_BYTE_SEX	0
#else
#	define	LITTLE_ENDIAN
#	define	MY_BYTE_SEX	1
#endif

struct ACE_Svc_Export CDR
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
  // = Constants defined by the CDR protocol.  

  // By defining these constants as enums we ensure they get inlined
  // and avoid pointless static memory allocations.

  enum 
  {
    // Note that some of these get reused as part of the standard
    // binary format: unsigned is the same size as its signed cousin,
    // float is CDR_LONG_SIZE, and double is CDR_LONGLONG_SIZE.

    SHORT_SIZE = 2,
    LONG_SIZE = 4,
    LONGLONG_SIZE = 8,
    LONGDOUBLE_SIZE = 16,

    MAX_ALIGNMENT = 16,
    // Maximal CDR 1.1 alignment: "quad precision" FP (i.e. "long
    // double", size as above).

    DEFAULT_BUFSIZE = 1430 
    // Ethernet MTU, less headers.  Default buffer size for
    // request/response messages.  These are normally stack-allocated,
    // and tuning may cause you to want to change this value.  The
    // best value depends on your particular application mix; you can
    // also change how buffers grow().  Most remote invocations
    // (statistically) are "small", and the default used here is
    // perhaps larger than most such messages.
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

  CORBA_Boolean put_byte (char c);
  // encode a byte in the CDR stream

  CORBA_Boolean put_short (CORBA_Short s);
  // encode a short in the CDR stream

  CORBA_Boolean put_long (CORBA_Long l);
  // encode a long into the CDR stream

  CORBA_Boolean put_longlong (const CORBA_LongLong &ll);
  // encode a longlong into the CDR stream

  CORBA_Boolean put_char (CORBA_Char c);
  // encode a char into the CDR stream

  CORBA_Boolean put_wchar (CORBA_WChar wc);
  // encode a wide char into the CDR stream

  CORBA_Boolean put_boolean (CORBA_Boolean b);
  // encode a boolean into the CDR stream

  CORBA_Boolean	put_octet (CORBA_Octet o);
  // encode a octet into the CDR stream

  CORBA_Boolean	put_ushort (CORBA_UShort s);
  // encode an unsigned short  into the CDR stream

  CORBA_Boolean	put_ulong (CORBA_ULong l);
  // encode an unsigned long into the CDR stream

  CORBA_Boolean	put_ulonglong (const CORBA_ULongLong &ll);
  // encode an unsigned longlong into the CDR stream

  CORBA_Boolean	put_float (float f);
  // encode a float into the CDR stream

  CORBA_Boolean	put_double (const double &d);
  // encode a double into the CDR stream

  CORBA_Boolean put_longdouble (CORBA_LongDouble &ld);
  // encode a longdouble into the CDR stream


  CORBA_TypeCode::traverse_status encode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
  // Marshaling. ... <context> really points to a <CDR>.

  // = DECODING SUPPORT 

  // Same assumptions are made as above, but a flag is tested to
  // determine whether decode should byteswap or not.  It's cheaper to
  // do it that way than to use virtual functions.

  CORBA_Boolean get_byte (char &c);
  // decode a byte from the CDR stream

  CORBA_Boolean get_short (CORBA_Short &s);
  // decode a short from the CDR stream

  CORBA_Boolean get_long (CORBA_Long &l);
  // decode a long from the CDR stream

  CORBA_Boolean get_longlong (CORBA_LongLong &ll);
  // decode a longlong from the CDR stream

  CORBA_Boolean get_char (CORBA_Char &o);
  // decode a char from the CDR stream

  CORBA_Boolean get_wchar (CORBA_WChar &wc);
  // decode a wide char from the CDR stream

  CORBA_Boolean get_boolean (CORBA_Boolean &b);
  // decode a boolean from the CDR stream

  CORBA_Boolean	get_octet (CORBA_Octet &o);
  // decode an octet from the CDR stream

  CORBA_Boolean	get_ushort (CORBA_UShort &s);
  // decode an unsigned short from the CDR stream

  CORBA_Boolean	get_ulong (CORBA_ULong &l);
  // decode an unsigned long from the CDR stream

  CORBA_Boolean	get_ulonglong (const CORBA_ULongLong &ull);
  // decode an unsigned longlong from the CDR stream

  CORBA_Boolean	get_float (float &f);
  // decode a float from the CDR stream

  CORBA_Boolean	get_double (double &d);
  // decode a double from the CDR stream

  CORBA_Boolean get_longdouble (CORBA_LongDouble &ld);
  // decode a longdouble from the CDR stream


  CORBA_TypeCode::traverse_status decode (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *,
                                                  CORBA_Environment &env);
  // Unmarshaling interpreter ... <context> really points to a <CDR>.

  CORBA_TypeCode::traverse_status deep_copy (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *data2,
                                                  CORBA_Environment &env);
  // Does a deep copy for hierarchical data types

  CORBA_TypeCode::traverse_status deep_free (CORBA_TypeCode_ptr tc,
                                                  const void *data,
                                                  const void *data2,
                                                  CORBA_Environment &env);
  // Deallocates memory for hierarchical data structures

  CDR (u_char *buf = 0,
       u_int len = 0,
       int byte_order = MY_BYTE_SEX,
       int consume_buf = 0,
       TAO_MarshalFactory *f = TAO_DEFAULT_MARSHAL_FACTORY);
  // constructor
  

  ~CDR (void);
  // destructor

  void *operator new (size_t, void *_FAR p);
  void *operator new (size_t s);
  void operator delete (void *p);

  // = Used mostly when interpreting typecodes.

  // These may change the state of a CDR buffer even when errors are
  // reported.

  CORBA_Boolean skip_string (void);
  // skip a string field in a typecode

  CORBA_Boolean skip_bytes (u_int nbytes);
  // skip given number of bytes in a typecode
    
  void setup_encapsulation (u_char *buf, u_int len);
  // Also used when interpreting typecodes, but more generally when
  // getting ready to read from encapsulations.  In such cases the
  // buffer alignment guarantees must be provided by the caller, this
  // code doesn't verify them.  These streams are "read only".

  CORBA_Boolean grow (size_t newlength);
  // Grow the buffer to the identified size ... if it's zero, just
  // grow it by a standard quantum (e.g. when encoding we can't know
  // in advance how big it will need to become).

  size_t bytes_remaining (void);
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
  // Shall we free the memory?

  u_char *buffer;
  // Points to the dynamically allocated memory.

  size_t length;
  // Length of the dynamically allocated memory.

  int do_byteswap;	
  // for decoding only.

private:
  static void swap_long(unsigned char *orig, CORBA_Long &target);
  // do byte swapping for longs

  static void swap_longlong(unsigned char *orig, CORBA_LongLong &target);
  // do byte swapping for longlongs

  static void swap_longdouble(unsigned char *orig, CORBA_LongDouble &target);
  // do byte swapping for longdoubles

  TAO_MarshalFactory  *factory_;
  // maintain a factory that can make specialized marshaling objects

  TAO_MarshalObject   *mobj_;
  // maintain an instance of a marshaling object. The CDR stream delegates the
  // marshaling activity to mobj_;
};

// In this ONE case, we make a substantial exception to how inline
// files are included.  Normally, we would conditionally include the
// inline file iff __ACE_INLINE__ is defined.  But, in the original,
// highly optimized Sun IIOP code, much of what is in the inline file
// was here ready to be inlined at a moments notice and ALWAYS.  So,
// in this ONE file, we defer to David Brownell's considerable prowess
// at creating typecode interpreters as well as to the ACE convention
// of placing inline functions into separate files.
#  if !defined(__ACE_INLINE__)
#    undef ACE_INLINE
#    define ACE_INLINE inline
#    define do_undef_on_ACE_INLINE
#  endif

#  include "cdr.i"
#  if defined(do_undef_on_ACE_INLINE)
#    undef do_undef_on_ACE_INLINE
#    undef ACE_INLINE
#    define ACE_INLINE
#  endif

#endif /* TAO_CDR_H */
