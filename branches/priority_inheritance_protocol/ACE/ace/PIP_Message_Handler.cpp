#include "ace/PIP_Message_Handler.h"

ACE_PIP_Message_Handler::ACE_PIP_Message_Handler()
  : handler_id_(0)
  , site_id_(0) 
{

}

ACE_PIP_Message_Handler::ACE_PIP_Message_Handler(ACE_UINT32 site_id, ACE_UINT32 handler_id)
  : handler_id_(handler_id)
  , site_id_(site_id)
{
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
          response = 0;
        }
    }
}

void ACE_PIP_Message_Handler::send_response(ACE_Message_Block* message,
                                                       ACE_UINT64         message_id)
{
  ACE_PIP_Invocation_Manager::instance()->process_outbound_response(message, message_id);
}



