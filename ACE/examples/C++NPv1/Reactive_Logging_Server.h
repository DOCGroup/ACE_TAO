/*
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#ifndef _REACTIVE_LOGGING_SERVER_H
#define _REACTIVE_LOGGING_SERVER_H

#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Log_Record.h"
#include "ace/Handle_Set.h"
#include "ace/Basic_Types.h"
#include "ace/Truncate.h"
#include "ace/os_include/os_fcntl.h"
#include "Iterative_Logging_Server.h"

class Reactive_Logging_Server : public Iterative_Logging_Server
{
public:
  Reactive_Logging_Server () = default;
  Reactive_Logging_Server (Reactive_Logging_Server const &) = delete;
  Reactive_Logging_Server (Reactive_Logging_Server &&) = delete;
  Reactive_Logging_Server &operator= (Reactive_Logging_Server const &) = delete;
  Reactive_Logging_Server &operator= (Reactive_Logging_Server &&) = delete;

protected:
  // Keep track of the acceptor socket handle and all the
  // connected stream socket handles.
  ACE_Handle_Set master_handle_set_;

  // Keep track of handles marked as active by <select>.
  ACE_Handle_Set active_handles_;

  int open (u_short logger_port) override {
    Iterative_Logging_Server::open (logger_port);
    master_handle_set_.set_bit (acceptor ().get_handle ());
    acceptor ().enable (ACE_NONBLOCK);
    return 0;
  }

  int wait_for_multiple_events () override {
    active_handles_ = master_handle_set_;
#if defined (ACE_WIN32)
    int const width = 0;
#else
    int const width = ACE_Utils::truncate_cast<int> (
      static_cast<intptr_t> (active_handles_.max_set ())) + 1;
#endif
    if (select (width,
                active_handles_.fdset (),
                nullptr,  // no write_fds
                nullptr,  // no except_fds
                nullptr) == -1) // no timeout
      {
        return -1;
      }
#if !defined (ACE_WIN32)
    active_handles_.sync (static_cast<ACE_HANDLE> (
      static_cast<intptr_t> (active_handles_.max_set ()) + 1));
#endif
    return 0;
  }

  int handle_connections () override {
    if (active_handles_.is_set (acceptor ().get_handle ())) {
      while (acceptor ().accept (logging_handler ().peer ()) == 0)
        master_handle_set_.set_bit
          (logging_handler ().peer ().get_handle ());

      // Remove acceptor handle from further consideration.
      active_handles_.clr_bit (acceptor ().get_handle ());
    }
    return 0;
  }

  int handle_data (ACE_SOCK_Stream *) override {
    ACE_Handle_Set_Iterator peer_iterator (active_handles_);

    for (ACE_HANDLE handle;
         (handle = peer_iterator ()) != ACE_INVALID_HANDLE;
         ) {
      logging_handler ().peer ().set_handle (handle);
      if (logging_handler ().log_record () == -1) {
        // Handle connection shutdown or comm failure.
        master_handle_set_.clr_bit (handle);
        logging_handler ().close ();
      }
    }
    return 0;
  }
};

#endif /* _REACTIVE_LOGGING_SERVER_H */
