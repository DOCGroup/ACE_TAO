// $Id$

/* -*- C++ -*- */
// HTTP_Client.h

//============================================================================
// = FILENAME
//    HTTP_Client.h
//
// = DESCRIPTION
//     This is the HTTP_Client class, which is the API for doing file
//     uploads/downloads.
//
// = AUTHOR
//    Stoyan Paunov
//
//============================================================================

#ifndef HTTP_Client_H
#define HTTP_Client_H


#include "ace/INET_Addr.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Message_Block.h"
#include "HTTP_Handler.h"

class HTTP_Client
  // = TITLE
  //     HTTP_Client is intended to provide application API to
  //     classes that wish to do network i/o at a very
  //     high level of abstraction.
  //
  // = This class provides the ability to retrieve data from
  //   the network, of specified length and offset, and potentially
  //   use any protocol "under the hood" to do so. It currently
  //   uses HTTP. See HTTP_Handler also.
{
public:
  HTTP_Client (void);
  ~HTTP_Client (void);

  int open (const ACE_TCHAR *filename,
            const ACE_TCHAR *hostname = ACE_DEFAULT_SERVER_HOST,
            u_short port = 80);
  // initializes the class with the given filename, hostname and port.
  // it should be called with the filename, before any read/write calls


  int read (ACE_Message_Block *mb);
  // starts a connection, and reads a file from the server into Message_Block mb

  int close ();
  // Frees memory allocated for filename.

private:
  ACE_INET_Addr inet_addr_;
  // store the internet address of the server

  ACE_TCHAR *filename_;
  // The filename

  ACE_Connector<HTTP_Handler, ACE_SOCK_CONNECTOR> connector_;
  // The connector endpoint to initiate the client connection

};

#endif /* HTTP_Client_H */
