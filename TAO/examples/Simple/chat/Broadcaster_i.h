/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Broadcaster_i.h
 *
 *  $Id$
 *
 *  Defines the implementation header for the Broadcaster interface.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef BROADCASTER_I_H
#define BROADCASTER_I_H

#include "BroadcasterS.h"
#include "ReceiverC.h"
#include "tao/Utils/ORB_Manager.h"
#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"

/**
 * @class Broadcaster_i
 *
 * @brief The implementation of the Broadcaster class, which is the
 * servant object for the chat server.
 */
class Broadcaster_i : public POA_Broadcaster
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Broadcaster_i (void);

  /// Destructor.
  virtual ~Broadcaster_i (void);

  /// Saves receiver references in a list.
  virtual void add (Receiver_ptr receiver,
                    const char *nickname);

  /// Removes receiver references from the list.
  virtual void remove (Receiver_ptr receiver);

  /// Called by Broadcaster clients to send messages.
  virtual void say (Receiver_ptr receiver,
                    const char *text);

public:
  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// Broadcasts the text to all registered clients.
  void broadcast (const char* text);

  /**
   * @class Receiver_Data
   *
   * @brief Per-client info.
   *
   * Saves the Receiver_var and user nickname.
   */
  class Receiver_Data
  {
  public:
    /// The == op required by the ACE_Unbounded set.
    bool operator == (const Receiver_Data &receiver_data) const;

    /// Stores the receiver reference.
    Receiver_var receiver_;

    /// Stores the client nickname.
    ACE_CString nickname_;
  };

  typedef ACE_Unbounded_Set<Receiver_Data>
          RECEIVER_SET;
  typedef ACE_Unbounded_Set_Iterator<Receiver_Data>
          RECEIVER_SET_ITERATOR;

  /// Set of registered clients.
  RECEIVER_SET receiver_set_;
};

#endif /* BROADCASTER_I_H */
