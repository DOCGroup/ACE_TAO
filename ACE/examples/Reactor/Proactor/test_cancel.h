/*
**
*/

#ifndef _TEST_CANCEL_H
#define _TEST_CANCEL_H

#include "ace/Asynch_IO.h"

/**
 * @brief Receives network data and writes it to a file.
 *
 * ACE_Asynch_Acceptor creates this class when a new connection arrives.
 */
class Receiver : public ACE_Service_Handler
{
public:
  Receiver ();
  ~Receiver () override;

  /// Called after a new connection has been accepted.
  void open (ACE_HANDLE handle,
             ACE_Message_Block &message_block) override;

protected:
  /// Reads data from the socket asynchronously.
  void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result) override;

private:
  /// Asynchronous reader for the socket.
  ACE_Asynch_Read_Stream rs_;

  /// Message block used for reading from the stream.
  ACE_Message_Block mb_;

  /// Handle for I/O with the remote peer.
  ACE_HANDLE handle_;
};

#endif /* _TEST_CANCEL_H */
