/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Broadcaster_i.h
 *
 *  Defines the implementation header for the Broadcaster interface.
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
  /// Constructor.
  Broadcaster_i () = default;

  /// Destructor.
  ~Broadcaster_i () override = default;

  Broadcaster_i (Broadcaster_i const &) = delete;
  Broadcaster_i &operator= (Broadcaster_i const &) = delete;
  Broadcaster_i (Broadcaster_i &&) = delete;
  Broadcaster_i &operator= (Broadcaster_i &&) = delete;

  /// Saves receiver references in a list.
  void add (Receiver_ptr receiver,
            char const *nickname) override;

  /// Removes receiver references from the list.
  void remove (Receiver_ptr receiver) override;

  /// Called by Broadcaster clients to send messages.
  void say (Receiver_ptr receiver,
            char const *text) override;

public:
  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// Broadcasts the text to all registered clients.
  void broadcast (char const *text);

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
    bool operator == (Receiver_Data const &receiver_data) const;

    /// Stores the receiver reference.
    Receiver_var receiver_;

    /// Stores the client nickname.
    ACE_CString nickname_;
  };

  using RECEIVER_SET = ACE_Unbounded_Set<Receiver_Data>;
  using RECEIVER_SET_ITERATOR = ACE_Unbounded_Set_Iterator<Receiver_Data>;

  /// Set of registered clients.
  RECEIVER_SET receiver_set_;
};

#endif /* BROADCASTER_I_H */
