// $Id$

#include "AMI_internal_i.h"

AMI_internal_i::AMI_internal_i (CORBA::ORB_ptr orb, ::CCM_AMI::MyFoo_ptr foo_receiver)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    foo_receiver_ (::CCM_AMI::MyFoo::_duplicate (foo_receiver))
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
      CCM_AMI::InternalException excep;
      excep.id = ex.ex.id;
      excep.error_string = CORBA::string_dup (ex.ex.error_string);
      throw CCM_AMI::InternalError (excep);
    }
  catch (...)
   {
      printf ("AMI CORBA :\t!!!!!UNKNOWN EXCEPTION!!!!!\n");
      CCM_AMI::InternalException excep;
      excep.id = 43;
      excep.error_string = CORBA::string_dup ("UNKNOWN");
      throw CCM_AMI::InternalError (excep);
   }
}

void
AMI_internal_i::shutdown (void)
{
  printf ("AMI CORBA :\tShutting down\n");
  this->orb_->shutdown (0);
}
