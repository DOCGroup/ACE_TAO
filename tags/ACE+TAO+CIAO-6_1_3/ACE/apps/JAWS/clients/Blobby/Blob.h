/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Blob.h
 *
 *  $Id$
 *
 *   This is the ACE_Blob class, which is the API for doing file
 *   uploads/downloads.
 *
 *
 *  @author Prashant Jain and Sumedh Mungee
 */
//=============================================================================


#ifndef ACE_BLOB_H
#define ACE_BLOB_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Message_Block.h"
#include "Blob_Handler.h"

/**
 * @class ACE_Blob
 *
 * @brief Blob is intended to provide application API to
 * classes that wish to do network i/o at a very
 * high level of abstraction.
 * = This class provides the ability to retrieve data from
 * the network, of specified length and offset, and potentially
 * use any protocol "under the hood" to do so. It currently
 * uses HTTP. See Blob_Handler also.
 */
class ACE_Blob
{
public:
  ACE_Blob (void);
  ~ACE_Blob (void);

  /// initializes the class with the given filename, hostname and port.
  /// it should be called with the filename, before any read/write calls
  int open (const ACE_TCHAR *filename,
            const ACE_TCHAR *hostname = ACE_DEFAULT_SERVER_HOST,
            u_short port = 80);


  /**
   * starts a connection, and reads a file from the server,
   *   of length and offset as specified, into Message_Block mb
   *   The message block should have capacity to hold length number
   *   of bytes
   */
  int read (ACE_Message_Block *mb, size_t length, size_t offset);

  /**
   * starts a connection, and writes a file to the server,
   *   of length and offset as specified, from Message_Block mb
   *   thus the message block should contain atleast length + offset
   *   bytes of data
   */
  int write (ACE_Message_Block *mb, size_t length, size_t offset);


  /// Frees memory allocated for filename.
  int close ();

private:
  /// store the internet address of the server
  ACE_INET_Addr inet_addr_;

  /// The filename
  ACE_TCHAR *filename_;

  /// The connector endpoint to initiate the client connection
  ACE_Connector<ACE_Blob_Handler, ACE_SOCK_CONNECTOR> connector_;

};

#endif /* ACE_BLOB_H */
