// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
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
// = AUTHORS
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//     Many enhancements added by Aniruddha Gokhale and Carlos O'Ryan.
//     ACE version by Jeff Parsons and Istvan Buki
//
// ============================================================================


#if !defined(ACE_CDR_H)
#define ACE_CDR_H

#include "ace/Message_Block.h"

class ACE_Export ACE_CDR
{
  // = TITLE
  //   Keep constants and some routines common to both Output and
  //   Input CDR streams.
  //
public:
  // = Constants defined by the CDR protocol.

  // By defining as many of these constants as possible as enums we ensure 
  // they get inlined and avoid pointless static memory allocations.

  enum
  {
    // Note that some of these get reused as part of the standard
    // binary format: unsigned is the same size as its signed cousin,
    // float is LONG_SIZE, and double is LONGLONG_SIZE.

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
    // Note how the CORBA CDR_LongDouble alignment requirements do not
    // match its size...

    MAX_ALIGNMENT = 8,
    // Maximal CDR 1.1 alignment: "quad precision" FP (i.e. "CDR_Long
    // double", size as above).

    DEFAULT_BUFSIZE = DEFAULT_CDR_BUFSIZE,
    // The default buffer size.
    // @@ TODO We want to add options to control this
    // default value, so this constant should be read as the default
    // default value ;-)

    EXP_GROWTH_MAX = DEFAULT_CDR_EXP_GROWTH_MAX,
    // The buffer size grows exponentially until it reaches this size;
    // afterwards it grows linearly using the next constant

    LINEAR_GROWTH_CHUNK = DEFAULT_CDR_LINEAR_GROWTH_CHUNK
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

  static void mb_align (ACE_Message_Block *mb);

  static int grow (ACE_Message_Block *mb, size_t minsize);
  // Increase the capacity of mb to contain at least <minsize> bytes.
  // If <minsize> is zero the size is increased by an amount at least
  // large enough to contain any of the basic IDL types.  Return -1 on
  // failure, 0 on success.
};


class ACE_Export ACE_OutputCDR
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
  friend class ACE_InputCDR;
  // For reading from an output CDR stream.

  ACE_OutputCDR (size_t size = 0, 
		 int byte_order = STREAM_BYTE_ORDER,
                 ACE_Allocator* buffer_allocator = 0,
                 ACE_Allocator* data_block_allocator = 0,
                 size_t memcpy_tradeoff =
                    DEFAULT_CDR_MEMCPY_TRADEOFF);
  // Default constructor, allocates <size> bytes in the internal
  // buffer, if <size> == 0 it allocates the default size.

  ACE_OutputCDR (char *data, 
                 size_t size,
                 int byte_order = STREAM_BYTE_ORDER,
                 ACE_Allocator* buffer_allocator = 0,
                 ACE_Allocator* data_block_allocator = 0,
                 size_t memcpy_tradeoff =
                    DEFAULT_CDR_MEMCPY_TRADEOFF);
  // Build a CDR stream with an initial buffer, it will *not* remove
  // <data>, since it did not allocated it.

  ACE_OutputCDR (ACE_Message_Block *data,
                 int byte_order = STREAM_BYTE_ORDER,
                 size_t memcpy_tradeoff =
                    DEFAULT_CDR_MEMCPY_TRADEOFF);
  // Build a CDR stream with an initial Message_Block chain, it will *not*
  // remove <data>, since it did not allocate it.

  ~ACE_OutputCDR (void);
  // destructor

  // = Special types.
  // These are needed for insertion and extraction of booleans,
  // octets, chars, and bounded strings.

  struct ACE_Export from_boolean
  {
    from_boolean (CDR_Boolean b);
    CDR_Boolean val_;
  };

  struct ACE_Export from_octet
  {
    from_octet (CDR_Octet o);
    CDR_Octet val_;
  };

  struct ACE_Export from_char
  {
    from_char (CDR_Char c);
    CDR_Char val_;
  };

  struct ACE_Export from_wchar
  {
    from_wchar (ACE_OS::WChar wc);
    ACE_OS::WChar val_;
  };

