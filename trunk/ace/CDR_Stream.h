// -*- C++ -*-

//=============================================================================
/**
 *  @file   CDR_Stream.h
 *
 *  $Id$
 *
 * ACE Common Data Representation (CDR) marshaling and demarshaling
 * classes.
 *
 * This implementation was inspired in the CDR class in SunSoft's
 * IIOP engine, but has a completely different implementation and a
 * different interface too.
 *
 * The current implementation assumes that the host has 1-byte,
 * 2-byte and 4-byte integral types, and that it has single
 * precision and double precision IEEE floats.
 * Those assumptions are pretty good these days, with Crays beign
 * the only known exception.
 *
 *  @author TAO version by
 *  @author Aniruddha Gokhale <gokhale@cs.wustl.edu>
 *  @author Carlos O'Ryan<coryan@cs.wustl.edu>
 *  @author ACE version by
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 *  @author Istvan Buki <istvan.buki@euronet.be>
 *  @author Codeset translation by
 *  @author Jim Rogers <jrogers@viasoft.com>
 */
//=============================================================================

#ifndef ACE_CDR_STREAM_H
#define ACE_CDR_STREAM_H

#include "ace/pre.h"

#include "ace/CDR_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "ace/Message_Block.h"

class ACE_Char_Codeset_Translator;
class ACE_WChar_Codeset_Translator;

class ACE_InputCDR;

/**
 * @class ACE_OutputCDR
 *
 * @brief A CDR stream for writing, i.e. for marshalling.
 *
 * This class is based on the the CORBA spec for Java (98-02-29),
 * java class omg.org.CORBA.portable.OutputStream.  It diverts in
 * a few ways:
 * + Operations taking arrays don't have offsets, because in C++
 *   it is easier to describe an array starting from x+offset.
 * + Operations return an error status, because exceptions are
 *   not widely available in C++ (yet).
 */
class ACE_Export ACE_OutputCDR
{
public:
  /**
   * The Codeset translators need access to some private members to
   * efficiently marshal arrays
   * For reading from an output CDR stream.
   */
  friend class ACE_Char_Codeset_Translator;
  friend class ACE_WChar_Codeset_Translator;
  friend class ACE_InputCDR;

  /// Default constructor, allocates <size> bytes in the internal
  /// buffer, if <size> == 0 it allocates the default size.
  ACE_OutputCDR (size_t size = 0,
                 int byte_order = ACE_CDR_BYTE_ORDER,
                 ACE_Allocator* buffer_allocator = 0,
                 ACE_Allocator* data_block_allocator = 0,
                 ACE_Allocator* message_block_allocator = 0,
                 size_t memcpy_tradeoff =
                   ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                 ACE_CDR::Octet major_version =
                   ACE_CDR_GIOP_MAJOR_VERSION,
                 ACE_CDR::Octet minor_version =
                   ACE_CDR_GIOP_MINOR_VERSION);

  /// Build a CDR stream with an initial buffer, it will *not* remove
  /// <data>, since it did not allocated it.  It's important to be careful
  /// with the alignment of <data>.
  /**
   * Create an output stream from an arbitrary buffer, care must be
   * exercised with alignment, because this contructor will align if
   * needed.  In this case <data> will not point to the start off the
   * output stream. begin()->rd_prt() points to the start off the
   * output stream.  See ACE_ptr_align_binary() to properly align a
   * pointer and use ACE_CDR::MAX_ALIGNMENT for the correct alignment.
  */
  ACE_OutputCDR (char *data,
                 size_t size,
                 int byte_order = ACE_CDR_BYTE_ORDER,
                 ACE_Allocator* buffer_allocator = 0,
                 ACE_Allocator* data_block_allocator = 0,
                 ACE_Allocator* message_block_allocator = 0,
                 size_t memcpy_tradeoff=
                   ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                 ACE_CDR::Octet giop_major_version =
                   ACE_CDR_GIOP_MAJOR_VERSION,
                 ACE_CDR::Octet giop_minor_version =
                   ACE_CDR_GIOP_MINOR_VERSION);

  /// Build a CDR stream with an initial Message_Block chain, it will
  /// *not* remove <data>, since it did not allocate it.
  ACE_OutputCDR (ACE_Message_Block *data,
                 int byte_order = ACE_CDR_BYTE_ORDER,
                 size_t memcpy_tradeoff=
                   ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                 ACE_CDR::Octet giop_major_version =
                   ACE_CDR_GIOP_MAJOR_VERSION,
                 ACE_CDR::Octet giop_minor_version =
                   ACE_CDR_GIOP_MINOR_VERSION);

  /// destructor
  ~ACE_OutputCDR (void);

  /**
   * Disambiguate overload when inserting booleans, octets, chars, and
   * bounded strings.
   */
  //@{ @name Helper classes

  struct ACE_Export from_boolean
  {
    from_boolean (ACE_CDR::Boolean b);
    ACE_CDR::Boolean val_;
  };

  struct ACE_Export from_octet
  {
    from_octet (ACE_CDR::Octet o);
    ACE_CDR::Octet val_;
  };

  struct ACE_Export from_char
  {
    from_char (ACE_CDR::Char c);
    ACE_CDR::Char val_;
  };

  struct ACE_Export from_wchar
  {
    from_wchar (ACE_CDR::WChar wc);
    ACE_CDR::WChar val_;
  };

