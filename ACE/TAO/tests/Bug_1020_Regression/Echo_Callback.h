#ifndef Echo_Callback__h_
#define Echo_Callback__h_
/**
 * @file Echo_Callback.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "TestS.h"

class Echo_Callback : public POA_Test::AMI_EchoHandler
{
public:
  Echo_Callback();

  virtual void echo_payload (Test::Payload const & ami_return_val
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC((CORBA::SystemException));
  virtual void echo_payload_excep (Test::AMI_EchoExceptionHolder * excep_holder
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC((CORBA::SystemException));
};

#endif /* Echo__h_ */
