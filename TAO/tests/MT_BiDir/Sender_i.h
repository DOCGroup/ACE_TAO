// -*- C++ -*-
#ifndef MT_BIDIR_RECEIVER_H
#define MT_BIDIR_RECEIVER_H
#include /**/ "ace/pre.h"

#include "SenderS.h"
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Manual_Event;
ACE_END_VERSIONED_NAMESPACE_DECL

/**
 * Simply print count how many bytes were received.
 */
class Sender_i
  : public virtual POA_Sender
{
public:
  /// Constructor
  Sender_i (CORBA::ULong no_clients,
            ACE_Manual_Event &event);

  ~Sender_i () override = default;

  // = The skeleton methods
  CORBA::Long receiver_object (Receiver_ptr recv) override;

  /// Public method defined locally
  void send_message ();

  Sender_i (Sender_i const &) = delete;
  Sender_i (Sender_i &&) = delete;
  Sender_i & operator= (Sender_i const &) = delete;
  Sender_i & operator= (Sender_i &&) = delete;

private:
  /// Synchronizing acces to this class
  TAO_SYNCH_MUTEX mutex_;

  /// An event for waking other threads
  ACE_Manual_Event &event_;

  /// An array of receiver pointers
  std::unique_ptr<Receiver_var[]> receivers_ {};

  /// Size of the <this->receivers_> array
  CORBA::ULong no_clients_;

  /// Receiver index
  CORBA::ULong last_index_ {};

  /// Payload that is being sent
  Receiver::Payload payload_;
};

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