  struct ACE_Export from_string
  {
    from_string (ACE_CDR::Char* s,
                 ACE_CDR::ULong b,
                 ACE_CDR::Boolean nocopy = 0);
    from_string (const ACE_CDR::Char* s,
                 ACE_CDR::ULong b,
                 ACE_CDR::Boolean nocopy = 0);
    ACE_CDR::Char *val_;
    ACE_CDR::ULong bound_;
    ACE_CDR::Boolean nocopy_;
  };

  struct ACE_Export from_wstring
  {
    from_wstring (ACE_CDR::WChar* ws,
                  ACE_CDR::ULong b,
                  ACE_CDR::Boolean nocopy = 0);
    from_wstring (const ACE_CDR::WChar* ws,
                  ACE_CDR::ULong b,
                  ACE_CDR::Boolean nocopy = 0);
    ACE_CDR::WChar *val_;
    ACE_CDR::ULong bound_;
    ACE_CDR::Boolean nocopy_;
  };
  //@}

  // Return 0 on failure and 1 on success.
  //@{ @name Write operations
  ACE_CDR::Boolean write_boolean (ACE_CDR::Boolean x);
  ACE_CDR::Boolean write_char (ACE_CDR::Char x);
  ACE_CDR::Boolean write_wchar (ACE_CDR::WChar x);
  ACE_CDR::Boolean write_octet (ACE_CDR::Octet x);
  ACE_CDR::Boolean write_short (ACE_CDR::Short x);
  ACE_CDR::Boolean write_ushort (ACE_CDR::UShort x);
  ACE_CDR::Boolean write_long (ACE_CDR::Long x);
  ACE_CDR::Boolean write_ulong (ACE_CDR::ULong x);
  ACE_CDR::Boolean write_longlong (const ACE_CDR::LongLong &x);
  ACE_CDR::Boolean write_ulonglong (const ACE_CDR::ULongLong &x);
  ACE_CDR::Boolean write_float (ACE_CDR::Float x);
  ACE_CDR::Boolean write_double (const ACE_CDR::Double &x);
  ACE_CDR::Boolean write_longdouble (const ACE_CDR::LongDouble &x);

  /// For string we offer methods that accept a precomputed length.
  ACE_CDR::Boolean write_string (const ACE_CDR::Char *x);
  ACE_CDR::Boolean write_string (ACE_CDR::ULong len,
                                 const ACE_CDR::Char *x);
  ACE_CDR::Boolean write_string (const ACE_CString &x);
  ACE_CDR::Boolean write_wstring (const ACE_CDR::WChar *x);
  ACE_CDR::Boolean write_wstring (ACE_CDR::ULong length,
                                  const ACE_CDR::WChar *x);
  //@}

  /// Note: the portion written starts at <x> and ends
  ///       at <x + length>.
  /// The length is *NOT* stored into the CDR stream.
  //@{ @name Array write operations
  ACE_CDR::Boolean write_boolean_array (const ACE_CDR::Boolean *x,
                                        ACE_CDR::ULong length);
  ACE_CDR::Boolean write_char_array (const ACE_CDR::Char *x,
                                     ACE_CDR::ULong length);
  ACE_CDR::Boolean write_wchar_array (const ACE_CDR::WChar* x,
                                      ACE_CDR::ULong length);
  ACE_CDR::Boolean write_octet_array (const ACE_CDR::Octet* x,
                                      ACE_CDR::ULong length);
  ACE_CDR::Boolean write_short_array (const ACE_CDR::Short *x,
                                      ACE_CDR::ULong length);
  ACE_CDR::Boolean write_ushort_array (const ACE_CDR::UShort *x,
                                       ACE_CDR::ULong length);
  ACE_CDR::Boolean write_long_array (const ACE_CDR::Long *x,
                                     ACE_CDR::ULong length);
  ACE_CDR::Boolean write_ulong_array (const ACE_CDR::ULong *x,
                                      ACE_CDR::ULong length);
  ACE_CDR::Boolean write_longlong_array (const ACE_CDR::LongLong* x,
                                         ACE_CDR::ULong length);
  ACE_CDR::Boolean write_ulonglong_array (const ACE_CDR::ULongLong *x,
                                          ACE_CDR::ULong length);
  ACE_CDR::Boolean write_float_array (const ACE_CDR::Float *x,
                                      ACE_CDR::ULong length);
  ACE_CDR::Boolean write_double_array (const ACE_CDR::Double *x,
                                       ACE_CDR::ULong length);
  ACE_CDR::Boolean write_longdouble_array (const ACE_CDR::LongDouble* x,
                                           ACE_CDR::ULong length);

  /// Write an octet array contained inside a MB, this can be optimized
  /// to minimize copies.
  ACE_CDR::Boolean write_octet_array_mb (const ACE_Message_Block* mb);
  //@}

