// $Id$

// ================================================================
//
// = LIBRARY
//    utils
//
// = FILENAME
//    nsadd.cpp
//
// = DESCRIPTION
//    Naming Service add utility
//
// = AUTHOR
//     Carlos O'Ryan
//
// ================================================================

#include "orbsvcs/CosNamingC.h"

CORBA::ORB_var orb;
int showIOR = 0;
int showNSonly = 0;

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      char *pname = argv[0];

      const char * ior = 0;
      const char * name = 0;
      int rebind = 0;
      while (argc > 0)
        {
          if (strcmp(*argv, "--ior") == 0)
            {
              if (argc == 1)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Error: --ior requires an argument\n"));
                  return 1;
                }
              argc--;
              argv++;
              ior = *argv;
            }
          else if (strcmp(*argv, "--name") == 0)
            {
              if (argc == 1)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Error: --name requires an argument\n"));
                  return 1;
                }
              argc--;
              argv++;
              name = *argv;
            }
          else if (strcmp(*argv, "--rebind") == 0)
            {
              rebind = 1;
            }
          else if (strncmp(*argv, "--", 2) == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Usage: %s --name <name> "
                          "--ior <ior> [ --rebind ]  \n", pname));
              return 1;
            }
          argc--;
          argv++;
        }
      if (ior == 0 || name == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s --name <name> "
                      "--ior <ior> [ --rebind ]  \n", pname));
          return 1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var nc_obj =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var root_nc =
        CosNaming::NamingContext::_narrow (nc_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (root_nc.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error: nil naming context\n"));
          return 1;
        }

      CosNaming::Name the_name (1);
      the_name.length (1);
      the_name[0].id = CORBA::string_dup (name);
      if (rebind)
        {
          root_nc->rebind (the_name, obj.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          root_nc->bind (the_name, obj.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in nsadd");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
