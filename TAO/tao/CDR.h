// This may look like C, but it's really -*- C++ -*-
// $Id$

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
//     Many enhancements added by Andy Gokhake and Carlos O'Ryan.
//
// ============================================================================

#if !defined (TAO_CDR_H)
#define TAO_CDR_H

class TAO_Export CDR
{
  // = TITLE
  //   The core marshaling primitive: a memory buffer, into which all
  //   the basic OMG-IDL datatypes can be placed ... or from which
  //   they can be retreived.
  //
  // = DESCRIPTION
  //   A particularly useful static member function for this buffer is
  //   an interpretive encoding routine, usable as a typecode
  //   interpreter callback.  Ditto for decoding.  These are used to
  //   support all OMG-IDL datatypes, even those not supported
  //   directly by put/get primitives.
  //
  //   Struct members are intentionally exposed; the functionality of
  //   this class, and hence the appropriate abstactions for them,
  //   hasn't quite settled down enough to settle on fast abstractions
  //   that let data be hidden without pointlessly sacrificing speed.
public:
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

    DEFAULT_BUFSIZE = 1430,
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

    EXP_GROWTH_MAX = 4096,
    // The buffer size grows exponentially until it reaches this size;
    // afterwards it grows linearly using the next constant

    LINEAR_GROWTH_CHUNK = 4096
    // Once exponential growth is ruled out the buffer size increases
    // in chunks of this size, note that this constants have the same
    // value right now, but it does not need to be so.
  };
  // = ENCODING SUPPORT

  // = Adjust pointers as needed, then store in the native byte order.
  //
  // There exist only routines to put byte, halfword (2 bytes), word
  // (4 bytes), doubleword (8 bytes) and quadword (16 byte) entities,
  // plus the interpretive encoder.

  CORBA::Boolean put_byte (char c);
  // encode a byte in the CDR stream

  CORBA::Boolean put_short (CORBA::Short s);
  // encode a short in the CDR stream

  CORBA::Boolean put_long (CORBA::Long l);
  // encode a long into the CDR stream

  CORBA::Boolean put_longlong (const CORBA::LongLong &ll);
  // encode a longlong into the CDR stream

  CORBA::Boolean put_char (CORBA::Char c);
  // encode a char into the CDR stream

  CORBA::Boolean put_wchar (CORBA::WChar wc);
  // encode a wide char into the CDR stream

  CORBA::Boolean put_boolean (CORBA::Boolean b);
  // encode a boolean into the CDR stream

  CORBA::Boolean put_octet (CORBA::Octet o);
  // encode a octet into the CDR stream

  CORBA::Boolean put_ushort (CORBA::UShort s);
  // encode an unsigned short  into the CDR stream

  CORBA::Boolean put_ulong (CORBA::ULong l);
  // encode an unsigned long into the CDR stream

  CORBA::Boolean put_ulonglong (const CORBA::ULongLong &ll);
  // encode an unsigned longlong into the CDR stream

  CORBA::Boolean put_float (CORBA::Float f);
  // encode a float into the CDR stream

  CORBA::Boolean put_double (const CORBA::Double &d);
  // encode a double into the CDR stream

  CORBA::Boolean put_longdouble (CORBA::LongDouble &ld);
  // encode a longdouble into the CDR stream

  CORBA::Boolean put_string (const char *str, CORBA::ULong len);
  // encode a string of length len

  CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                           const void *data,
                                           const void *,
                                           CORBA::Environment &env);
  // Marshaling. ... <context> really points to a <CDR>.

  // = DECODING SUPPORT

  // Same assumptions are made as above, but a flag is tested to
  // determine whether decode should byteswap or not.  It's cheaper to
  // do it that way than to use virtual functions.

  CORBA::Boolean get_byte (char &c);
  // decode a byte from the CDR stream

  CORBA::Boolean get_short (CORBA::Short &s);
  // decode a short from the CDR stream

  CORBA::Boolean get_long (CORBA::Long &l);
  // decode a long from the CDR stream

  CORBA::Boolean get_longlong (CORBA::LongLong &ll);
  // decode a longlong from the CDR stream

  CORBA::Boolean get_char (CORBA::Char &o);
  // decode a char from the CDR stream

  CORBA::Boolean get_wchar (CORBA::WChar &wc);
  // decode a wide char from the CDR stream

  CORBA::Boolean get_boolean (CORBA::Boolean &b);
  // decode a boolean from the CDR stream

  CORBA::Boolean get_octet (CORBA::Octet &o);
  // decode an octet from the CDR stream

  CORBA::Boolean get_ushort (CORBA::UShort &s);
  // decode an unsigned short from the CDR stream

  CORBA::Boolean get_ulong (CORBA::ULong &l);
  // decode an unsigned long from the CDR stream

  CORBA::Boolean get_ulonglong (CORBA::ULongLong &ull);
  // decode an unsigned longlong from the CDR stream

  CORBA::Boolean get_float (CORBA::Float &f);
  // decode a float from the CDR stream

  CORBA::Boolean get_double (CORBA::Double &d);
  // decode a double from the CDR stream

  CORBA::Boolean get_longdouble (CORBA::LongDouble &ld);
  // decode a longdouble from the CDR stream

  CORBA::Boolean get_string (char *&str, CORBA::ULong len);
  // decode a string. Length includes the terminating 0

  CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                           const void *data,
                                           const void *,
                                           CORBA::Environment &env);
  // Unmarshaling interpreter ... <context> really points to a <CDR>.

  CDR (char *buf = 0,
       size_t len = 0,
       int byte_order = TAO_ENCAP_BYTE_ORDER,
       int consume_buf = 0,
       TAO_Marshal_Factory *f = TAO_Marshal::DEFAULT_MARSHAL_FACTORY);
  // constructor

  CDR (const CDR& rhs);
  // Copy constructor, build a new stream that points to the same data
  // as <rhs>.
  // Using this new stream for writing results in undefined behavior.

  ~CDR (void);
  // destructor

  // = Used mostly when interpreting typecodes.

  // These may change the state of a CDR buffer even when errors are
  // reported.

  CORBA::Boolean skip_string (void);
  // skip a string field in a typecode

  // TODO: This methods should be private and the classes that need it
  // (TypeCode, Exception, etc.) would be declared friend.

  CORBA::Boolean get_encapsulation (char*& buf, CORBA::ULong& len);
  // Returns an encapsulated buffer (such as a string) stored inside
  // the CDR.
  // TODO: This method should be private and the classes that need it
  // (TypeCode, Exception, etc.) would be declared friend.

  CORBA::Boolean get_string (char*& buf);
  // Returns an encapsulated string stored inside the CDR; but without
  // any copying.

  CORBA::Boolean rd_ptr (size_t n);
  // Move the read pointer <n> bytes ahead, it is used to skip
  // portions of the stream, specially in typecodes.

  CORBA::Boolean wr_ptr (size_t n);
  // Move the write pointer <n> bytes ahead, it is used to when the
  // CDR is read from a socket to set the end of the message.

  void setup_encapsulation (char *buf, u_int len);
  // Also used when interpreting typecodes, but more generally when
  // getting ready to read from encapsulations.  In such cases the
  // buffer alignment guarantees must be provided by the caller, this
  // code doesn't verify them.  These streams are "read only".

  void setup_indirection (CDR& cdr, CORBA::Long offset);
  // Set the CDR to point to the stream in <cdr>.
  // The stream is read-only from then on.

  CORBA::Boolean grow (size_t newlength);
  // Grow the buffer to the identified size ... if it's zero, just
  // grow it by a standard quantum (e.g. when encoding we can't know
  // in advance how big it will need to become).

  size_t bytes_remaining (void);
  // Some code needs to know how much is left on encode or decode.

  int good_bit (void) const;
  // If zero then some error has ocurred.

  char *buffer (void) const;
  // Return the internal buffer.

  size_t length (void) const;
  // Return the internal buffer length (how many bytes in the buffer
  // contain useful data).

  size_t size (void) const;
  // Return the internal buffer capacity.

  void reset (void);
  // Reset the read and write pointers to the start of the buffer.

  int do_byteswap;
  // for decoding only.
  // TODO: It could be used for encoding also, for instance, if all
  // the machines in a network but one are little endian it makes
  // sense to make that machine swap the bytes on write.  At least
  // some people would like such a feature.

