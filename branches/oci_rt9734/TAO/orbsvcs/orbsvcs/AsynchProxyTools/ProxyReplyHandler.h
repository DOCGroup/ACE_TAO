//
// $Id$
//
#include "tao/DynamicInterface/DII_Reply_Handler.h"
#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/DynamicInterface/Request.h"
class ProxyReplyHandler : public virtual TAO_DII_Reply_Handler
                        , public virtual Messaging::ReplyHandler
{
  public:
    ProxyReplyHandler ();
    virtual ~ProxyReplyHandler ();

  // Callback method for deferred synchronous requests.
  virtual void handle_response (TAO_InputCDR &incoming);
  virtual void handle_excep (TAO_InputCDR &incoming,
                             CORBA::ULong reply_status);

  virtual handle_response_i () = 0;
  virtual handle_excep_i () = 0;
};
