#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"
#include "TriggerC.h"

const char *id = 0;
double load = 1.0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:l:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'i':
          id = get_opts.opt_arg ();
          break;
        case 'l':
          load = strtod (get_opts.opt_arg (), 0);
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:%s "
                             "-i <id>"
                             "-l <load>"
                             "\n",
                             argv [0]),
                            -1);
      }
  if (!id)
    {
      return -1;
    }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");


      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to resolve the Name Service.\n"),
                            -1);
        }

      CosNaming::NamingContext_var namingContext =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (id);

      CORBA::Object_var tmp = namingContext->resolve (name);

      ::CIAO::RACE::Trigger_var trigger =
          ::CIAO::RACE::Trigger::_narrow (tmp.in ());

      if (CORBA::is_nil (trigger.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Obj reference of %s "
                             "is nil!\n", id) , -1);
        }
      trigger->load (load);

      orb->destroy ();

    }
  catch (::CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Exception caught:");
      return 1;
    }
  return 0;
}
