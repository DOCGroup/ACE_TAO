
#include "ServerRequest_Interceptor2.h"
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}

void
Hello::ping ()
{
}

CORBA::Boolean
Hello::has_ft_request_service_context ()
{
  return ServerRequest_Interceptor2::has_ft_request_sc_;
}
