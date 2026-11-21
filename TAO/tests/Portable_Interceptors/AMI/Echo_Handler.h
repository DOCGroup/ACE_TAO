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
  Echo_Handler () = default;
  Echo_Handler (Echo_Handler const &) = delete;
  Echo_Handler (Echo_Handler &&) = delete;
  Echo_Handler &operator= (Echo_Handler const &) = delete;
  Echo_Handler &operator= (Echo_Handler &&) = delete;

  void echo_operation (char const *retval) override;
  void echo_operation_excep (::Messaging::ExceptionHolder *h) override;
  void shutdown () override;
  void shutdown_excep (::Messaging::ExceptionHolder *h) override;

  CORBA::ULong replies () const;

private:
  CORBA::ULong replies_ {};
};

#endif /* Echo_Handler__h_ */
