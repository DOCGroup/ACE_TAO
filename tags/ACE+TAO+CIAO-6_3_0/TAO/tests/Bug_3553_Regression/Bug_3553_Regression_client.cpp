// $Id$

#include "TestC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/Policy_Current.h"
#include "tao/Policy_Current.h"
#include "ace/Get_Opt.h"

int cache_size = 512;
int port_nr = 27530;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c:p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        cache_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        port_nr = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-c <transport_cache_size>\n"
                           "-p <server_port_nr>\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args(argc, argv) != 0)
          return 1;

      // Get the RTORB.
      CORBA::Object_var obj = orb->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var rtorb = RTCORBA::RTORB::_narrow (obj.in());
      //create the private connections policy. This means that every connection
      // to the server uses his own socket.
      CORBA::PolicyList policies (1);
      policies.length (1);
      policies[0] = rtorb->create_private_connection_policy ();

      CORBA::Object_var pol_current_object = orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (pol_current_object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      Test::Hello_var *hello_array = 0;
      ACE_NEW_RETURN (hello_array, Test::Hello_var [cache_size], -1);

      int iter = 1;
      for (iter = 1; iter <= cache_size; ++iter)
        {
          char object_string[256];
          char reference_string[256];
          ACE_OS::sprintf (reference_string, "TransportCacheTest%d", iter);
          ACE_OS::sprintf (object_string, "corbaloc:iiop:localhost:%d/", port_nr);
          ACE_OS::strcat (object_string, reference_string);

          CORBA::Object_var hello_obj = orb->string_to_object (object_string);
          orb->register_initial_reference (reference_string, hello_obj.in ());

          CORBA::String_var ior_string = orb->object_to_string (hello_obj.in());
          ACE_DEBUG((LM_DEBUG, ACE_TEXT("IOR string for reference %d : %C\n"),
            iter, ior_string.in ()));
          hello_array[iter-1] = Test::Hello::_narrow(hello_obj.in ());
        }
      //now we've got the references -> call each and everyone of them
      for (iter = 0; iter < cache_size; ++iter)
        {
          Test::Hello_var hello = hello_array[iter];
          if (CORBA::is_nil (hello.in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                 ACE_TEXT ("Nil Test::Hello reference\n")),
                                 1);
            }

          CORBA::String_var the_string = hello->get_string ();

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C> from reference %d\n",
                      the_string.in (), iter + 1));
        }
      //shutdown the server
      if (iter >= 0)
        hello_array[0]->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
