// $Id$

/* -*- C++ -*- */
// HTTP_Handler.h, Stoyan

//============================================================================
// = FILENAME
//    HTTP_Handler.h
//
// = DESCRIPTION
//     HTTP_Handler is a base class for HTTP_Reader and
//     HTTP_Writer which are created in response to calls to
//     read/write, as appropriate
//
// = AUTHOR
//    Stoyan Paunov
//
//============================================================================

#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Message_Block.h"

class HTTP_Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
	//class to retrieve data via HTTP
{
public:
  HTTP_Handler (void);
  // Null constructor, insures that it works properly with Connector

  HTTP_Handler (ACE_Message_Block *mb,
                    ACE_TCHAR *filename);
  // Always use this constructor to make HTTP_Handlers

  int byte_count (void);
  // returns the number of bytes read/written in the last operation.

  virtual int open (void * = 0);
  // Activate this instance of the <HTTP_Handler>

  virtual int close (u_long flags = 0);
  // Close down the Blob

  ~HTTP_Handler (void);

protected:
  virtual int send_request (void);
  virtual int receive_reply (void);

  ACE_Message_Block *mb_;
  ACE_TCHAR *filename_;
  int bytecount_;
  enum
  {
    MAX_HEADER_SIZE = 2048,
    // The handler assumes that the first 2048 bytes of a server response
    // contains the header

	MTU = BUFSIZ
	//set the MAX_TRANSMISSION_UNIT (MTU) = BUFSIZ as defined by OS
  };
};

class HTTP_Reader : public HTTP_Handler
{
public:
  HTTP_Reader (ACE_Message_Block *mb,
                   ACE_TCHAR *filename,
                   const char *request_prefix = "GET",
                   const char *request_suffix = "HTTP/1.0\r\n\r\n");

private:
  int send_request (void);
  int receive_reply (void);
  const char *request_prefix_;
  const char *request_suffix_;
};

#endif /* HTTP_HANDLER_H */
