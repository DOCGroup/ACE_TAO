/**
 * @file Echo.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Echo__h_
#define Echo__h_

#include "TestS.h"
#include "tao/PortableServer/PortableServer.h"

class Echo
  : public POA_Test::Echo
  , public PortableServer::RefCountServantBase
{
public:
  Echo(CORBA::ORB_ptr orb);

  virtual char * echo_operation(char const * the_input
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  virtual void shutdown(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};

#endif /* Echo__h_ */