  struct ACE_Export from_string
  {
    from_string (CDR_Char* s,
                 CDR_ULong b,
                 CDR_Boolean nocopy = 0);
    CDR_Char *val_;
    CDR_ULong bound_;
    CDR_Boolean nocopy_;
  };

  // = We have one method per basic IDL type....
  // They return 0 on failure and 1 on success.
  CDR_Boolean write_boolean (CDR_Boolean x);
  CDR_Boolean write_char (CDR_Char x);
  CDR_Boolean write_wchar (ACE_OS::WChar x);
  CDR_Boolean write_octet (CDR_Octet x);
  CDR_Boolean write_short (CDR_Short x);
  CDR_Boolean write_ushort (CDR_UShort x);
  CDR_Boolean write_long (CDR_Long x);
  CDR_Boolean write_ulong (CDR_ULong x);
  CDR_Boolean write_longlong (const CDR_LongLong &x);
  CDR_Boolean write_ulonglong (const CDR_ULongLong &x);
  CDR_Boolean write_float (CDR_Float x);
  CDR_Boolean write_double (const CDR_Double &x);
  CDR_Boolean write_longdouble (const CDR_LongDouble &x);

  // For string we offer methods that accept a precomputed length.
  CDR_Boolean write_string (const CDR_Char *x);
  CDR_Boolean write_string (CDR_ULong len, const CDR_Char *x);
  CDR_Boolean write_string (const ACE_CString &x);
  CDR_Boolean write_wstring (const ACE_OS::WChar *x);
  CDR_Boolean write_wstring (CDR_ULong length, 
                             const ACE_OS::WChar *x);

  // = We add one method to write arrays of basic IDL types.
  // Note: the portion written starts at <x> and ends at <x + length>.
  // The length is *NOT* stored into the CDR stream.
  CDR_Boolean write_boolean_array (CDR_Boolean *x,
                                   CDR_ULong length);
  CDR_Boolean write_char_array (const CDR_Char *x, 
                                CDR_ULong length);
  CDR_Boolean write_wchar_array (const ACE_OS::WChar* x,
                                 CDR_ULong length);
  CDR_Boolean write_octet_array (const CDR_Octet* x,
                                 CDR_ULong length);
  CDR_Boolean write_short_array (const CDR_Short *x, 
                                 CDR_ULong length);
  CDR_Boolean write_ushort_array (const CDR_UShort *x, 
                                  CDR_ULong length);
  CDR_Boolean write_long_array (const CDR_Long *x, 
                                CDR_ULong length);
  CDR_Boolean write_ulong_array (const CDR_ULong *x, 
                                 CDR_ULong length);
  CDR_Boolean write_longlong_array (const CDR_LongLong* x,
                                    CDR_ULong length);
  CDR_Boolean write_ulonglong_array (const CDR_ULongLong *x,
                                     CDR_ULong length);
  CDR_Boolean write_float_array (const CDR_Float *x, 
                                 CDR_ULong length);
  CDR_Boolean write_double_array (const CDR_Double *x,
                                  CDR_ULong length);
  CDR_Boolean write_longdouble_array (const CDR_LongDouble* x,
                                      CDR_ULong length);

  CDR_Boolean write_octet_array_mb (const ACE_Message_Block* mb);
  // Write an octet array contained inside a MB, this can be optimized
  // to minimize copies.

  // = We have one method per basic IDL type....
  // They return 0 on failure and 1 on success.
  CDR_Boolean append_boolean (ACE_InputCDR &);
  CDR_Boolean append_char (ACE_InputCDR &);
  CDR_Boolean append_wchar (ACE_InputCDR &);
  CDR_Boolean append_octet (ACE_InputCDR &);
  CDR_Boolean append_short (ACE_InputCDR &);
  CDR_Boolean append_ushort (ACE_InputCDR &);
  CDR_Boolean append_long (ACE_InputCDR &);
  CDR_Boolean append_ulong (ACE_InputCDR &);
  CDR_Boolean append_longlong (ACE_InputCDR &);
  CDR_Boolean append_ulonglong (ACE_InputCDR &);
  CDR_Boolean append_float (ACE_InputCDR &);
  CDR_Boolean append_double (ACE_InputCDR &);
  CDR_Boolean append_longdouble (ACE_InputCDR &);

