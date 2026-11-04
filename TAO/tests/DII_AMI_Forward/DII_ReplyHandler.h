// -*-  C++ -*-


#ifndef DII_REPLY_HANDLER_H
#define DII_REPLY_HANDLER_H

#include "tao/DynamicInterface/DII_Reply_Handler.h"

class DII_ReplyHandler :
  public virtual TAO_DII_Reply_Handler,
  public virtual Messaging::ReplyHandler
{
public:
  DII_ReplyHandler (bool &reply_notifier);
  DII_ReplyHandler (DII_ReplyHandler const &) = delete;
  DII_ReplyHandler (DII_ReplyHandler &&) = delete;
  DII_ReplyHandler &operator= (DII_ReplyHandler const &) = delete;
  DII_ReplyHandler &operator= (DII_ReplyHandler &&) = delete;
  ~DII_ReplyHandler () override = default;

  void handle_response (TAO_InputCDR &incoming) override;
  void handle_excep (TAO_InputCDR &incoming,
                     CORBA::ULong reply_status) override;
  void handle_location_forward (TAO_InputCDR &incoming,
                                CORBA::ULong reply_status) override;
private:
  bool &got_reply_;
};


#endif // DII_REPLY_HANDLER_H
