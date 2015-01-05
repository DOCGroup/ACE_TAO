// -*- C++ -*-

//=============================================================================
/**
 *  @file    StrCharStream.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_STRCHARSTREAM_H_
#define _ACEXML_STRCHARSTREAM_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/CharStream.h"

/**
 * @class ACEXML_StrCharStream
 *
 * An implementation of ACEXML_CharStream for reading
 * input from a null-terminated ACEXML_Char string.
 */
class ACEXML_Export ACEXML_StrCharStream : public ACEXML_CharStream
{
public:
  /// Default constructor.
  ACEXML_StrCharStream (void);

  /// Destructor
  virtual ~ACEXML_StrCharStream (void);

  /// Initializing StrCharStream with @a str and @a name
  int open (const ACEXML_Char *str, const ACEXML_Char* name);

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
   *  Determine the encoding of the file.
   */
  virtual int determine_encoding (void);

  /**
   * Read the next ACEXML_Char.  Return -1 if we are not able to
   * return an ACEXML_Char, 0 if EOS is reached, or 1 if succeed.
   */
  virtual int get (ACEXML_Char& ch);

  /**
   * Read the next batch of ACEXML_Char strings
   */
  virtual int read (ACEXML_Char *str, size_t len);

  /**
   * Peek the next ACEXML_Char in the CharStream.  Return the
   * character if succeess, -1 if EOS is reached.
   */
  virtual int peek (void);

  /*
   * Get the character encoding for a byte stream or URI.
   */
  virtual const ACEXML_Char *getEncoding (void);

  /*
   * Get the systemId for the underlying CharStream
   */
  virtual const ACEXML_Char* getSystemId (void);

  /**
   *  Resets the pointer to the beginning of the stream.
   */
  virtual void rewind (void);

private:
  ACEXML_Char *start_;
  ACEXML_Char *ptr_;
  ACEXML_Char *end_;
  ACEXML_Char* encoding_;
  ACEXML_Char* name_;

};

#include /**/ "ace/post.h"

#endif /* _ACEXML_STRCHARSTREAM_H_ */