  CDR_Boolean append_wstring (ACE_InputCDR &);
  CDR_Boolean append_string (ACE_InputCDR &);

  int good_bit (void) const;
  // Returns 0 if an error has ocurred, the only expected error is to
  // run out of memory.

  void reset (void);
  // Reuse the CDR stream to write on the old buffer.

  size_t total_length (void) const;
  // Add the length of each message block in the chain.

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

private:
  ACE_OutputCDR (const ACE_OutputCDR& rhs);
  ACE_OutputCDR& operator= (const ACE_OutputCDR& rhs);
  // disallow copying...

  CDR_Boolean write_1 (const CDR_Octet *x);
  CDR_Boolean write_2 (const CDR_UShort *x);
  CDR_Boolean write_4 (const CDR_ULong *x);
  CDR_Boolean write_8 (const CDR_ULongLong *x);
  CDR_Boolean write_16 (const CDR_LongDouble *x);

  CDR_Boolean write_array (const void* x, 
                           size_t size, 
                           size_t align,
		           CDR_ULong length);
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

  int adjust (size_t size, 
              char*& buf);
  // Returns (in <buf>) the next position in the buffer aligned to
  // <size>, it advances the Message_Block wr_ptr past the data
  // (i.e. <buf> + <size>). If necessary it grows the Message_Block
  // buffer.
  // Sets the good_bit to 0 and returns a -1 on failure.

  int adjust (size_t size, 
              size_t align, 
              char*& buf);
  // As above, but now the size and alignment requirements may be
  // different.
  
  int grow_and_adjust (size_t size, 
                       size_t align, 
                       char*& buf);
  // Grow the CDR stream. When it returns <buf> contains a pointer to
  // memory in the CDR stream, with at least <size> bytes ahead of it
  // and aligned to an <align> boundary. It moved the wr_ptr()
  // to <buf + size>.

  int do_byte_swap (void) const;
  // If non-zero then this stream is writing in non-native byte order,
  // this is only meaningful if ACE_ENABLE_SWAP_ON_WRITE is defined.

private:
  ACE_Message_Block start_;
  // The start of the chain of message blocks.

  ACE_Message_Block* current_;
  // The current block in the chain were we are writing.

  ACE_Allocator* buffer_allocator_;
  ACE_Allocator* data_block_allocator_;
  // Allocators for stream growth.

  int do_byte_swap_;
  // If not zero swap bytes at writing so the created CDR stream byte
  // order does *not* match the machine byte order.
  // The motivation for such a beast is that in some setting a few
  // (fast) machines can be serving hundreds of slow machines with the
  // opposite byte order, so it makes sense (as a load balancing
  // device) to put the responsability in the writers.
  // THIS IS NOT A STANDARD IN CORBA, USE AT YOUR OWN RISK

  int good_bit_;
  // Set to 0 when an error ocurrs.

  size_t memcpy_tradeoff_;
  // Break-even point for copying.
};

