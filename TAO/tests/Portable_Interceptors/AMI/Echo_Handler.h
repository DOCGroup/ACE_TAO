/**
 * @file Echo_Handler.h
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
  Echo_Handler();

  virtual void echo_operation(char const * retval);
  virtual void echo_operation_excep(::Messaging::ExceptionHolder * h);
  virtual void shutdown();
  virtual void shutdown_excep(::Messaging::ExceptionHolder * h);

  CORBA::ULong replies () const;

private:
  CORBA::ULong replies_;
};

#endif /* Echo_Handler__h_ */
