// $Id$

#include "ControllerC.h"
#include "ace/Get_Opt.h"

//IOR file of the Controller
const char * ior = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:s");
  int c = 0;
  int ret = 0;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
          break;

        case 's':
          ret = 1;
          break;

        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR, "Usage:  %s\n"
                             "-k <Controller IOR> "
                             "(defaults to file://Controller.ior)\n",
                             "-s Start the controller "
                             "(defaults to stop the controller)\n",
                             argv [0]),
                            -1);
          break;
        }

    }

  if (ior  == 0)
    {
      ior = "file://Controller.ior";
    }

  return ret;
}

int
main (int argc, char *argv[])
{
  try
    {

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");
      int ret = parse_args (argc, argv);
      if (ret == -1)
        {
          return -1;
        }

      CORBA::Object_var obj = orb->string_to_object (ior);

      CIAO::RACE::Trigger_var trigger =
        CIAO::RACE::Trigger::_narrow (obj.in ());

      if (CORBA::is_nil (trigger.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire Controller's objref\n"),
                            -1);
        }

      if (ret == 1)
        {
          trigger->start_controller ();
        }
      else
        {
          trigger->stop_controller ();
        }
      orb->destroy ();
    }

  catch (CORBA::Exception & ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Unknown exception \n");
      return -1;
    }
  return 0;
}
