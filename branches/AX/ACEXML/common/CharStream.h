// -*- C++ -*-  $Id$

#ifndef _ACEXML_CHARSTREAM_H_
#define _ACEXML_CHARSTREAM_H_

#include "common/XML_Types.h"

class ACEXML_Export ACEXML_CharStream
{
  // ACEXML_InputStream defines the basic opertion the parser
  // could use to retrieve XML charater sequence.  The sequence
  // can be read from a file or a character buffer.
public:

  /**
   * Virtual destructor, must have.
   */
  virtual ~ACEXML_CharStream () = 0;

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
  virtual int read (ACEXML_Char *str,
                    size_t len) = 0;

  /**
   * Peek the next ACEXML_Char in the CharStream.  Return the
   * character if succeess, -1 if EOS is reached.
   */
  virtual int peek (void) = 0;

};

#endif /* _ACEXML_CHARSTREAM_H_ */
