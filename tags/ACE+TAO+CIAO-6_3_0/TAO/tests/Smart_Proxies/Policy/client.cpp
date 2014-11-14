
//=============================================================================
/**
 *  @file     client.cpp
 *
 *  $Id$
 *
 *   This is the client program that tests TAO's Smart Proxy extension.
 *
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"
#include "testC.h"
#include "Smart_Proxy_Impl.h"

const ACE_TCHAR *ior1 = ACE_TEXT("file://test1.ior");
const ACE_TCHAR *ior2 = ACE_TEXT("file://test2.ior");
int one_shot_factory = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:j:f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        ior1 = get_opts.opt_arg ();
        break;
      case 'j':
        ior2 = get_opts.opt_arg ();
        break;
       case 'f':
        one_shot_factory = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i -j -f"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
run_test (CORBA::ORB_ptr orb_ptr,
          int target)
{
  CORBA::ORB_var orb = CORBA::ORB::_duplicate (orb_ptr);
  CORBA::Object_var object;
  try
    {
      if (target == 1)
        {
          object =
            orb->string_to_object (ior1);
        }
      else
        {
          object =
            orb->string_to_object (ior2);
        }

      Test_var server =
        Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference is nil\n"),
                          1);

      server->method (0);

      server->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client-side exception:");
    }
return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // To use the smart proxy it is necessary to allocate the
      // user-defined smart factory on the heap as the smart proxy
      // generated classes take care of destroying the object. This
      // way it a win situation for the application developer who
      // doesnt have to make sure to destoy it and also for the smart
      // proxy designer who now can manage the lifetime of the object
      // much surely.
      // By default this factory is permanent (i.e. registered for
      // this interface) but if there is a need for flexibility per
      // object instance then <one_shot_factory> needs to be set to 0.
      Smart_Test_Factory *test_factory = 0;
      ACE_NEW_RETURN (test_factory,
                      Smart_Test_Factory (one_shot_factory),
                      -1);

      ACE_UNUSED_ARG (test_factory);

      run_test (orb.in (), 1);
      run_test (orb.in (), 2);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client-side exception:");
      return 1;
    }

  return 0;
}
