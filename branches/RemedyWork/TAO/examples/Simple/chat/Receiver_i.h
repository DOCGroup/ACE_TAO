/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Receiver_i.h
 *
 *  $Id$
 *
 *  Defines the implementation header for the Receiver interface.
 *
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
  // = Initialization and termination methods.
  /// Constructor.
  Receiver_i (void);

  /// Destructor.
  virtual ~Receiver_i (void);

  /// Receives a message string.
  virtual void message (const char *msg);

  /**
   * Called when the chat server is going away.  The client
   * implementation should shutdown the chat client in response to
   * this.
   */
  virtual void shutdown (void);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;
};

#endif /* RECEIVER_I_H  */
