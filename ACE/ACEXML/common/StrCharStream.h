// -*- C++ -*-

//=============================================================================
/**
 *  @file    StrCharStream.h
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
  ACEXML_StrCharStream ();

  /// Destructor
  ~ACEXML_StrCharStream () override;

  /// Initializing StrCharStream with @a str and @a name
  int open (const ACEXML_Char *str, const ACEXML_Char* name);

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
   *  Determine the encoding of the file.
   */
  virtual int determine_encoding ();

  /**
   * Read the next ACEXML_Char.  Return -1 if we are not able to
   * return an ACEXML_Char, 0 if EOS is reached, or 1 if succeed.
   */
  int get (ACEXML_Char& ch) override;

  /**
   * Read the next batch of ACEXML_Char strings
   */
  int read (ACEXML_Char *str, size_t len) override;

  /**
   * Peek the next ACEXML_Char in the CharStream.  Return the
   * character if succeess, -1 if EOS is reached.
   */
  int peek () override;

  /*
   * Get the character encoding for a byte stream or URI.
   */
  const ACEXML_Char *getEncoding () override;

  /*
   * Get the systemId for the underlying CharStream
   */
  const ACEXML_Char* getSystemId () override;

  /**
   *  Resets the pointer to the beginning of the stream.
   */
  void rewind () override;

private:
  ACEXML_Char *start_;
  ACEXML_Char *ptr_;
  ACEXML_Char *end_;
  ACEXML_Char* encoding_;
  ACEXML_Char* name_;
};

#include /**/ "ace/post.h"

#endif /* _ACEXML_STRCHARSTREAM_H_ */