  /**
   * Return 0 on failure and 1 on success.
   */
  //@{ @name Append contents of own CDR stream to another
  ACE_CDR::Boolean append_boolean (ACE_InputCDR &);
  ACE_CDR::Boolean append_char (ACE_InputCDR &);
  ACE_CDR::Boolean append_wchar (ACE_InputCDR &);
  ACE_CDR::Boolean append_octet (ACE_InputCDR &);
  ACE_CDR::Boolean append_short (ACE_InputCDR &);
  ACE_CDR::Boolean append_ushort (ACE_InputCDR &);
  ACE_CDR::Boolean append_long (ACE_InputCDR &);
  ACE_CDR::Boolean append_ulong (ACE_InputCDR &);
  ACE_CDR::Boolean append_longlong (ACE_InputCDR &);
  ACE_CDR::Boolean append_ulonglong (ACE_InputCDR &);
  ACE_CDR::Boolean append_float (ACE_InputCDR &);
  ACE_CDR::Boolean append_double (ACE_InputCDR &);
  ACE_CDR::Boolean append_longdouble (ACE_InputCDR &);

  ACE_CDR::Boolean append_wstring (ACE_InputCDR &);
  ACE_CDR::Boolean append_string (ACE_InputCDR &);
  //@}

  /// Returns 0 if an error has ocurred, the only expected error is to
  /// run out of memory.
  int good_bit (void) const;

  /// Reuse the CDR stream to write on the old buffer.
  void reset (void);

  /// Add the length of each message block in the chain.
  size_t total_length (void) const;

  /**
   * Return the start of the message block chain for this CDR stream.
   * NOTE: The complete CDR stream is represented by a chain of
   * message blocks.
   */
  const ACE_Message_Block *begin (void) const;

  /// Return the last message in the chain that is is use.
  const ACE_Message_Block *end (void) const;

  /// Return the <current_> message block in chain.
  const ACE_Message_Block *current (void) const;

  /**
   * Access the underlying buffer (read only).    NOTE: This
   * method only returns a pointer to the first block in the
   * chain.
   */
  const char *buffer (void) const;

  /**
   * Return the start and size of the internal buffer.  NOTE: This
   * method only returns information about the first block in the
   * chain.
   */
  size_t length (void) const;

  /**
   * Utility function to allow the user more flexibility.
   * Pads the stream up to the nearest <alignment>-byte boundary.
   * Argument MUST be a power of 2.
   * Returns 0 on success and -1 on failure.
   */
  int align_write_ptr (size_t alignment);

  /// Access the codeset translators. They can be nil!
  ACE_Char_Codeset_Translator *char_translator (void) const;
  ACE_WChar_Codeset_Translator *wchar_translator (void) const;

  /**
   * Return alignment of the wr_ptr(), with respect to the start of
   * the CDR stream.  This is not the same as the alignment of
   * current->wr_ptr()!
   */
  size_t current_alignment (void) const;

  /**
   * Returns (in <buf>) the next position in the buffer aligned to
   * <size>, it advances the Message_Block wr_ptr past the data
   * (i.e. <buf> + <size>). If necessary it grows the Message_Block
   * buffer.  Sets the good_bit to 0 and returns a -1 on failure.
   */
  int adjust (size_t size,
              char *&buf);

  /// As above, but now the size and alignment requirements may be
  /// different.
  int adjust (size_t size,
              size_t align,
              char *&buf);

  /// If non-zero then this stream is writing in non-native byte order,
  /// this is only meaningful if ACE_ENABLE_SWAP_ON_WRITE is defined.
  int do_byte_swap (void) const;

  /// For use by a gateway, which creates the output stream for the
  /// reply to the client in its native byte order, but which must
  /// send the reply in the byte order of the target's reply to the
  /// gateway.
  void reset_byte_order (int byte_order);

  /// set GIOP version info
  int set_version (ACE_CDR::Octet major, ACE_CDR::Octet minor);

private:
  /// disallow copying...
  ACE_OutputCDR (const ACE_OutputCDR& rhs);
  ACE_OutputCDR& operator= (const ACE_OutputCDR& rhs);

  ACE_CDR::Boolean write_1 (const ACE_CDR::Octet *x);
  ACE_CDR::Boolean write_2 (const ACE_CDR::UShort *x);
  ACE_CDR::Boolean write_4 (const ACE_CDR::ULong *x);
  ACE_CDR::Boolean write_8 (const ACE_CDR::ULongLong *x);
  ACE_CDR::Boolean write_16 (const ACE_CDR::LongDouble *x);

  /**
   * write an array of <length> elements, each of <size> bytes and the
   * start aligned at a multiple of <align>. The elements are assumed
   * to be packed with the right alignment restrictions.  It is mostly
   * designed for buffers of the basic types.
   *
   * This operation uses <memcpy>; as explained above it is expected
   * that using assignment is faster that <memcpy> for one element,
   * but for several elements <memcpy> should be more efficient, it
   * could be interesting to find the break even point and optimize
   * for that case, but that would be too platform dependent.
   */
  ACE_CDR::Boolean write_array (const void *x,
                                size_t size,
                                size_t align,
                                ACE_CDR::ULong length);

  /**
   * Grow the CDR stream. When it returns <buf> contains a pointer to
   * memory in the CDR stream, with at least <size> bytes ahead of it
   * and aligned to an <align> boundary. It moved the <wr_ptr> to <buf
   * + size>.
   */
  int grow_and_adjust (size_t size,
                       size_t align,
                       char *&buf);

private:
  /// The start of the chain of message blocks.
  ACE_Message_Block start_;

  /// The current block in the chain were we are writing.
  ACE_Message_Block *current_;

