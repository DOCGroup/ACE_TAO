#include "TX_ObjectC.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Get_Opt.h"

#include <cstring>
#include <string>

static constexpr char cert_file[] = "cacert.pem";

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  long number_iterations {1};
  long data_size {3461724};
  bool shutdown_server {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:i:d:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'i':
        options.number_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'd':
        options.data_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        options.shutdown_server = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-k <ior> "
                           "-i <iterations> "
                           "-d <datasize> "
                           "-x shutdown "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      std::string env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const obj = orb->string_to_object (options.ior);

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Nil server object reference\n"),
                          -1);

      TX_Object_var const tx_object = TX_Object::_narrow (obj.in ());
      DataSeq data_input {};
      CORBA::ULong const len = static_cast<CORBA::ULong> (options.data_size);
      data_input.length (len);

      // Fill in some useless data.
      for (CORBA::ULong i = 0; i < len; ++i)
        data_input[i] = static_cast<CORBA::Octet> (i % 9);

      for (long iteration {}; iteration < options.number_iterations; ++iteration)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Sending octet sequence of length:\t%u\n",
                      data_input.length ()));

          tx_object->send (data_input);

          DataSeq_var data_output {};
          tx_object->recv (data_output.out ());

          ACE_DEBUG ((LM_DEBUG,
                      "Received octet sequence of length:\t%u\n",
                      data_output->length ()));

          if (data_output->length () != len
              || std::memcmp (data_input.get_buffer (),
                              data_output->get_buffer (),
                              len) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "ERROR: Received octet sequence does not match "
                               "the one that was sent.\n"),
                              -1);
        }

      if (options.shutdown_server)
        tx_object->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"
              "SSLIOP Big_Request test passed.\n"));
  return 0;
}
