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

#include "common/CharStream.h"

/**
 * @class ACEXML_StrCharStream StrCharStream.h "common/StrCharStream.h"
 *
 * An implementation of ACEXML_CharStream for reading
 * input from a null-terminated ACEXML_Char string.
 */
class ACEXML_Export ACEXML_StrCharStream : public ACEXML_CharStream
{
public:
  /// Default constructor.
  ACEXML_StrCharStream (void);

  /// Initializing Constructor.
  ACEXML_StrCharStream (const ACEXML_Char *str);

  /// Destructor
  virtual ~ACEXML_StrCharStream (void);

  /// Initializing and reset the StrCharStream with @a str.
  int open (const ACEXML_Char *str);

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
   * return an ACEXML_Char, 0 if EOS is reached, or 1 if succeed.
   */
  virtual int get (ACEXML_Char& ch);

  /**
   * Read the next batch of ACEXML_Char strings
   */
  virtual int read (ACEXML_Char *str,
                    size_t len);

  /**
   * Peek the next ACEXML_Char in the CharStream.  Return the
   * character if succeess, -1 if EOS is reached.
   */
  virtual int peek (void);

private:
  ACEXML_Char *start_;

  ACEXML_Char *ptr_;

  ACEXML_Char *end_;
};

#endif /* _ACEXML_STRCHARSTREAM_H_ */