private:
  static void swap_long (char *orig, CORBA::Long &target);
  // do byte swapping for longs

  static void swap_ulonglong (char *orig, CORBA::ULongLong &target);
  // do byte swapping for longlongs

  static void swap_longdouble (char *orig, CORBA::LongDouble &target);
  // do byte swapping for longdoubles

  static void mb_align (ACE_Message_Block* mb);

  CORBA::Boolean adjust_to_put (size_t size,
                                char*& buf);
  // Returns (in <buf>) the next position in the buffer aligned to
  // <size>, it advances the Message_Block wr_ptr past the data
  // (i.e. <buf> + <size>). If necessary it grows the Message_Block
  // buffer.  Returns B_FALSE on failure.

  CORBA::Boolean adjust_to_get (size_t size,
                                char*& buf);
  // Returns (in <buf>) the next position in the buffer aligned to
  // <size>, it sets the Message_Block rd_ptr past the data
  // (i.e. <buf> + <size>).  Returns B_FALSE on failure.

private:
  ACE_Message_Block *mb_;
  // The buffer is stored in a Message_Block, future implementations
  // will take advantage of the chaining features of it to provide
  // minimal copying encapsulation.

  TAO_Marshal_Factory *factory_;
  // Maintain a factory that can make specialized marshaling objects.

  TAO_Marshal_Object *mobj_;
  // Maintain an instance of a marshaling object. The CDR stream
  // delegates the marshaling activity to mobj_.

  int good_bit_;
  // Set to 0 when an error ocurrs.
};

