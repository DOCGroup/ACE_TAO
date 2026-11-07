#include <cstdio>
#include <memory>
#include <string>

#include "middle_i.h"

void
eat_args (int &argc, ACE_TCHAR *argv[], int argp, int how_many)
{
  for (int marg = argp; marg + how_many < argc; ++marg)
    {
      argv[marg] = argv[marg + how_many];
    }
  argc -= how_many;
}

bool
parse_args (int &argc, ACE_TCHAR *argv[],
            ACE_TCHAR const *&ior_output_file,
            ACE_TCHAR const *&ior_input_file)
{
  int argp {1};
  while (argp < argc)
    {
      ACE_TCHAR const *arg = argv[argp];
      if (arg[0] == '-' && arg[1] == 'o' && argp + 1 < argc)
        {
          if (ior_output_file != nullptr)
            {
              ACE_ERROR ((LM_DEBUG,
                          "Middle (%P|%t) duplicate -o options\n"));
              return false;
            }
          // capture output file name
          // then remove it from arguemnt list
          ior_output_file = argv[argp + 1];
          eat_args (argc, argv, argp, 2);
        }
      else if (arg[0] == '-' && arg[1] == 'f' && argp + 1 < argc)
        {
          if (ior_input_file != nullptr)
            {
              ACE_ERROR ((LM_DEBUG,
                          "Middle (%P|%t) duplicate -f options\n"));
              return false;
            }
          // capture input file name
          // then remove it from arguemnt list
          ior_input_file = argv[argp + 1];
          eat_args (argc, argv, argp, 2);
        }
      else
        {
          ++argp;
          // Just ignore unknown arguments.
        }
    }
  if (ior_output_file == nullptr)
    {
      ACE_ERROR ((LM_DEBUG,
                  "Middle (%P|%t) missing required -o option\n"));
      return false;
    }
  if (ior_input_file == nullptr)
    {
      ACE_ERROR ((LM_DEBUG,
                  "Middle (%P|%t) missing required -f option\n"));
      return false;
    }
  return true;
}

bool
read_ior (ACE_TCHAR const *file_name, std::string &ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> input_file (
    ACE_OS::fopen (file_name, "r"), ACE_OS::fclose);
  if (!input_file)
    return false;

  char buffer[512];
  size_t bytes_read {};
  while ((bytes_read = std::fread (buffer, 1, sizeof (buffer), input_file.get ())) != 0)
    ior.append (buffer, bytes_read);

  return std::ferror (input_file.get ()) == 0 && !ior.empty ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior_output_file {};
  ACE_TCHAR const *ior_input_file {};
  ACE_DEBUG ((LM_DEBUG,
              "Middle (%P|%t) started\n"));

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc,
                         argv);

      if (!parse_args (argc, argv, ior_output_file, ior_input_file))
        {
          return -1;
        }

      ///////////////////////////////
      // Prepare to be a CORBA server
      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      ///////////////////////////////
      // Prepare to be a CORBA client
      std::string input_ior;
      if (!read_ior (ior_input_file, input_ior))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot read input IOR file: %s",
                           ior_input_file),
                          -1);

      // Convert the IOR to an object reference.
      CORBA::Object_var const object = orb->string_to_object (input_ior.c_str ());

      // narrow the object reference to a ThreeTier reference
      ThreeTier_var const target = ThreeTier::_narrow (object.in ());

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

      // The POA borrows this stack servant until orb->run() returns.
      Middle_i middle (orb.in (), target.in ());
      /////////////////////////////////
      // Activate server side mechanism
      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&middle);

      CORBA::Object_var const servant_object = root_poa->id_to_reference (id.in ());

      ThreeTier_var const server =
        ThreeTier::_narrow (servant_object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             ior_output_file),
                            -1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();
      orb->run ();
    }
  catch (CORBA::UserException const &userex)
    {
      userex._tao_print_exception ("Middle: User Exception in main");
      return -1;
    }
  catch (CORBA::SystemException const &sysex)
    {
      sysex._tao_print_exception ("Middle: System Exception in main");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Middle (%P|%t) exits\n"));

  return 0;
}
