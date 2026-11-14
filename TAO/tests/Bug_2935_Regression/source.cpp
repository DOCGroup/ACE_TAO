#include <cstdio>
#include <memory>
#include <string>

#include "source_i.h"

// A ThreeTier client that calls tick and/or tock

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
parse_args (int &argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_input_file)
{
  int argp {1};
  while (argp < argc)
    {
      ACE_TCHAR const *arg = argv[argp];
      if (arg[0] == '-' && arg[1] == 'f' && argp + 1 < argc)
        {
          if (ior_input_file != nullptr)
            {
              ACE_ERROR ((LM_DEBUG,
                          "Sink (%P|%t) duplicate -f options\n"));
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
  if (ior_input_file == nullptr)
    {
      ACE_ERROR ((LM_DEBUG,
                  "Sink (%P|%t) missing required -f option\n"));
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
  int result {};
  ACE_TCHAR const *ior_input_file {};
  ACE_DEBUG ((LM_DEBUG,
              "Source (%P|%t) started\n"));
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      if (!parse_args (argc, argv, ior_input_file))
        {
          return -1;
        }

      std::string input_ior;
      if (!read_ior (ior_input_file, input_ior))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot read input IOR file: %s",
                           ior_input_file),
                          -1);

      // Convert the IOR to an object reference.
      CORBA::Object_var const object = orb->string_to_object (input_ior.c_str ());

      // narrow the object reference to a ThreeTier reference
      ThreeTier_var const server = ThreeTier::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "IOR does not refer to a ThreeTier implementation"),
                            -1);
        }

      Source_i source (server.in ());
      result = source.run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      result = -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Source (%P|%t) exits\n"));
  return result;
}
