/* -*- C++ -*- */
// $Id$

#ifndef ACE_ASYNCH_CONNECTOR_C
#define ACE_ASYNCH_CONNECTOR_C

#include "ace/Asynch_Connector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Asynch_Connector, "$Id$")

#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
// This only works on platforms that support async I/O.

#include "ace/Flag_Manip.h"
#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"
#include "ace/INET_Addr.h"


template <class HANDLER>
ACE_Asynch_Connector<HANDLER>::ACE_Asynch_Connector (void)
  : pass_addresses_ (0),
    validate_new_connection_ (0)
{
}

template <class HANDLER>
ACE_Asynch_Connector<HANDLER>::~ACE_Asynch_Connector (void)
{
  //this->asynch_connect_.close ();
}

template <class HANDLER> int
ACE_Asynch_Connector<HANDLER>::open (int pass_addresses,
                                     ACE_Proactor *proactor,
                                     int validate_new_connection)
{
  this->proactor (proactor);
  this->pass_addresses_ = pass_addresses;
  this->validate_new_connection_ = validate_new_connection;

  // Initialize the ACE_Asynch_Connect
  if (this->asynch_connect_.open (*this,
                                  ACE_INVALID_HANDLE,
                                  0,
                                  this->proactor ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("ACE_Asynch_Connect::open")),
                      -1);
  return 0;
}

template <class HANDLER> int
ACE_Asynch_Connector<HANDLER>::connect (const ACE_INET_Addr & remote_sap,
                                        const ACE_INET_Addr & local_sap,
                                        int reuse_addr,
                                        const void *act)
{
  // Initiate asynchronous connect
  if (this->asynch_connect_.connect (ACE_INVALID_HANDLE,
                                     remote_sap,
                                     local_sap,
                                     reuse_addr,
                                     act) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("ACE_Asynch_Connect::connect")),
                      -1);
  return 0;
}

template <class HANDLER> void
ACE_Asynch_Connector<HANDLER>::handle_connect (const ACE_Asynch_Connect::Result &result)
{
  // Variable for error tracking
  int error = 0;

  // If the asynchronous connect fails.
  if (!result.success () ||
      result.connect_handle () == ACE_INVALID_HANDLE)
    {
      error = 1;
    }

  if (result.error () != 0)
    {
      error = 1;
    }

  // set blocking mode 
  if (!error &&
      ACE_Flag_Manip::clr_flags
        (result.connect_handle (), ACE_NONBLOCK) != 0)
    {
      error = 1;
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("%p\n"),
                  ACE_LIB_TEXT ("ACE_Asynch_Connector::handle_connect : Set blocking mode")));
    }

  // Parse the addresses.
  ACE_INET_Addr local_address;
  ACE_INET_Addr remote_address;
  if (!error &&
      (this->validate_new_connection_ || this->pass_addresses_))
    this->parse_address (result,
                         remote_address,
                         local_address);

  // Call validate_connection even if there was an error - it's the only
  // way the application can learn the connect disposition.
  if (this->validate_new_connection_ &&
      this->validate_connection (result, remote_address, local_address) == -1)
    {
      error = 1;
    }

  HANDLER *new_handler = 0;
  if (!error)
    {
      // The Template method
      new_handler = this->make_handler ();
      if (new_handler == 0)
        {
          error = 1;
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("ACE_Asynch_Connector::handle_connect : Making of new handler failed")));
        }
    }

  // If no errors
  if (!error)
    {
      // Update the Proactor.
      new_handler->proactor (this->proactor ());

      // Pass the addresses
      if (this->pass_addresses_)
        new_handler->addresses (remote_address,
                                local_address);

      // Pass the ACT
      if (result.act () != 0)
        new_handler->act (result.act ());

      // Set up the handler's new handle value
      new_handler->handle (result.connect_handle ());

      ACE_Message_Block  mb;

      // Initiate the handler with empty message block;
      new_handler->open (result.connect_handle (), mb);
    }

  // On failure, no choice but to close the socket
  if (error &&
      result.connect_handle() != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (result.connect_handle ());
}

