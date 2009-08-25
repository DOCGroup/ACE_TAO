// $Id$

#include "AMI_internal_i.h"

AMI_internal_i::AMI_internal_i (CORBA::ORB_ptr orb, ::CCM_AMI::AMI_foo_ptr foo_receiver)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    foo_receiver_ (::CCM_AMI::AMI_foo::_duplicate (foo_receiver))
{
}

CORBA::Long
AMI_internal_i::foo (const char * in_str,
                 CORBA::String_out out_str)
{
  try
    {
      printf ("AMI CORBA :\tReceived string <%s>. Try passing it to the Receiver component\n", in_str);
      CORBA::Long result = foo_receiver_->foo (CORBA::string_dup (in_str), out_str);
      return result;
    }
  catch (CCM_AMI::InternalError& ex)
    {
      printf ("AMI CORBA :\tCORRECT EXCEPTION -> re-throwing\n");
      throw CCM_AMI::InternalError (ex.id, CORBA::string_dup  (ex.error_string));
    }
  catch (...)
   {
     printf ("AMI CORBA :\t!!!!!UNKNOWN EXCEPTION!!!!!\n");
      throw CCM_AMI::InternalError (43, "UNKNOWN");
   }
}

void
AMI_internal_i::shutdown (void)
{
  printf ("AMI CORBA :\tShutting down\n");
  this->orb_->shutdown (0);
}
