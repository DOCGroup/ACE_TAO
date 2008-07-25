// -*- C++ -*-

//=============================================================================
/**
 *  @file PIP_Message_Handler.h
 * 
 *  $Id:
 *
 *  @author John Moore <ljohn7@gmail.com>
 */
//=============================================================================


#ifndef _PIP_MESSAGE_HANDLER_H_
#define _PIP_MESSAGE_HANDLER_H_

#include <ace/Message_Block.h>
#include <ace/PIP_Messages.h>
#include <ace/Event_Handler.h>
#include <ace/PIP_Invocation_Manager.h>

/**
 * @class ACE_PIP_Message_Handler
 * @brief ACE_PIP_Message_Handler is a user-level message handler interface.
 *        
 * ACE_PIP_Message_Handler is a user-level message handler interface. It can
 * be extended to provide user-level processing of messages before passing
 * the message block to the priority-inheritance protocol infrastructure.
 *
 * Each handler has an associated (site id, handler id) tuple with which
 * it can be address by other handlersy
 */
class ACE_Export ACE_PIP_Message_Handler
{
  public:
   
   /// Constructor that sets site and handler ID
   ACE_PIP_Message_Handler(ACE_UINT32 site_id, ACE_UINT32 handler_id);
   ACE_PIP_Message_Handler();

   /// Destroy handler
   virtual ~ACE_PIP_Message_Handler(){}
   
   /// Process message received by priority inheritance infrastructure
   virtual void process_incoming_message(ACE_Message_Block* message, 
                                         ACE_UINT64 message_id) = 0;
                                                
   /// Get the handler id
   ACE_UINT32 handler_id() const;

   /// Set the handler id
   void handler_id(ACE_UINT32 id);
   
   /// Get the site id
   ACE_UINT32 site_id() const;

   /// Set the site id
   void site_id(ACE_UINT32 id);

  protected:
   
   ACE_UINT32 handler_id_;
   ACE_UINT32 site_id_;
   ACE_INET_Addr my_address_;
   
   // Pass a message to a remote handler
   virtual void send_request(ACE_Message_Block*  message,
                             ACE_UINT64          message_id,
                             ACE_Message_Block*& response);
   
   // Pass a response message to a remote handler
   virtual void send_response(ACE_Message_Block* message,
                              ACE_UINT64         message_id);
      
};

#if defined (__ACE_INLINE__)
#include "PIP_Message_Handler.inl"
#endif /* __ACE_INLINE__ */

#endif

