// -*- C++ -*-

//=============================================================================
/**
 *  @file    FileCharStream.h
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
  ACEXML_FileCharStream ();

  /// Destructor
  ~ACEXML_FileCharStream () override;

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
  int available () override;

  /**
   * Close this stream and release all resources used by it.
   */
  int close () override;

  /**
   * Read the next ACEXML_Char.  Return -1 if we are not able to
   * return an ACEXML_Char, 0 if succees.
   */
  int get (ACEXML_Char& ch) override;

  /**
   * Read the next batch of ACEXML_Char strings
   */
  int read (ACEXML_Char *str,
                    size_t len) override;

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

  /*
   * Get the character encoding for the file.
   */
  const ACEXML_Char *getEncoding () override;

  /*
   * Get the systemId for the underlying CharStream
   */
  const ACEXML_Char* getSystemId () override;

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
