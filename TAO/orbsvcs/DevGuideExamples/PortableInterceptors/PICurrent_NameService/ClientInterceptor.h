#ifndef CLIENTINTERCEPTOR_H
#define CLIENTINTERCEPTOR_H

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI/ClientRequestInterceptorA.h"
#include "tao/PI/PICurrentC.h"
#include "MessengerC.h"

class ClientInterceptor:
  public virtual PortableInterceptor::ClientRequestInterceptor,
  public virtual ::CORBA::LocalObject
{
 public:
  ClientInterceptor (Messenger_var theMessage,
                     PortableInterceptor::Current_ptr thePic,
                     PortableInterceptor::SlotId theSlot);

  ~ClientInterceptor () override;

  ClientInterceptor (ClientInterceptor const &) = delete;
  ClientInterceptor (ClientInterceptor &&) = delete;
  ClientInterceptor &operator= (ClientInterceptor const &) = delete;
  ClientInterceptor &operator= (ClientInterceptor &&) = delete;

  char * name () override;

  void destroy () override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

 private:
  char const * myname_;
  Messenger_var messenger;
  PortableInterceptor::Current_ptr pic;
  PortableInterceptor::SlotId slot;
};

#endif
