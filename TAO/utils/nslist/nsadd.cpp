//=============================================================================
/**
 *  @file    nsadd.cpp
 *
 *  Naming Service add utility
 *
 *  @author  Carlos O'Ryan  enhanced Nov 6
 *  @author 2002 Paul Caffrey <denginere@hotmail.com>  redone   Jun 21 2006 Simon Massey <sma@prismtech.com>
 */
//=============================================================================

#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  struct Options
  {
    bool failed {};
    bool quiet {};
    bool rebind {};
    bool context {};
    ACE_TCHAR const *ior {};
    ACE_TCHAR const *name_service {};
    ACE_TCHAR kind_separator {ACE_TEXT('.')};
    ACE_TCHAR context_separator[2] {ACE_TEXT('/'), 0};
    ACE_TCHAR *name {};
  };
  Options options;
  CosNaming::Name the_name {};
  CORBA::ORB_var orb;

  int err {};

  try
    {
      // Contact the orb
      orb = CORBA::ORB_init (argc, argv);

      // Scan through the command line options
      ACE_TCHAR const *const pname = argv[0];

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
                      options.failed = true;
                    }
                  else
                    {
                      ++argv;
                      if (options.name_service)
                        {
                          ACE_DEBUG ((LM_DEBUG,
                                     "Error: more than one --ns.\n"));
                          options.failed = true;
                        }
                      else
                      {
                        options.name_service = *argv;
                      }
                    }
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT("--quiet")))
                {
                  options.quiet = true;
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--name")))
                {
                  if (options.name)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: more than one --name\n"));
                      options.failed = true;
                    }
                  else if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --name requires an argument\n"));
                      options.failed = true;
                    }
                  else
                  {
                    options.name = *++argv;
                  }
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--ctxsep")))
                {
                  if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --ctxsep requires a character\n"));
                      options.failed = true;
                    }
                  else if (1 != ACE_OS::strlen(*++argv))
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --ctxsep takes a single character (not %s)\n", *argv));
                      options.failed = true;
                    }
                  else
                  {
                    options.context_separator[0] = (*argv)[0];
                  }
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--kindsep")))
                {
                  if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --kindsep requires a character\n"));
                      options.failed = true;
                    }
                  else if (1 != ACE_OS::strlen(*++argv))
                    {
                       ACE_DEBUG ((LM_DEBUG,
                                  "Error: --kindsep takes a single character (not %s)\n", *argv));
                      options.failed = true;
                    }
                  else
                  {
                    options.kind_separator = (*argv)[0];
                  }
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--ior")))
                {
                  if (options.ior)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                        "Error: ior already specified\n"));
                      options.failed = true;
                    }
                  else if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "Error: --ior requires an IOR\n"));
                      options.failed = true;
                    }
                  else
                  {
                    options.ior = *++argv;
                  }
                }
              else if ((0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--ctx")))
                       ||(0 == ACE_OS::strcmp(*argv, ACE_TEXT ("--newcontext"))))
                {
                  options.context = true;
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--rebind")))
                {
                  options.rebind = true;
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                             "Unknown option %s\n", *argv));
                  options.failed = true;
                }
            }
        }

      if (options.failed || !options.name || (!options.ior && !options.context))
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
      if (options.name_service)
        {
          nc_obj = orb->string_to_object (options.name_service);
        }
      else
        {
          nc_obj = orb->resolve_initial_references ("NameService");
        }

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
      if (options.ior)
        {
          obj = orb->string_to_object (options.ior);
        }

      // Assemble the name from the user string given
      ACE_TCHAR *cp;
      while ((cp = ACE_OS::strtok (options.name, options.context_separator)) != nullptr)
        {
          CORBA::ULong const index = the_name.length ();
          the_name.length (index+1);
          ACE_TCHAR *kind = ACE_OS::strchr (cp, options.kind_separator);
          if (kind)
            {
              *kind = '\0';
              the_name[index].kind= CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(++kind));
            }
          the_name[index].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(cp));
          options.name = nullptr; // way strtok works
        }

      // Now attempt the (re)bind
      try
        {
          if (!options.ior)
            {
              obj= root_nc->bind_new_context (the_name);
            }
          else if (options.context)
            {
              CosNaming::NamingContext_var this_nc =
                CosNaming::NamingContext::_narrow (obj.in ());

              if (options.rebind)
                {
                  root_nc->rebind_context (the_name, this_nc.in ());
                }
              else
                {
                  root_nc->bind_context (the_name, this_nc.in ());
                }
            }
          else if (options.rebind)
            {
              root_nc->rebind (the_name, obj.in ());
            }
          else
            {
              root_nc->bind (the_name, obj.in ());
            }
          if (!options.quiet)
            {
              CORBA::ULong index {};
              for (; index < the_name.length () - 1u; ++index)
                {
                  if (the_name[index].kind && the_name[index].kind[0])
                    ACE_DEBUG ((LM_DEBUG, "Found ID: %C  (Kind: %C)\n",
                               the_name[index].id.in(),
                               the_name[index].kind.in()));
                  else
                    ACE_DEBUG ((LM_DEBUG, "Found ID: %C\n",
                               the_name[index].id.in()));
                }
              if (!options.ior)
                {
                  ACE_DEBUG ((LM_DEBUG, "Created sub-context"));
                }
              else if (options.rebind)
                {
                  ACE_DEBUG ((LM_DEBUG, "ReBound"));
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG, "Bound"));
                }
              ACE_DEBUG ((LM_DEBUG, " ID: %C",
                         the_name[index].id.in()));
              if (the_name[index].kind && the_name[index].kind[0])
                ACE_DEBUG ((LM_DEBUG, "  (Kind: %C)\n",
                           the_name[index].kind.in()));
              ACE_DEBUG ((LM_DEBUG, "\n"));
            }
        }
      catch (const CosNaming::NamingContext::NotFound& nf)
        {
          if (CosNaming::NamingContext::missing_node != nf.why)
          {
            throw; // report error to outer try/catch
          }

          CosNaming::Name fullName (the_name);
          CORBA::ULong index {};
          CORBA::ULong found = the_name.length () - nf.rest_of_name.length ();
          CORBA::ULong &limit = found;
          if (options.quiet)
            {
              index= found;
            }
          else
            {
              for (index= 0; index < found; ++index)
                {
                  if (the_name[index].kind && the_name[index].kind[0])
                    ACE_DEBUG ((LM_DEBUG, "Found ID: %C  (Kind: %C)\n",
                               the_name[index].id.in(),
                               the_name[index].kind.in()));
                  else
                    ACE_DEBUG ((LM_DEBUG, "Found ID: %C\n",
                               the_name[index].id.in()));
                }
            }

          // need to create the following missing naming contexts
          limit= fullName.length() - ((options.ior)?1:0);
          while (index < limit)
            {
              // As soon as we limit the length, later names are gone so
              // we have to recopy them.
              the_name.length( index + 1 );
              the_name[index].id= CORBA::string_dup (fullName[index].id);
              the_name[index].kind= CORBA::string_dup (fullName[index].kind);

              if (!options.quiet)
                {
                  if (the_name[index].kind && the_name[index].kind[0])
                    ACE_DEBUG ((LM_DEBUG, "Creating sub-context ID: %C  (Kind: %C)\n",
                               the_name[index].id.in(),
                               the_name[index].kind.in()));
                  else
                    ACE_DEBUG ((LM_DEBUG, "Creating sub-context ID: %C\n",
                               the_name[index].id.in()));
                }

              CosNaming::NamingContext_var this_nc =
                root_nc->bind_new_context (the_name);
// report error to outer try/catch
              ++index;
            }

          if (options.ior)
            {
              // As soon as we limit the length, later names are gone so
              // we have to recopy them.
              the_name.length( index + 1 );
              the_name[index].id= CORBA::string_dup (fullName[index].id);
              the_name[index].kind= CORBA::string_dup (fullName[index].kind);
              if (!options.quiet)
                {
                  if (options.rebind)
                  {
                    ACE_DEBUG ((LM_DEBUG, "Re"));
                  }
                  ACE_DEBUG ((LM_DEBUG, "Binding ID: %C",
                             the_name[index].id.in()));
                  if (the_name[index].kind && the_name[index].kind[0])
                    ACE_DEBUG ((LM_DEBUG, "  (Kind: %C)\n",
                               the_name[index].kind.in()));
                  ACE_DEBUG ((LM_DEBUG, "\n"));
                }
              // Now re-attempt the (re)bind
              if (options.context)
                {
                  CosNaming::NamingContext_var this_nc =
                    CosNaming::NamingContext::_narrow (obj.in ());
// This is not a naming context, report to outer

                  if (options.rebind)
                    {
                      root_nc->rebind_context (the_name, this_nc.in ());
                    }
                  else
                    {
                      root_nc->bind_context (the_name, this_nc.in ());
                    }
                }
              else if (options.rebind)
                {
                  root_nc->rebind (the_name, obj.in ());
                }
              else
                {
                  root_nc->bind (the_name, obj.in ());
                }
            }
        }
    }
  catch (const CosNaming::NamingContext::NotFound& nf)
    {
      CORBA::ULong index {};
      ACE_DEBUG ((LM_DEBUG, "\nError:\n"));
      for (; index < the_name.length () - nf.rest_of_name.length (); ++index)
        {
          if (the_name[index].kind && the_name[index].kind[0])
             ACE_DEBUG ((LM_DEBUG, "ID: %C  (Kind: %C)\n",
               the_name[index].id.in(),
               the_name[index].kind.in()));
          else
             ACE_DEBUG ((LM_DEBUG, "ID: %C\n",
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
      for (index = 0u; index < nf.rest_of_name.length (); ++index)
        {
          if (nf.rest_of_name[index].kind && nf.rest_of_name[index].kind[0])
             ACE_DEBUG ((LM_DEBUG, "ID: %C  (Kind: %C)\n",
               nf.rest_of_name[index].id.in(),
               nf.rest_of_name[index].kind.in()));
          else
             ACE_DEBUG ((LM_DEBUG, "ID: %C\n",
               nf.rest_of_name[index].id.in()));
        }
      ++err;
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "\nError:\n"));
      ex._tao_print_exception ("Exception in nsadd");
      ++err;
    }

  try
    {
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "\nError:\n"));
      ex._tao_print_exception ("Exception in while shutting down");
      ++err;
    }
  return err;
}

