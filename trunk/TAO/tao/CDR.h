// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    CDR.h
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
//     Many enhancements added by Aniruddha Gokhale and Carlos O'Ryan.
//
// ============================================================================

#ifndef TAO_CDR_H
#define TAO_CDR_H

class TAO_Export CDR
{
  // = TITLE
  //   Keep constants and some routines common to both Output and
  //   Input CDR streams.
  //
public:
  // = Constants defined by the CDR protocol.

  // By defining these constants as enums we ensure they get inlined
  // and avoid pointless static memory allocations.

  enum
  {
    // Note that some of these get reused as part of the standard
    // binary format: unsigned is the same size as its signed cousin,
    // float is CDR_LONG_SIZE, and double is CDR_LONGLONG_SIZE.

    OCTET_SIZE = 1,
    SHORT_SIZE = 2,
    LONG_SIZE = 4,
    LONGLONG_SIZE = 8,
    LONGDOUBLE_SIZE = 16,

    OCTET_ALIGN = 1,
    SHORT_ALIGN = 2,
    LONG_ALIGN = 4,
    LONGLONG_ALIGN = 8,
    LONGDOUBLE_ALIGN = 8,
    // Note how the CORBA::LongDouble alignment requirements do not
    // match its size...

    MAX_ALIGNMENT = 8,
    // Maximal CDR 1.1 alignment: "quad precision" FP (i.e. "long
    // double", size as above).

    DEFAULT_BUFSIZE = TAO_DEFAULT_CDR_BUFSIZE,
    // The default buffer size.
    // @@ TODO We want to add options to the ORB to control this
    // default value, so this constant should be read as the, default
    // default value ;-)

    EXP_GROWTH_MAX = TAO_DEFAULT_CDR_EXP_GROWTH_MAX,
    // The buffer size grows exponentially until it reaches this size;
    // afterwards it grows linearly using the next constant

    LINEAR_GROWTH_CHUNK = TAO_DEFAULT_CDR_LINEAR_GROWTH_CHUNK
    // Once exponential growth is ruled out the buffer size increases
    // in chunks of this size, note that this constants have the same
    // value right now, but it does not need to be so.
  };

  static void swap_2 (const char *orig, char *target);
  static void swap_4 (const char *orig, char *target);
  static void swap_8 (const char *orig, char *target);
  static void swap_16 (const char *orig, char *target);
  // Do byte swapping for each basic IDL type size.
  // There exist only routines to put byte, halfword (2 bytes), word
  // (4 bytes), doubleword (8 bytes) and quadword (16 byte); because
  // those are the IDL basic type sizes.

  static void mb_align (ACE_Message_Block* mb);

  static int grow (ACE_Message_Block *mb, size_t minsize);
  // Increase the capacity of mb to contain at least <minsize> bytes.
  // If <minsze> is zero the size is increased by an amount at least
  // large enough to contain any of the basic IDL types.  Return -1 on
  // failure, 0 on success.
};

class TAO_Export TAO_OutputCDR
{
  //
  // = TITLE
  //   A CDR stream for writing, i.e. for marshalling.
  //
  // = DESCRIPTION
  //   This class is based on the the CORBA spec for Java (98-02-29),
  //   java class omg.org.CORBA.portable.OutputStream.
  //   It diverts in a few ways:
  //     + Operations taking arrays don't have offsets, because in C++
  //     it is easier to describe an array starting from x+offset.
  //     + Operations return an error status, because exceptions are
  //     not widely available in C++ (yet).
  //
  //   A particularly useful static member function for this buffer is
  //   an interpretive encoding routine, usable as a typecode
  //   interpreter callback.  Ditto for decoding.  These are used to
  //   support all OMG-IDL datatypes, even those not supported
  //   directly by put/get primitives.
  //
public:
  friend class TAO_InputCDR;
  // For reading from a output CDR stream.

  TAO_OutputCDR (size_t size = 0,
                 int byte_order = TAO_ENCAP_BYTE_ORDER,
                 TAO_Marshal_Factory *f =
                     TAO_Marshal::DEFAULT_MARSHAL_FACTORY,
                 ACE_Allocator* buffer_allocator = 0,
                 ACE_Allocator* data_block_allocator = 0);
  // Default constructor, allocates <size> bytes in the internal
  // buffer, if <size> == 0 it allocates the default size.

