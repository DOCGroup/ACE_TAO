/**
 * @file Client_Interceptor.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Client_Interceptor__h_
#define Client_Interceptor__h_

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"

class Echo_Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor
  , public virtual ::CORBA::LocalObject
{
public:
  Echo_Client_Request_Interceptor (void);

  static unsigned long request_count;
  static unsigned long reply_count;
  static unsigned long other_count;
  static unsigned long exception_count;

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);
};

#endif /* Client_Interceptor__h_ */
