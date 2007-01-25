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
//     redone   Jun 21 2006 Simon Massey <sma@prismtech.com>
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
  CosNaming::Name the_name (0);
  CORBA::ORB_var orb;
  const char *ior = 0;

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
        rebind = false,
        context = false;
      int
        argc = argcon.get_argc ();
      ACE_TCHAR
        **argv = argcon.get_TCHAR_argv ();
      const char
        *const pname = ACE_TEXT_ALWAYS_CHAR (argv[0]),
        *nameService = 0;
      char
        kindsep= '.',
        ctxsep[]= "/",
        *name = 0;

      if (0 < argc)
        {
          while (--argc > 0)
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
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT("--quiet")))
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
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--ior")))
                {
                  if (ior)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                        "Error: ior already specified\n"));
                      failed = true;
                    }
                  else if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "Error: --ior requires an IOR\n"));
                      failed = true;
                    }
                  else
                    ior = ACE_TEXT_ALWAYS_CHAR (*++argv);
                }
              else if ((0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--ctx")))
                       ||(0 == ACE_OS::strcmp(*argv, ACE_TEXT ("--newcontext"))))
                {
                  context = true;
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--rebind")))
                {
                  rebind = true;
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                             "Unknown option %s\n", ACE_TEXT_ALWAYS_CHAR (*argv)));
                  failed = true;
                }
            }
        }

      if (failed || !name || (!ior && !context))
        {
          ACE_DEBUG ((LM_DEBUG,
            "\nUsage:\n"
            "  %s --name <name> --ior <ior>\n"
            "  %s --name <name> --ctx\n"
            "  %s --name <name> --ior <ior> --ctx\n"
            "optional:\n"
            "  --ns <ior>\n"
            "  --rebind\n"
            "  --ctxsep  <character>\n"
            "  --kindsep <character>\n"
            "  --newcontext  {same as --ctx}\n"
            "  --quiet\n\n"
            "where <name> uses the --ctxsep (defaults to /) to separate\n"
            "sub-contexts and --kindsep (defaults to .) to separate ID\n"
            "and Kind. If --ctx is NOT given; binds a final object.\n"
            "If --ctx is given without --ior; binds a new naming context.\n"
            "If both --ior and --ctx are specified; binds the existing\n"
            "naming context specified. If --rebind is given, will replace\n"
            "existing bindings of the same type (object or context).\n"
            "Connects to the default NameService unless --ns is given.\n"
            "Displays all ID/Kinds found/created on path unless --quiet\n"
            "is given.\n",
            pname, pname, pname));
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

      // Get the object reference from the IOR string given
      CORBA::Object_var obj;
      if (ior)
        {
          obj = orb->string_to_object (ior);
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

      // Now attempt the (re)bind
      try
        {
          if (!ior)
            obj= root_nc->bind_new_context (the_name);
          else if (context)
            {
              CosNaming::NamingContext_var this_nc =
                CosNaming::NamingContext::_narrow (obj.in ());

              if (rebind)
                root_nc->rebind_context (the_name, this_nc.in ());
              else
                root_nc->bind_context (the_name, this_nc.in ());
            }
          else if (rebind)
            root_nc->rebind (the_name, obj.in ());
          else
            root_nc->bind (the_name, obj.in ());
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
              if (!ior)
                ACE_DEBUG ((LM_DEBUG, "Created sub-context"));
              else if (rebind)
                ACE_DEBUG ((LM_DEBUG, "ReBound"));
              else
                ACE_DEBUG ((LM_DEBUG, "Bound"));
              ACE_DEBUG ((LM_DEBUG, " ID: %s",
                         the_name[index].id.in()));
              if (the_name[index].kind && the_name[index].kind[0])
                ACE_DEBUG ((LM_DEBUG, "  (Kind: %s)\n",
                           the_name[index].kind.in()));
              ACE_DEBUG ((LM_DEBUG, "\n"));
            }
        }
      catch (const CosNaming::NamingContext::NotFound& nf)
        {
          if (CosNaming::NamingContext::missing_node != nf.why)
            throw; // report error to outer try/catch

          CosNaming::Name fullName (the_name);
          int
            index,
            found= the_name.length() - nf.rest_of_name.length(),
            &limit= found;
          if (quiet)
            index= found;
          else
            {
              for (index= 0; index < found; ++index)
                {
                  if (the_name[index].kind && the_name[index].kind[0])
                    ACE_DEBUG ((LM_DEBUG, "Found ID: %s  (Kind: %s)\n",
                               the_name[index].id.in(),
                               the_name[index].kind.in()));
                  else
                    ACE_DEBUG ((LM_DEBUG, "Found ID: %s\n",
                               the_name[index].id.in()));
                }
            }

          // need to create the following missing naming contexts
          limit= fullName.length() - ((ior)?1:0);
          while (index < limit)
            {
              // As soon as we limit the length, later names are gone so
              // we have to recopy them.
              the_name.length( index + 1 );
              the_name[index].id= CORBA::string_dup (fullName[index].id);
              the_name[index].kind= CORBA::string_dup (fullName[index].kind);

              if (!quiet)
                {
                  if (the_name[index].kind && the_name[index].kind[0])
                    ACE_DEBUG ((LM_DEBUG, "Creating sub-context ID: %s  (Kind: %s)\n",
                               the_name[index].id.in(),
                               the_name[index].kind.in()));
                  else
                    ACE_DEBUG ((LM_DEBUG, "Creating sub-context ID: %s\n",
                               the_name[index].id.in()));
                }

              CosNaming::NamingContext_var this_nc =
                root_nc->bind_new_context (the_name);
// report error to outer try/catch
              ++index;
            }

          if (ior)
            {
              // As soon as we limit the length, later names are gone so
              // we have to recopy them.
              the_name.length( index + 1 );
              the_name[index].id= CORBA::string_dup (fullName[index].id);
              the_name[index].kind= CORBA::string_dup (fullName[index].kind);
              if (!quiet)
                {
                  if (rebind)
                    ACE_DEBUG ((LM_DEBUG, "Re"));
                  ACE_DEBUG ((LM_DEBUG, "Binding ID: %s",
                             the_name[index].id.in()));
                  if (the_name[index].kind && the_name[index].kind[0])
                    ACE_DEBUG ((LM_DEBUG, "  (Kind: %s)\n",
                               the_name[index].kind.in()));
                  ACE_DEBUG ((LM_DEBUG, "\n"));
                }
              // Now re-attempt the (re)bind
              if (context)
                {
                  CosNaming::NamingContext_var this_nc =
                    CosNaming::NamingContext::_narrow (obj.in ());
// This is not a naming context, report to outer

                  if (rebind)
                    root_nc->rebind_context (the_name, this_nc.in ());
                  else
                    root_nc->bind_context (the_name, this_nc.in ());
                }
              else if (rebind)
                root_nc->rebind (the_name, obj.in ());
              else
                root_nc->bind (the_name, obj.in ());
            }
        }
    }
  catch (const CosNaming::NamingContext::NotFound& nf)
    {
      unsigned int index;
      ACE_DEBUG ((LM_DEBUG, "\nError:\n"));
      for (index= 0u; index < the_name.length()-nf.rest_of_name.length(); ++index)
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
      ex._tao_print_exception ("Exception in nsadd");
      orb->destroy ();
      return 1;
    }

  orb->destroy ();
  return 0;
}