  /**
   * Is the current block writable.  When we steal a buffer from the
   * user and just chain it into the message block we are not supposed
   * to write on it, even if it is past the start and end of the
   * buffer.
   */
  int current_is_writable_;

  /**
   * The current alignment as measured from the start of the buffer.
   * Usually this coincides with the alignment of the buffer in
   * memory, but, when we chain another buffer this "quasi invariant"
   * is broken.
   * The current_alignment is used to readjust the buffer following
   * the stolen message block.
   */
  size_t current_alignment_;

  /**
   * If not zero swap bytes at writing so the created CDR stream byte
   * order does *not* match the machine byte order.  The motivation
   * for such a beast is that in some setting a few (fast) machines
   * can be serving hundreds of slow machines with the opposite byte
   * order, so it makes sense (as a load balancing device) to put the
   * responsability in the writers.  THIS IS NOT A STANDARD IN CORBA,
   * USE AT YOUR OWN RISK
   */
  int do_byte_swap_;

  /// Set to 0 when an error ocurrs.
  int good_bit_;

  /// Break-even point for copying.
  size_t memcpy_tradeoff_;

  /// GIOP version information
  ACE_CDR::Octet major_version_;
  ACE_CDR::Octet minor_version_;

protected:
  /// If not nil, invoke for translation of character and string data.
  ACE_Char_Codeset_Translator *char_translator_;
  ACE_WChar_Codeset_Translator *wchar_translator_;
};

// ****************************************************************

/**
 * @class ACE_InputCDR
 *
 * @brief A CDR stream for reading, i.e. for demarshalling.
 *
 * This class is based on the the CORBA spec for Java (98-02-29),
 * java class omg.org.CORBA.portable.InputStream.  It diverts in a
 * few ways:
 * + Operations to retrieve basic types take parameters by
 * reference.
 * + Operations taking arrays don't have offsets, because in C++
 * it is easier to describe an array starting from x+offset.
 * + Operations return an error status, because exceptions are
 * not widely available in C++ (yet).
 */
class ACE_Export ACE_InputCDR
{
public:
  /// The translator need privileged access to efficiently demarshal
  /// arrays and the such
  friend class ACE_Char_Codeset_Translator;
  friend class ACE_WChar_Codeset_Translator;

  /**
   * Create an input stream from an arbitrary buffer.  The buffer must
   * be properly aligned because this contructor will *not* work if
   * the buffer is aligned unproperly.  See ACE_ptr_align_binary() for
   * instructions on how to align a pointer properly and use
   * ACE_CDR::MAX_ALIGNMENT for the correct alignment.
   */
  ACE_InputCDR (const char *buf,
                size_t bufsiz,
                int byte_order = ACE_CDR_BYTE_ORDER,
                ACE_CDR::Octet major_version =
                  ACE_CDR_GIOP_MAJOR_VERSION,
                ACE_CDR::Octet minor_version =
                  ACE_CDR_GIOP_MINOR_VERSION);

  /// Create an empty input stream. The caller is responsible for
  /// putting the right data and providing the right alignment.
  ACE_InputCDR (size_t bufsiz,
                int byte_order = ACE_CDR_BYTE_ORDER,
                ACE_CDR::Octet major_version =
                  ACE_CDR_GIOP_MAJOR_VERSION,
                ACE_CDR::Octet minor_version =
                  ACE_CDR_GIOP_MINOR_VERSION);

  /// Create an input stream from an ACE_Message_Block
  ACE_InputCDR (const ACE_Message_Block *data,
                int byte_order = ACE_CDR_BYTE_ORDER,
                ACE_CDR::Octet major_version =
                  ACE_CDR_GIOP_MAJOR_VERSION,
                ACE_CDR::Octet minor_version =
                  ACE_CDR_GIOP_MINOR_VERSION);

  /// Create an input stream from an ACE_Data_Block. The <flag>
  /// indicates  whether the <data> can be deleted by the CDR stream
  /// or not
  ACE_InputCDR (ACE_Data_Block *data,
                ACE_Message_Block::Message_Flags flag = 0,
                int byte_order = ACE_CDR_BYTE_ORDER,
                ACE_CDR::Octet major_version =
                  ACE_CDR_GIOP_MAJOR_VERSION,
                ACE_CDR::Octet minor_version =
                  ACE_CDR_GIOP_MINOR_VERSION);

  /// Create an input stream from an ACE_Data_Block. It also sets the
  /// read and write pointers at the desired positions. This would be
  /// helpful if the applications desires to create a new CDR stream
  /// from a semi-processed datablock.
  ACE_InputCDR (ACE_Data_Block *data,
                ACE_Message_Block::Message_Flags flag,
                size_t read_pointer_position,
                size_t write_pointer_position,
                int byte_order = ACE_CDR_BYTE_ORDER,
                ACE_CDR::Octet major_version =
                  ACE_CDR_GIOP_MAJOR_VERSION,
                ACE_CDR::Octet minor_version =
                  ACE_CDR_GIOP_MINOR_VERSION);

  /**
   * These make a copy of the current stream state, but do not copy
   * the internal buffer, so the same stream can be read multiple
   * times efficiently.
   */
  ACE_InputCDR (const ACE_InputCDR& rhs);

  ACE_InputCDR& operator= (const ACE_InputCDR& rhs);

