// $Id$

// ============================================================================
//
// = DESCRIPTION
//   Implement an adapter between the ACE Reactor and the
//   ACE_RMCast_IO_UDP
//
// = AUTHOR
//   Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_UDP_EVENT_HANDLER_H
#define ACE_RMCAST_UDP_EVENT_HANDLER_H
#include "ace/pre.h"

#include "RMCast_Export.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_IO_UDP;
class ACE_INET_Addr;

class ACE_RMCast_Export ACE_RMCast_UDP_Event_Handler : public ACE_Event_Handler
{
public:
  ACE_RMCast_UDP_Event_Handler (ACE_RMCast_IO_UDP *receiver);
  // Constructor

  ~ACE_RMCast_UDP_Event_Handler (void);
  // Destructor

  // = The Event_Handler methods
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);

private:
  ACE_RMCast_IO_UDP *io_udp_;
  // The sender
};

#if defined (__ACE_INLINE__)
#include "RMCast_UDP_Event_Handler.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_UDP_EVENT_HANDLER_H */
