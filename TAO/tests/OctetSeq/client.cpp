// $Id$

#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "testC.h"

ACE_RCSID(OctetSeq, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;
ACE_RANDR_TYPE seed = 0;
int verbose = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "vk:i:s:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        seed = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'v':
        verbose = 1;
        break;

     case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-s <seed> "
                           "-v "
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
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Database_var server =
        Test::Database::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      if (seed == 0)
        {
          seed = ACE_OS::time (0);
          ACE_DEBUG ((LM_DEBUG, "Seed value is %d\n", seed));
        }

      const int nelements = 64; // @@ TODO
      const int maxsize = 1024;
      Test::OctetSeq elements[nelements];

      for (int i = 0; i != niterations; ++i)
        {
          CORBA::ULong r = ACE_OS::rand_r (seed);
          Test::Index idx = (r % nelements);

          if (i % 100 == 0)
            {
              for (int j = 0; j != nelements; ++j)
                {
                  CORBA::ULong r = ACE_OS::rand_r (seed);
                  CORBA::ULong l = r % maxsize;
                  elements[j].length (l);
                  CORBA::Double token = 0;
                  for (CORBA::ULong k = 0; k != l; ++k)
                    {
                      r = ACE_OS::rand_r (seed);
                      elements[j][k] = (r % 128);
                      token += r;
                    }
                  CORBA::Double returned_token;
                  server->set (Test::Index (j),
                               elements[j],
                               token,
                               returned_token
                               ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  if (token != returned_token)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  "ERROR - invalid token <%f> returned,"
                                  " expecting %f in (%d,%d)\n",
                                  returned_token, token, i, j));
                    }
                }
            }

          CORBA::ULong crc_remote =
            server->get_crc (idx ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::ULong crc_local =
            ACE::crc32 (ACE_reinterpret_cast (const char*,
                                              elements[idx].get_buffer ()),
                        elements[idx].length ());

          if (crc_local != crc_remote)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "CRC mismatch for element %d\n", idx));
            }
          else if (verbose == 1)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "CRC <%u> matched for element %d\n",
                          crc_remote, idx));
            }

        }

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
