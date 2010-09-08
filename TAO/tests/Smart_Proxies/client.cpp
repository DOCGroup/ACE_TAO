// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/Smart_Proxy
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is the client program that tests TAO's Smart Proxy extension.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=========================================================================

#include "ace/Get_Opt.h"
#include "testC.h"
#include "Smart_Proxy_Impl.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        ior = ACE_OS::strdup (get_opts.opt_arg ());
      break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      if (parse_args (argc, argv) != 0)
        return 1;

       CORBA::Object_var object =
        orb->string_to_object (ior);

      // To use the smart proxy it is necessary to allocate the
      // user-defined smart factory on the heap as the smart proxy
      // generated classes take care of destroying the object. This
      // way it a win situation for the application developer who
      // doesnt have to make sure to destoy it and also for the smart
      // proxy designer who now can manage the lifetime of the object
      // much surely.
      Smart_Test_Factory *test_factory = 0;
      ACE_NEW_RETURN (test_factory,
                      Smart_Test_Factory,
                      -1);

      ACE_UNUSED_ARG (test_factory);

      Test_var server =
        Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil.\n",
                           ior),
                          1);

      try
        {
          CORBA::String_var sm_ior = orb->object_to_string (server.in ());
          if (Smart_Test_Proxy::fake_ior () != sm_ior.in ())
            {
              status = 1;
              ACE_ERROR ((LM_ERROR,
                          "ERROR: The Smart Proxy IOR is:\n%C\n"
                          "but should have been: %C\n",
                          sm_ior.in (),
                          Smart_Test_Proxy::fake_ior ().c_str ()));
            }
        }
      catch (const CORBA::MARSHAL& ex)
        {
          status = 1;
          ex._tao_print_exception ("Unexpected MARSHAL exception:");
        }

      server->method (0);

      server->shutdown ();

      // The following sleep is a hack to make sure the above oneway
      // request gets sent before we exit. Otherwise, at least on
      // Windows XP, the server may not even get the request.
      ACE_Time_Value tv (0, 100000);
      ACE_OS::sleep(tv);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client-side exception:");
      status = 1;
    }

  return status;
}
