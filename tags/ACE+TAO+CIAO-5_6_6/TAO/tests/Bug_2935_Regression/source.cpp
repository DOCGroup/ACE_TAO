//$Id$

# include "source_i.h"

// A ThreeTier client that calls tick and/or tock

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
      const char * arg = argv[argp];
      if(arg[0] == '-' && arg[1] == 'f' && argp + 1 < argc)
        {
          if (ior_input_file != 0)
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
           argp += 1;
           // just ignore unknown arguments
        }
    }
  if (ior_input_file == 0)
    {
      ACE_ERROR ((LM_DEBUG,
                  "Sink (%P|%t) missing required -f option\n"));
      return false;
    }
  return true;

}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  ACE_DEBUG ((LM_DEBUG,
              "Source (%P|%t) started\n"));
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      if (!parse_args (argc, argv))
        {
          return -1;
        }

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
      ThreeTier_var server = ThreeTier::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "IOR does not refer to a ThreeTier implementation"),
                              -1);
        }

      Source_i source (server.in ());
      result = source.run();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      result = -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Source (%P|%t) exits\n"));
  return result;
}
