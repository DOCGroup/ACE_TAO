#include "MessengerC.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "ace/Get_Opt.h"

#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
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
      // Initialize orb
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};
      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      // Destringify ior
      CORBA::Object_var const obj =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      // Narrow
      Messenger_var const messenger = Messenger::_narrow (obj.in ());
      if (CORBA::is_nil (messenger.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Panic: nil reference\n"),
                            1);
        }

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var const obj2 =
        orb->resolve_initial_references ("CodecFactory");

      IOP::CodecFactory_var const codec_factory =
        IOP::CodecFactory::_narrow (obj2.in ());

      // Set up a structure that contains information necessary to
      // create a GIOP 1.2 CDR encapsulation Codec.
      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 2;

      // Obtain the CDR encapsulation Codec.
      IOP::Codec_var const codec = codec_factory->create_codec (encoding);

      CORBA::String_var const user_name = CORBA::string_dup ("Ru");

      CORBA::Any user_name_as_any {};
      user_name_as_any <<= user_name.in ();

      CORBA::OctetSeq_var const client_user_name = codec->encode (user_name_as_any);

      messenger->send_message (client_user_name.in ());

      messenger->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("Caught CORBA exception: ");
      return 1;
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
