/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#ifndef _LOGGING_HANDLER_H
#define _LOGGING_HANDLER_H

#include "ace/FILE_IO.h"
#include "ace/SOCK_Stream.h"

class ACE_Message_Block;

class Logging_Handler
{
protected:
  ACE_FILE_IO *log_file_; // Pointer to a log file.

  ACE_SOCK_Stream logging_peer_; // Connected to the client.

public:
  // Initialization and termination methods.
  Logging_Handler (ACE_FILE_IO &log_file): log_file_ (&log_file) {}
  Logging_Handler (ACE_FILE_IO &log_file,
                   ACE_HANDLE handle): log_file_ (&log_file)
  { logging_peer_.set_handle (handle); }
  Logging_Handler (ACE_FILE_IO &log_file,
                   const ACE_SOCK_Stream &logging_peer)
    : log_file_ (&log_file), logging_peer_ (logging_peer) {}
  Logging_Handler (const ACE_SOCK_Stream &logging_peer)
    : log_file_ (0), logging_peer_ (logging_peer) {}
  int close () { return logging_peer_.close (); }

  // Receive one log record from a connected client.   Returns
  // length of record on success and <mblk> contains the
  // hostname, <mblk->cont()> contains the log record header
  // (the byte order and the length) and the data.  Returns -1 on
  // failure or connection close.
  int recv_log_record (ACE_Message_Block *&log_record);

  // Write one record to the log file.  The <mblk> contains the
  // hostname and the <mblk->cont> contains the log record.
  // Returns length of record written on success, or -1 on failure.
  int write_log_record (ACE_Message_Block *log_record);

  // Log one record by calling <recv_log_record> and
  // <write_log_record>.  Returns 0 on success and -1 on failure.
  int log_record ();

  // Accessor method.
  ACE_SOCK_Stream &peer () { return logging_peer_; }
};

#endif /* _LOGGING_HANDLER_H */
