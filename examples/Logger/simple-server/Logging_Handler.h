/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    Logging_Handler.h 
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CLIENT_HANDLER_H)
#define _CLIENT_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"

class Logging_Handler : public ACE_Event_Handler
  // = TITLE
  //     Receive client message from the remote clients.
  // 
  // = DESCRIPTION
  //     This class demonstrates how to receive messages from remote
  //     clients using the notification mechanisms in the
  //     <ACE_Reactor>.  In addition, it also illustrates how to
  //     utilize the <ACE_Reactor> timer mechanisms, as well.
{
public:
  Logging_Handler (void);  

  // = Hooks for opening and closing handlers.
  virtual int open (void);
  virtual int close (void);

  ACE_SOCK_Stream &peer (void);
  // Conversion operators.

protected:
  // = Demultiplexing hooks.
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg); 

  // = Really should be private...
  virtual ~Logging_Handler (void); 
  // Ensure dynamic allocation.

private:
  char host_name_[MAXHOSTNAMELEN + 1];
  // Host we are connected to.

  ACE_SOCK_Stream cli_stream_; 
  // Connection with client 
};

#endif /* _CLIENT_HANDLER_H */