  /// When interpreting indirected TypeCodes it is useful to make a
  /// "copy" of the stream starting in the new position.
  ACE_InputCDR (const ACE_InputCDR& rhs,
                size_t size,
                ACE_CDR::Long offset);

  /// This creates an encapsulated stream, the first byte must be (per
  /// the spec) the byte order of the encapsulation.
  ACE_InputCDR (const ACE_InputCDR& rhs,
                size_t size);

  /// Create an input CDR from an output CDR.
  ACE_InputCDR (const ACE_OutputCDR& rhs,
                ACE_Allocator* buffer_allocator = 0,
                ACE_Allocator* data_block_allocator = 0,
                ACE_Allocator* message_block_allocator = 0);

  /// Helper class to transfer the contents from one input CDR to
  /// another without requiring any extra memory allocations, data
  /// copies or too many temporaries.
  struct ACE_Export Transfer_Contents
  {
    Transfer_Contents (ACE_InputCDR &rhs);

    ACE_InputCDR &rhs_;
  };
  /// Transfer the contents from <rhs> to a new CDR
  ACE_InputCDR (Transfer_Contents rhs);

  /// Destructor
  ~ACE_InputCDR (void);

  /// Disambiguate overloading when extracting octets, chars,
  /// booleans, and bounded strings
  //@{ @name Helper classes

  struct ACE_Export to_boolean
  {
    to_boolean (ACE_CDR::Boolean &b);
    ACE_CDR::Boolean &ref_;
  };

  struct ACE_Export to_char
  {
    to_char (ACE_CDR::Char &c);
    ACE_CDR::Char &ref_;
  };

  struct ACE_Export to_wchar
  {
    to_wchar (ACE_CDR::WChar &wc);
    ACE_CDR::WChar &ref_;
  };

  struct ACE_Export to_octet
  {
    to_octet (ACE_CDR::Octet &o);
    ACE_CDR::Octet &ref_;
  };

  struct ACE_Export to_string
  {
    /// The constructor taking a non-const string is
    /// now deprecated (C++ mapping 00-01-02), but we
    /// keep it around for backward compatibility.
    to_string (ACE_CDR::Char *&s,
               ACE_CDR::ULong b);
    to_string (const ACE_CDR::Char *&s,
               ACE_CDR::ULong b);
    const ACE_CDR::Char *&val_;
    ACE_CDR::ULong bound_;
  };

  struct ACE_Export to_wstring
  {
    /// The constructor taking a non-const wstring is
    /// now deprecated (C++ mapping 00-01-02), but we
    /// keep it around for backward compatibility.
    to_wstring (ACE_CDR::WChar *&ws,
                ACE_CDR::ULong b);
    to_wstring (const ACE_CDR::WChar *&ws,
                ACE_CDR::ULong b);
    const ACE_CDR::WChar *&val_;
    ACE_CDR::ULong bound_;
  };
  //@}

  /**
   * Return 0 on failure and 1 on success.
   */
  //@{ @name Read basic IDL types
  ACE_CDR::Boolean read_boolean (ACE_CDR::Boolean& x);
  ACE_CDR::Boolean read_char (ACE_CDR::Char &x);
  ACE_CDR::Boolean read_wchar (ACE_CDR::WChar& x);
  ACE_CDR::Boolean read_octet (ACE_CDR::Octet& x);
  ACE_CDR::Boolean read_short (ACE_CDR::Short &x);
  ACE_CDR::Boolean read_ushort (ACE_CDR::UShort &x);
  ACE_CDR::Boolean read_long (ACE_CDR::Long &x);
  ACE_CDR::Boolean read_ulong (ACE_CDR::ULong &x);
  ACE_CDR::Boolean read_longlong (ACE_CDR::LongLong& x);
  ACE_CDR::Boolean read_ulonglong (ACE_CDR::ULongLong& x);
  ACE_CDR::Boolean read_float (ACE_CDR::Float &x);
  ACE_CDR::Boolean read_double (ACE_CDR::Double &x);
  ACE_CDR::Boolean read_longdouble (ACE_CDR::LongDouble &x);

  ACE_CDR::Boolean read_string (ACE_CDR::Char *&x);
  ACE_CDR::Boolean read_string (ACE_CString &x);
  ACE_CDR::Boolean read_wstring (ACE_CDR::WChar*& x);
  //@}

  /**
   * The buffer <x> must be large enough to contain <length>
   * elements.
   * Return 0 on failure and 1 on success.
   */
  //@{ @name Read basic IDL types arrays
  ACE_CDR::Boolean read_boolean_array (ACE_CDR::Boolean* x,
                                       ACE_CDR::ULong length);
  ACE_CDR::Boolean read_char_array (ACE_CDR::Char *x,
                                    ACE_CDR::ULong length);
  ACE_CDR::Boolean read_wchar_array (ACE_CDR::WChar* x,
                                     ACE_CDR::ULong length);
  ACE_CDR::Boolean read_octet_array (ACE_CDR::Octet* x,
                                     ACE_CDR::ULong length);
  ACE_CDR::Boolean read_short_array (ACE_CDR::Short *x,
                                     ACE_CDR::ULong length);
  ACE_CDR::Boolean read_ushort_array (ACE_CDR::UShort *x,
                                      ACE_CDR::ULong length);
  ACE_CDR::Boolean read_long_array (ACE_CDR::Long *x,
                                    ACE_CDR::ULong length);
  ACE_CDR::Boolean read_ulong_array (ACE_CDR::ULong *x,
                                     ACE_CDR::ULong length);
  ACE_CDR::Boolean read_longlong_array (ACE_CDR::LongLong* x,
                                        ACE_CDR::ULong length);
  ACE_CDR::Boolean read_ulonglong_array (ACE_CDR::ULongLong* x,
                                         ACE_CDR::ULong length);
  ACE_CDR::Boolean read_float_array (ACE_CDR::Float *x,
                                     ACE_CDR::ULong length);
  ACE_CDR::Boolean read_double_array (ACE_CDR::Double *x,
                                      ACE_CDR::ULong length);
  ACE_CDR::Boolean read_longdouble_array (ACE_CDR::LongDouble* x,
                                          ACE_CDR::ULong length);
  //@}

