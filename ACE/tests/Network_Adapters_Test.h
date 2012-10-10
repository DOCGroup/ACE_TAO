// -*- C++ -*-

//=============================================================================
/**
 *  @file    Network_Adapters_Test.h
 *
 *  $Id$
 *
 *  Definitions for Network_Adapters_Test.cpp.
 *
 *
 *  @author Robert S. Iakobashvili <roberti@go-WLAN.com> <coroberti@walla.co.il> Gonzalo A. Diethelm <gonzalo.diethelm@aditiva.com> made aceing
 */
//=============================================================================


#ifndef ACE_NETWORK_ADAPTERS_TEST_H
#define ACE_NETWORK_ADAPTERS_TEST_H

#include "ace/Ping_Socket.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_ICMP_SUPPORT) && (ACE_HAS_ICMP_SUPPORT == 1)

#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/Event_Handler.h"
#include "ace/Mutex.h"

/*
 *  Echo_Handler performs echo-checks against a single ICMP echo-point.
 */
class Echo_Handler : public ACE_Event_Handler
{
public:
  // = Initialization and termination methods.

  /// Default constructor
  Echo_Handler (void);

  /// Destructor
  virtual ~Echo_Handler (void);

  /**
   * Initialization of a handler, performing echo-checks against a
   * SINGLE echo-point (supposed to be a really reliable, like hub,
   * router).
   *
   * <reactor> - to be used for demultiplexing of any input and
   * timeout
   * <reply_wait> - time to wait for reply
   * <remote_addr> - pointer to the remote address to sent to ICMP
   * ECHO_CHECK datagram
   * <success_status> - a pointer to char to be set as a 0 - on
   * success, and 1 - when failed
   * <max_attempts_num> - maximum number of attempts to perform
   * <local_addr> - the local address to bind the underlaying
   * ACE::Ping_Socket; useful for checks of local network adapters
   * <connect_to_remote> - whether to connect the underlaying
   * ACE::Ping_Socket to the remote address (1), or not (0)
   */
  int open (ACE_Reactor * const    reactor,
            const ACE_Time_Value & reply_wait,
            const ACE_INET_Addr &  remote_addr,
            ACE_TCHAR *            success_status = 0,
            size_t                 max_attempts_num = 1,
            const ACE_Addr &       local_addr = ACE_Addr::sap_any,
            int                    connect_to_remote = 0);

  /**
   * Initialization of a handler, performing echo-checks against
   * MULTIPLE echo-points.
   *
   * <reactor> - to be used for demultiplexing of any input and
   * timeout;
   * <reply_wait> - time to wait for reply;
   * <remote_addrs> - an array of remote addresses to sent to ICMP
   * ECHO_CHECK datagram;
   * <success_status> - an array of chars, each of them representing
   * a respective remote address to be set as a 0 - on success, and
   * 1 - when failed;
   * <max_attempts_num> - maximum number of attempts to perform;
   * <local_addr> - the local address to bind the underlaying
   * ACE::Ping_Socket; useful for checks of the local network adapters
   * connectivity;
   */
  int open (ACE_Reactor * const    reactor,
            ACE_Time_Value const & reply_wait,
            ACE_INET_Addr const    remote_addrs[],
            size_t                 number_remotes,
            ACE_TCHAR *            success_status = 0,
            size_t                 max_attempts_num = 1,
            ACE_Addr const &       local_addr = ACE_Addr::sap_any);

  /// Returns reference to the ACE::Ping_Socket. Necessary for ACE_Reactor.
  virtual ACE_HANDLE get_handle (void) const;

  /**
   * Takes care of the input. Reads the incoming ICMP datagrams and
   * calls for process_incoming () of the ping_socket for processing.
   */
  virtual int handle_input (ACE_HANDLE handle);

  /*
   * Decides, if we need to continue checks (when at least a single
   * address not returned ICMP_ECHO_REPLY and number of attempts,
   * set in open () not expired).  If yes, calls for dispatch_echo_checks (),
   * if not returns -1 to initiate clean-up.
   */
  virtual int handle_timeout (ACE_Time_Value const & tv,
                              void const * arg = 0);

  /// Makes clean-up
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  /// Calls send_echo_check() for all remote_addrs_
  int dispatch_echo_checks (int first_call = 0);

  /// Access to ping_socket.
  ACE_Ping_Socket& ping_socket (void);

  /// Returns 1 if a single char of success_status_ is 0
  /// (connected).
  int does_echo_test_successful (void);

private:

  /// Wrapper for sending/receiving ICMPs.
  ACE_Ping_Socket ping_socket_;

  /// Time to wait for reply.
  ACE_Time_Value reply_wait_;

  /// Remote address to ping on it
  ACE_INET_Addr *remote_addrs_;

  /// Number of remote echo points
  size_t number_remotes_;

  /// When 0 - success, 1 - failed.
  ACE_TCHAR *success_status_;

  /// If 1 - we 'own'
  int delete_success_status_;

  /// Maximum number of attempts.
  size_t max_attempts_num_;

  /// The number of the current attempt.
  size_t current_attempt_;

  /// Whether to make connect to the remote address or not. May be
  /// buggy on some platforms.
  int connect_to_remote_;
};


/*
 * Class Stop_Handler - the most important class of the process.
 * Knows how to stop all this business.
 */
class Stop_Handler : public ACE_Event_Handler
{
public:
#ifdef ACE_HAS_THREADS
  typedef ACE_Atomic_Op<ACE_Mutex, long> counter_sig;
#else
  typedef long counter_sig;
#endif

  // Constructor.
  Stop_Handler (ACE_Reactor * const reactor = ACE_Reactor::instance ());

  // Destructor.
  virtual ~Stop_Handler (void);

  // Initialization. Registers this for SIGINT, SIGTERM and SIGQUIT.
  virtual int open (void);

  // De-registers this from the reactor and stops reactors event_loop.
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  // Called by reactor from the notification queue.
  virtual int handle_input (ACE_HANDLE);

  // Dispatches handle_input () notification.
  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0);

  virtual int handle_timeout (ACE_Time_Value const & current_time,
                              void const * act = 0);

  // Register handler with us for stopping.
  virtual int register_handler (ACE_Event_Handler * handler);

  // Unregister handler, registered before with us for stopping.
  virtual int unregister_handler (ACE_Event_Handler * handler);

private:

  enum
    {
      HANDLERS_TO_STOP_TABLE_SIZE = 10
    };

  // Flag to prevent multiple dispatching of handle_input ().
  counter_sig counter_;

  // Table to place here pointers to all tasks in the process.
  ACE_Event_Handler * handlers_to_stop_[HANDLERS_TO_STOP_TABLE_SIZE];

  ACE_Sig_Set registered_signals_;
};


/*
 * TODO comment
 */
class Repeats_Handler : public ACE_Event_Handler
{
public:
  // Constructor.
  Repeats_Handler (void);

  // Destructor.
  virtual ~Repeats_Handler (void);

  // Initialization.
  virtual int open (Echo_Handler *check_handler,
                    ACE_Reactor * const reactor = ACE_Reactor::instance (),
                    unsigned int seconds_timer = 60);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  // dispatches a new echo-checks series
  virtual int handle_timeout (ACE_Time_Value const & current_time,
                              void const * act = 0);

private:
  // an instance of a handler
  Echo_Handler * check_handler_;

  // timer in seconds to repeat the checks
  unsigned int seconds_timer_;

  // counts repeats
  unsigned long counter_;
};

#endif  /* ACE_HAS_ICMP_SUPPORT == 1 */

#endif /* ACE_NETWORK_ADAPTERS_TEST_H */
