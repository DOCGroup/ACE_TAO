// $Id$

// Define classes used with templates in server_loggerd.h.

#ifndef __SERVER_LOGGERD_H
#define __SERVER_LOGGERD_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"


class Options
{
  // = TITLE
  //     Keeps track of the options.
public:
  void parse_args (int argc, char *argv[]);
  u_short port (void);

private:
  u_short port_;
  // Port number;
};


class Logging_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
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
  // = Initialization and termination methods.
  Logging_Handler (void);

  // = Hooks for opening and closing handlers.
  virtual int open (void *);

protected:
  // = Demultiplexing hooks.
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);

private:
  char peer_name_[MAXHOSTNAMELEN + 1];
  // Host we are connected to.
};

#endif /* __SERVER_LOGGERD_H */