  /**
   * Return 0 on failure and 1 on success.
   */
  //@{ @name Skip elements
  ACE_CDR::Boolean skip_boolean (void);
  ACE_CDR::Boolean skip_char (void);
  ACE_CDR::Boolean skip_wchar (void);
  ACE_CDR::Boolean skip_octet (void);
  ACE_CDR::Boolean skip_short (void);
  ACE_CDR::Boolean skip_ushort (void);
  ACE_CDR::Boolean skip_long (void);
  ACE_CDR::Boolean skip_ulong (void);
  ACE_CDR::Boolean skip_longlong (void);
  ACE_CDR::Boolean skip_ulonglong (void);
  ACE_CDR::Boolean skip_float (void);
  ACE_CDR::Boolean skip_double (void);
  ACE_CDR::Boolean skip_longdouble (void);
  //@}

  /**
   * The next field must be a string, this method skips it. It is
   * useful in parsing a TypeCode.
   * Return 0 on failure and 1 on success.
   */
  ACE_CDR::Boolean skip_wstring (void);
  ACE_CDR::Boolean skip_string (void);

  /// Skip <n> bytes in the CDR stream.
  /// Return 0 on failure and 1 on success.
  ACE_CDR::Boolean skip_bytes (size_t n);

  /// returns zero if a problem has been detected.
  int good_bit (void) const;

  /**
   * Return the start of the message block chain for this CDR stream.
   * NOTE: In the current implementation the chain has length 1, but
   * we are planning to change that.
   */
  const ACE_Message_Block* start (void) const;

  // = The following functions are useful to read the contents of the
  //   CDR stream from a socket or file.

  /**
   * Grow the internal buffer, reset <rd_ptr> to the first byte in the
   * new buffer that is properly aligned, and set <wr_ptr> to <rd_ptr>
   * + newsize
   */
  int grow (size_t newsize);

  /**
   * After reading and partially parsing the contents the user can
   * detect a change in the byte order, this method will let him
   * change it.
   */
  void reset_byte_order (int byte_order);

  /// Re-initialize the CDR stream, copying the contents of the chain
  /// of message_blocks starting from <data>.
  void reset (const ACE_Message_Block *data,
              int byte_order);

  /// Steal the contents from the current CDR.
  ACE_Message_Block *steal_contents (void);

  /// Steal the contents of <cdr> and make a shallow copy into this
  /// stream.
  void steal_from (ACE_InputCDR &cdr);

  /// Exchange data blocks with the caller of this method. The read
  /// and write pointers are also exchanged.
  /// Note: We now do only with the start_ message block.
  void exchange_data_blocks (ACE_InputCDR &cdr);

  /// Copy the data portion from the <cdr> to this cdr and return the
  /// data content (ie. the ACE_Data_Block) from this CDR to the
  /// caller. The caller is responsible for managing the memory of the
  /// returned ACE_Data_Block.
  ACE_Data_Block* clone_from (ACE_InputCDR &cdr);

  /// Re-initialize the CDR stream, forgetting about the old contents
  /// of the stream and allocating a new buffer (from the allocators).
  void reset_contents (void);

  /// Returns the current position for the rd_ptr....
  char* rd_ptr (void);

  /// Returns the current position for the wr_ptr....
  char* wr_ptr (void);

  /// Return how many bytes are left in the stream.
  size_t length (void) const;

  /**
   * Utility function to allow the user more flexibility.
   * Skips up to the nearest <alignment>-byte boundary.
   * Argument MUST be a power of 2.
   * Returns 0 on success and -1 on failure.
   */
  int align_read_ptr (size_t alignment);

  /// If non-zero then this stream is writing in non-native byte order,
  /// this is only meaningful if ACE_ENABLE_SWAP_ON_WRITE is defined.
  int do_byte_swap (void) const;

  /// If <do_byte_swap> returns 0, this returns ACE_CDR_BYTE_ORDER else
  /// it returns !ACE_CDR_BYTE_ORDER.
  int byte_order (void) const;

  /// Access the codeset translators. They can be nil!
  ACE_Char_Codeset_Translator *char_translator (void) const;
  ACE_WChar_Codeset_Translator *wchar_translator (void) const;

  /**
   * Returns (in <buf>) the next position in the buffer aligned to
   * <size>, it advances the Message_Block rd_ptr past the data
   * (i.e. <buf> + <size>).  Sets the good_bit to 0 and returns a -1
   * on failure.
   */
  int adjust (size_t size,
              char *&buf);

