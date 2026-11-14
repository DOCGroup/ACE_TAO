// -*- C++ -*-

//=============================================================================
/**
 *  @file    ZipCharStream.h
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_ZIPCHARSTREAM_H_
#define _ACEXML_ZIPCHARSTREAM_H_

#ifdef USE_ZZIP

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/CharStream.h"
#include "ACEXML/common/Encoding.h"
#include "zzip/zzip.h"

#include <memory>

// Ugly wart to get aroung a macro version of read defined in zzip.h. Evil...
#ifdef read
#undef read
#endif

/**
 * @class ACEXML_ZipCharStream
 *
 * An implementation of ACEXML_CharStream for reading input from a ZIP archive.
 */
class ACEXML_Export ACEXML_ZipCharStream : public ACEXML_CharStream
{
public:
  ACEXML_ZipCharStream ();

  ACEXML_ZipCharStream (ACEXML_ZipCharStream const &) = delete;
  ACEXML_ZipCharStream (ACEXML_ZipCharStream &&) = delete;
  ACEXML_ZipCharStream &operator= (ACEXML_ZipCharStream const &) = delete;
  ACEXML_ZipCharStream &operator= (ACEXML_ZipCharStream &&) = delete;

  ~ACEXML_ZipCharStream () override;

  /// Open a file.
  int open (ACEXML_Char const *name);

  /**
   * Returns the available ACEXML_Char in the buffer.  -1
   * if the object is not initialized properly.
   */
  int available () override;

  /**
   * Close this stream and release all resources used by it.
   */
  int close () override;

  /**
   * Read the next ACEXML_Char.  Return -1 if we are not able to
   * return an ACEXML_Char, 0 if EOF is reached, or 1 if succeed.
   */
  int get (ACEXML_Char &ch) override;

  /**
   * Read the next batch of ACEXML_Char strings
   */
  int read (ACEXML_Char *str, size_t len) override;

  /**
   *  Determine the encoding of the file.
   */
  virtual int determine_encoding ();

  /**
   * Peek the next ACEXML_Char in the CharStream.  Return the
   * character if success, -1 if EOF is reached.
   */
  int peek () override;

  /**
   *  Resets the file pointer to the beginning of the stream.
   */
  void rewind () override;

  /// Get the character encoding for a byte stream or URI.
  ACEXML_Char const *getEncoding () override;

  /// Get the systemId for the underlying CharStream.
  ACEXML_Char const *getSystemId () override;

protected:
  /** Read the next character as a normal character. Return -1 if EOF is
   *  reached, else return 0.
   */
  virtual int getchar_i (char &ch);

  /**
   * Peek @c offset bytes into the stream and return the character at @c
   * offset. If EOF is reached, return -1.
   */
  virtual int peekchar_i (ACE_OFF_T offset = 0);

private:
#if defined (ACE_USES_WCHAR)
  /**
   *  Read the next character from the stream taking into account the
   *  encoding of the file.
   */
  int get_i (ACEXML_Char &ch);

  /**
   *  Return the next character from the stream taking into account the
   *  encoding of the file. Subsequent call to get() returns this
   *  character.
   */
  int peek_i ();

#endif /* ACE_USES_WCHAR */

  std::unique_ptr<ACEXML_Char[]> filename_ {};
  std::unique_ptr<ACEXML_Char[]> encoding_ {};
  ACE_OFF_T size_ {};
  ZZIP_FILE *infile_ {};
  char buf_[80] {};
  int pos_ {};
  int limit_ {};
};

#include /**/ "ace/post.h"

#endif /* USE_ZZIP */

#endif /* _ACEXML_ZIPCHARSTREAM_H_ */
