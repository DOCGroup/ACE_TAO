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
  Receiver_i ();

  /// Destructor.
  virtual ~Receiver_i ();

  /// Receives a message string.
  virtual void message (const char *msg);

  /**
   * Called when the chat server is going away.  The client
   * implementation should shutdown the chat client in response to
   * this.
   */
  virtual void shutdown ();

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;
};

#endif /* RECEIVER_I_H  */
