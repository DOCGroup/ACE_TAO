/* -*- C++ -*- */
/**
 * @file ECG_Simple_Mcast_EH.h
 *
 * $Id$
 *
 * @author Marina Spivak <marina@atdesk.com>
 *
 */
#ifndef TAO_ECG_SIMPLE_MCAST_EH_H
#define TAO_ECG_SIMPLE_MCAST_EH_H
#include /**/ "ace/pre.h"

#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/event_export.h"
#include "orbsvcs/Event/ECG_Adapters.h"
#include "ace/SOCK_Dgram_Mcast.h"

/**
 * @class TAO_ECG_Simple_Mcast_EH
 *
 * @brief Simple Event Handler for mcast messages.  Listens for
 *        messages on a single mcast group.  Useful in environments
 *        where event traffic is not separated into different groups.
 *        Calls handle_input (ACE_SOCK_Dgram_Mcast dgram) on a specified
 *        TAO_ECG_Dgram_Handler to inform it about message arrival.
 *
 *        NOT THREAD-SAFE.
 */
class TAO_RTEvent_Export TAO_ECG_Simple_Mcast_EH
  : public ACE_Event_Handler
  , public TAO_ECG_Handler_Shutdown
{
public:

  /// Initialization and termination methods.
  //@{
  /// Constructor.
  /// Messages received by this EH will be forwarded to the <recv>.
  /*
   * See comments for <receiver_> data member on why raw pointer is
   * used for the <recv> argument.
   */
  TAO_ECG_Simple_Mcast_EH (TAO_ECG_Dgram_Handler *recv);

  /// Destructor.
  virtual ~TAO_ECG_Simple_Mcast_EH (void);

  /// Join the specified multicast group and register itself with the
  /// reactor.
  /// To insure proper resource clean up, if open () is successful,
  /// the user MUST call shutdown () when handler is no longer needed
  /// (and its reactor still exists).
  int open (const char * mcast_addr,
            const ACE_TCHAR *net_if = 0);

  /// TAO_ECG_Handler_Shutdown method.
  /// Unsubscribe from the reactor and close the datagram.
  virtual int shutdown (void);
  //@}

  /// Main method - reactor callback.  Notify <receiver_> that
  /// <dgram_> is ready for reading.
  virtual int handle_input (ACE_HANDLE fd);

private:

  // Socket on which we listen for messages.
  ACE_SOCK_Dgram_Mcast dgram_;

  /// We callback to this object when a message arrives.
  /*
   * We can keep a raw pointer to the receiver (even though it may
   * be a refcounted object) because receiver guarantees
   * to notify us (by calling shutdown ()) before going away.
   *
   * We have to use raw pointer instead of a refcounting mechanism
   * here to avoid a circular refcounting dependency between
   * receiver and handler.
   */
  TAO_ECG_Dgram_Handler* receiver_;
};

#if defined(__ACE_INLINE__)
#include "ECG_Simple_Mcast_EH.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ECG_SIMPLE_Mcast_EH_H */
