/*
** $Id$
*/

#ifndef _TEST_CANCEL_H
#define _TEST_CANCEL_H

#include "ace/Asynch_IO.h"

class Receiver : public ACE_Service_Handler
{
  // = TITLE
  //
  //     Receiver
  //
  // = DESCRIPTION
  //
  //     The class will be created by ACE_Asynch_Acceptor when new
  //     connections arrive.  This class will then receive data from
  //     the network connection and dump it to a file.

public:
  Receiver (void);
  ~Receiver (void);

  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);
  // This is called after the new connection has been accepted.

protected:
  // These methods are called by the framework

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This is called when asynchronous read from the socket complete

private:
  ACE_Asynch_Read_Stream rs_;
  // rs (read stream): for reading from a socket

  ACE_Message_Block mb_;
  // Message block to read from the stream.

  ACE_HANDLE handle_;
  // Handle for IO to remote peer
};

#endif /* _TEST_CANCEL_H */
