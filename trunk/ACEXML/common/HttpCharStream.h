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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/CharStream.h"
#include "ACEXML/common/URL_Addr.h"
#include "ACEXML/common/Mem_Map_Stream.h"
#include "ace/streams.h"

/**
 * @class ACEXML_HttpCharStream HttpCharStream.h "ACEXML/common/HttpCharStream.h"
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
  virtual int read (ACEXML_Char *str,
                    size_t len);

  /**
   *  Peek the next ACEXML_Char in the CharStream.  Return the
   *  character if succeess, -1 if EOS is reached.
   */
  virtual int peek (void);


private:

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

  ACEXML_Mem_Map_Stream::Connector* connector_;

  off_t size_;

};

#endif /* _ACEXML_HTTPCHARSTREAM_H_ */
