//$Id$

#include "tao/Messaging/MessagingS.h"
#include "tao/DynamicInterface/DII_Reply_Handler.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"

class My_DII_Reply_Handler : public virtual TAO_DII_Reply_Handler,
                             public virtual Messaging::ReplyHandler
{
  //    Used to handle replies from the server and to complete the
  //    call back to the client.
  //
public:
  My_DII_Reply_Handler (TAO_AMH_DSI_Response_Handler_ptr rph,
                        CORBA::ORB_var orb);
  virtual ~My_DII_Reply_Handler (void);

  // Callback method for deferred synchronous requests.
  virtual void handle_response (TAO_InputCDR &incoming);
  virtual void handle_excep (TAO_InputCDR &incoming,
                             CORBA::ULong reply_status);

private:
  TAO_AMH_DSI_Response_Handler_var response_handler_;
  CORBA::ORB_var orb_;
};
