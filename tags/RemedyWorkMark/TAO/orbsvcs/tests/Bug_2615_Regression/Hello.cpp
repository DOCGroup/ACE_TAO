//
// $Id$
//

#include "ServerRequest_Interceptor2.h"
#include "Hello.h"



Hello::Hello (CORBA::ORB_ptr orb, Test::Hello_ptr, CORBA::ULong)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}

void
Hello::ping (void)
{
  return;
}

CORBA::Boolean
Hello::has_ft_request_service_context (void)
{
  return ServerRequest_Interceptor2::has_ft_request_sc_;
}
