// $Id$

// ================================================================
//
// = LIBRARY
//    utils
//
// = FILENAME
//    nsdel.cpp
//
// = DESCRIPTION
//    Naming Service del utility
//
// = AUTHOR
//     Carlos O'Ryan <coryan@uci.edu>
//     enhanced Jan 15, 2001 Paul Caffrey <denginere@hotmail.com>
//
// ================================================================

#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Argv_Type_Converter.h"

CORBA::ORB_var orb;
int showIOR = 0;
int showNSonly = 0;

const char* USAGE = "Usage: %s [ --name <name> ] [--destroy]\n";
int
ACE_TMAIN (int argcw, ACE_TCHAR *argvw[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  int i;
  int destroy_after_unbind = 0;
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
      const ACE_TCHAR *name = 0;

      while (argc > 0)
        {
          if (ACE_OS::strcmp (*argv, ACE_TEXT ("--name")) == 0)
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
          else if (ACE_OS::strcmp (*argv, ACE_TEXT ("--destroy")) == 0)
            {
              destroy_after_unbind = 1;
            }
          else if (ACE_OS::strncmp(*argv, ACE_TEXT ("--"), 2) == 0)
            {
              ACE_DEBUG ((LM_DEBUG, USAGE, pname));
              return 1;
            }
          argc--;
          argv++;
        }

      if (name == 0)
        {
          ACE_DEBUG ((LM_DEBUG, USAGE, pname));
          return 1;
        }
        // make a copy

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
            if (cp == NULL)
              {
                printf ("break, null token\n");
                break;
              }
            bp = 0; // way strtok works
          }

        for (i=0; i<ntoks; i++)
          ACE_OS::printf ("%s\n", toks[i]);

        // now assign name = toks[ntoks]
        char lastname[BUFSIZ];
        ACE_OS::strcpy (lastname, toks[ntoks - 1]);

        // search for '.' in name; if exists then the part after '.' is the kind
        char *kind = ACE_OS::strchr (lastname, '.');

        if (kind != 0)
          {
            *kind = 0;
            kind++;
            ACE_OS::printf ("name='%s'\n", lastname);
            ACE_OS::printf ( "kind='%s'\n" , kind );
          }
        else
          ACE_OS::printf ("name=%s\n", lastname );

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

      CosNaming::Name the_name (ntoks);
      the_name.length (ntoks);
      for (i=0; i<(ntoks - 1); i++) // up to last directory entry
        the_name[i].id = CORBA::string_dup (toks[i]);

      the_name[ntoks - 1].id = CORBA::string_dup (lastname);
      if (kind != 0)
        the_name[ntoks - 1].kind = CORBA::string_dup (kind);

      // This needs to be bounded by a try/catch
      CORBA::Object_var the_context = root_nc->resolve (the_name ACE_ENV_ARG_PARAMETER);
      root_nc->unbind (the_name ACE_ENV_ARG_PARAMETER );

      if (destroy_after_unbind)
        {
          CosNaming::NamingContext_var nc =
            CosNaming::NamingContext::_narrow (the_context.in () ACE_ENV_ARG_PARAMETER);
          if (! CORBA::is_nil(nc.in()))
            nc->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
        }

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in nsdel");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
