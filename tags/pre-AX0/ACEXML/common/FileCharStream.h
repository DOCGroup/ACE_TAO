// -*- C++ -*-

//=============================================================================
/**
 *  @file    FileCharStream.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_FILECHARSTREAM_H_
#define _ACEXML_FILECHARSTREAM_H_

#include "common/CharStream.h"
#include "ace/streams.h"

/**
 * @class ACEXML_FileCharStream FileCharStream.h "common/FileCharStream.h"
 *
 * An implementation of ACEXML_CharStream for reading
 * input from a file.
 */
class ACEXML_Export ACEXML_FileCharStream : public ACEXML_CharStream
{
public:
  /// Default constructor.
  ACEXML_FileCharStream (void);

  /// Construct and opening a file.
  ACEXML_FileCharStream (const ACEXML_Char *name);

  /// Destructor
  virtual ~ACEXML_FileCharStream (void);

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
  ACEXML_Char *filename_;

  FILE *infile_;

};

#endif /* _ACEXML_FILECHARSTREAM_H_ */
