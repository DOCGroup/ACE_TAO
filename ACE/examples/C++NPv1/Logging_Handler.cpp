/*
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#include "ace/ACE.h"
#include "ace/CDR_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Record.h"
#include "ace/Truncate.h"
#include "ace/Message_Block.h"
#include "ace/OS_NS_string.h"
#include <memory>
#include <new>

#include "Logging_Handler.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/os_include/os_netdb.h"

namespace
{
  void release_message_block (ACE_Message_Block *message_block)
  {
    if (message_block != nullptr)
      message_block->release ();
  }
}

using Message_Block_Owner = std::unique_ptr<ACE_Message_Block,
                                            void (*) (ACE_Message_Block *)>;

int Logging_Handler::recv_log_record (ACE_Message_Block *&mblk)
{
  mblk = nullptr;

  // Put <logging_peer>'s hostname in new message block.
  ACE_INET_Addr peer_addr;
  logging_peer_.get_remote_addr (peer_addr);
  Message_Block_Owner record (
    new (std::nothrow) ACE_Message_Block (MAXHOSTNAMELEN + 1),
    &release_message_block);
  if (!record)
    {
      errno = ENOMEM;
      return -1;
    }
  peer_addr.get_host_name (record->wr_ptr (), MAXHOSTNAMELEN);
  record->wr_ptr (ACE_OS::strlen (record->wr_ptr ()) + 1); // Go past name

  // Allocate a message block for the payload; initially at least
  // large enough to hold the header, but needs some room for
  // alignment.
  Message_Block_Owner payload (
    new (std::nothrow) ACE_Message_Block (ACE_DEFAULT_CDR_BUFSIZE),
    &release_message_block);
  if (!payload)
    {
      errno = ENOMEM;
      return -1;
    }
  // Align the Message Block for a CDR stream
  ACE_CDR::mb_align (payload.get ());
  if (logging_peer_.recv_n (payload->wr_ptr (), 8) == 8) {
    payload->wr_ptr (8);               // Reflect addition of 8 bytes

    // Create a CDR stream to parse the 8-byte header.
    ACE_InputCDR cdr (payload.get ());

    // Extract the byte-order and use helper methods to
    // disambiguate octet, booleans, and chars.
    ACE_CDR::Boolean byte_order {};
    cdr >> ACE_InputCDR::to_boolean (byte_order);

    // Set the byte-order on the stream...
    cdr.reset_byte_order (byte_order);

    // Extract the length
    ACE_CDR::ULong length {};
    cdr >> length;

    // Ensure there's sufficient room for log record payload.
    ACE_CDR::grow (payload.get (), 8 + ACE_CDR::MAX_ALIGNMENT + length);

    // Use <recv_n> to obtain the contents.
    if (logging_peer_.recv_n (payload->wr_ptr (), length) > 0) {
      payload->wr_ptr (length);   // Reflect additional bytes
      // Chain the payload to mblk via the contination field.
      record->cont (payload.release ());
      mblk = record.release ();
      return length;
    }
  }
  // The message-block owners release both blocks on receive failure.
  return -1;
}

int
Logging_Handler::write_log_record (ACE_Message_Block *mblk)
{
  // Peer hostname is in the <mblk> and the log record data
  // is in its continuation.
  if (log_file_.send_n (mblk) == -1)
    return -1;
  if (ACE::debug ()) {
    // Build a CDR stream from the log record data.
    ACE_InputCDR cdr (mblk->cont ());
    ACE_CDR::Boolean byte_order {};
    ACE_CDR::ULong length {};
    // Extract the byte-order and length, ending up at the start
    // of the log record itself. Use the byte order to properly
    // set the CDR stream for extracting the contents.
    cdr >> ACE_InputCDR::to_boolean (byte_order);
    cdr.reset_byte_order (byte_order);
    cdr >> length;
    ACE_Log_Record log_record;
    cdr >> log_record;  // Finally extract the <ACE_log_record>.
    log_record.print (mblk->rd_ptr (), 1, cerr);
  }
  return ACE_Utils::truncate_cast<int> (mblk->total_length ());
}


int Logging_Handler::log_record ()
{
  ACE_Message_Block *mblk {};
  if (recv_log_record (mblk) == -1)
    return -1;
  else {
    Message_Block_Owner owner (mblk, &release_message_block);
    int const result = write_log_record (owner.get ());
    return result == -1 ? -1 : 0;
  }
}
