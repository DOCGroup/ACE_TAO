/*
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#ifndef _REACTIVE_LOGGING_SERVER_EX_H
#define _REACTIVE_LOGGING_SERVER_EX_H

#include "ace/ACE.h"
#include "ace/FILE_IO.h"
#include "ace/Handle_Set.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Record.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "Logging_Server.h"
#include "Logging_Handler.h"
#include "ace/Null_Mutex.h"
#include "ace/Truncate.h"
#include "ace/os_include/os_fcntl.h"
#include <memory>

using LOG_MAP = ACE_Hash_Map_Manager<ACE_HANDLE, ACE_FILE_IO *, ACE_Null_Mutex>;

class Reactive_Logging_Server_Ex : public Logging_Server
{
protected:
  // Associate an active handle to an <ACE_FILE_IO> pointer.
  LOG_MAP log_map_;

  // Keep track of acceptor socket and all the connected
  // stream socket handles.
  ACE_Handle_Set master_handle_set_;

  // Keep track of read handles marked as active by <select>.
  ACE_Handle_Set active_read_handles_;

  int open (u_short port) override {
    Logging_Server::open (port);
    master_handle_set_.set_bit (acceptor ().get_handle ());
    acceptor ().enable (ACE_NONBLOCK);
    return 0;
  }

  int wait_for_multiple_events () override {
    active_read_handles_ = master_handle_set_;
    int width = ACE_Utils::truncate_cast<int> ((intptr_t)active_read_handles_.max_set ()) + 1;

    return ACE::select (width, active_read_handles_);
  }

  int handle_connections () override {
    ACE_SOCK_Stream logging_peer;

    while (acceptor ().accept (logging_peer) != -1) {
      std::unique_ptr<ACE_FILE_IO> log_file = std::make_unique<ACE_FILE_IO> ();

      // Use the client's hostname as the logfile name.
      make_log_file (*log_file, &logging_peer);

      // Add the new <logging_peer>'s handle to the map and
      // to the set of handles we <select> for input.
      if (log_map_.bind (logging_peer.get_handle (), log_file.get ()) == 0)
        {
          log_file.release ();
          master_handle_set_.set_bit (logging_peer.get_handle ());
        }
    }
    active_read_handles_.clr_bit (acceptor ().get_handle ());
    return 0;
  }

  int handle_data (ACE_SOCK_Stream *) override {
    ACE_Handle_Set_Iterator peer_iterator (active_read_handles_);

    for (ACE_HANDLE handle;
         (handle = peer_iterator ()) != ACE_INVALID_HANDLE;
         ) {
      ACE_FILE_IO *log_file = nullptr;
      log_map_.find (handle, log_file);
      Logging_Handler logging_handler (*log_file, handle);

      if (logging_handler.log_record () == -1) {
        logging_handler.close ();
        master_handle_set_.clr_bit (handle);
        log_map_.unbind (handle);
        std::unique_ptr<ACE_FILE_IO> log_file_owner (log_file);
        log_file_owner->close ();
      }
    }
    return 0;
  }
};

#endif /* _REACTIVE_LOGGING_SERVER_EX_H */