class ACE_Export ACE_InputCDR
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
  ACE_InputCDR (const char* buf, 
                size_t bufsiz,
                int byte_order = STREAM_BYTE_ORDER);
  // Create an input stream from an arbitrary buffer, care must be
  // exercised wrt alignment, because this contructor will *not* work
  // if the buffer is unproperly aligned.

  ACE_InputCDR (size_t bufsiz,
                int byte_order = STREAM_BYTE_ORDER);
  // Create an empty input stream. The caller is responsible for
  // putting the right data and providing the right alignment.

  ACE_InputCDR (ACE_Message_Block *data,
                int byte_order =
                    STREAM_BYTE_ORDER);
  // Create an input stream from an ACE_Message_Block

  ACE_InputCDR (ACE_Data_Block *data,
                int byte_order =
                    STREAM_BYTE_ORDER);
  // Create an input stream from an ACE_Data_Block

  ACE_InputCDR (const ACE_InputCDR& rhs);
  ACE_InputCDR& operator= (const ACE_InputCDR& rhs);
  // Make a copy of the current stream state, but does not copy the
  // internal buffer; so the same stream can be read multiple times
  // efficiently.

  ACE_InputCDR (const ACE_InputCDR& rhs,
                size_t size,
                CDR_Long offset);
  // When interpreting indirected TypeCodes it is useful to make a
  // "copy" of the stream starting in the new position.

  ACE_InputCDR (const ACE_InputCDR& rhs,
                size_t size);
  // This creates an encapsulated stream, the first byte must be (per
  // the spec) the byte order of the encapsulation.

  ACE_InputCDR (const ACE_OutputCDR& rhs,
                ACE_Allocator* buffer_allocator = 0,
                ACE_Allocator* data_block_allocator = 0);
  // Create an input CDR from an output CDR.

  ~ACE_InputCDR (void);
  // Destructor

  // = Special types.
  // These extract octets, chars, booleans, and bounded strings

  struct ACE_Export to_boolean
  {
    to_boolean (CDR_Boolean &b);
    CDR_Boolean &ref_;
  };

  struct ACE_Export to_char
  {
    to_char (CDR_Char &c);
    CDR_Char &ref_;
  };

  struct ACE_Export to_wchar
  {
    to_wchar (ACE_OS::WChar &wc);
    ACE_OS::WChar &ref_;
  };

  struct ACE_Export to_octet
  {
    to_octet (CDR_Octet &o);
    CDR_Octet &ref_;
  };

  struct ACE_Export to_string
  {
    to_string (CDR_Char *&s, CDR_ULong b);
    CDR_Char *&val_;
    CDR_ULong bound_;
  };

  // = We have one method per basic IDL type....
  // They return 0 on failure and 1 on success.
  CDR_Boolean read_boolean (CDR_Boolean& x);
  CDR_Boolean read_char (CDR_Char &x);
  CDR_Boolean read_wchar (ACE_OS::WChar& x);
  CDR_Boolean read_octet (CDR_Octet& x);
  CDR_Boolean read_short (CDR_Short &x);
  CDR_Boolean read_ushort (CDR_UShort &x);
  CDR_Boolean read_long (CDR_Long &x);
  CDR_Boolean read_ulong (CDR_ULong &x);
  CDR_Boolean read_longlong (CDR_LongLong& x);
  CDR_Boolean read_ulonglong (CDR_ULongLong& x);
  CDR_Boolean read_float (CDR_Float &x);
  CDR_Boolean read_double (CDR_Double &x);
  CDR_Boolean read_longdouble (CDR_LongDouble &x);

  CDR_Boolean read_string (CDR_Char *&x);
  CDR_Boolean read_string (ACE_CString &x);
  CDR_Boolean read_wstring (ACE_OS::WChar*& x);

  // = One method for each basic type...
  // The buffer <x> must be large enough to contain <length>
  // elements.
  // They return -1 on failure and 0 on success.
  CDR_Boolean read_boolean_array (CDR_Boolean* x,
                                  CDR_ULong length);
  CDR_Boolean read_char_array (CDR_Char *x, 
                               CDR_ULong length);
  CDR_Boolean read_wchar_array (ACE_OS::WChar* x,
                                CDR_ULong length);
  CDR_Boolean read_octet_array (CDR_Octet* x,
                                CDR_ULong length);
  CDR_Boolean read_short_array (CDR_Short *x, 
                                CDR_ULong length);
  CDR_Boolean read_ushort_array (CDR_UShort *x, 
                                 CDR_ULong length);
  CDR_Boolean read_long_array (CDR_Long *x, 
                               CDR_ULong length);
  CDR_Boolean read_ulong_array (CDR_ULong *x, 
                                CDR_ULong length);
  CDR_Boolean read_longlong_array (CDR_LongLong* x,
                                   CDR_ULong length);
  CDR_Boolean read_ulonglong_array (CDR_ULongLong* x,
                                    CDR_ULong length);
  CDR_Boolean read_float_array (CDR_Float *x, 
                                CDR_ULong length);
  CDR_Boolean read_double_array (CDR_Double *x, 
                                 CDR_ULong length);
  CDR_Boolean read_longdouble_array (CDR_LongDouble* x,
                                     CDR_ULong length);

  // = We have one method per basic IDL type....
  // They return 0 on failure and 1 on success.
  CDR_Boolean skip_boolean (void);
  CDR_Boolean skip_char (void);
  CDR_Boolean skip_wchar (void);
  CDR_Boolean skip_octet (void);
  CDR_Boolean skip_short (void);
  CDR_Boolean skip_ushort (void);
  CDR_Boolean skip_long (void);
  CDR_Boolean skip_ulong (void);
  CDR_Boolean skip_longlong (void);
  CDR_Boolean skip_ulonglong (void);
  CDR_Boolean skip_float (void);
  CDR_Boolean skip_double (void);
  CDR_Boolean skip_longdouble (void);

  CDR_Boolean skip_wstring (void);
  CDR_Boolean skip_string (void);
  // The next field must be a string, this method skips it. It is
  // useful in parsing a TypeCode.
  // Return 0 on failure and 1 on success.

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
  CDR_Boolean read_1 (CDR_Octet *x);
  CDR_Boolean read_2 (CDR_UShort *x);
  CDR_Boolean read_4 (CDR_ULong *x);
  CDR_Boolean read_8 (CDR_ULongLong *x);
  CDR_Boolean read_16 (CDR_LongDouble *x);

  // Several types can be read using the same routines, since TAO
  // tries to use native types with known size for each CORBA type.
  // We could use void* or char* to make the interface more
  // consistent, but using native types let us exploit the strict
  // alignment requirements of CDR streams and implement the
  // operations using asignment.

  CDR_Boolean read_array (void* x, 
                          size_t size, 
                          size_t align,
		          CDR_ULong length);
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

  void rd_ptr (size_t offset);
  char* end (void);
  // CDR_Short cuts for the underlying message block.

  int adjust (size_t size, 
              char *&buf);
  // Returns (in <buf>) the next position in the buffer aligned to
  // <size>, it advances the Message_Block rd_ptr past the data
  // (i.e. <buf> + <size>).
  // Sets the good_bit to 0 and returns a -1 on failure.

  int adjust (size_t size, 
              size_t align, 
              char *&buf);
  // As above, but now the size and alignment requirements may be
  // different.
  
