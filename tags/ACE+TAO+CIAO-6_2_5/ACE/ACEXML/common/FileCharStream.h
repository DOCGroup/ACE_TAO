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

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/CharStream.h"
#include "ACEXML/common/Encoding.h"

/**
 * @class ACEXML_FileCharStream
 *
 * An implementation of ACEXML_CharStream for reading input from a file.
 */
class ACEXML_Export ACEXML_FileCharStream : public ACEXML_CharStream
{
public:
  /// Default constructor.
  ACEXML_FileCharStream (void);

  /// Destructor
  virtual ~ACEXML_FileCharStream (void);

  /// Open a file.
  int open (const ACEXML_Char *name);

  /**
   * Accept an already opened file. The stream does not
   * assume ownership of open_file.
   */
  int use_stream (FILE* open_file, const ACEXML_Char *name);

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
   * return an ACEXML_Char, 0 if succees.
   */
  virtual int get (ACEXML_Char& ch);

  /**
   * Read the next batch of ACEXML_Char strings
   */
  virtual int read (ACEXML_Char *str,
                    size_t len);

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
   * Get the character encoding for the file.
   */
  virtual const ACEXML_Char *getEncoding (void);

  /*
   * Get the systemId for the underlying CharStream
   */
  virtual const ACEXML_Char* getSystemId (void);

private:

  /** Read the next character as a normal character. Return -1 if EOF is
   *  reached, else return 0.
   */
  int getchar_i (char& ch);

#if defined (ACE_USES_WCHAR)
  /**
   *  Read the next character from the stream taking into account the
   *  encoding of the file.
   */
  int get_i (ACEXML_Char& ch);

  /**
   *  Read the next character from the stream taking into account the
   *  encoding of the file. Subsequent call to get() returns this
   *  character.
   */
  int peek_i (void);

#endif /* ACE_USES_WCHAR */

  /// internal accept an already opened file.
  int use_stream_i (FILE* open_file, const ACEXML_Char *name);

  ACEXML_Char*  filename_;
  ACEXML_Char*  encoding_;
  ACE_OFF_T     size_;
  FILE*         infile_;
  bool          close_infile_;
  // This is needed to ensure that we can implement a peek operation on a
  // UTF-16 encoded file. It is a bit hackish, but there is no other way of
  // implementing a peek() as the standard I/O FILE* guarantees only one
  // pushback.
  ACEXML_Char   peek_;
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_FILECHARSTREAM_H_ */
