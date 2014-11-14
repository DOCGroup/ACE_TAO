// -*- C++ -*-

//=============================================================================
/**
 *  @file    CharStream.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_CHARSTREAM_H_
#define _ACEXML_CHARSTREAM_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_CharStream
 *
 * ACEXML_CharStream is an abstract class (interface) which defines the
 * basic opertions a parser could use to retrieve XML charater sequence.
 * The sequence can be read from a file or a character buffer.
 */
class ACEXML_Export ACEXML_CharStream
{
public:

  /**
   * Virtual destructor, must have.
   */
  virtual ~ACEXML_CharStream (void) = 0;

  /**
   * Returns the available ACEXML_Char in the buffer.  -1
   * if the object is not initialized properly.
   */
  virtual int available (void) = 0;

  /**
   * Close this stream and release all resources used by it.
   */
  virtual int close (void) = 0;

  /**
   * Read the next ACEXML_Char.  Return -1 if we are not able to
   * return an ACEXML_Char, 0 if EOS is reached, or 1 if succeed.
   */
  virtual int get (ACEXML_Char& ch) = 0;

  /**
   * Read the next batch of ACEXML_Char strings
   */
  virtual int read (ACEXML_Char *str, size_t len) = 0;

  /**
   * Peek the next ACEXML_Char in the CharStream.  Return the
   * character if succeess, -1 if EOS is reached.
   */
  virtual int peek (void) = 0;

  /**
   *  Resets the pointer to the beginning of the stream.
   */
  virtual void rewind (void) = 0;

  /*
   * Get the character encoding for a byte stream or URI.
   */
  virtual const ACEXML_Char *getEncoding (void)  = 0;

  /*
   * Get the systemId for the underlying CharStream
   */
  virtual const ACEXML_Char* getSystemId (void) = 0;

};

#include /**/ "ace/post.h"


#endif /* _ACEXML_CHARSTREAM_H_ */
