// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Asynch_Acceptor.h
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu)
//
// ============================================================================

#ifndef ACE_ASYNCH_ACCEPTOR_H
#define ACE_ASYNCH_ACCEPTOR_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
// This only works on platforms that support async i/o.

#include "ace/Asynch_IO.h"

// Forward declarations
class ACE_Message_Block;
class ACE_INET_Addr;

template <class HANDLER>
class ACE_Asynch_Acceptor : public ACE_Handler
{
  // = TITLE
  //     This class is an example of the Acceptor Pattern.  This class
  //     will accept new connections and create new HANDLER to handle
  //     the new connections.
  //
  // = DESCRIPTION
  //     Unlike the <ACE_Acceptor>, however, this class is designed to
  //     be used asynchronously.
public:
  ACE_Asynch_Acceptor (void);
  // A do nothing constructor.

  virtual ~ACE_Asynch_Acceptor (void);
  // Virtual destruction

  virtual int open (const ACE_INET_Addr &address,
                    size_t bytes_to_read = 0,
                    int pass_addresses = 0,
                    int backlog = 5,
                    int reuse_addr = 1,
                    ACE_Proactor *proactor = 0,
                    int validate_new_connection = 0,
                    int reissue_accept = 1,
                    int number_of_initial_accepts = -1);
  // This starts the listening process at the port specified by
  // <address>.  ACE_Asynch_Acceptor initiates the AcceptEx calls with
  // <bytes_to_read>.  The buffer for the initial data will be created
  // by ACE_Asynch_Acceptor.  This buffer will be passed to the
  // handler in the <ACE_Service_Handler::open> callback.  If this
  // buffer is required past the <open> callback, the
  // ACE_Service_Handler must copy the data.  If the <pass_addresses>
  // flag is set, ACE_Asynch_Acceptor will call
  // <ACE_Service_Handler::addresses> before calling
  // <ACE_Service_Handler::open>.  The <backlog> parameter specifies
  // the listen backlog and the outstanding AcceptEx calls.
  // <number_of_initial_accepts> is the number of asynchronous accepts
  // that are started at the end of <open>.  If
  // <number_of_initial_accepts> is -1, then
  // <number_of_initial_accepts> is set to <backlog> and hence
  // <backlog> number of asynchronous accepts are started.

  virtual ACE_HANDLE get_handle (void) const;
  // Get the underlying handle.

  virtual void set_handle (ACE_HANDLE handle);
  // Set the underlying listen handle. It is the user's responsibility
  // to make sure that the old listen handle has been appropriately
  // closed and the all outstanding asynchronous operations have
  // either completed or have been canceled on the old listen handle.

  virtual int accept (size_t bytes_to_read = 0, const void *act = 0);
  // This initiates a new asynchronous accept through the <AcceptEx>
  // call.

  virtual int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel accept
  // operations issued by other threads.

  virtual int validate_new_connection (const ACE_INET_Addr &remote_address);
  // Template method for address validation.
  //
  // Default implemenation always validates the remote address.

  virtual int should_reissue_accept (void);
  // Template method for deciding whether to reissue accept.
  //
  // Default implemenation always returns this->reissue_accept_.

  //
  // These are low level tweaking methods
  //

  virtual int pass_addresses (void) const;
  virtual void pass_addresses (int new_value);
  // Set and get flag that indicates if parsing and passing of
  // addresses to the service_handler is necessary.

  virtual int validate_new_connection (void) const;
  virtual void validate_new_connection (int new_value);
  // Set and get flag that indicates if address validation is
  // required.

  virtual int reissue_accept (void) const;
  virtual void reissue_accept (int new_value);
  // Set and get flag that indicates if a new accept should be
  // reissued when a accept completes.

  virtual int bytes_to_read (void) const;
  virtual void bytes_to_read (int new_value);
  // Set and get bytes to be read with the <accept> call.

  static size_t address_size (void);
  // This is required by the AcceptEx call.

protected:

  virtual void handle_accept (const ACE_Asynch_Accept::Result &result);
  // This is called when an outstanding accept completes.

  ACE_HANDLE handle (void) const;
  // Return the listen handle.

  void parse_address (const ACE_Asynch_Accept::Result &result,
                      ACE_INET_Addr &remote_address,
                      ACE_INET_Addr &local_address);
  // This parses the address from read buffer.

  virtual HANDLER *make_handler (void);
  // This is the template method used to create new handler.
  // Subclasses must overwrite this method if a new handler creation
  // strategy is required.

private:
  ACE_HANDLE listen_handle_;
  // Handle used to listen for new connections.

  ACE_Asynch_Accept asynch_accept_;
  // <Asynch_Accept> used to make life easier :-)

  int pass_addresses_;
  // Flag that indicates if parsing of addresses is necessary.

  int validate_new_connection_;
  // Flag that indicates if address validation is required.

  int reissue_accept_;
  // Flag that indicates if a new accept should be reissued when a
  // accept completes.

  int bytes_to_read_;
  // Bytes to be read with the <accept> call.
};

#if defined (__ACE_INLINE__)
#include "ace/Asynch_Acceptor.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Asynch_Acceptor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Asynch_Acceptor.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS */
#endif /* ACE_ASYNCH_ACCEPTOR_H */