  /// As above, but now the size and alignment requirements may be
  /// different.
  int adjust (size_t size,
              size_t align,
              char *&buf);

protected:
  /// The start of the chain of message blocks, even though in the
  /// current version the chain always has length 1.
  ACE_Message_Block start_;

  /// The CDR stream byte order does not match the one on the machine,
  /// swapping is needed while reading.
  int do_byte_swap_;

  /// set to 0 when an error occurs.
  int good_bit_;

  /// If not nil, invoke for translation of character and string data.
  ACE_Char_Codeset_Translator *char_translator_;
  ACE_WChar_Codeset_Translator *wchar_translator_;

private:
  ACE_CDR::Boolean read_1 (ACE_CDR::Octet *x);
  ACE_CDR::Boolean read_2 (ACE_CDR::UShort *x);
  ACE_CDR::Boolean read_4 (ACE_CDR::ULong *x);
  ACE_CDR::Boolean read_8 (ACE_CDR::ULongLong *x);
  ACE_CDR::Boolean read_16 (ACE_CDR::LongDouble *x);

  // Several types can be read using the same routines, since TAO
  // tries to use native types with known size for each CORBA type.
  // We could use void* or char* to make the interface more
  // consistent, but using native types let us exploit the strict
  // alignment requirements of CDR streams and implement the
  // operations using asignment.

  /**
   * Read an array of <length> elements, each of <size> bytes and the
   * start aligned at a multiple of <align>. The elements are assumed
   * to be packed with the right alignment restrictions.  It is mostly
   * designed for buffers of the basic types.
   *
   * This operation uses <memcpy>; as explained above it is expected
   * that using assignment is faster that <memcpy> for one element,
   * but for several elements <memcpy> should be more efficient, it
   * could be interesting to find the break even point and optimize
   * for that case, but that would be too platform dependent.
   */
  ACE_CDR::Boolean read_array (void* x,
                               size_t size,
                               size_t align,
                               ACE_CDR::ULong length);

  /// Move the rd_ptr ahead by <offset> bytes.
  void rd_ptr (size_t offset);

  /// Points to the continuation field of the current message block.
  char* end (void);

  ACE_CDR::Octet major_version_;
  ACE_CDR::Octet minor_version_;
};

// ****************************************************************

/**
 * @class ACE_Char_Codeset_Translator
 *
 * @brief Codeset translation routines common to both Output and Input
 * CDR streams.
 *
 * This class is a base class for defining codeset translation
 * routines to handle the character set translations required by
 * both CDR Input streams and CDR Output streams.
 */
class ACE_Export ACE_Char_Codeset_Translator
{
public:
  /// Read a single character from the stream, converting from the
  /// stream codeset to the native codeset
  virtual ACE_CDR::Boolean read_char (ACE_InputCDR&,
                                      ACE_CDR::Char&) = 0;

  /// Read a string from the stream, including the length, converting
  /// the characters from the stream codeset to the native codeset
  virtual ACE_CDR::Boolean read_string (ACE_InputCDR&,
                                        ACE_CDR::Char *&) = 0;

  /// Read an array of characters from the stream, converting the
  /// characters from the stream codeset to the native codeset.
  virtual ACE_CDR::Boolean read_char_array (ACE_InputCDR&,
                                            const ACE_CDR::Char*,
                                            ACE_CDR::ULong) = 0;

  /// Write a single character to the stream, converting from the
  /// native codeset to the stream codeset
  virtual ACE_CDR::Boolean write_char (ACE_OutputCDR&,
                                       ACE_CDR::Char) = 0;

  /// Write a string to the stream, including the length, converting
  /// from the native codeset to the stream codeset
  virtual ACE_CDR::Boolean write_string (ACE_OutputCDR&,
                                         ACE_CDR::ULong,
                                         const ACE_CDR::Char*) = 0;

  /// Write an array of characters to the stream, converting from the
  /// native codeset to the stream codeset
  virtual ACE_CDR::Boolean write_char_array (ACE_OutputCDR&,
                                             const ACE_CDR::Char*,
                                             ACE_CDR::ULong) = 0;

protected:
  /// Children have access to low-level routines because they cannot
  /// use read_char or something similar (it would recurse).
  ACE_CDR::Boolean read_1 (ACE_InputCDR& input,
                           ACE_CDR::Octet *x);
  ACE_CDR::Boolean write_1 (ACE_OutputCDR& output,
                            const ACE_CDR::Octet *x);

  /// Efficiently read <length> elements of size <size> each from
  /// <input> into <x>; the data must be aligned to <align>.
  ACE_CDR::Boolean read_array (ACE_InputCDR& input,
                               void* x,
                               size_t size,
                               size_t align,
                               ACE_CDR::ULong length);

  /**
   * Efficiently write <length> elements of size <size> from <x> into
   * <output>. Before inserting the elements enough padding is added
   * to ensure that the elements will be aligned to <align> in the
   * stream.
   */
  ACE_CDR::Boolean write_array (ACE_OutputCDR& output,
                                const void *x,
                                size_t size,
                                size_t align,
                                ACE_CDR::ULong length);

  /**
   * Exposes the stream implementation of <adjust>, this is useful in
   * many cases to minimize memory allocations during marshaling.
   * On success <buf> will contain a contiguous area in the CDR stream
   * that can hold <size> bytes aligned to <align>.
   * Results
   */
  int adjust (ACE_OutputCDR& out,
              size_t size,
              size_t align,
              char *&buf);

