/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Asynch_Acceptor.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali (irfan@cs.wustl.edu)
 */
//=============================================================================

#ifndef ACE_ASYNCH_ACCEPTOR_H
#define ACE_ASYNCH_ACCEPTOR_H
#include "ace/pre.h"

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

/**
 * @class ACE_Asynch_Acceptor
 *
 * @brief This class is an example of the Acceptor Pattern.  This class
 * will accept new connections and create new HANDLER to handle
 * the new connections.
 *
 * Unlike the <ACE_Acceptor>, however, this class is designed to
 * be used asynchronously.
 */
template <class HANDLER>
class ACE_Asynch_Acceptor : public ACE_Handler
{
public:
  /// A do nothing constructor.
  ACE_Asynch_Acceptor (void);

  /// Virtual destruction
  virtual ~ACE_Asynch_Acceptor (void);

  /**
   * This starts the listening process at the port specified by
   * <address>.  ACE_Asynch_Acceptor initiates the AcceptEx calls with
   * <bytes_to_read>.  The buffer for the initial data will be created
   * by ACE_Asynch_Acceptor.  This buffer will be passed to the
   * handler in the <ACE_Service_Handler::open> callback.  If this
   * buffer is required past the <open> callback, the
   * ACE_Service_Handler must copy the data.  If the <pass_addresses>
   * flag is set, ACE_Asynch_Acceptor will call
   * <ACE_Service_Handler::addresses> before calling
   * <ACE_Service_Handler::open>.  The <backlog> parameter specifies
   * the listen backlog and the outstanding AcceptEx calls.
   * <number_of_initial_accepts> is the number of asynchronous accepts
   * that are started at the end of <open>.  If
   * <number_of_initial_accepts> is -1, then
   * <number_of_initial_accepts> is set to <backlog> and hence
   * <backlog> number of asynchronous accepts are started.
   */
  virtual int open (const ACE_INET_Addr &address,
                    size_t bytes_to_read = 0,
                    int pass_addresses = 0,
                    int backlog = ACE_DEFAULT_BACKLOG,
                    int reuse_addr = 1,
                    ACE_Proactor *proactor = 0,
                    int validate_new_connection = 0,
                    int reissue_accept = 1,
                    int number_of_initial_accepts = -1);

  /// Get the underlying handle.
  virtual ACE_HANDLE get_handle (void) const;

  /**
   * Set the underlying listen handle. It is the user's responsibility
   * to make sure that the old listen handle has been appropriately
   * closed and the all outstanding asynchronous operations have
   * either completed or have been canceled on the old listen handle.
   */
  virtual void set_handle (ACE_HANDLE handle);

  /// This initiates a new asynchronous accept through the <AcceptEx>
  /// call.
  virtual int accept (size_t bytes_to_read = 0, const void *act = 0);

  /**
   * This cancels all pending accepts operations that were issued by
   * the calling thread.
   * Windows NT- The function does not cancel accept operations
   *             issued by other threads
   * POSIX     - all OK, it delegates cancelation to the
   *             ACE_POSIX_Asynch_Accept
   *
   */
  virtual int cancel (void);

  /**
   * Template method for address validation.
   *
   * Default implemenation always validates the remote address.
   */
  virtual int validate_new_connection (const ACE_INET_Addr &remote_address);

  /**
   * Template method for deciding whether to reissue accept.
   *
   * Default implemenation always returns this->reissue_accept_.
   */
  virtual int should_reissue_accept (void);

  //
  // These are low level tweaking methods
  //

  /// Set and get flag that indicates if parsing and passing of
  /// addresses to the service_handler is necessary.
  virtual int pass_addresses (void) const;
  virtual void pass_addresses (int new_value);

  /// Set and get flag that indicates if address validation is
  /// required.
  virtual int validate_new_connection (void) const;
  virtual void validate_new_connection (int new_value);

  /// Set and get flag that indicates if a new accept should be
  /// reissued when a accept completes.
  virtual int reissue_accept (void) const;
  virtual void reissue_accept (int new_value);

  /// Set and get bytes to be read with the <accept> call.
  virtual int bytes_to_read (void) const;
  virtual void bytes_to_read (int new_value);

  /// This is required by the AcceptEx call.
  static size_t address_size (void);

protected:

  /// This is called when an outstanding accept completes.
  virtual void handle_accept (const ACE_Asynch_Accept::Result &result);

  /// Return the listen handle.
  ACE_HANDLE handle (void) const;
  /// Set the listen handle.
  void handle (ACE_HANDLE h);

  /// This parses the address from read buffer.
  void parse_address (const ACE_Asynch_Accept::Result &result,
                      ACE_INET_Addr &remote_address,
                      ACE_INET_Addr &local_address);

  /// Return the asynch accept object.
  ACE_Asynch_Accept &asynch_accept (void);

  /**
   * This is the template method used to create new handler.
   * Subclasses must overwrite this method if a new handler creation
   * strategy is required.
   */
  virtual HANDLER *make_handler (void);

private:
  /// Handle used to listen for new connections.
  ACE_HANDLE listen_handle_;

  /// <Asynch_Accept> used to make life easier :-)
  ACE_Asynch_Accept asynch_accept_;

  /// Flag that indicates if parsing of addresses is necessary.
  int pass_addresses_;

  /// Flag that indicates if address validation is required.
  int validate_new_connection_;

  /// Flag that indicates if a new accept should be reissued when a
  /// accept completes.
  int reissue_accept_;

  /// Bytes to be read with the <accept> call.
  int bytes_to_read_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Asynch_Acceptor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Asynch_Acceptor.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS */
#include "ace/post.h"
#endif /* ACE_ASYNCH_ACCEPTOR_H */