private:
  ACE_Message_Block start_;
  // The start of the chain of message blocks, even though in the
  // current version the chain always has length 1.

  int do_byte_swap_;
  // The CDR stream byte order does not match the one on the machine,
  // swapping is needed while reading.

  int good_bit_;
  // set to 0 when an error occurs.
};

#if defined(__ACE_INLINE__)
# include "ace/CDR.i"
#else

// CDR output operators for primitive types
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os, 
                                          CDR_Short x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os, 
                                          CDR_UShort x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os, 
                                          CDR_Long x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os, 
                                          CDR_ULong x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os,
                                          CDR_LongLong x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os,
                                          CDR_ULongLong x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR& os,
                                          CDR_LongDouble x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os, 
                                          CDR_Float x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os, 
                                          CDR_Double x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os, 
                                          const ACE_CString &x);

// CDR output operator from helper classes
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os,
                                          ACE_OutputCDR::from_boolean x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os,
                                          ACE_OutputCDR::from_char x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os,
                                          ACE_OutputCDR::from_wchar x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os,
                                          ACE_OutputCDR::from_octet x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os,
                                          ACE_OutputCDR::from_string x);
extern ACE_Export CDR_Boolean operator<< (ACE_OutputCDR &os,
                                          const char* x);
// CDR input operators for primitive types
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is, 
                                          CDR_Short &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is, 
                                          CDR_UShort &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is, 
                                          CDR_Long &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is, 
                                          CDR_ULong &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          CDR_LongLong &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          CDR_ULongLong &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          CDR_LongDouble &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is, 
                                          CDR_Float &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is, 
                                          CDR_Double &x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is, 
                                          ACE_CString &x);

// CDR input operator from helper classes
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          ACE_InputCDR::to_boolean x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          ACE_InputCDR::to_char x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          ACE_InputCDR::to_wchar x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          ACE_InputCDR::to_octet x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          ACE_InputCDR::to_string x);
extern ACE_Export CDR_Boolean operator>> (ACE_InputCDR &is,
                                          char*& x);

#endif /* __ACE_INLINE */

#endif /* ACE_CDR_H */
