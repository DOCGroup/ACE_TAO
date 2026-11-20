
//=============================================================================
/**
 *  @file    nsdel.cpp
 *
 *  Naming Service del utility
 *
 *  @author  Carlos O'Ryan <coryan@uci.edu>  enhanced Jan 15
 *  @author 2001 Paul Caffrey <denginere@hotmail.com>  redone   Jun 21
 *  @author 2006 Simon Massey <sma@prismtech.com>
 */
//=============================================================================

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Messaging/Messaging.h"
#include "tao/PolicyC.h"
#include "ace/Time_Value.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Argv_Type_Converter.h"

//============================================================================
namespace
{
  //==========================================================================
  CORBA::Object_ptr
  set_rtt (CORBA::ORB_ptr orb,
           CORBA::Object_ptr obj,
           ACE_Time_Value const &relative_roundtrip_timeout)
  {
    if (relative_roundtrip_timeout != ACE_Time_Value::zero)
    {
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
      TimeBase::TimeT roundTripTimeoutVal;
      ORBSVCS_Time::Time_Value_to_TimeT (roundTripTimeoutVal,
                                       relative_roundtrip_timeout);

      CORBA::Any anyObjectVal;
      anyObjectVal <<= roundTripTimeoutVal;
      CORBA::PolicyList polList (1);
      polList.length (1);

      polList[0] = orb->create_policy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
               anyObjectVal);

      CORBA::Object_var obj2 = obj->_set_policy_overrides(polList, CORBA::SET_OVERRIDE);
      polList[0]->destroy();
      return obj2._retn();
#else
      ACE_DEBUG ((LM_DEBUG, "RTT not supported in TAO build.\n"));
#endif
    }
    return CORBA::Object::_duplicate(obj);
  }
} // end of local unnamed namespace

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  struct Options
  {
    bool failed {};
    bool quiet {};
    bool destroy {};
    ACE_TCHAR const *name_service {};
    ACE_TCHAR kind_separator {ACE_TEXT('.')};
    ACE_TCHAR context_separator[2] {ACE_TEXT('/'), 0};
    ACE_TCHAR *name {};
    ACE_Time_Value relative_roundtrip_timeout {ACE_Time_Value::zero};
  };
  Options options;
  int err {};
  CosNaming::Name the_name {};
  CORBA::ORB_var orb;

  try
    {
      // Contact the orb
      orb = CORBA::ORB_init (argc, argv);

      // Scan through the command line options
      ACE_TCHAR const *const pname = argv[0];

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
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--quiet")))
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
                    options.name = *(++argv);
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
                  else if (1 != ACE_OS::strlen(*(++argv)))
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
                  else if (1 != ACE_OS::strlen(*(++argv)))
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
              else if (0 == ACE_OS::strcmp(*argv, ACE_TEXT ("--rtt")))
                {
                  if (options.relative_roundtrip_timeout != ACE_Time_Value::zero)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --rtt given more than once\n"));
                      options.failed = true;
                    }
                  else if (!--argc || !ACE_OS::ace_isdigit ((*(++argv))[0]))
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --rtt requires a number\n"));
                      options.failed = true;
                    }
                 else
                 {
                   options.relative_roundtrip_timeout.set (ACE_OS::atoi (*argv), 0);
                 }
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--destroy")))
                {
                  options.destroy = true;
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                             "Unknown option %s\n", *argv));
                  options.failed = true;
                }
            }
        }

      if (!options.name || options.failed)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nUsage:\n  %s --name <name>\n"
                      "optional:\n"
                      "  --ns <ior>\n"
                      "  --ctxsep  <character>\n"
                      "  --kindsep <character>\n"
                      "  --destroy\n"
                      "  --quiet\n"
                      "  --rtt <seconds> {Sets the relative round trip timeout policy}\n\n",
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
      if (options.name_service)
        {
          nc_obj = orb->string_to_object (options.name_service);
        }
      else
        {
          nc_obj = orb->resolve_initial_references ("NameService");
        }

      nc_obj = set_rtt(orb.in(), nc_obj.in (), options.relative_roundtrip_timeout);
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

      // Attempt to locate the object and destroy/unbind it
      CORBA::Object_var
        obj = root_nc->resolve (the_name);
      root_nc->unbind (the_name);
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
          ACE_DEBUG ((LM_DEBUG, "UnBound ID: %C",
                     the_name[index].id.in()));
          if (the_name[index].kind && the_name[index].kind[0])
            ACE_DEBUG ((LM_DEBUG, "  (Kind: %C)\n",
                       the_name[index].kind.in()));
          ACE_DEBUG ((LM_DEBUG, "\n"));
        }

      if (!options.quiet || options.destroy) {
        bool failure = false;
        try {
          obj = set_rtt(orb.in (), obj.in (), options.relative_roundtrip_timeout);
          CosNaming::NamingContext_var this_nc =
          CosNaming::NamingContext::_narrow (obj.in ());
          if (!CORBA::is_nil (this_nc.in ()))
            {
              if (options.destroy)
                {
                  if (!options.quiet)
                  {
                    ACE_DEBUG ((LM_DEBUG,"Destroying\n"));
                  }
                  this_nc->destroy( );
                }
              else if (!options.quiet)
                {
                  CORBA::String_var str =
                    orb->object_to_string (obj.in ());
                  ACE_DEBUG ((LM_DEBUG,
                    "\n*** Possibly Orphaned Naming Context ***\n%C\n\n", str.in()));
                }
            }
          else if (options.destroy && !options.quiet)
            ACE_DEBUG ((LM_DEBUG,"Can't Destroy object, it is not a naming context!\n"));
        }
      catch (const CORBA::OBJECT_NOT_EXIST&)
        {
          if (!options.quiet)
          {
            ACE_DEBUG ((LM_DEBUG, "{Object does not exist!}\n"));
          }
          failure = true;
        }
      catch (const CORBA::TRANSIENT&)
        {
          if (!options.quiet)
          {
            ACE_DEBUG ((LM_DEBUG, "{Object is transient!}\n"));
          }
          failure = true;
        }
      catch (const CORBA::TIMEOUT&)
        {
          if (!options.quiet)
          {
            ACE_DEBUG ((LM_DEBUG, "{Operation timed out!}\n"));
          }
          failure = true;
        }

        if (failure && !options.quiet)
          {
            if (options.destroy) {
              ACE_DEBUG ((LM_DEBUG, "Failed to destroy context.\n"));
            }
            else {
              ACE_DEBUG ((LM_DEBUG, "Failed to check for orphaned naming context.\n"));
            }
          }
      }
    }
  catch (const CosNaming::NamingContext::NotFound& nf)
    {
      CORBA::ULong index {};
      CORBA::ULong const limit = the_name.length () - nf.rest_of_name.length ();
      ACE_DEBUG ((LM_DEBUG, "\nError:\n"));
      for (; index < limit; ++index)
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
      ex._tao_print_exception ("Exception in nsdel");
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
