// $Id$

#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

ACE_RCSID(DSI_Gateway, client, "$Id$")

const char *ior = "file://gateway.ior";
int niterations = 5;
int do_shutdown = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "xk:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.optarg;
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      Structure the_in_structure;
      the_in_structure.seq.length (10);

      for (int i = 0; i != niterations; ++i)
        {
          the_in_structure.i = i;
          CORBA::String_var name = CORBA::string_dup ("the name");

          Structure_var the_out_structure;

          CORBA::Long r =
            server->test_method (i,
                                 the_in_structure,
                                 the_out_structure.out (),
                                 name.inout (),
                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "DSI_Simpler_Server ====\n"
                          "    x = %d\n"
                          "    i = %d\n"
                          "    length = %d\n"
                          "    name = <%s>\n",
                          r,
                          the_out_structure->i,
                          the_out_structure->seq.length (),
                          name.in ()));
            }

          if (r != i)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) unexpected result = %d for %d",
                          r, i));
            }
        }

      if (do_shutdown)
        {
          server->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
