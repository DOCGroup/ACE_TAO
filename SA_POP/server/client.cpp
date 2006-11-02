#include "DriverC.h"
#include "ace/Get_Opt.h"

const char *ior = "file://server.ior";
const char *plan = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:d:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'd':
        plan = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-d <deployment plan uri> "
                           "\n",
                           argv [0]),
                          -1);
      }
  if (plan != 0)
    {
      // Indicates sucessful parsing of the command line
      return 0;
    }

  else
    {
      ACE_ERROR ((LM_ERROR, "URI of the deployment plan is necessary!\n"));
      return -1;
    }
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      ::CIAO::RACE::SA_POP::Driver_var driver =
          ::CIAO::RACE::SA_POP::Driver::_narrow(tmp.in ());

      if (CORBA::is_nil (driver.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Driver reference <%s>\n", ior), 1);
        }

      driver->deploy_plan (plan);

      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Exception caught:");
      return 1;
    }
  return 0;
}
