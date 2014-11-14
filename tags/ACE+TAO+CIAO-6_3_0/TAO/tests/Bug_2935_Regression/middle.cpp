// $Id$
#include "middle_i.h"

const ACE_TCHAR * ior_output_file = 0;

const ACE_TCHAR * ior_input_file = 0;
char input_ior[5000];


void eat_args (int & argc, ACE_TCHAR *argv[], int argp, int how_many)
{
  for (int marg = argp; marg + how_many < argc; ++marg)
    {
      argv[marg] = argv[marg + how_many];
    }
  argc -= how_many;
}

bool parse_args (int & argc, ACE_TCHAR *argv[])
{
  int argp = 1;
  while (argp < argc)
    {
      const ACE_TCHAR * arg = argv[argp];
      if(arg[0] == '-' && arg[1] == 'o' && argp + 1 < argc)
        {
          if (ior_output_file != 0)
            {
              ACE_ERROR ((LM_DEBUG,
                          "Middle (%P|%t) duplicate -o options\n"));
              return false;
            }
          // capture output file name
          // then remove it from arguemnt list
          ior_output_file = argv[argp + 1];
          eat_args(argc, argv, argp, 2);
        }
      else if(arg[0] == '-' && arg[1] == 'f' && argp + 1 < argc)
        {
          if (ior_input_file != 0)
            {
              ACE_ERROR ((LM_DEBUG,
                          "Middle (%P|%t) duplicate -f options\n"));
              return false;
            }
          // capture input file name
          // then remove it from arguemnt list
          ior_input_file = argv[argp + 1];
          eat_args(argc, argv, argp, 2);
        }
      else
        {
           argp += 1;
           // just ignore unknown arguments
        }
    }
  if (ior_output_file == 0)
    {
      ACE_ERROR ((LM_DEBUG,
                  "Middle (%P|%t) missing required -o option\n"));
      return false;
    }
  return true;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              "Middle (%P|%t) started\n"));

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      if ( ! parse_args (argc, argv))
        {
          return -1;
        }

      ///////////////////////////////
      // Prepare to be a CORBA server
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      ///////////////////////////////
      // Prepare to be a CORBA client
      FILE *input_file = ACE_OS::fopen (ior_input_file, "r");
      if (input_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open input IOR file: %s",
                             ior_input_file),
                            -1);
        }
      ACE_OS::fread (input_ior, 1, sizeof(input_ior), input_file);
      ACE_OS::fclose (input_file);

      // Convert the IOR to an object reference.
      CORBA::Object_var object =
        orb->string_to_object (input_ior);

      // narrow the object reference to a ThreeTier reference
      ThreeTier_var target = ThreeTier::_narrow (object.in ());

      if (CORBA::is_nil (target.in ()))
        {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "IOR does not refer to a ThreeTier implementation"),
                              -1);
        }

      // We should have a good connection now
      // temporary: check it out
      //target->tick();
      //target->tock();

      Middle_i middle (orb.in(), target.in ());
      if (middle.parse_args (argc, argv) )
        {
          /////////////////////////////////
          // Activate server side mechanism
          PortableServer::ObjectId_var id =
            root_poa->activate_object (&middle);

          CORBA::Object_var object = root_poa->id_to_reference (id.in ());

          ThreeTier_var server =
            ThreeTier::_narrow (object.in ());

          CORBA::String_var ior =
            orb->object_to_string (server.in ());

          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          poa_manager->activate ();
          orb->run ();
        }
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("Middle: User Exception in main");
      return -1;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("Middle: System Exception in main");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Middle (%P|%t) exits\n"));

  return 0;
}
