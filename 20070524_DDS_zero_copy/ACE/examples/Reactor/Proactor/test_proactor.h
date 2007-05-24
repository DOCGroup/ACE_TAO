/*
** $Id$
*/

#ifndef _TEST_PROACTOR_H
#define _TEST_PROACTOR_H

#include "ace/Asynch_IO.h"

class Receiver : public ACE_Service_Handler
{
  // = TITLE
  //     The class will be created by <ACE_Asynch_Acceptor> when new
  //     connections arrive.  This class will then receive data from
  //     the network connection and dump it to a file.
public:
  // = Initialization and termination.
  Receiver (void);
  ~Receiver (void);

  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);
  // This is called after the new connection has been accepted.

protected:
  // These methods are called by the framework

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This is called when asynchronous <read> operation from the socket
  // complete.

  virtual void handle_write_file (const ACE_Asynch_Write_File::Result &result);
  // This is called when an asynchronous <write> to the file
  // completes.

private:
  int initiate_read_stream (void);
  // Initiate an asynchronous <read> operation on the socket.

  ACE_Asynch_Read_Stream rs_;
  // rs (read stream): for reading from a socket.

  ACE_HANDLE dump_file_;
  // File for dumping data.

  ACE_Asynch_Write_File wf_;
  // wf (write file): for writing to a file.

  u_long file_offset_;
  // Offset for the file.

  ACE_HANDLE handle_;
  // Handle for IO to remote peer.
};

#endif /* _TEST_PROACTOR_H */
