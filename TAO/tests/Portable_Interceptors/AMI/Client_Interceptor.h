/**
 * @file Client_Interceptor.h
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
  Echo_Client_Request_Interceptor () = default;
  Echo_Client_Request_Interceptor (Echo_Client_Request_Interceptor const &) = delete;
  Echo_Client_Request_Interceptor (Echo_Client_Request_Interceptor &&) = delete;
  Echo_Client_Request_Interceptor &operator= (Echo_Client_Request_Interceptor const &) = delete;
  Echo_Client_Request_Interceptor &operator= (Echo_Client_Request_Interceptor &&) = delete;

  static unsigned long request_count;
  static unsigned long reply_count;
  static unsigned long other_count;
  static unsigned long exception_count;

  char * name () override;
  // Canonical name of the interceptor.

  void destroy () override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;
};

#endif /* Client_Interceptor__h_ */
