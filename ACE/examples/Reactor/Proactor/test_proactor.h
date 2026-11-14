/*
**
*/

#ifndef _TEST_PROACTOR_H
#define _TEST_PROACTOR_H

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
  /// @name Framework callbacks
  /// @{
  void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result) override;

  void handle_write_file (const ACE_Asynch_Write_File::Result &result) override;
  /// @}

private:
  /// Initiates an asynchronous read operation on the socket.
  int initiate_read_stream ();

  /// Asynchronous reader for the socket.
  ACE_Asynch_Read_Stream rs_;

  /// File receiving the data.
  ACE_HANDLE dump_file_;

  /// Asynchronous writer for the file.
  ACE_Asynch_Write_File wf_;

  /// Current offset in the output file.
  u_long file_offset_;

  /// Handle for I/O with the remote peer.
  ACE_HANDLE handle_;
};

#endif /* _TEST_PROACTOR_H */
