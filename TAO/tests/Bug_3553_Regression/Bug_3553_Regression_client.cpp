#include "TestC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/Policy_Current.h"
#include "ace/Get_Opt.h"
#include <string>
#include <vector>

int
parse_args (int argc, ACE_TCHAR *argv[], int &cache_size, int &port_number)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("c:p:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'c':
        cache_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        port_number = ACE_OS::atoi (get_opts.opt_arg ());
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      int cache_size {512};
      int port_number {27530};

      if (parse_args (argc, argv, cache_size, port_number) != 0)
          return 1;

      // Get the RTORB.
      CORBA::Object_var const obj = orb->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var const rtorb = RTCORBA::RTORB::_narrow (obj.in ());
      //create the private connections policy. This means that every connection
      // to the server uses his own socket.
      CORBA::PolicyList policies (1);
      policies.length (1);
      policies[0] = rtorb->create_private_connection_policy ();

      CORBA::Object_var const pol_current_object = orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (pol_current_object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      std::vector<Test::Hello_var> hello_array (cache_size);

      for (int iteration {1}; iteration <= cache_size; ++iteration)
        {
          std::string const reference_string = "TransportCacheTest" + std::to_string (iteration);
          std::string const object_string = "corbaloc:iiop:localhost:"
            + std::to_string (port_number) + "/" + reference_string;

          CORBA::Object_var const hello_obj = orb->string_to_object (object_string.c_str ());
          orb->register_initial_reference (reference_string.c_str (), hello_obj.in ());

          CORBA::String_var const ior_string = orb->object_to_string (hello_obj.in ());
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("IOR string for reference %d : %C\n"),
                      iteration, ior_string.in ()));
          hello_array[iteration - 1] = Test::Hello::_narrow (hello_obj.in ());
        }
      //now we've got the references -> call each and everyone of them
      for (int iteration {}; iteration < cache_size; ++iteration)
        {
          Test::Hello_var const &hello = hello_array[iteration];
          if (CORBA::is_nil (hello.in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                 ACE_TEXT ("Nil Test::Hello reference\n")),
                                 1);
            }

          CORBA::String_var const the_string = hello->get_string ();

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C> from reference %d\n",
                      the_string.in (), iteration + 1));
        }
      //shutdown the server
      if (!hello_array.empty ())
        hello_array.front ()->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
