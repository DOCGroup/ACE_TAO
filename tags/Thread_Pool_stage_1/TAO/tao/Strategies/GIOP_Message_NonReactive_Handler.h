// This may look like C, but it's really -*- C++ -*-
// -*- C++ -*-
// ===================================================================
/**
 *  @file   GIOP_Message_NonReactive_Handler.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 **/
// ===================================================================

#ifndef TAO_GIOP_MESSAGE_NON_REACTIVE_HANDLER_H
#define TAO_GIOP_MESSAGE_NON_REACTIVE_HANDLER_H
#include "ace/pre.h"

#include "strategies_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CDR.h"
#include "tao/GIOP_Message_Reactive_Handler.h"

class TAO_Transport;
class TAO_ORB_Core;
class TAO_GIOP_Message_NonReactive_Base;



/**
 * @class TAO_GIOP_Message_NonReactive_Handler
 *
 * @brief GIOP specific non-reactive message handler class
 *
 * This class does some of the message handling for GIOP. The class
 * is non-reactive, in the sense that the read calls on the socket
 * wait till all the data relevant for a GIOP message is read. This
 * strategy can be used by protocols which uses the reactor for
 * signaling rather than for data arrival.
 *
 * @todo Need to investigate why this strategy would be required at a
 *  higher level??
 */

class TAO_Strategies_Export TAO_GIOP_Message_NonReactive_Handler : public TAO_GIOP_Message_Reactive_Handler
{
public:

  /// Ctor
  TAO_GIOP_Message_NonReactive_Handler (TAO_ORB_Core *orb_core,
                                        TAO_GIOP_Message_NonReactive_Base *mesg_base,
                                        size_t input_cdr_size = ACE_CDR::DEFAULT_BUFSIZE);

  /// This call reads the message out of the socket and does parsing
  /// of the GIOP header.
  int read_parse_message (TAO_Transport *transport,
                          ACE_Time_Value *max_wait_time = 0);

  /// Return the CDR stream
  TAO_InputCDR &input_cdr (void);

private:

  /// Helper function that reads data from the transport to the buffer
  /// pointed by <buf> of size <size>. We assume that the transport
  /// has been set in the blocking mode for reading. Read is performed
  /// as long as we get all the data.
  int read_message (TAO_Transport *transport,
                    char *buf,
                    size_t size,
                    ACE_Time_Value *max_wait_time);

private:

  /// Our Message base
  TAO_GIOP_Message_NonReactive_Base *mesg_base_;

  /// The input cdr stream in which the incoming data is stored.
  TAO_InputCDR input_cdr_;
};

#if defined (__ACE_INLINE__)
# include "GIOP_Message_NonReactive_Handler.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_NON_REACTIVE_HANDLER_H*/
