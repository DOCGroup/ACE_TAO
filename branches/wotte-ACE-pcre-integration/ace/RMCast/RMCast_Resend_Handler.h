// $Id$

#ifndef ACE_RMCAST_RESEND_HANDLER_H
#define ACE_RMCAST_RESEND_HANDLER_H
#include /**/ "ace/pre.h"

#include "RMCast_Export.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Retransmission;

/// Implement an adapter to resend messages in the
/// ACE_RMCast_Retransmission layer, but based on Reactor based
/// timeouts.
class ACE_RMCast_Export ACE_RMCast_Resend_Handler : public ACE_Event_Handler
{
public:
  /// Constructor, save io_udp as the Adaptee in the Adapter pattern.
  ACE_RMCast_Resend_Handler (ACE_RMCast_Retransmission *retransmission);

  /// Destructor
  ~ACE_RMCast_Resend_Handler (void);

  //@{
  //! Documented in ACE_Event_Handler class
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);
  //@}

private:
  //! The adaptee
  ACE_RMCast_Retransmission *retransmission_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Resend_Handler.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_RMCAST_RESEND_HANDLER_H */
