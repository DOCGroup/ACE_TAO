// $Id$

#ifndef ACE_RMCAST_UDP_EVENT_HANDLER_H
#define ACE_RMCAST_UDP_EVENT_HANDLER_H
#include "ace/pre.h"

#include "ace/RMCast/RMCast_Export.h"
#include "ace/Demux/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_IO_UDP;
class ACE_INET_Addr;

//! Implement an Adapter for the ACE_RMCast_IO_UDP class
/*!
 * Applications may wish to use the ACE_Reactor to demultiplex I/O
 * events for an ACE_RMCast_IO_UDP object.  However other application
 * may choose to make ACE_RMCast_IO_UDP active, or they may dedicate
 * their own threads for its events.
 * To avoid couplin ACE_RMCast_IO_UDP with the Reactor we don't make
 * it derived from ACE_Event_Handler or any other class in the Reactor
 * framework, instead, this simple Adapter can forward the Reactor
 * messages to an ACE_RMCast_IO_UDP object.
 */
class ACE_RMCast_Export ACE_RMCast_UDP_Event_Handler : public ACE_Event_Handler
{
public:
  //! Constructor, save io_udp as the Adaptee in the Adapter pattern.
  ACE_RMCast_UDP_Event_Handler (ACE_RMCast_IO_UDP *io_udp);

  //! Destructor
  /*!
   * Notice that this class does not own the ACE_RMCast_IO_UDP
   * adaptee, so it does not destroy it.
   */
  ~ACE_RMCast_UDP_Event_Handler (void);

  //@{
  //! Documented in ACE_Event_Handler class
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);
  //@}

private:
  //! The adaptee
  ACE_RMCast_IO_UDP *io_udp_;
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_UDP_Event_Handler.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_UDP_EVENT_HANDLER_H */
