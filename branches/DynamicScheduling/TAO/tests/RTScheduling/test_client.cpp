//$Id$
#include "testC.h"

main (int argc, char* argv [])
{
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc,
                     argv,
                     ""
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  ACE_SString ior;
  ior.rep (argv [1]);
  
  CORBA::Object_var object = 
    orb->string_to_object (ior.c_str ()
			   ACE_ENV_ARG_PARAMETER)
    ACE_CHECK;

  foo_impl_var foo_i = foo_impl::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
 
  int something = 6;
  foo_i->bar (something);
  orb->run ();
}
