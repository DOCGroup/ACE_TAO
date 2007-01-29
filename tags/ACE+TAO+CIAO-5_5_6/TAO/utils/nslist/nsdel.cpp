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
//     redone   Jun 21, 2006 Simon Massey <sma@prismtech.com>
//
// ================================================================

#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Argv_Type_Converter.h"

int
ACE_TMAIN (int argcw, ACE_TCHAR *argvw[])
{
  CosNaming::Name the_name (0);
  CORBA::ORB_var orb;

  try
    {
      // Contact the orb
      ACE_Argv_Type_Converter argcon (argcw, argvw);
      orb = CORBA::ORB_init (argcon.get_argc (), argcon.get_ASCII_argv (),
                             "");

      // Scan through the command line options
      bool
        failed = false,
        quiet = false,
        destroy = false;
      int
        argc = argcon.get_argc ();
      ACE_TCHAR
        **argv = argcon.get_TCHAR_argv ();
      const char
        *const pname = ACE_TEXT_ALWAYS_CHAR (argv[0]),
        *nameService = 0;
      char
        kindsep = '.',
        ctxsep[] = "/",
        *name = 0;

      if (0 < argc)
        {
          while (0 < --argc)
            {
              ++argv;
              if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--ns")))
                {
                  if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --ns requires an argument\n"));
                      failed= true;
                    }
                  else
                    {
                      ++argv;
                      if (nameService)
                        {
                          ACE_DEBUG ((LM_DEBUG,
                                     "Error: more than one --ns.\n"));
                          failed= true;
                        }
                      else
                        nameService = ACE_TEXT_ALWAYS_CHAR (*argv);
                    }
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--quiet")))
                {
                  quiet = true;
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--name")))
                {
                  if (name)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: more than one --name\n"));
                      failed = true;
                    }
                  else if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "Error: --name requires an argument\n"));
                      failed = true;
                    }
                  else
                    name = ACE_TEXT_ALWAYS_CHAR (*++argv);
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--ctxsep")))
                {
                  if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "Error: --ctxsep requires a character\n"));
                      failed = true;
                    }
                  else if (1 != ACE_OS::strlen(*++argv))
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --ctxsep takes a single character (not %s)\n", *argv));
                      failed = true;
                    }
                  else
                    ctxsep[0] = ACE_TEXT_ALWAYS_CHAR (*argv)[0];
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--kindsep")))
                {
                  if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "Error: --kindsep requires a character\n"));
                      failed = true;
                    }
                  else if (1 != ACE_OS::strlen(*++argv))
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --kindsep takes a single character (not %s)\n", *argv));
                      failed = true;
                    }
                  else
                    kindsep = ACE_TEXT_ALWAYS_CHAR (*argv)[0];
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--destroy")))
                {
                  destroy = true;
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                             "Unknown option %s\n", *argv));
                  failed = true;
                }
            }
        }

      if (!name || failed)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nUsage:\n  %s --name <name>\n"
                      "optional:\n"
                      "  --ns <ior>\n"
                      "  --ctxsep  <character>\n"
                      "  --kindsep <character>\n"
                      "  --destroy\n"
                      "  --quiet\n\n"
                      "where <name> uses the --ctxsep character (defaults to /)\n"
                      "to separate sub-contexts and --kindsep (defaults to .)\n"
                      "to separate ID & Kind. Will destroy a naming context\n"
                      "before unbinding if --destroy is given, otherwise it\n"
                      "will orphan them. Connects to default NameService\n"
                      "unless --ns is given. Displays all ID/Kinds found\n"
                      "on path unless --quiet is given.\n",
                      pname));
          orb->destroy ();
          return 1;
        }

      // Contact the name service
      CORBA::Object_var nc_obj;
      if (nameService)
        nc_obj = orb->string_to_object (nameService);
      else
        nc_obj = orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var root_nc =
        CosNaming::NamingContext::_narrow (nc_obj.in ());

      if (CORBA::is_nil (root_nc.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error: nil naming context\n"));
          orb->destroy ();
          return 1;
        }

      // Assemble the name from the user string given
      char *cp;
      while (0 != (cp = ACE_OS::strtok (name, ctxsep)))
        {
          const int index= the_name.length();
          the_name.length (index+1);
          char *kind = (char *)ACE_OS::strchr (cp, kindsep);
          if (kind)
            {
              *kind = '\0';
              the_name[index].kind= CORBA::string_dup (++kind);
            }
          the_name[index].id = CORBA::string_dup (cp);
          name = 0; // way strtok works
        }

      // Attempt to locate the object and destroy/unbind it
      CORBA::Object_var
        obj = root_nc->resolve( the_name );
      CosNaming::NamingContext_var this_nc =
        CosNaming::NamingContext::_narrow (obj.in ());
      if (!CORBA::is_nil (this_nc.in ()))
        {
          if (destroy)
            {
              if (!quiet)
                ACE_DEBUG ((LM_DEBUG,"Destroying\n"));
              this_nc->destroy( );
            }
          else if (!quiet)
            {
              CORBA::String_var str =
                orb->object_to_string (obj.in ());
              ACE_DEBUG ((LM_DEBUG,
                         "\n*** Possiably Orphaned Naming Context ***\n%s\n\n", str.in()));
            }
        }
      else if (destroy && !quiet)
        ACE_DEBUG ((LM_DEBUG,"Can't Destroy object, it is not a naming context!\n"));
      root_nc->unbind (the_name );
      if (!quiet)
        {
          unsigned int index;
          for (index= 0u; index < the_name.length()-1u; ++index)
            {
              if (the_name[index].kind && the_name[index].kind[0])
                ACE_DEBUG ((LM_DEBUG, "Found ID: %s  (Kind: %s)\n",
                           the_name[index].id.in(),
                           the_name[index].kind.in()));
              else
                ACE_DEBUG ((LM_DEBUG, "Found ID: %s\n",
                           the_name[index].id.in()));
            }
          ACE_DEBUG ((LM_DEBUG, "UnBound ID: %s",
                     the_name[index].id.in()));
          if (the_name[index].kind && the_name[index].kind[0])
            ACE_DEBUG ((LM_DEBUG, "  (Kind: %s)\n",
                       the_name[index].kind.in()));
          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
    }
  catch (const CosNaming::NamingContext::NotFound& nf)
    {
      unsigned int index;
      const unsigned int limit= the_name.length()-nf.rest_of_name.length();
      ACE_DEBUG ((LM_DEBUG, "\nError:\n"));
      for (index= 0u; index < limit; ++index)
        {
          if (the_name[index].kind && the_name[index].kind[0])
             ACE_DEBUG ((LM_DEBUG, "ID: %s  (Kind: %s)\n",
               the_name[index].id.in(),
               the_name[index].kind.in()));
          else
             ACE_DEBUG ((LM_DEBUG, "ID: %s\n",
               the_name[index].id.in()));
        }
      const char *why= "Unknown reason";
      switch (nf.why)
        {
        case CosNaming::NamingContext::missing_node:
          why= "\nThe following node is missing";
          break;
        case CosNaming::NamingContext::not_context:
          why= "\nThe following is a final object binding, not a naming context";
          break;
        case CosNaming::NamingContext::not_object:
          why= "\nThe following is a naming context, not a final object binding";
          break;
        }
      nf._tao_print_exception (why);
      for (index= 0u; index < nf.rest_of_name.length(); ++index)
        {
          if (nf.rest_of_name[index].kind && nf.rest_of_name[index].kind[0])
             ACE_DEBUG ((LM_DEBUG, "ID: %s  (Kind: %s)\n",
               nf.rest_of_name[index].id.in(),
               nf.rest_of_name[index].kind.in()));
          else
             ACE_DEBUG ((LM_DEBUG, "ID: %s\n",
               nf.rest_of_name[index].id.in()));
        }
      orb->destroy ();
      return 1;
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "\nError:\n"));
      ex._tao_print_exception ("Exception in nsdel");
      orb->destroy ();
      return 1;
    }

  orb->destroy ();
  return 0;
}
