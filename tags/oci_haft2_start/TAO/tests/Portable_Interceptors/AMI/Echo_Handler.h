/**
 * @file Echo_Handler.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Echo_Handler__h_
#define Echo_Handler__h_

#include "TestS.h"
#include "tao/PortableServer/PortableServer.h"

class Echo_Handler
  : public POA_Test::AMI_EchoHandler
  , public PortableServer::RefCountServantBase
{
public:
  Echo_Handler(void);

  virtual void echo_operation(char const * retval
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  virtual void echo_operation_excep(Test::AMI_EchoExceptionHolder * h
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  virtual void shutdown(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  virtual void shutdown_excep(Test::AMI_EchoExceptionHolder * h
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
};

#endif /* Echo_Handler__h_ */