#if !defined(__ACE_INLINE__)
extern CDR &operator<< (CDR &cdr, CORBA::Boolean x);
extern CDR &operator<< (CDR &cdr, CORBA::Octet x);
extern CDR &operator<< (CDR &cdr, CORBA::Short x);
extern CDR &operator<< (CDR &cdr, CORBA::UShort x);
extern CDR &operator<< (CDR &cdr, CORBA::Long x);
extern CDR &operator<< (CDR &cdr, CORBA::ULong x);
extern CDR &operator<< (CDR &cdr, CORBA::LongLong x);
extern CDR &operator<< (CDR &cdr, CORBA::ULongLong x);
extern CDR &operator<< (CDR &cdr, CORBA::Float x);
extern CDR &operator<< (CDR &cdr, CORBA::Double x);
extern CDR &operator<< (CDR &cdr, CORBA::Char x);
extern CDR &operator<< (CDR &cdr, CORBA::WChar x);
extern CDR &operator>> (CDR &cdr, CORBA::Boolean &x);
extern CDR &operator>> (CDR &cdr, CORBA::Octet &x);
extern CDR &operator>> (CDR &cdr, CORBA::Short &x);
extern CDR &operator>> (CDR &cdr, CORBA::UShort &x);
extern CDR &operator>> (CDR &cdr, CORBA::Long &x);
extern CDR &operator>> (CDR &cdr, CORBA::ULong &x);
extern CDR &operator>> (CDR &cdr, CORBA::LongLong &x);
extern CDR &operator>> (CDR &cdr, CORBA::ULongLong &x);
extern CDR &operator>> (CDR &cdr, CORBA::Float &x);
extern CDR &operator>> (CDR &cdr, CORBA::Double &x);
extern CDR &operator>> (CDR &cdr, CORBA::Char &x);
extern CDR &operator>> (CDR &cdr, CORBA::WChar &x);
#endif /* __ACE_INLINE */

#endif /* TAO_CDR_H */
