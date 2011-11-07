// $Id$

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

  virtual ~ClientInterceptor ();

  virtual char * name ();

  virtual void destroy ();

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

 private:
  const char *myname_;
  Messenger_var messenger;
  PortableInterceptor::Current_ptr pic;
  PortableInterceptor::SlotId slot;
};

#endif
