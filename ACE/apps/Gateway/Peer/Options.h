// -*- C++ -*-

//=============================================================================
/**
 *  @file    Options.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef OPTIONS_H
#define OPTIONS_H

#include "../Gateway/Event.h"
#include "ace/svc_export.h"

/**
 * @class Options
 *
 * @brief Singleton that consolidates all Options for a peerd.
 */
class ACE_Svc_Export Options
{
public:
  // = Options that can be enabled/disabled.
  enum
  {
    VERBOSE = 01,
    SUPPLIER_ACCEPTOR = 02,
    CONSUMER_ACCEPTOR = 04,
    SUPPLIER_CONNECTOR = 010,
    CONSUMER_CONNECTOR = 020
  };

  /// Return Singleton.
  static Options *instance (void);

  /// Parse the arguments and set the options.
  void parse_args (int argc, ACE_TCHAR *argv[]);

  // = Accessor methods.
  /// Determine if an option is enabled.
  int enabled (int option) const;

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

  /// Our connector port host, i.e., the host running the gatewayd
  /// process.
  const ACE_TCHAR *connector_host (void) const;

  /// Duration between disconnects.
  long timeout (void) const;

  /// The maximum size of the queue.
  long max_queue_size (void) const;

  /// Returns a reference to the connection id.
  CONNECTION_ID &connection_id (void);

private:
  enum
  {
    MAX_QUEUE_SIZE = 1024 * 1024 * 16,
    // We'll allow up to 16 megabytes to be queued per-output
    // channel!!!!  This is clearly a policy in search of
    // refinement...

    DEFAULT_TIMEOUT = 60
    // By default, disconnect the peer every minute.
  };

  /// Ensure Singleton.
  Options (void);

  /// Explain usage and exit.
  void print_usage_and_die (void);

  /// Singleton.
  static Options *instance_;

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

  /// Our connector host, i.e., where the gatewayd process is running.
  const ACE_TCHAR *connector_host_;

  /// The amount of time to wait before disconnecting from the Peerd.
  long timeout_;

  /// The maximum size that the queue can grow to.
  long max_queue_size_;

  /// The connection id.
  CONNECTION_ID connection_id_;
};

#endif /* OPTIONS_H */
