// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "testC.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"

#include <array>
#include <cstring>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  int niterations {5};
  unsigned int seed {};
  bool verbose {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("vk:i:s:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        options.seed = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'v':
        options.verbose = true;
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      Test::Database_var server =
        Test::Database::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior),
                            1);
        }

#if (TAO_HAS_MINIMUM_CORBA == 0)
      CORBA::String_var repository_id =
        server->_repository_id ();

      if (std::strcmp (repository_id.in (), "IDL:Test/Database:1.0") != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Repository id is wrong <%C>\n",
                             repository_id.in ()),
                            1);
        }
#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

      if (options.seed == 0)
        {
          options.seed = static_cast<unsigned int> (ACE_OS::time (nullptr));
          ACE_DEBUG ((LM_DEBUG, "Seed value is %d\n", options.seed));
        }

      constexpr size_t nelements = 64; // @@ TODO
      constexpr size_t maxsize = 1024;
      std::array<Test::OctetSeq, nelements> elements {};

      for (int i = 0; i != options.niterations; ++i)
        {
          CORBA::ULong r = ACE_OS::rand_r (&options.seed);
          Test::Index const idx = static_cast<Test::Index> (r % nelements);

          if (i % 100 == 0)
            {
              for (size_t j = 0; j != nelements; ++j)
                {
                  CORBA::ULong fill_random = ACE_OS::rand_r (&options.seed);
                  CORBA::ULong const l = fill_random % maxsize;
                  elements[j].length (l);
                  CORBA::Double token {};
                  for (CORBA::ULong k = 0; k != l; ++k)
                    {
                      fill_random = ACE_OS::rand_r (&options.seed);
                      elements[j][k] = (fill_random % 128);
                      token += fill_random;
                    }
                  CORBA::Double returned_token {};
                  server->set (static_cast<Test::Index> (j),
                               elements[j],
                               token,
                               returned_token);

                  if (!ACE::is_equal (token, returned_token))
                    {
                      ACE_ERROR ((LM_ERROR,
                                  "ERROR - invalid token <%f> returned,"
                                  " expecting %f in (%d,%d)\n",
                                  returned_token, token, i, static_cast<int> (j)));
                    }
                }
            }

          CORBA::ULong crc_remote =
            server->get_crc (idx);

          CORBA::ULong crc_local =
            ACE::crc32 (elements[idx].get_buffer (),
                        elements[idx].length ());

          if (crc_local != crc_remote)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "CRC mismatch for element %d\n", idx));
            }
          else if (options.verbose)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "CRC <%u> matched for element %d\n",
                          crc_remote, idx));
            }

        }

      server->shutdown ();
      ACE_OS::sleep (1);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
