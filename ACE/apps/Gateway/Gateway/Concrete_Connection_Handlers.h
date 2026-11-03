/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Concrete_Connection_Handlers.h
 *
 *  These are all the subclasses of Connection_Handler that define the
 *  appropriate threaded/reactive Consumer/Supplier behavior.
 *
 *  @author Doug Schmidt <d.schmidt@vanderbilt.edu>
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
  explicit Supplier_Handler (const Connection_Config_Info &);
  Supplier_Handler (Supplier_Handler const &) = delete;
  Supplier_Handler (Supplier_Handler &&) = delete;
  Supplier_Handler &operator= (Supplier_Handler const &) = delete;
  Supplier_Handler &operator= (Supplier_Handler &&) = delete;

protected:
  // = All the following methods are upcalls, so they can be protected.

  /// Receive and process peer events.
  int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE) override;

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
  explicit Consumer_Handler (const Connection_Config_Info &);
  Consumer_Handler (Consumer_Handler const &) = delete;
  Consumer_Handler (Consumer_Handler &&) = delete;
  Consumer_Handler &operator= (Consumer_Handler const &) = delete;
  Consumer_Handler &operator= (Consumer_Handler &&) = delete;

  /// Send an event to a Consumer (may be queued if necessary).
  int put (ACE_Message_Block *event,
           ACE_Time_Value * = nullptr) override;

protected:
  /// Finish sending event when flow control conditions abate.
  int handle_output (ACE_HANDLE) override;

  /// Perform a non-blocking put().
  int nonblk_put (ACE_Message_Block *mb);

  /// Send an event to a Consumer.
  virtual ssize_t send (ACE_Message_Block *);

  /// Receive and process shutdowns from a Consumer.
  int handle_input (ACE_HANDLE) override;
};

/**
 * @class Thr_Consumer_Handler
 *
 * @brief Runs each <Consumer_Handler> in a separate thread.
 */
class Thr_Consumer_Handler : public Consumer_Handler
{
public:
  explicit Thr_Consumer_Handler (const Connection_Config_Info &);
  Thr_Consumer_Handler (Thr_Consumer_Handler const &) = delete;
  Thr_Consumer_Handler (Thr_Consumer_Handler &&) = delete;
  Thr_Consumer_Handler &operator= (Thr_Consumer_Handler const &) = delete;
  Thr_Consumer_Handler &operator= (Thr_Consumer_Handler &&) = delete;

  /// Initialize the threaded Consumer_Handler object and spawn a new
  /// thread.
  int open (void *) override;

  /// Send a message to a peer.
  int put (ACE_Message_Block *, ACE_Time_Value * = nullptr) override;

protected:
  /// Called when Peer shutdown unexpectedly.
  int handle_input (ACE_HANDLE) override;

  /// Transmit peer messages.
  int svc () override;

  /**
   * When thread started, connection become blocked, so no need to use
   * handle_close to reinitiate the connection_handler, so should
   * override this function to justify if controlling is in thread or
   * not. If yes, handle_close do nothing, otherwise, it call parent
   * handle_close().
   */
  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK) override;

private:
  /// If the controlling is in thread's svc() or not.
  bool in_thread_;
};

/**
 * @class Thr_Supplier_Handler
 *
 * @brief Runs each <Supplier_Handler> in a separate thread.
 */
class Thr_Supplier_Handler : public Supplier_Handler
{
public:
  explicit Thr_Supplier_Handler (const Connection_Config_Info &pci);
  Thr_Supplier_Handler (Thr_Supplier_Handler const &) = delete;
  Thr_Supplier_Handler (Thr_Supplier_Handler &&) = delete;
  Thr_Supplier_Handler &operator= (Thr_Supplier_Handler const &) = delete;
  Thr_Supplier_Handler &operator= (Thr_Supplier_Handler &&) = delete;

  /// Initialize the object and spawn a new thread.
  int open (void *) override;

protected:
  /**
   * When thread started, connection become blocked, so no need to use
   * handle_close to reinitiate the connection_handler, so should
   * override this function to justify if controlling is in thread or
   * not. If yes, handle_close do nothing, otherwise, it call parent
   * handle_close().
   */
  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK) override;

  /// Transmit peer messages.
  int svc () override;

private:
  /// If the controlling is in thread's svc() or not.
  bool in_thread_;
};

#endif /* CONCRETE_CONNECTION_HANDLER */
