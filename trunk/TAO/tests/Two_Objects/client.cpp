// $Id$

#include "Two_ObjectsC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Test, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "r:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'r':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-r <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  // Used to declare the CORBA::Environment variable
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse the arguments
      if (parse_args (argc, argv) != 0)
        return 1;

      // Convert the ior string to an object reference.
      // The object reference obtained is a reference to the factory
      // object.
      CORBA::Object_var tmp =
        orb->string_to_object(ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the object reference to the appropriate type
      Two_Objects_Test::Object_Factory_var factory =
        Two_Objects_Test::Object_Factory::_narrow(tmp.in ()
                                                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Two_Objects_Test::Object_Factory reference <%s>\n",
                             ior),
                            1);
        }

      Two_Objects_Test::First_var first;
      Two_Objects_Test::Second_var second;

      // Use the factory object to create the first and second interfaces
      first = factory->create_first();
      second = factory->create_second();

      // Call the oneway method
      first->oneway_method (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Client : one way call done\n"));

      Two_Objects_Test::Octet_Seq_var reply_seq =
      second->twoway_method (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Client : length of returned data is %d\n",
                     reply_seq->length() ));

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
