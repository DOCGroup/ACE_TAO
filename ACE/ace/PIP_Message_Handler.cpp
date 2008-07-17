#include "ace/PIP_Message_Handler.h"
#include "ace/PIP_Invocation_Manager.h"

ACE_PIP_Message_Handler::ACE_PIP_Message_Handler()
  : handler_id_(0)
  , site_id_(0) 
{

}

ACE_PIP_Message_Handler::ACE_PIP_Message_Handler(ACE_UINT32 site_id, ACE_UINT32 handler_id)
  : site_id_(site_id)
  , handler_id_(handler_id)
{
}

ACE_PIP_Protocol_Message* ACE_PIP_Message_Handler::create_protocol_message(ACE_UINT64 message_id,
                                                                           bool       reply_expected,
                                                                           ACE_UINT32 source_handler_id,
                                                                           ACE_UINT32 source_site_id,
                                                                           ACE_UINT32 destination_handler_id,
                                                                           ACE_UINT32 destination_site_id,
                                                                           ACE_UINT32 message_priority,
                                                                           ACE_PIP_Protocol_Message::Message_Type type,
                                                                           const std::string& data_payload)
{
   // setup the proto message header
   ACE_PIP_Protocol_Message* proto_message = new ACE_PIP_Protocol_Message;
   proto_message->set_message_id(message_id);
   proto_message->set_message_type(type);

   ACE_PIP_Data_Message* data_message = new ACE_PIP_Data_Message;
   data_message->set_reply_expected(reply_expected);

   data_message->set_source_handler_ID(source_handler_id);
   data_message->set_source_site_ID(source_site_id);
   data_message->set_destination_handler_ID(destination_handler_id);
   data_message->set_destination_site_ID(destination_site_id);
   
   data_message->set_message_priority(message_priority);

   // Create data message header and body, then pass to protocol message to be parsed
   // and unpacked
   ACE_Message_Block* header = new ACE_Message_Block(sizeof(ACE_PIP_Data_Message));
   ACE_Message_Block* body   = new ACE_Message_Block(data_payload.length() + 1);

   ACE_OS::memcpy(body->wr_ptr(), data_payload.c_str(), data_payload.length() + 1);
   body->wr_ptr(data_payload.length() + 1);
   
   // attach the data body to the header
   header->next(body);
   
   // pack the header values into the message block
   // set the write ptr ahead so pack() will know to put it back where it should be
   header->wr_ptr(sizeof(ACE_PIP_Data_Message));
   data_message->block_ = header;
   data_message->pack();
   
   proto_message->set_next(data_message);
   return proto_message;
}

void ACE_PIP_Message_Handler::send_request(ACE_Message_Block*  message,
                                           ACE_UINT64          message_id,
                                           ACE_Message_Block*& response)
{
  ACE_Future<ACE_Message_Block*>* response_holder(0);
  ACE_PIP_Invocation_Manager::instance()->process_outbound_request(message, message_id, response_holder);
  if (response_holder)
    {
      if (response_holder->get(response) == -1)
        {
          std::cerr << "Error receiving response in ::send_request" << std::endl;
          response = 0;
        }
    }
}

void ACE_PIP_Message_Handler::send_response(ACE_Message_Block* message,
                                            ACE_UINT64         message_id)
{
  ACE_PIP_Invocation_Manager::instance()->process_outbound_response(message, message_id);
}


ACE_UINT32 ACE_PIP_Message_Handler::get_handler_id() const
{
  return handler_id_;
}
void ACE_PIP_Message_Handler::set_handler_id(ACE_UINT32 id)
{
  handler_id_ = id;
}

ACE_UINT32 ACE_PIP_Message_Handler::get_site_id() const
{
  return site_id_;
}

void ACE_PIP_Message_Handler::set_site_id(ACE_UINT32 id)
{
  site_id_ = id;
}


