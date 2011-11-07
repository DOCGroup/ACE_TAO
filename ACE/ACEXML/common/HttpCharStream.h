// -*- C++ -*-

//=============================================================================
/**
 *  @file    HttpCharStream.h
 *
 *  $Id$
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_HTTPCHARSTREAM_H_
#define _ACEXML_HTTPCHARSTREAM_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/CharStream.h"
#include "ACEXML/common/URL_Addr.h"
#include "ACEXML/common/Mem_Map_Stream.h"

/**
 * @class ACEXML_HttpCharStream
 *
 * An implementation of ACEXML_CharStream for reading input from a URL.
 */
class ACEXML_Export ACEXML_HttpCharStream : public ACEXML_CharStream
{
public:
  /// Default constructor.
  ACEXML_HttpCharStream (void);

  /// Destructor
  virtual ~ACEXML_HttpCharStream (void);

  /// Open an URL.
  int open (const ACEXML_Char *url);

  /**
   *  Returns the available ACEXML_Char in the buffer.  -1
   *  if the object is not initialized properly.
   */
  virtual int available (void);

  /**
   *  Close this stream and release all resources used by it.
   */
  virtual int close (void);

  /**
   *  Read the next ACEXML_Char.  Return -1 if we are not able to
   *  return an ACEXML_Char, 0 if EOS is reached, or 1 if succeed.
   */
  virtual int get (ACEXML_Char& ch);

  /**
   *  Read the next batch of ACEXML_Char strings
   */
  virtual int read (ACEXML_Char *str, size_t len);

  /**
   *  Peek the next ACEXML_Char in the CharStream.  Return the
   *  character if succeess, -1 if EOS is reached.
   */
  virtual int peek (void);

  /**
   *  Resets the file pointer to the beginning of the stream.
   */
  virtual void rewind (void);

  /**
   *  Determine the encoding of the file.
   */
  virtual int determine_encoding (void);

  /**
   * Get the encoding of the file
   */
  virtual const ACEXML_Char* getEncoding (void);

  /*
   * Get the systemId for the underlying CharStream
   */
  virtual const ACEXML_Char* getSystemId (void);


private:

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

  /**
   *  Send a HTTP/1.1 request to fetch the contents of the URL.
   */
  int send_request (void);

  /**
   *  Fetch the URL and save it in backing store.
   */
  int get_url (size_t& len);

  ACEXML_Char *url_;

  ACEXML_URL_Addr* url_addr_;

  ACEXML_Mem_Map_Stream* stream_;

  Connector* connector_;

  ACE_OFF_T size_;

  ACE_OFF_T data_offset_;

  ACEXML_Char* encoding_;

};


#include /**/ "ace/post.h"

#endif /* _ACEXML_HTTPCHARSTREAM_H_ */
