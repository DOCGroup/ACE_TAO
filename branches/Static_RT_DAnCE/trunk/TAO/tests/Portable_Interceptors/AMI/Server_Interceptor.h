/**
 * @file Server_Interceptor.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Server_Interceptor__h_
#define Server_Interceptor__h_

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"

class Echo_Server_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor
  , public virtual TAO_Local_RefCounted_Object
{
public:
  Echo_Server_Request_Interceptor ();

  virtual char * name (void);

  virtual void destroy (void);

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr);

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr);
};

#endif /* Server_Interceptor__h_ */
