#ifndef CLIENTINTERCEPTOR_H
#define CLIENTINTERCEPTOR_H

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI/ClientRequestInterceptorA.h"
#include "tao/PI/PICurrentC.h"
#include "MessengerC.h"

class ClientInterceptor:
  public virtual PortableInterceptor::ClientRequestInterceptor,
  public virtual TAO_Local_RefCounted_Object
{
  
 public:
  ClientInterceptor (Messenger_var theMessage, 
                     PortableInterceptor::Current_ptr thePic,
                     PortableInterceptor::SlotId theSlot);
  
  virtual ~ClientInterceptor ();  

  virtual char * name ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

 private:
  const char *myname_;
  Messenger_var messenger;
  PortableInterceptor::Current_ptr pic;
  PortableInterceptor::SlotId slot;
};

#endif
