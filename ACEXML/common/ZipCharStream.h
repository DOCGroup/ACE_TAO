// -*- C++ -*-

//=============================================================================
/**
 *  @file    ZipCharStream.h
 *
 *  $Id$
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_ZIPCHARSTREAM_H_
#define _ACEXML_ZIPCHARSTREAM_H_

#ifdef ACEXML_HAS_ZZIPLIB

#include "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/CharStream.h"
#include "ACEXML/common/Encoding.h"
#include "zziplib.h"

// Ugly wart to get aroung a macro version of read defined in zziplib.h. Evil...
#ifdef read
#define ACEXML_save_read read
#undef read
#endif

/**
 * @class ACEXML_ZipCharStream ZipCharStream.h "ACEXML/common/ZipCharStream.h"
 *
 * An implementation of ACEXML_CharStream for reading input from a ZIP archive.
 */
class ACEXML_Export ACEXML_ZipCharStream : public ACEXML_CharStream
{
public:
  /// Default constructor.
  ACEXML_ZipCharStream (void);

  /// Destructor
  virtual ~ACEXML_ZipCharStream (void);

  /// Open a file.
  int open (const ACEXML_Char *name);

  /**
   * Returns the available ACEXML_Char in the buffer.  -1
   * if the object is not initialized properly.
   */
  virtual int available (void);

  /**
   * Close this stream and release all resources used by it.
   */
  virtual int close (void);

  /**
   * Read the next ACEXML_Char.  Return -1 if we are not able to
   * return an ACEXML_Char, 0 if EOF is reached, or 1 if succeed.
   */
  virtual int get (ACEXML_Char& ch);

  /**
   * Read the next batch of ACEXML_Char strings
   */
  virtual int read (ACEXML_Char *str, size_t len);

  /**
   *  Determine the encoding of the file.
   */
  virtual int determine_encoding (void);


  /**
   * Peek the next ACEXML_Char in the CharStream.  Return the
   * character if success, -1 if EOF is reached.
   */
  virtual int peek (void);

  /**
   *  Resets the file pointer to the beginning of the stream.
   */
  virtual void rewind (void);

  /*
   * Get the character encoding for a byte stream or URI.
   */
  virtual const ACEXML_Char *getEncoding (void);

  /*
   * Get the systemId for the underlying CharStream
   */
  virtual const ACEXML_Char* getSystemId (void);

protected:

  /** Read the next character as a normal character. Return -1 if EOF is
   *  reached, else return 0.
   */
  virtual int getchar_i (char& ch);

  /**
   * Peek @c offset bytes into the stream and return the character at @c
   * offset. If EOF is reached, return -1.
   */
  virtual int peekchar_i (off_t offset = 0);

private:

#if defined (ACE_USES_WCHAR)
  /**
   *  Read the next character from the stream taking into account the
   *  encoding of the file.
   */
  int get_i (ACEXML_Char& ch);

  /**
   *  Return the next character from the stream taking into account the
   *  encoding of the file. Subsequent call to get() returns this
   *  character.
   */
  int peek_i (void);

#endif /* ACE_USES_WCHAR */

  ACEXML_Char*  filename_;
  ACEXML_Char*  encoding_;
  off_t         size_;
  ZZIP_FILE*    infile_;
  char          buf_[80];
  int           pos_;
  int           limit_;
};

#include "ace/post.h"

#ifdef ACEXML_save_read
#define read ACEXML_save_read
#undef ACEXML_save_read
#endif

#endif /* ACEXML_HAS_ZZIPLIB */

#endif /* _ACEXML_ZIPCHARSTREAM_H_ */
