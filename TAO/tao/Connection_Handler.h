/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tao
//
// = FILENAME
//   Connection_Handler.h
//
// = AUTHOR
//    Bala Natarajan  <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CONNECTION_HANDLER_H
#define TAO_CONNECTION_HANDLER_H
#include "ace/pre.h"

#include "ace/SOCK.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Connection_Cache_Manager.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORB_Core;
class TAO_ORB_Core_TSS_Resources;
class ACE_Reactor;
class ACE_Event_Handler;

class TAO_Connection_Handler
{
  // = TITLE
  //     TAO_Connection_Handler
  //
  // = DESCRIPTION
  //     This class is an abstraction for the connection handlers. The
  //     connections handler in every protocol can derive from this
  //     class as well as the ACE_Svc_Handler specialised for the
  //     right protocol. This way, most of the common code for the
  //     different protocls would be in this implementation. Further,
  //     this class wold be of immense use in storing the handlers in
  //     the Cache for TAO. This would help in purging entries which
  //     is generally accompanied by closing the open handles and
  //     deleting memory associated with the handlers.

  //     Note: This class has NOT abstracted the GIOP specific
  //     details. It is just to be safe so that, we can reuse this
  //     class for any messaging protocol underneath. This way we need
  //     not touch the Cache setup even when  using other protocols (I
  //     mean messaging). BUT, I doubt whether this abstraction will
  //     satisfy the needs of other messaging protocols. (will not?).

public:

  TAO_Connection_Handler (void);
  // Constructor

  TAO_Connection_Handler (TAO_ORB_Core *orb_core);
  // Constructor

  virtual ~TAO_Connection_Handler (void);
  // Destructor

  void cache_map_entry (
      TAO_Connection_Cache_Manager::HASH_MAP_ENTRY *entry);

  TAO_Connection_Cache_Manager::HASH_MAP_ENTRY *cache_map_entry (void);
  // Set/Get the Cache Map entry

  int make_idle (void);
  // Make ourselves ready for use

  void incr_ref_count (void);
  // Increment the reference count

  void decr_ref_count (void);
  // Decrement the reference count

  CORBA::Boolean is_registered (void);
  void is_registered (CORBA::Boolean);
  // Get and set method for the flag that indicates whether the
  // handler has been registered with the reactor or not.

  virtual ACE_HANDLE fetch_handle (void) = 0;
  // Get the underlying handle

protected:

  TAO_ORB_Core *orb_core (void);
  // Return our TAO_ORB_Core pointer

  TAO_ORB_Core_TSS_Resources* tss_resources (void);
  // Return our TSS Resources pointer

  int set_socket_option (ACE_SOCK &sock,
                         int snd_size,
                         int rcv_size);
  // Set options on the socket

  int svc_i (void);
  // This method is invoked from the svc () method of the Svc_Handler
  // Object.

  virtual int handle_input_i (ACE_HANDLE = ACE_INVALID_HANDLE,
                              ACE_Time_Value *max_wait_time = 0) = 0;
  // Need to be implemented by the underlying protocol

  ACE_Time_Value *handle_timeout_i (const ACE_Time_Value &,
                                    const void *);
  // Implementation of the method handle_timout () which would be
  // called when the buffering timer expires.

  int handle_cleanup_i (ACE_Reactor *reactor,
                        ACE_Event_Handler *handler);
  // Implementation of the call  handle_cleanup () in
  // Service_Handler.

private:

  TAO_ORB_Core *orb_core_;
  // Pointer to the TAO_ORB_Core

  TAO_ORB_Core_TSS_Resources *tss_resources_;
  // Cached tss resources of the ORB that activated this object.

  u_long ref_count_;
  // Reference count to the number of external references -- ie. the
  // count of the number of places our references are being held.

  TAO_Connection_Cache_Manager::HASH_MAP_ENTRY *cache_map_entry_;
  // The cache map entry -- where we are in the Connection Cache

  CORBA::Boolean is_registered_;
  // Are we registered with the reactor?
};

#if defined (__ACE_INLINE__)
#include "tao/Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

#endif /*TAO_CONNECTION_HANDLER_H*/
