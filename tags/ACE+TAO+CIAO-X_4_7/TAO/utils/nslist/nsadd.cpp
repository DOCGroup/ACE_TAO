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
//     enhanced Nov 6, 2002 Paul Caffrey <denginere@hotmail.com>
//
// ================================================================

#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/Argv_Type_Converter.h"

CORBA::ORB_var orb;
int showIOR = 0;
int showNSonly = 0;

int
ACE_TMAIN (int argcw, ACE_TCHAR *argvw[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_Argv_Type_Converter argcon (argcw, argvw);
      CORBA::ORB_var orb =
        CORBA::ORB_init (argcon.get_argc (), argcon.get_ASCII_argv (),
                         "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int argc = argcon.get_argc ();
      ACE_TCHAR** argv = argcon.get_TCHAR_argv ();

      ACE_TCHAR *pname = argv[0];

      const ACE_TCHAR * ior = 0;
      const ACE_TCHAR * name = 0;
      int rebind = 0;
      int newcontext = 0;
      while (argc > 0)
        {
          if (ACE_OS::strcmp (*argv, ACE_TEXT ("--ior")) == 0)
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
          else if (ACE_OS::strcmp (*argv, ACE_TEXT ("--name")) == 0)
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
          else if (ACE_OS::strcmp (*argv, ACE_TEXT ("--rebind")) == 0)
            {
              rebind = 1;
            }
          else if (ACE_OS::strcmp(*argv, ACE_TEXT ("--newcontext")) == 0)
          {
             newcontext = 1;
          }
          else if (ACE_OS::strncmp (*argv, ACE_TEXT ("--"), 2) == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Usage: %s --name <name> "
                          "--ior <ior> [ --rebind ] "
                          "[ --newcontext ] \n", pname));
              return 1;
            }

          argc--;
          argv++;
        }
      if (( ior == 0 && newcontext == 0 ) || name == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s --name <name> "
                      "--ior <ior> [ --rebind ]  \n", pname));
          return 1;
        }

      CORBA::Object_var obj;
      if (ior)
        {
          obj = orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior) ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

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
      char buf[BUFSIZ];
      ACE_OS::strcpy (buf, ACE_TEXT_ALWAYS_CHAR (name));
      char *bp = &buf[0];
      char *cp = 0;
      int ntoks = 0;
      char *toks[20];
      while ((cp = ACE_OS::strtok (bp, "/")) != 0)
        {
          toks[ntoks] = cp;
          ntoks++;
          if (cp == 0)
            break;
          bp = 0; // way strtok works
        }

      // now assign name = toks[ntoks]
      char lastname[BUFSIZ];
      ACE_OS::strcpy (lastname, toks[ntoks-1]);
      // search for '.' in name; if exists then the part after '.' is the kind
      char *kind = ACE_OS::strchr (lastname, '.');
      if (kind != 0)
        {
          *kind = 0;
          kind++;
        }

      // Create any necessary subcontexts.
      for (CORBA::Long subs = 1; subs < ntoks; subs++)
      {
        ACE_TRY_EX(inner)
        {
          CosNaming::Name the_context (subs);
          the_context.length (subs);
          for (CORBA::Long i=0; i<subs; i++)
          {
            the_context[i].id = CORBA::string_dup (toks[i]);
          }
          root_nc->bind_new_context(the_context);
          ACE_TRY_CHECK_EX(inner);
        }
        ACE_CATCHANY
        {
        }
        ACE_ENDTRY;
      }

      ACE_TRY_CHECK;
      CosNaming::Name the_name (ntoks);
      the_name.length (ntoks);
      for (CORBA::Long i=0; i<ntoks; i++)
        the_name[i].id = CORBA::string_dup (toks[i]);

      if (rebind)
        {
          root_nc->rebind (the_name, obj.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (!newcontext)
        {
          root_nc->bind (the_name, obj.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if(newcontext)
      {
         root_nc->bind_new_context(the_name);
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


