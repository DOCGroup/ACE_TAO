/* -*- C++ -*- */
// $Id$

//============================================================================
//
// = LIBRARY
//    JAWS
// 
// = FILENAME
//    Blob.h
//
// = DESCRIPTION
//     ACE_Blob_Handler is a base class for ACE_Blob_Reader and
//     ACE_Blob_Writer which are created in response to calls to
//     read/write, as appropriate 
//
// = AUTHOR
//    Prashant Jain and Sumedh Mungee
// 
//============================================================================

#if !defined (ACE_BLOB_HANDLER_H)
#define ACE_BLOB_HANDLER_H

#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/Svc_Handler.h"
#include "ace/Message_Block.h"

class ACE_Blob_Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //     Blob is intended to provide application API to
  //     classes that wish to do network i/o at a very
  //     high level of abstraction.
  //    
  // = This class provides the ability to retrieve data from
  //   the network, of specified length and offset, and potentially
  //   use any protocol "under the hood" to do so. It currently
  //   uses HTTP. See Blob_Handler also.
{
public:
  ACE_Blob_Handler (void);
  // Null constructor, insures that it works properly with Connector

  ACE_Blob_Handler (ACE_Message_Block *mb, size_t length, size_t offset, char *filename);
  // Always use this constructor to make Blob_Handlers

  int byte_count (void);
  // returns the number of bytes read/written in the last operation.

  virtual int open (void * = 0);
  // Activate this instance of the <ACE_Blob_Handler>

  virtual int close (u_long flags = 0);
  // Close down the Blob
  
  ~ACE_Blob_Handler (void);

protected:
  virtual int send_request (void);
  virtual int receive_reply (void);

  ACE_Message_Block *mb_;
  size_t length_;
  size_t offset_;
  char *filename_;
  int bytecount_;
  enum 
  {
    MAX_HEADER_SIZE = 2048
    // The handler assumes that the first 2048 bytes of a server response
    // contains the header
  };
};

class ACE_Blob_Reader : public ACE_Blob_Handler
{
public:
  ACE_Blob_Reader (ACE_Message_Block *mb, 
                   size_t length, 
                   size_t offset, 
                   char *filename, 
                   char *request_prefix = "GET",
                   char *request_suffix = "HTTP/1.0\r\n\r\n");

private:
  int send_request (void);
  int receive_reply (void);
  const char *request_prefix_;
  const char *request_suffix_;
};

class ACE_Blob_Writer : public ACE_Blob_Handler
{
public:
  ACE_Blob_Writer (ACE_Message_Block *mb, 
                   size_t length, 
                   size_t offset, 
                   char *filename,
                   char *request_prefix = "PUT",
                   char *request_suffix = "HTTP/1.0\nContent-length:");

private:
  int send_request (void);
  int receive_reply (void);
  const char *request_prefix_;
  const char *request_suffix_;
};

#endif /* ACE_BLOB_HANDLER_H */
