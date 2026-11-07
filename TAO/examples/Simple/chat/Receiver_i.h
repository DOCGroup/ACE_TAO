/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Receiver_i.h
 *
 *  Defines the implementation header for the Receiver interface.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef RECEIVER_I_H
#define RECEIVER_I_H

#include "ReceiverS.h"

/**
 * @class Receiver_i
 *
 * @brief Receiver object implementation
 *
 * This class has methods that are called by the chat server.
 */
class Receiver_i : public POA_Receiver
{
public:
  /// Constructor.
  Receiver_i () = default;

  /// Destructor.
  ~Receiver_i () override = default;

  Receiver_i (Receiver_i const &) = delete;
  Receiver_i &operator= (Receiver_i const &) = delete;
  Receiver_i (Receiver_i &&) = delete;
  Receiver_i &operator= (Receiver_i &&) = delete;

  /// Receives a message string.
  void message (char const *msg) override;

  /**
   * Called when the chat server is going away.  The client
   * implementation should shutdown the chat client in response to
   * this.
   */
  void shutdown () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_ {};
};

#endif /* RECEIVER_I_H  */
