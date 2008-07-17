// -*- C++ -*-

//=============================================================================
/**
 *  @file PIP_Message_Handler.h
 * 
 *
 *  @author John Moore
 */
//=============================================================================


#ifndef _PIP_MESSAGE_HANDLER_H_
#define _PIP_MESSAGE_HANDLER_H_

#include "ace/Message_Block.h"
#include "ace/PIP_Messages.h"
#include "ace/Event_Handler.h"

class ACE_Export ACE_PIP_Message_Handler
{
 public:

  ACE_PIP_Message_Handler(ACE_UINT32 site_id, ACE_UINT32 message_id);
  ACE_PIP_Message_Handler();
  virtual ~ACE_PIP_Message_Handler(){}

  virtual void process_incoming_message(ACE_Message_Block* message, 
                                        ACE_UINT64 message_id) = 0;
  
  ACE_UINT32 get_handler_id() const;
  void set_handler_id(ACE_UINT32 id);

  ACE_UINT32 get_site_id() const;
  void set_site_id(ACE_UINT32 id);

  
  
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
  
  ACE_PIP_Protocol_Message* create_protocol_message(ACE_UINT64 message_id,
                                                    bool       reply_expected,
                                                    ACE_UINT32 source_handler_id,
                                                    ACE_UINT32 source_site_id,
                                                    ACE_UINT32 destination_handler_id,
                                                    ACE_UINT32 destination_site_id,
                                                    ACE_UINT32 message_priority,
                                                    ACE_PIP_Protocol_Message::Message_Type type,
                                                    const std::string& data_payload);
  
};

#endif

