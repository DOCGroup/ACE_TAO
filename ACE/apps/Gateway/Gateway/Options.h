/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Options.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/svc_export.h"
#include "ace/Thread_Mutex.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/Synch_Traits.h"

/**
 * @class Options
 *
 * @brief Singleton that consolidates all Options for a gatewayd.
 */
class ACE_Svc_Export Options
{
public:
  // = Options that can be enabled/disabled.
  enum
  {
    // = The types of threading strategies.
    REACTIVE = 0,
    OUTPUT_MT = 1,
    INPUT_MT = 2,

    VERBOSE = 01,
    DEBUGGING = 02,

    SUPPLIER_ACCEPTOR = 04,
    CONSUMER_ACCEPTOR = 010,
    SUPPLIER_CONNECTOR = 020,
    CONSUMER_CONNECTOR = 040
  };

  /// Return Singleton.
  static Options *instance (void);

  /// Termination.
  ~Options (void);

  /// Parse the arguments and set the options.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /**
   * Print the gateway supported parameters.
   * = Accessor methods.
   * Determine if an option is enabled.
   */
  void print_usage(void);
  int enabled (int option) const;

  /**
   * Gets the locking strategy used for serializing access to the
   * reference count in <ACE_Message_Block>.  If it's 0, then there's
   * no locking strategy and we're using a REACTIVE concurrency
   * strategy.
   */
  ACE_Lock_Adapter<ACE_SYNCH_MUTEX> *locking_strategy (void) const;

  /// Set the locking strategy used for serializing access to the
  /// reference count in <ACE_Message_Block>.
  void locking_strategy (ACE_Lock_Adapter<ACE_SYNCH_MUTEX> *);

  /// Number of seconds after connection establishment to report
  /// throughput.
  long performance_window (void) const;

  /// 0 == blocking connects, ACE_NONBLOCK == non-blocking connects.
  int blocking_semantics (void) const;

  /// Size of the socket queue (0 means "use default").
  int socket_queue_size (void) const;

  /// i.e., REACTIVE, OUTPUT_MT, and/or INPUT_MT.
  u_long threading_strategy (void) const;

  /**
   * Our acceptor port number, i.e., the one that we passively listen
   * on for connections to arrive from a gatewayd and create a
   * Supplier.
   */
  u_short supplier_acceptor_port (void) const;

  /**
   * Our acceptor port number, i.e., the one that we passively listen
   * on for connections to arrive from a gatewayd and create a
   * Consumer.
   */
  u_short consumer_acceptor_port (void) const;

  /// The connector port number, i.e., the one that we use to actively
  /// establish connections with a gatewayd and create a Supplier.
  u_short supplier_connector_port (void) const;

  /// The connector port number, i.e., the one that we use to actively
  /// establish connections with a gatewayd and create a Consumer.
  u_short consumer_connector_port (void) const;

  /// Name of the connection configuration file.
  const ACE_TCHAR *connection_config_file (void) const;

  /// Name of the consumer map configuration file.
  const ACE_TCHAR *consumer_config_file (void) const;

  /// The maximum retry timeout delay.
  long max_timeout (void) const;

  /// The maximum size of the queue.
  long max_queue_size (void) const;

  /// Returns a reference to the next available connection id;
  CONNECTION_ID &connection_id (void);

private:
  enum
  {
    MAX_QUEUE_SIZE = 1024 * 1024 * 16,
    // We'll allow up to 16 megabytes to be queued per-output proxy.

    MAX_TIMEOUT = 32
    // The maximum timeout for trying to re-establish connections.
  };

  /// Initialization.
  Options (void);

  /// Options Singleton instance.
  static Options *instance_;

  /**
   * Points to the locking strategy used for serializing access to the
   * reference count in <ACE_Message_Block>.  If it's 0, then there's
   * no locking strategy and we're using a REACTIVE concurrency
   * strategy.
   */
  ACE_Lock_Adapter<ACE_SYNCH_MUTEX> *locking_strategy_;

  /// Number of seconds after connection establishment to report
  /// throughput.
  long performance_window_;

  /// 0 == blocking connects, ACE_NONBLOCK == non-blocking connects.
  int blocking_semantics_;

  /// Size of the socket queue (0 means "use default").
  int socket_queue_size_;

  /// i.e., REACTIVE, OUTPUT_MT, and/or INPUT_MT.
  u_long threading_strategy_;

  /// Flag to indicate if we want verbose diagnostics.
  u_long options_;

  /**
   * The acceptor port number, i.e., the one that we passively listen
   * on for connections to arrive from a gatewayd and create a
   * Supplier.
   */
  u_short supplier_acceptor_port_;

  /**
   * The acceptor port number, i.e., the one that we passively listen
   * on for connections to arrive from a gatewayd and create a
   * Consumer.
   */
  u_short consumer_acceptor_port_;

  /// The connector port number, i.e., the one that we use to actively
  /// establish connections with a gatewayd and create a Supplier.
  u_short supplier_connector_port_;

  /// The connector port number, i.e., the one that we use to actively
  /// establish connections with a gatewayd and create a Consumer.
  u_short consumer_connector_port_;

  /// The maximum retry timeout delay.
  long max_timeout_;

  /// The maximum size of the queue.
  long max_queue_size_;

  /// The next available connection id.
  CONNECTION_ID connection_id_;

  /// Name of the connection configuration file.
  ACE_TCHAR connection_config_file_[MAXPATHLEN + 1];

  /// Name of the consumer map configuration file.
  ACE_TCHAR consumer_config_file_[MAXPATHLEN + 1];
};

#endif /* OPTIONS_H */
