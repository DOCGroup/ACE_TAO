/* -*- C++ -*- */
#ifndef SP_CONNECTOR_H
#define SP_CONNECTOR_H

#include "ace/Svc_Handler.h"
#include "ace/Service_Config.h"
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SPIPE_Stream.h"
#include "ace/Connector.h"
#include "ace/SPIPE_Connector.h"
#include "ace/Sig_Adapter.h"

class Peer_Handler : public ACE_Svc_Handler<ACE_SPIPE_STREAM, ACE_NULL_SYNCH>
{
public:
  /// @name Initialization
  /// @{

  /// <iterations> is the number of buffers to send.  If <iterations>
  /// == 0, then read from stdin.
  explicit Peer_Handler (int iterations = 0);

  ~Peer_Handler () override = default;
  Peer_Handler (Peer_Handler const &) = delete;
  Peer_Handler (Peer_Handler &&) = delete;
  Peer_Handler &operator= (Peer_Handler const &) = delete;
  Peer_Handler &operator= (Peer_Handler &&) = delete;

  /// Activate the handler when connection is established.
  int open (void * = nullptr) override;
  /// @}

  /// @name Demultiplexing hooks
  /// @{
  int handle_input (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE handle = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK) override;
  /// @}

  ACE_HANDLE get_handle () const override;

private:
  void display_menu ();

  /// No. of buffers to send.
  int iterations_ {};
};

class IPC_Client : public ACE_Connector<Peer_Handler, ACE_SPIPE_CONNECTOR>
{
public:
  /// @name Initialization
  /// @{
  IPC_Client ();
  ~IPC_Client () override = default;
  IPC_Client (IPC_Client const &) = delete;
  IPC_Client (IPC_Client &&) = delete;
  IPC_Client &operator= (IPC_Client const &) = delete;
  IPC_Client &operator= (IPC_Client &&) = delete;

  /// @}
  /// @name Dynamic linking hooks
  /// @{
  /// Initialize the IPC client.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Destroy the IPC client.
  int fini () override;
  /// @}

  /// Run the svc.
  virtual int svc ();

  /// Report connection errors.
  int handle_close (ACE_HANDLE, ACE_Reactor_Mask) override;

private:
  /// Parse command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Number of times to send a buffer.
  int iterations_ {};

  /// Meeting place for pipe.
  std::basic_string<ACE_TCHAR> rendezvous_ {ACE_TEXT ("acepipe")};

  /// Keeps track of when we shut down due to receipt of the SIGINT
  /// signal.
  ACE_Sig_Adapter done_handler_;
};

#endif /* SP_CONNECTOR_H */
