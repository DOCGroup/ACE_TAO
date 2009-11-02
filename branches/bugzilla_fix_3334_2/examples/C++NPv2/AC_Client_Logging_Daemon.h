/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _AC_CLIENT_LOGGING_DAEMON_H
#define _AC_CLIENT_LOGGING_DAEMON_H

#include "ace/Handle_Set.h"
#include "ace/Log_Record.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch_Traits.h"

class AC_CLD_Connector;

class AC_Output_Handler
  : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
public:
  enum { QUEUE_MAX = sizeof (ACE_Log_Record) * ACE_IOV_MAX };

  virtual int open (void *); // Initialization hook method.

  // Entry point into the <AC_Output_Handler>.
  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);

protected:
  AC_CLD_Connector *connector_;

  // Handle disconnects from the logging server.
  virtual int handle_input (ACE_HANDLE handle);

  // Hook method forwards log records to server logging daemon.
  virtual int svc ();

  // Send the buffered log records using a gather-write operation.
  virtual int send (ACE_Message_Block *chunk[], size_t &count);
};

class AC_Input_Handler
  : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
public:
  AC_Input_Handler (AC_Output_Handler *handler = 0)
    : output_handler_ (handler) {}
  virtual int open (void *); // Initialization hook method.
  virtual int close (u_long = 0); // Shutdown hook method.

protected:
  // Reactor hook methods.
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = 0);

  // Pointer to the output handler.
  AC_Output_Handler *output_handler_;

  // Keep track of connected client handles.
  ACE_Handle_Set connected_clients_;
};

#endif /* _AC_CLIENT_LOGGING_DAEMON_H */
