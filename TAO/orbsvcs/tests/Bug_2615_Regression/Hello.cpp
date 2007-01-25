//
// $Id$
//

#include "ServerRequest_Interceptor2.h"
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello::Hello (CORBA::ORB_ptr orb, Test::Hello_ptr, CORBA::ULong)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}

void
Hello::ping (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}

CORBA::Boolean
Hello::has_ft_request_service_context (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ServerRequest_Interceptor2::has_ft_request_sc_;
}