template <class HANDLER> int
ACE_Asynch_Connector<HANDLER>::validate_connection
  (const ACE_Asynch_Connect::Result &,
   const ACE_INET_Addr & /* remote_address */,
   const ACE_INET_Addr & /* local_address */)
{
  // Default implementation always validates the remote address.
  return 0;
}

template <class HANDLER> int
ACE_Asynch_Connector<HANDLER>::cancel (void)
{
  return this->asynch_connect_.cancel ();
}

template <class HANDLER> void
ACE_Asynch_Connector<HANDLER>::parse_address (const ACE_Asynch_Connect::Result &result,
                                              ACE_INET_Addr &remote_address,
                                              ACE_INET_Addr &local_address)
{
  // Getting the addresses.
  sockaddr_in local_addr;
  sockaddr_in remote_addr;

  // Get the length.
  int local_size = sizeof (local_addr);
  int remote_size = sizeof (remote_addr);

  // Get the local address.
  if (ACE_OS::getsockname (result.connect_handle (),
                           ACE_reinterpret_cast (sockaddr *,
                                                 &local_addr),
                           &local_size) < 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT("%p\n"),
                ACE_LIB_TEXT("ACE_Asynch_Connector::<getsockname> failed")));

  // Get the remote address.
  if (ACE_OS::getpeername (result.connect_handle (),
                           ACE_reinterpret_cast (sockaddr *,
                                                 &remote_addr),
                           &remote_size) < 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT("%p\n"),
                ACE_LIB_TEXT("ACE_Asynch_Connector::<getpeername> failed")));

  // Set the addresses.
  local_address.set  (&local_addr,  local_size);
  remote_address.set (&remote_addr, remote_size);

#if 0
  // @@ Just debugging.
  char local_address_buf  [BUFSIZ];
  char remote_address_buf [BUFSIZ];

  if (local_address.addr_to_string (local_address_buf,
                                    sizeof local_address_buf) == -1)
    ACE_ERROR ((LM_ERROR,
                "Error:%p:can't obtain local_address's address string"));

  ACE_DEBUG ((LM_DEBUG,
              "ACE_Asynch_Connector<HANDLER>::parse_address : "\
              "Local address %s\n",
              local_address_buf));

  if (remote_address.addr_to_string (remote_address_buf,
                                     sizeof remote_address_buf) == -1)
    ACE_ERROR ((LM_ERROR,
                "Error:%p:can't obtain remote_address's address string"));

  ACE_DEBUG ((LM_DEBUG,
              "ACE_Asynch_Connector<HANDLER>::parse_address : "\
              "Remote address %s\n",
              remote_address_buf));
#endif /* 0 */

  return;
}


template <class HANDLER> ACE_Asynch_Connect &
ACE_Asynch_Connector<HANDLER>::asynch_connect (void)
{
  return this->asynch_connect_;
}

template <class HANDLER> HANDLER *
ACE_Asynch_Connector<HANDLER>::make_handler (void)
{
  // Default behavior
  HANDLER *handler = 0;
  ACE_NEW_RETURN (handler, HANDLER, 0);
  return handler;
}

template <class HANDLER> int
ACE_Asynch_Connector<HANDLER>::pass_addresses (void) const
{
  return this->pass_addresses_;
}

template <class HANDLER> void
ACE_Asynch_Connector<HANDLER>::pass_addresses (int new_value)
{
  this->pass_addresses_ = new_value;
}

template <class HANDLER> int
ACE_Asynch_Connector<HANDLER>::validate_new_connection (void) const
{
  return this->validate_new_connection_;
}

template <class HANDLER> void
ACE_Asynch_Connector<HANDLER>::validate_new_connection (int new_value)
{
  this->validate_new_connection_ = new_value;
}

#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS */
#endif /* ACE_ASYNCH_CONNECTOR_C */