  TAO_OutputCDR (char *data, size_t size,
                 int byte_order = TAO_ENCAP_BYTE_ORDER,
                 TAO_Marshal_Factory *f =
                     TAO_Marshal::DEFAULT_MARSHAL_FACTORY,
                 ACE_Allocator* buffer_allocator = 0,
                 ACE_Allocator* data_block_allocator = 0);
  // Build a CDR stream with an initial buffer, it will *not* remove
  // <data>, since it did not allocated it.

  TAO_OutputCDR (ACE_Message_Block *data,
                 int byte_order = TAO_ENCAP_BYTE_ORDER,
                 TAO_Marshal_Factory *f =
                     TAO_Marshal::DEFAULT_MARSHAL_FACTORY);
  // Build a CDR stream with an initial Message_Block chain, it will *not*
  // remove <data>, since it did not allocate it.

  ~TAO_OutputCDR (void);
  // destructor

  // = We have one method per basic IDL type....
  CORBA_Boolean write_boolean (CORBA::Boolean x);
  CORBA_Boolean write_char (CORBA::Char x);
  CORBA_Boolean write_wchar (CORBA::WChar x);
  CORBA_Boolean write_octet (CORBA::Octet x);
  CORBA_Boolean write_short (CORBA::Short x);
  CORBA_Boolean write_ushort (CORBA::UShort x);
  CORBA_Boolean write_long (CORBA::Long x);
  CORBA_Boolean write_ulong (CORBA::ULong x);
  CORBA_Boolean write_longlong (const CORBA::LongLong &x);
  CORBA_Boolean write_ulonglong (const CORBA::ULongLong &x);
  CORBA_Boolean write_float (CORBA::Float x);
  CORBA_Boolean write_double (const CORBA::Double &x);
  CORBA_Boolean write_longdouble (const CORBA::LongDouble &x);

  CORBA_Boolean write_string (const CORBA::Char *x);
  CORBA_Boolean write_string (CORBA::ULong length, const CORBA::Char *x);
  CORBA_Boolean write_wstring (const CORBA::WChar *x);
  CORBA_Boolean write_wstring (CORBA::ULong length, const CORBA::WChar *x);
  // For string we offer methods that accept a precomputed length.

  // = We add one method to write arrays of basic IDL types.
  // Note: the portion written starts at <x> and ends at <x + length>.
  // The length is *NOT* stored into the CDR stream.
  CORBA_Boolean write_boolean_array (const CORBA::Boolean* x,
                                     CORBA::ULong length);
  CORBA_Boolean write_char_array (const CORBA::Char* x,
                                  CORBA::ULong length);
  CORBA_Boolean write_wchar_array (const CORBA::WChar* x,
                                   CORBA::ULong length);
  CORBA_Boolean write_octet_array (const CORBA::Octet* x,
                                   CORBA::ULong length);
  CORBA_Boolean write_short_array (const CORBA::Short* x,
                                   CORBA::ULong length);
  CORBA_Boolean write_ushort_array (const CORBA::UShort* x,
                                    CORBA::ULong length);
  CORBA_Boolean write_long_array (const CORBA::Long* x,
                                  CORBA::ULong length);
  CORBA_Boolean write_ulong_array (const CORBA::ULong* x,
                                   CORBA::ULong length);
  CORBA_Boolean write_longlong_array (const CORBA::LongLong* x,
                                      CORBA::ULong length);
  CORBA_Boolean write_ulonglong_array (const CORBA::ULongLong* x,
                                       CORBA::ULong length);
  CORBA_Boolean write_float_array (const CORBA::Float* x,
                                   CORBA::ULong length);
  CORBA_Boolean write_double_array (const CORBA::Double* x,
                                    CORBA::ULong length);
  CORBA_Boolean write_longdouble_array (const CORBA::LongDouble* x,
                                        CORBA::ULong length);

  CORBA_Boolean write_octet_array_mb (const ACE_Message_Block* mb);
  // Write an octet array contained inside a MB, this can be optimized
  // to minimize copies.

