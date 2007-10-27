// -*-  C++ -*-

//
// $Id$
//

#ifndef DII_REPLY_HANDLER_H
#define DII_REPLY_HANDLER_H

#include "tao/DynamicInterface/DII_Reply_Handler.h"

class DII_ReplyHandler :
  public virtual TAO_DII_Reply_Handler,
  public virtual Messaging::ReplyHandler
{
public:
  DII_ReplyHandler (bool &reply_notifier);
  virtual ~DII_ReplyHandler ();

  virtual void handle_response (TAO_InputCDR &incoming);
  virtual void handle_excep (TAO_InputCDR &incoming,
                             CORBA::ULong reply_status);
  virtual void handle_location_forward (TAO_InputCDR &incoming,
                                        CORBA::ULong reply_status);
private:
  bool &got_reply_;
};


#endif // DII_REPLY_HANDLER_H
