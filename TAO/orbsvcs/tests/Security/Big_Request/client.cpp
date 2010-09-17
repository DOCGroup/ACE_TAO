// $Id$

#include "TX_ObjectC.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
const char *cert_file = "cacert.pem";
long number_iterations = 1;
long data_size = 3461724;
bool shutdown_server = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:d:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'i':
        number_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'd':
        data_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        shutdown_server = true;
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

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_CString env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (::parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj =
        orb->string_to_object (ior);

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Nil server object reference\n"),
                          -1);

      TX_Object_var txObject =
        TX_Object::_narrow (obj.in ());

      DataSeq data_input;

      CORBA::ULong const len = data_size;

      data_input.length (len);

      // Fill in some useless data.
      for (CORBA::ULong i = 0; i < len; ++i)
        {
          data_input[i] = i % 9;
        }

      for (int iteration = 0; iteration < number_iterations; ++iteration)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Sending octet sequence of length:\t%u\n",
                      data_input.length ()));

          txObject->send (data_input);

          DataSeq_var data_output;

          txObject->recv (data_output.out ());

          ACE_DEBUG ((LM_DEBUG,
                      "Received octet sequence of length:\t%u\n",
                      data_output->length ()));

          // Sanity check
          if (data_output->length () != len
              || ACE_OS::memcmp (data_input.get_buffer (),
                                 data_output->get_buffer (),
                                 len) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "ERROR: Received octet sequence does not match "
                                 "the one that was sent.\n"),
                                -1);
            }
        }

      if (shutdown_server)
        {
          txObject->shutdown ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ERROR");

      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"
              "SSLIOP Big_Request test passed.\n"));

  return 0;
}
