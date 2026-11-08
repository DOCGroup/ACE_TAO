#include "MessengerC.h"
#include "ace/Get_Opt.h"
#include <iostream>

struct Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  bool call_shutdown {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x"));
  int c {};

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'x':
        options.call_shutdown = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s\n"
                           "-k <ior> "
                           "-x - call shutdown on server\n",
                           argv[0]),
                           -1);
    }
  }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    // Parse arguments.
    Options options;
    if (parse_args (argc, argv, options) != 0)
      return 1;

    // Read and destringify the Messenger object's IOR.
    CORBA::Object_var const obj = orb->string_to_object (options.ior);
    if (CORBA::is_nil (obj.in ())) {
      std::cerr << "Could not get Messenger IOR." << std::endl;
      return 1;
    }

    // Narrow the IOR to a Messenger object reference.
    Messenger_var const messenger = Messenger::_narrow (obj.in ());
    if (CORBA::is_nil (messenger.in ())) {
      std::cerr << "IOR was not a Messenger object reference." << std::endl;
      return 1;
    }

    // Send a message the the Messenger object.
    CORBA::String_var message = CORBA::string_dup ("Hello!");
    messenger->send_message ("TAO User", "TAO Test", message.inout ());

    // Print the Messenger's reply.
    std::cout << "Reply: " << message.in () << std::endl;

    // Shutdown server?
    if (options.call_shutdown)
    {
      messenger->shutdown ();
      orb->perform_work ();
    }

    orb->destroy ();
  }
  catch (CORBA::Exception const &ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