  // = We have one method per basic IDL type....
  // They return 0 on failure and 1 on success.
  CORBA_Boolean append_boolean (TAO_InputCDR &);
  CORBA_Boolean append_char (TAO_InputCDR &);
  CORBA_Boolean append_wchar (TAO_InputCDR &);
  CORBA_Boolean append_octet (TAO_InputCDR &);
  CORBA_Boolean append_short (TAO_InputCDR &);
  CORBA_Boolean append_ushort (TAO_InputCDR &);
  CORBA_Boolean append_long (TAO_InputCDR &);
  CORBA_Boolean append_ulong (TAO_InputCDR &);
  CORBA_Boolean append_longlong (TAO_InputCDR &);
  CORBA_Boolean append_ulonglong (TAO_InputCDR &);
  CORBA_Boolean append_float (TAO_InputCDR &);
  CORBA_Boolean append_double (TAO_InputCDR &);
  CORBA_Boolean append_longdouble (TAO_InputCDR &);
  CORBA_Boolean append_wstring (TAO_InputCDR &);
  CORBA_Boolean append_string (TAO_InputCDR &);

  CORBA::TypeCode::traverse_status append (CORBA::TypeCode_ptr tc,
                                           TAO_InputCDR *src,
                                           CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Append the contents of the CDR stream based on information
  // described by <tc>; returning any errors in <env>.

  // @@ TODO: do we want a special method to write an array of
  // strings and wstrings?

  // = TAO specific methods.

  void reset (void);
  // Reuse the CDR stream to write on the old buffer.

  int good_bit (void) const;
  // Returns 0 if an error has ocurred, the only expected error is to
  // run out of memory.

  const ACE_Message_Block* begin (void) const;
  // Return the start of the message block chain for this CDR stream.
  // NOTE: The complete CDR stream is represented by a chain of
  // message blocks.

  const ACE_Message_Block* end (void) const;
  // Return the last message in the chain that is is use.

  const char* buffer (void) const;
  size_t length (void) const;
  // Return the start and size of the internal buffer.
  // NOTE: This methods only return information about the first block
  // in the chain.

  size_t total_length (void) const;
  // Add the length of each message block in the chain.

  CORBA::TypeCode::traverse_status encode (CORBA::TypeCode_ptr tc,
                                           const void *data,
                                           const void *,
                                           CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Marshalls the contents of <data> as described by the TypeCode in
  // <tc>. Any errors are reported though the <env> parameter.

private:
  TAO_OutputCDR (const TAO_OutputCDR& rhs);
  TAO_OutputCDR& operator= (const TAO_OutputCDR& rhs);
  // disallow copying...

  int adjust (size_t size, char*& buf);
  // Returns (in <buf>) the next position in the buffer aligned to
  // <size>, it advances the Message_Block wr_ptr past the data
  // (i.e. <buf> + <size>). If necessary it grows the Message_Block
  // buffer.
  // Sets the good_bit to 0 and returns a -1 on failure.

  int adjust (size_t size, size_t align, char*& buf);
  // As above, but now the size and alignment requirements may be
  // different.

  int grow_and_adjust (size_t size, size_t align, char*& buf);
  // Grow the CDR stream. When it returns <buf> contains a pointer to
  // memory in the CDR stream, with at least <size> bytes ahead of it
  // and aligned to an <align> boundary. It moved the wr_ptr()
  // to <buf + size>.

  CORBA_Boolean write_1 (const CORBA::Octet* x);
  CORBA_Boolean write_2 (const CORBA::UShort* x);
  CORBA_Boolean write_4 (const CORBA::ULong* x);
  CORBA_Boolean write_8 (const CORBA::ULongLong* x);
  CORBA_Boolean write_16 (const CORBA::LongDouble* x);
  // Several types can be written using the same routines, since TAO
  // tries to use native types with known size for each CORBA type.
  // We could use void* or char* to make the interface more
  // consistent, but using native types let us exploit the strict
  // alignment requirements of CDR streams and implement the
  // operations using asignment.

  CORBA_Boolean write_array (const void* x, size_t size, size_t align,
                             CORBA::ULong length);
  // write an array of <length> elements, each of <size> bytes and
  // the start aligned at a multiple of <align>. The elements are
  // assumed to be packed with the right alignment restrictions.
  // It is mostly designed for buffers of the basic types.
  //
  // This operation uses memcpy(); as explained above it is expected
  // that using assignment is faster that memcpy() for one element,
  // but for several elements memcpy() should be more efficient, it
  // could be interesting to find the break even point and optimize
  // for that case, but that would be too platform dependent.

  int do_byte_swap (void) const;
  // If non-zero then this stream is writing in non-native byte order,
  // this is only meaningful if TAO_ENABLE_SWAP_ON_WRITE is defined.

private:
  ACE_Message_Block start_;
  // The start of the chain of message blocks.

  ACE_Message_Block* current_;
  // The current block in the chain were we are writing.

  TAO_Marshal_Factory *factory_;
  // maintain a factory that can make specialized marshaling objects

  int do_byte_swap_;
  // If not zero swap bytes at writing so the created CDR stream byte
  // order does *not* match the machine byte order.
  // The motivation for such a beast is that in some setting a few
  // (fast) machines can be serving hundreds of slow machines with the
  // opposite byte order, so it makes sense (as a load balancing
  // device) to put the responsability in the writers.
  // THIS IS NOT A STANDARD IN CORBA, USE AT YOUR OWN RISK (btw, isn't
  // that true for everything in ACE/TAO and your OS vendor?)

  int good_bit_;
  // Set to 0 when an error ocurrs.
};

class TAO_Export TAO_InputCDR
{
  //
  // = TITLE
  //   A CDR stream for reading, i.e. for demarshalling.
  //
  // = DESCRIPTION
  //   This class is based on the the CORBA spec for Java (98-02-29),
  //   java class omg.org.CORBA.portable.InputStream.
  //   It diverts in a few ways:
  //     + Operations to retrieve basic types take parameters by
  //     reference.
  //     + Operations taking arrays don't have offsets, because in C++
  //     it is easier to describe an array starting from x+offset.
  //     + Operations return an error status, because exceptions are
  //     not widely available in C++ (yet).
  //
  //   A particularly useful static member function for this buffer is
  //   an interpretive encoding routine, usable as a typecode
  //   interpreter callback.  Ditto for decoding.  These are used to
  //   support all OMG-IDL datatypes, even those not supported
  //   directly by put/get primitives.
public:
  friend class TAO_GIOP;
  // This class is able to "read" an InputCDR from a socket.

  TAO_InputCDR (const char* buf, size_t bufsiz,
                int byte_order = TAO_ENCAP_BYTE_ORDER,
                TAO_Marshal_Factory *f =
                     TAO_Marshal::DEFAULT_MARSHAL_FACTORY);
  // Create an input stream from an arbitrary buffer, care must be
  // exercised wrt alignment, because this contructor will *not* work
  // if the buffer is unproperly aligned.

  TAO_InputCDR (size_t bufsiz,
                int byte_order = TAO_ENCAP_BYTE_ORDER,
                TAO_Marshal_Factory *f =
                     TAO_Marshal::DEFAULT_MARSHAL_FACTORY);
  // Create an empty input stream. The caller is responsible for
  // putting the right data and providing the right alignment.

  TAO_InputCDR (ACE_Message_Block *data,
                int byte_order =
                    TAO_ENCAP_BYTE_ORDER,
                TAO_Marshal_Factory *f =
                    TAO_Marshal::DEFAULT_MARSHAL_FACTORY);
  // Create an input stream from an ACE_Message_Block

  TAO_InputCDR (ACE_Data_Block *data,
                int byte_order =
                    TAO_ENCAP_BYTE_ORDER,
                TAO_Marshal_Factory *f =
                    TAO_Marshal::DEFAULT_MARSHAL_FACTORY);
  // Create an input stream from an ACE_Data_Block

  TAO_InputCDR (const TAO_InputCDR& rhs);
  TAO_InputCDR& operator= (const TAO_InputCDR& rhs);
  // Make a copy of the current stream state, but does not copy the
  // internal buffer; so the same stream can be read multiple times
  // efficiently.

  TAO_InputCDR (const TAO_InputCDR& rhs,
                size_t size,
                CORBA::Long offset);
  // When interpreting indirected TypeCodes it is useful to make a
  // "copy" of the stream starting in the new position.

  TAO_InputCDR (const TAO_InputCDR& rhs,
                size_t size);
  // This creates an encapsulated stream, the first byte must be (per
  // the spec) the byte order of the encapsulation.

  TAO_InputCDR (const TAO_OutputCDR& rhs,
                ACE_Allocator* buffer_allocator = 0,
                ACE_Allocator* data_block_allocator = 0);
  // Create an input CDR from an output CDR.

  ~TAO_InputCDR (void);
  // destructor

  // = We have one method per basic IDL type....
  // They return 0 on failure and 1 on success.
  CORBA_Boolean read_boolean (CORBA::Boolean& x);
  CORBA_Boolean read_char (CORBA::Char& x);
  CORBA_Boolean read_wchar (CORBA::WChar& x);
  CORBA_Boolean read_octet (CORBA::Octet& x);
  CORBA_Boolean read_short (CORBA::Short& x);
  CORBA_Boolean read_ushort (CORBA::UShort& x);
  CORBA_Boolean read_long (CORBA::Long& x);
  CORBA_Boolean read_ulong (CORBA::ULong& x);
  CORBA_Boolean read_longlong (CORBA::LongLong& x);
  CORBA_Boolean read_ulonglong (CORBA::ULongLong& x);
  CORBA_Boolean read_float (CORBA::Float& x);
  CORBA_Boolean read_double (CORBA::Double& x);
  CORBA_Boolean read_longdouble (CORBA::LongDouble& x);
  CORBA_Boolean read_string (CORBA::Char*& x);
  CORBA_Boolean read_wstring (CORBA::WChar*& x);

  // = One method for each basic IDL type...
  // The buffer <x> must be large enough to contain <length>
  // elements.
  // They return 0 on failure and 1 on success.
  CORBA_Boolean read_boolean_array (CORBA::Boolean* x,
                                    CORBA::ULong length);
  CORBA_Boolean read_char_array (CORBA::Char* x,
                                 CORBA::ULong length);
  CORBA_Boolean read_wchar_array (CORBA::WChar* x,
                                  CORBA::ULong length);
  CORBA_Boolean read_octet_array (CORBA::Octet* x,
                                  CORBA::ULong length);
  CORBA_Boolean read_short_array (CORBA::Short* x,
                                  CORBA::ULong length);
  CORBA_Boolean read_ushort_array (CORBA::UShort* x,
                                   CORBA::ULong length);
  CORBA_Boolean read_long_array (CORBA::Long* x,
                                 CORBA::ULong length);
  CORBA_Boolean read_ulong_array (CORBA::ULong* x,
                                  CORBA::ULong length);
  CORBA_Boolean read_longlong_array (CORBA::LongLong* x,
                                     CORBA::ULong length);
  CORBA_Boolean read_ulonglong_array (CORBA::ULongLong* x,
                                      CORBA::ULong length);
  CORBA_Boolean read_float_array (CORBA::Float* x,
                                  CORBA::ULong length);
  CORBA_Boolean read_double_array (CORBA::Double* x,
                                   CORBA::ULong length);
  CORBA_Boolean read_longdouble_array (CORBA::LongDouble* x,
                                       CORBA::ULong length);

  // = TAO specific methods.

  CORBA::TypeCode::traverse_status decode (CORBA::TypeCode_ptr tc,
                                           const void *data,
                                           const void *,
                                           CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Demarshall the contents of the CDR stream into <data> as
  // described by <tc>; returning any errors in <env>.

  // = We have one method per basic IDL type....
  // They return 0 on failure and 1 on success.
  CORBA_Boolean skip_boolean (void);
  CORBA_Boolean skip_char (void);
  CORBA_Boolean skip_wchar (void);
  CORBA_Boolean skip_octet (void);
  CORBA_Boolean skip_short (void);
  CORBA_Boolean skip_ushort (void);
  CORBA_Boolean skip_long (void);
  CORBA_Boolean skip_ulong (void);
  CORBA_Boolean skip_longlong (void);
  CORBA_Boolean skip_ulonglong (void);
  CORBA_Boolean skip_float (void);
  CORBA_Boolean skip_double (void);
  CORBA_Boolean skip_longdouble (void);
  CORBA_Boolean skip_wstring (void);
  CORBA_Boolean skip_string (void);
  // The next field must be a string, this method skips it. It is
  // useful in parsing a TypeCode.
  // Return 0 on failure and 1 on success.

  CORBA_Boolean skip_bytes (size_t n);
  // Skip <n> bytes in the CDR stream.
  // Return 0 on failure and 1 on success.

  CORBA::TypeCode::traverse_status skip (CORBA::TypeCode_ptr tc,
                                         CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Skip the contents of the CDR stream based on information
  // described by <tc>; returning any errors in <env>.

  int good_bit (void) const;
  // returns zero if a problem has been detected.

  const ACE_Message_Block* start (void) const;
  // Return the start of the message block chain for this CDR stream.
  // NOTE: In the current implementation the chain has length 1, but
  // we are planning to change that.

  char* rd_ptr (void);
  // Returns the current position for the rd_ptr....

  size_t length (void) const;
  // Return how many bytes are left in the stream.

private:
  void rd_ptr (size_t offset);
  char* end (void);
  // short cuts for the underlying message block.

  int adjust (size_t size, char*& buf);
  // Returns (in <buf>) the next position in the buffer aligned to
  // <size>, it advances the Message_Block rd_ptr past the data
  // (i.e. <buf> + <size>).
  // Sets the good_bit to 0 and returns a -1 on failure.

  int adjust (size_t size, size_t align, char*& buf);
  // As above, but now the size and alignment requirements may be
  // different.

  CORBA_Boolean read_1 (CORBA::Octet* x);
  CORBA_Boolean read_2 (CORBA::UShort* x);
  CORBA_Boolean read_4 (CORBA::ULong* x);
  CORBA_Boolean read_8 (CORBA::ULongLong* x);
  CORBA_Boolean read_16 (CORBA::LongDouble* x);
  // Several types can be read using the same routines, since TAO
  // tries to use native types with known size for each CORBA type.
  // We could use void* or char* to make the interface more
  // consistent, but using native types let us exploit the strict
  // alignment requirements of CDR streams and implement the
  // operations using asignment.

  CORBA_Boolean read_array (void* x, size_t size, size_t align,
                            CORBA::ULong length);
  // read an array of <length> elements, each of <size> bytes and
  // the start aligned at a multiple of <align>. The elements are
  // assumed to be packed with the right alignment restrictions.
  // It is mostly designed for buffers of the basic types.
  //
  // This operation uses memcpy(); as explained above it is expected
  // that using assignment is faster that memcpy() for one element,
  // but for several elements memcpy() should be more efficient, it
  // could be interesting to find the break even point and optimize
  // for that case, but that would be too platform dependent.

private:
  ACE_Message_Block start_;
  // The start of the chain of message blocks, even though in the
  // current version the chain always has length 1.

  TAO_Marshal_Factory *factory_;
  // Maintain a factory that can make specialized marshaling objects.

  int do_byte_swap_;
  // The CDR stream byte order does not match the one on the machine,
  // swapping is needed while reading.

  int good_bit_;
  // set to 0 when an error occurs.
};

#if defined(__ACE_INLINE__)
# include "tao/CDR.i"
#else
// CDR output operators for primitive types
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::Short x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::UShort x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::Long x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::ULong x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::LongLong x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::ULongLong x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::LongDouble x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::Float x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            CORBA::Double x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            const CORBA::Any &x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            const CORBA::Object *x);
extern TAO_Export CORBA_Boolean operator<< (TAO_OutputCDR& cdr,
                                            const CORBA::TypeCode *x);

// CDR output operator from helper classes
extern TAO_Export CORBA_Boolean operator << (TAO_OutputCDR& cdr,
                                             CORBA::Any::from_boolean x);
extern TAO_Export CORBA_Boolean operator << (TAO_OutputCDR& cdr,
                                             CORBA::Any::from_char x);
extern TAO_Export CORBA_Boolean operator << (TAO_OutputCDR& cdr,
                                             CORBA::Any::from_wchar x);
extern TAO_Export CORBA_Boolean operator << (TAO_OutputCDR& cdr,
                                             CORBA::Any::from_octet x);
extern TAO_Export CORBA_Boolean operator << (TAO_OutputCDR& cdr,
                                             CORBA::Any::from_string x);
extern TAO_Export CORBA_Boolean operator << (TAO_OutputCDR& cdr,
                                             const char* x);

// CDR input operators for primitive types
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::Short &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::UShort &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::Long &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::ULong &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::LongLong &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::ULongLong &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::LongDouble &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::Float &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::Double &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::Any &x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::Object *&x);
extern TAO_Export CORBA_Boolean operator>> (TAO_InputCDR& cdr,
                                            CORBA::TypeCode *&x);

// CDR output operator from helper classes
extern TAO_Export CORBA_Boolean operator >> (TAO_InputCDR& cdr,
                                             CORBA::Any::to_boolean x);
extern TAO_Export CORBA_Boolean operator >> (TAO_InputCDR& cdr,
                                             CORBA::Any::to_char x);
extern TAO_Export CORBA_Boolean operator >> (TAO_InputCDR& cdr,
                                             CORBA::Any::to_wchar x);
extern TAO_Export CORBA_Boolean operator >> (TAO_InputCDR& cdr,
                                             CORBA::Any::to_octet x);
extern TAO_Export CORBA_Boolean operator >> (TAO_InputCDR& cdr,
                                             CORBA::Any::to_string x);
extern TAO_Export CORBA_Boolean operator >> (TAO_InputCDR& cdr,
                                             char*& x);

#endif /* __ACE_INLINE */

#endif /* TAO_CDR_H */
