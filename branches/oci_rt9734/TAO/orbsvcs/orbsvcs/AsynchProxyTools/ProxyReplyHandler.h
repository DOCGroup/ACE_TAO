// -*- C++ -*-

//
// $Id$
//

#ifndef TAO_PROXY_REPLY_HANDLER_H
#define TAO_PROXY_REPLY_HANDLER_H

#include "orbsvcs/AsynchProxyTools/AsynchProxyTools_Export.h"


#include "tao/DynamicInterface/DII_Reply_Handler.h"
#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/DynamicInterface/Request.h"

class TAO_AsynchProxyTools_Export TAO_ProxyReplyHandler :
  public virtual TAO_DII_Reply_Handler,
  public virtual Messaging::ReplyHandler
{
  public:
    TAO_ProxyReplyHandler ();
    virtual ~TAO_ProxyReplyHandler ();

  // Callback method for deferred synchronous requests.
  virtual void handle_response (TAO_InputCDR &incoming);
  virtual void handle_excep (TAO_InputCDR &incoming,
                             CORBA::ULong reply_status);

  virtual void handle_response_i () = 0;
  virtual void handle_excep_i () = 0;
};

#endif // TAO_PROXY_REPLY_HANDLER_H
