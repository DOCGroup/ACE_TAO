/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Concrete_Connection_Handlers.h
 *
 *  $Id$
 *
 *  These are all the subclasses of Connection_Handler that define the
 *  appropriate threaded/reactive Consumer/Supplier behavior.
 *
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef CONCRETE_CONNECTION_HANDLER
#define CONCRETE_CONNECTION_HANDLER

#include "Connection_Handler.h"

/**
 * @class Supplier_Handler
 *
 * @brief Handles reception of Events from Suppliers.
 *
 * Performs framing and error checking on Events.  Intended to
 * run reactively, i.e., in one thread of control using a
 * Reactor for demuxing and dispatching.
 */
class Supplier_Handler : public Connection_Handler
{
public:
  // = Initialization method.
  Supplier_Handler (const Connection_Config_Info &);

protected:
  // = All the following methods are upcalls, so they can be protected.

  /// Receive and process peer events.
  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);

  /// Receive an event from a Supplier.
  virtual int recv (ACE_Message_Block *&);

  /**
   * This delegates to the <Event_Channel> to do the actual
   * processing.  Typically, it forwards the <event> to its
   * appropriate Consumer.
   */
  int process (ACE_Message_Block *event);

  /// Keep track of event fragment to handle non-blocking recv's from
  /// Suppliers.
  ACE_Message_Block *msg_frag_;
};

/**
 * @class Consumer_Handler
 *
 * @brief Handles transmission of events to Consumers.
 *
 * Performs queueing and error checking.  Intended to run
 * reactively, i.e., in one thread of control using a Reactor
 * for demuxing and dispatching.  Also uses a Reactor to handle
 * flow controlled output connections.
 */
class Consumer_Handler : public Connection_Handler
{
public:
  // = Initialization method.
  Consumer_Handler (const Connection_Config_Info &);

  /// Send an event to a Consumer (may be queued if necessary).
  virtual int put (ACE_Message_Block *event,
                   ACE_Time_Value * = 0);

protected:
  /// Finish sending event when flow control conditions abate.
  virtual int handle_output (ACE_HANDLE);

  /// Perform a non-blocking put().
  int nonblk_put (ACE_Message_Block *mb);

  /// Send an event to a Consumer.
  virtual ssize_t send (ACE_Message_Block *);

  /// Receive and process shutdowns from a Consumer.
  virtual int handle_input (ACE_HANDLE);
};

/**
 * @class Thr_Consumer_Handler
 *
 * @brief Runs each <Consumer_Handler> in a separate thread.
 */
class Thr_Consumer_Handler : public Consumer_Handler
{
public:
  Thr_Consumer_Handler (const Connection_Config_Info &);

  /// Initialize the threaded Consumer_Handler object and spawn a new
  /// thread.
  virtual int open (void *);

  /// Send a message to a peer.
  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);

protected:
  /// Called when Peer shutdown unexpectedly.
  virtual int handle_input (ACE_HANDLE);

  /// Transmit peer messages.
  virtual int svc (void);

  /**
   * When thread started, connection become blocked, so no need to use
   * handle_close to reinitiate the connection_handler, so should
   * override this function to justify if controlling is in thread or
   * not. If yes, handle_close do nothing, otherwise, it call parent
   * handle_close().
   */
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

private:
  /// If the controlling is in thread's svc() or not.
  int in_thread_;
};

/**
 * @class Thr_Supplier_Handler
 *
 * @brief Runs each <Supplier_Handler> in a separate thread.
 */
class Thr_Supplier_Handler : public Supplier_Handler
{
public:
  Thr_Supplier_Handler (const Connection_Config_Info &pci);

  /// Initialize the object and spawn a new thread.
  virtual int open (void *);

protected:
  /**
   * When thread started, connection become blocked, so no need to use
   * handle_close to reinitiate the connection_handler, so should
   * override this function to justify if controlling is in thread or
   * not. If yes, handle_close do nothing, otherwise, it call parent
   * handle_close().
   */
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

  /// Transmit peer messages.
  virtual int svc (void);

private:
  /// If the controlling is in thread's svc() or not.
  int in_thread_;
};

#endif /* CONCRETE_CONNECTION_HANDLER */
