// $Id$
#include "sink_i.h"
#include "ace/OS_NS_unistd.h"

const char * ior_output_file = 0;

void eat_args (int & argc, char *argv[], int argp, int how_many)
{
  for (int marg = argp; marg + how_many < argc; ++marg)
    {
      argv[marg] = argv[marg + how_many];
    }
  argc -= how_many;
}

bool parse_args (int & argc, char *argv[])
{
  int argp = 1;
  while (argp < argc)
    {
      const char * arg = argv[argp];
      if(arg[0] == '-' && arg[1] == 'o' && argp + 1 < argc)
        {
          if (ior_output_file != 0)
            {
              ACE_ERROR ((LM_DEBUG,
                          "Sink (%P|%t) duplicate -o options\n"));
              return false;
            }
          // capture output file name
          // then remove it from arguemnt list
          ior_output_file = argv[argp + 1];
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
                  "Sink (%P|%t) missing required -o option\n"));
      return false;
    }
  return true;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              "Sink (%P|%t) started\n"));

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "");

      if ( ! parse_args (argc, argv))
        {
          return -1;
        }
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Sink_i sink (orb.in());
      if (sink.parse_args (argc, argv) )
        {
          PortableServer::ObjectId_var id =
            root_poa->activate_object (&sink);

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
      userex._tao_print_exception ("SINK: User Exception in main");
      return -1;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("SINK: System Exception in main");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Sink (%P|%t) exits\n"));

  return 0;
}

