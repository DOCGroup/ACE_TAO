/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Blob_Handler.h
 *
 *   ACE_Blob_Handler is a base class for ACE_Blob_Reader and
 *   ACE_Blob_Writer which are created in response to calls to
 *   read/write, as appropriate
 *
 *  @author Prashant Jain and Sumedh Mungee
 */
//=============================================================================


#ifndef ACE_BLOB_HANDLER_H
#define ACE_BLOB_HANDLER_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Message_Block.h"
#include <string>

/**
 * @class ACE_Blob_Handler
 *
 * @brief Blob is intended to provide application API to
 * classes that wish to do network i/o at a very
 * high level of abstraction.
 * = This class provides the ability to retrieve data from
 * the network, of specified length and offset, and potentially
 * use any protocol "under the hood" to do so. It currently
 * uses HTTP. See Blob_Handler also.
 */
class ACE_Blob_Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  /// Null constructor, insures that it works properly with Connector
  ACE_Blob_Handler () = default;

  /// Always use this constructor to make Blob_Handlers
  ACE_Blob_Handler (ACE_Message_Block *mb,
                    size_t length,
                    size_t offset,
                    ACE_TCHAR const *filename);

  /// returns the number of bytes read/written in the last operation.
  int byte_count () const;

  /// Activate this instance of the <ACE_Blob_Handler>
  int open (void * = nullptr) override;

  /// Close down the Blob
  int close (u_long flags = 0) override;

  ~ACE_Blob_Handler () override = default;

  ACE_Blob_Handler (ACE_Blob_Handler const &) = delete;
  ACE_Blob_Handler (ACE_Blob_Handler &&) = delete;
  ACE_Blob_Handler &operator= (ACE_Blob_Handler const &) = delete;
  ACE_Blob_Handler &operator= (ACE_Blob_Handler &&) = delete;

protected:
  virtual int send_request ();
  virtual int receive_reply ();

  ACE_Message_Block *mb_ {}; // Borrowed from the caller.
  size_t length_ {};
  size_t offset_ {};
  std::basic_string<ACE_TCHAR> filename_;
  int bytecount_ {};
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
                   ACE_TCHAR const *filename,
                   char const *request_prefix = "GET",
                   char const *request_suffix = "HTTP/1.0\r\n\r\n");

  ACE_Blob_Reader (ACE_Blob_Reader const &) = delete;
  ACE_Blob_Reader (ACE_Blob_Reader &&) = delete;
  ACE_Blob_Reader &operator= (ACE_Blob_Reader const &) = delete;
  ACE_Blob_Reader &operator= (ACE_Blob_Reader &&) = delete;

private:
  int send_request () override;
  int receive_reply () override;
  char const *request_prefix_;
  char const *request_suffix_;
};

class ACE_Blob_Writer : public ACE_Blob_Handler
{
public:
  ACE_Blob_Writer (ACE_Message_Block *mb,
                   size_t length,
                   size_t offset,
                   ACE_TCHAR const *filename,
                   char const *request_prefix = "PUT",
                   char const *request_suffix = "HTTP/1.0\nContent-length:");

  ACE_Blob_Writer (ACE_Blob_Writer const &) = delete;
  ACE_Blob_Writer (ACE_Blob_Writer &&) = delete;
  ACE_Blob_Writer &operator= (ACE_Blob_Writer const &) = delete;
  ACE_Blob_Writer &operator= (ACE_Blob_Writer &&) = delete;

private:
  int send_request () override;
  int receive_reply () override;
  char const *request_prefix_;
  char const *request_suffix_;
};

#endif /* ACE_BLOB_HANDLER_H */
