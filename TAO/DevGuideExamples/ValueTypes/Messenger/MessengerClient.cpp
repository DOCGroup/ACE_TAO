// $Id$

#include "_pch.h"
#include "Message_i.h"
#include <iostream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
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

int ACE_TMAIN (int argc, ACE_TCHAR* argv[]) {
  try {

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;
    // Normally we wouldn't have to register the factory in the client, but
    // in this case the valuetype will be returned as an inout parameter, and
    // we'll need the factory to support this.
    MessageFactory::register_new_factory(* orb.in());

    CORBA::Object_var obj = orb->string_to_object(ior);
    Messenger_var tst = Messenger::_narrow(obj.in());
    ACE_ASSERT(! CORBA::is_nil(tst.in()));

    Message_var msg = new MessageImpl("Son", "Mom", "Dinner's Ready.", "Hurry home.");

    if (tst->send_message(msg)) {

      std::cout << "Message sent successfully.\n";
      msg->print();
      std::cout << std::endl;

    } else {
      std::cout << "Message refused." << std::endl;
    }

    while (orb->work_pending()) {
      orb->perform_work();
    }

    orb->destroy();

  } catch(const CORBA::Exception& e) {
    std::cerr << e << std::endl;
    return 1;
  }

  return 0;
}
