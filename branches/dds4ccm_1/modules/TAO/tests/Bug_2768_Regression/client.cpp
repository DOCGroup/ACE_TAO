// $Id$

#include "MessengerC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("");

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
  // Indicates sucessful parsing of the command line
  return 0;
}


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    //init the orb
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var object_temp =
      orb->string_to_object (ior);

    SimpleMessenger::Messenger_var messenger =
      SimpleMessenger::Messenger::_unchecked_narrow( object_temp.in ());

    CORBA::PolicyList_var unused;
    messenger->_validate_connection(unused);

    messenger->send();

    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

