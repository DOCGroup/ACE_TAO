// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSL_Connector.h
 *
 *  $Id$
 *
 *  ACE_SSL-specific implementations of the core classes in
 *  ace/Connector.h.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef ACE_SSL_CONNECTOR_H
#define ACE_SSL_CONNECTOR_H

#include "ace/pre.h"

#include "ace/Connector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class ACE_SSL_Connector
 *
 * @brief Class that facilitates non-blocking SSL connections by use
 *        of the Template Method design pattern.
 *
 * The standard ACE_Connector class assumes that a connection is
 * established once the stream handle becomes writable.  However, that
 * only holds for protocols with a single handshake, such as TCP.
 * Protocols with multiple handshakes, such as SSL, require special
 * handling to prevent the connection from being considered completed
 * once the stream handle becomes writable.
 */
template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
class ACE_SSL_Connector
  : public ACE_Connector<SVC_HANDLER, ACE_PEER_CONNECTOR_2>
{
public:

  /// Constructor.
  /**
   * @note See the ACE_Connector constructor documentation for
   *       details.
   */
  ACE_SSL_Connector (ACE_Reactor *r = ACE_Reactor::instance (),
                     int flags = 0);

protected:

  /**
   * handle_input() is normally called when a non-blocking connection
   * fails, i.e. when the stream handle becomes readable.  However,
   * once the first (TCP) handshake is completed, the second (SSL)
   * handshake may need to perform reads.  In that case, the stream
   * handle becoming readable should not be considered a connection
   * failure.
   */
  virtual int handle_input (ACE_HANDLE);

  /**
   * handle_output() is normally called when a non-blocking connection
   * succeeds, i.e. when the stream handle becomes writable.  However,
   * this only applies for single handshake protocols, such as TCP.
   * Multi-handshake protocols, such as SSL, require additional
   * handling once the initial handshake is completed.
   */
  virtual int handle_output (ACE_HANDLE);

};

// ------------------------------------------------------------

/**
 * @class ACE_SSL_Strategy_Connector
 *
 * @brief Class that facilitates non-blocking SSL connections by use
 *        of the Template Method design pattern.
 *
 * This class is analogous to the ACE_Strategy_Connector class, but
 * utilizes the functionality introduced by the ACE_SSL_Connector
 * class to handle non-blocking SSL connections correctly.
 */
template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
class ACE_SSL_Strategy_Connector
  : public ACE_Strategy_Connector <SVC_HANDLER, ACE_PEER_CONNECTOR_2>
{
public:

  ACE_SSL_Strategy_Connector (ACE_Reactor *r = ACE_Reactor::instance (),
                              ACE_Creation_Strategy<SVC_HANDLER> * = 0,
                              ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2> * = 0,
                              ACE_Concurrency_Strategy<SVC_HANDLER> * = 0,
                              int flags = 0);

protected:

  /**
   * handle_input() is normally called when a non-blocking connection
   * fails, i.e. when the stream handle becomes readable.  However,
   * once the first (TCP) handshake is completed, the second (SSL)
   * handshake may need to perform reads.  In that case, the stream
   * handle becoming readable should not be considered a connection
   * failure.
   */
  virtual int handle_input (ACE_HANDLE);

  /**
   * handle_output() is normally called when a non-blocking connection
   * succeeds, i.e. when the stream handle becomes writable.  However,
   * this only applies for single handshake protocols, such as TCP.
   * Multi-handshake protocols, such as SSL, require additional
   * handling once the initial handshake is completed.
   */
  virtual int handle_output (ACE_HANDLE);

};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "SSL_Connector.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("SSL_Connector.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include "ace/post.h"

#endif  /* ACE_SSL_CONNECTOR_H */
