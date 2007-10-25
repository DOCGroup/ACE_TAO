// -*-  C++ -*-

#ifndef DII_REPLY_HANDLER_H
#define DII_REPLY_HANDLER_H

#include "tao/DynamicInterface/DII_Reply_Handler.h"
#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/DynamicInterface/Request.h"

class DII_ReplyHandler :
  public virtual TAO_DII_Reply_Handler,
  public virtual Messaging::ReplyHandler
{
public:
  DII_ReplyHandler ();
  virtual ~DII_ReplyHandler ();

  // Callback method for deferred synchronous requests.
  virtual void handle_response (TAO_InputCDR &incoming);
  virtual void handle_excep (TAO_InputCDR &incoming,
                             CORBA::ULong reply_status);
  virtual void handle_location_forward (TAO_InputCDR &incoming,
                                        CORBA::ULong reply_status);

};


#endif // TAO_PROXY_REPLY_HANDLER_H
