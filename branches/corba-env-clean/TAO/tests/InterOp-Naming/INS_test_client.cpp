// $Id$


#include "INSC.h"

int
main (int argc, char *argv[])
{
  int i = 0;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Retrieve a reference to the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (argc < 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "No Name specified to be resolved\n"));
          return -1;
        }

      CORBA::Object_var objref;

      for (i = 1; i < argc; ++i)
        {
          if (ACE_OS::strcmp (argv[i], "-l") == 0)
            {
              // List initial services
              CORBA::ORB::ObjectIdList_var list =
                orb->list_initial_services (TAO_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              size_t length = list->length ();

              if (length > 0)
                {
                  ACE_DEBUG ((LM_INFO,
                              "Listing initial references...\n"));

                  for (size_t n = 0; n < length; ++n)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "  Reference %u: %s\n",
                                  n,
                                  list[n].in ()));
                    }
                }
              else
                {
                  // We should at least get TAO's initial references,
                  // such as "RootPOA", "NameService", etc.
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "\n"
                                     "No initial references. "
                                     "This condition shouldn't happen.\n"),
                                    -1);
                }
            }
          else
            {
              objref = orb->resolve_initial_references (argv[i] TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (CORBA::is_nil (objref.in ()))
                ACE_ERROR_RETURN ((LM_ERROR,
                                   " (%P|%t) Unable to resolve the "
                                   "given name.\n"),
                                  -1);

              INS_var server = INS::_narrow (objref.in ()
                                                 TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              ACE_DEBUG ((LM_DEBUG,
                          "Resolved IOR for %s : %s\n",
                          argv[i],
                          orb->object_to_string (server.in ())));

              CORBA::String_var test_ins_result =
                server->test_ins (TAO_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              ACE_DEBUG ((LM_DEBUG,
                          "\nResult of Remote Call : %s\n",
                          test_ins_result.in ()));
            }
        }
    }
  ACE_CATCH (CORBA::ORB::InvalidName, name)
    {
      ACE_DEBUG ((LM_DEBUG, "Cannot resolve <%s>\n", argv[i]));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception:");
    }

  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