  /// Used by derived classes to set errors in the CDR stream.
  void good_bit (ACE_OutputCDR& out, int bit);
};

// ****************************************************************

/**
 * @class ACE_WChar_Codeset_Translator
 *
 * @brief Codeset translation routines common to both Output and Input
 * CDR streams.
 *
 * This class is a base class for defining codeset translation
 * routines to handle the character set translations required by
 * both CDR Input streams and CDR Output streams.
 */
class ACE_Export ACE_WChar_Codeset_Translator
{
public:
  virtual ACE_CDR::Boolean read_wchar (ACE_InputCDR&,
                                       ACE_CDR::WChar&) = 0;
  virtual ACE_CDR::Boolean read_wstring (ACE_InputCDR&,
                                         ACE_CDR::WChar *&) = 0;
  virtual ACE_CDR::Boolean read_wchar_array (ACE_InputCDR&,
                                             const ACE_CDR::WChar*,
                                             ACE_CDR::ULong) = 0;
  virtual ACE_CDR::Boolean write_wchar (ACE_OutputCDR&,
                                        ACE_CDR::WChar) = 0;
  virtual ACE_CDR::Boolean write_wstring (ACE_OutputCDR&,
                                          ACE_CDR::ULong,
                                          const ACE_CDR::WChar*) = 0;
  virtual ACE_CDR::Boolean write_wchar_array (ACE_OutputCDR&,
                                              const ACE_CDR::WChar*,
                                              ACE_CDR::ULong) = 0;

protected:
  /// Children have access to low-level routines because they cannot
  /// use read_char or something similar (it would recurse).
  ACE_CDR::Boolean read_1 (ACE_InputCDR& input,
                           ACE_CDR::Octet *x);
  ACE_CDR::Boolean read_2 (ACE_InputCDR& input,
                           ACE_CDR::UShort *x);
  ACE_CDR::Boolean read_4 (ACE_InputCDR& input,
                           ACE_CDR::ULong *x);
  ACE_CDR::Boolean write_1 (ACE_OutputCDR& output,
                            const ACE_CDR::Octet *x);
  ACE_CDR::Boolean write_2 (ACE_OutputCDR& output,
                            const ACE_CDR::UShort *x);
  ACE_CDR::Boolean write_4 (ACE_OutputCDR& output,
                            const ACE_CDR::ULong *x);

  /// Efficiently read <length> elements of size <size> each from
  /// <input> into <x>; the data must be aligned to <align>.
  ACE_CDR::Boolean read_array (ACE_InputCDR& input,
                               void* x,
                               size_t size,
                               size_t align,
                               ACE_CDR::ULong length);

  /**
   * Efficiently write <length> elements of size <size> from <x> into
   * <output>. Before inserting the elements enough padding is added
   * to ensure that the elements will be aligned to <align> in the
   * stream.
   */
  ACE_CDR::Boolean write_array (ACE_OutputCDR& output,
                                const void *x,
                                size_t size,
                                size_t align,
                                ACE_CDR::ULong length);

  /**
   * Exposes the stream implementation of <adjust>, this is useful in
   * many cases to minimize memory allocations during marshaling.
   * On success <buf> will contain a contiguous area in the CDR stream
   * that can hold <size> bytes aligned to <align>.
   * Results
   */
  int adjust (ACE_OutputCDR& out,
              size_t size,
              size_t align,
              char *&buf);

  /// Used by derived classes to set errors in the CDR stream.
  void good_bit (ACE_OutputCDR& out, int bit);
};

// @@ These operators should not be inlined since they force SString.h
//    to be included in this header.
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               const ACE_CString &x);

extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CString &x);


#if defined (__ACE_INLINE__)
# include "ace/CDR_Stream.i"
#else /* __ACE_INLINE__ */

// Not used by CORBA or TAO
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::Char x);
// CDR output operators for primitive types

extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::Short x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::UShort x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::Long x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::ULong x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::LongLong x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::ULongLong x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR& os,
                                               ACE_CDR::LongDouble x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::Float x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_CDR::Double x);

// CDR output operator from helper classes

extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_OutputCDR::from_boolean x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_OutputCDR::from_char x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_OutputCDR::from_wchar x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_OutputCDR::from_octet x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_OutputCDR::from_string x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               ACE_OutputCDR::from_wstring x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               const ACE_CDR::Char* x);
extern ACE_Export ACE_CDR::Boolean operator<< (ACE_OutputCDR &os,
                                               const ACE_CDR::WChar* x);

// Not used by CORBA or TAO
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::Char &x);
// CDR input operators for primitive types

extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::Short &x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::UShort &x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::Long &x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::ULong &x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::LongLong &x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::ULongLong &x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::LongDouble &x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::Float &x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::Double &x);

// CDR input operator from helper classes

extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_InputCDR::to_boolean x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_InputCDR::to_char x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_InputCDR::to_wchar x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_InputCDR::to_octet x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_InputCDR::to_string x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_InputCDR::to_wstring x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::Char*& x);
extern ACE_Export ACE_CDR::Boolean operator>> (ACE_InputCDR &is,
                                               ACE_CDR::WChar*& x);

#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* ACE_CDR_STREAM_H */
