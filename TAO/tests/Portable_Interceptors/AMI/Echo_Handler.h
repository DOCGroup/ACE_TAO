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
{
public:
  Echo_Handler(void);

  virtual void echo_operation(char const * retval)
    ACE_THROW_SPEC((CORBA::SystemException));
  virtual void echo_operation_excep(::Messaging::ExceptionHolder * h)
    ACE_THROW_SPEC((CORBA::SystemException));
  virtual void shutdown(void)
    ACE_THROW_SPEC((CORBA::SystemException));
  virtual void shutdown_excep(::Messaging::ExceptionHolder * h)
    ACE_THROW_SPEC((CORBA::SystemException));

  CORBA::ULong replies (void) const;

private:
  CORBA::ULong replies_;
};

#endif /* Echo_Handler__h_ */
