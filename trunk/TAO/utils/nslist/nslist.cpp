//=============================================================================
/**
 *  @file    nslist.cpp
 *
 *  $Id$
 *
 *  Naming Service listing utility
 *
 *
 *  @author  Thomas Lockhart, NASA/JPL <Thomas.Lockhart@jpl.nasa.gov>
 *  @date 1999-06-03
 *  Enhanced 21 Jun, 2006 Simon Massey <sma@prismtech.com>
 */
//=============================================================================

#include "ace/SString.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/Endpoint.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/ORB_Constants.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/os_include/os_ctype.h"

//============================================================================
namespace
{
  CORBA::ORB_var orb;
  bool
    showIOR = false,    // Default decodes endpoints
    showCtxIOR = false, // Default no displaying naming context ior
    noLoops = false;    // Default draw loopback arrows
  const char
    *myTree = "|",      // Default string to draw tree "tram-lines"
    *myNode = "+";      // Default string to draw tree node end-points
  int
    sizeMyTree,         // Initialised by main to strlen (myTree)
    sizeMyNode,         // Initialised by main to strlen (myNode)
    maxDepth= 0;        // Limit to display depth (default unlimited)

  void list_context (const CosNaming::NamingContext_ptr,
                     int level
                     ACE_ENV_ARG_DECL);

  //==========================================================================
  class NestedNamingContexts
  {
  public:
    static void add (const CosNaming::NamingContext_ptr nc)
    {
      (void) new NestedNamingContexts( nc ); // This is not a leak (see constructor)
    }

    static void remove ()
    {
      const NestedNamingContexts *const pThisOne= pBottom;
      delete pThisOne; // i.e. delete pBottom; Attempt to stop over-optimisation by BORLAND
    }

    static int hasBeenSeen (const CosNaming::NamingContext_ptr nc)
    {
      int level= 1;
      for (const NestedNamingContexts *pMyNode= pBottom;
           pMyNode;
           ++level, pMyNode= pMyNode->pNext)
        {
          if (pMyNode->pnc->_is_equivalent (nc))
            return level; // Loops backwards this number of levels
        }

      return 0; // Not seen before
    }

  private:
    static const NestedNamingContexts
      *pBottom; // Our current lowest level
    const CosNaming::NamingContext_ptr
      pnc; // This level's Naming Context
    const NestedNamingContexts
      *const pNext; // Next highest level

    NestedNamingContexts (const CosNaming::NamingContext_ptr nc)
      :pnc(nc), pNext(pBottom) // Adds the old list to this!
    {
      this->pBottom= this; // Doesn't leak this (it's the new start)
    }

    ~NestedNamingContexts ()
    {
      this->pBottom= this->pNext; // this node removed from list.
    }

    // Outlaw copying
    NestedNamingContexts (const NestedNamingContexts &);
    NestedNamingContexts &operator= (const NestedNamingContexts &);
  };
  const NestedNamingContexts *NestedNamingContexts::pBottom= 0;

  //==========================================================================
  void
  get_tag_name (CORBA::ULong tag, ACE_CString& tag_string)
  {
    if (tag == IOP::TAG_INTERNET_IOP)
      tag_string = "IIOP";
    else if (tag == TAO_TAG_UIOP_PROFILE)
      tag_string = "UIOP";
    else if (tag == TAO_TAG_SHMEM_PROFILE)
      tag_string = "SHMEM";
#ifdef TAO_TAG_DIOP_PROFILE
    else if (tag == TAO_TAG_DIOP_PROFILE)
      tag_string = "GIOP over UDP";
#endif /* TAO_TAG_DIOP_PROFILE */
    else
      {
        char buffer[32]= {'\0'};
        ACE_OS::sprintf( buffer, "%08x (%u)", tag, tag );
        (tag_string = "Unknown tag: ") += buffer;
      }
  }

  //==========================================================================
  void
  display_endpoint_info (CORBA::Object_ptr obj, const int level)
  {
    TAO_Stub *stub = obj->_stubobj ();
    if (!stub)
      {
        ACE_DEBUG ((LM_DEBUG, " {Invalid Stub}\n"));
        return;
      }

    TAO_Profile* profile = stub->profile_in_use ();
    if (!profile)
      {
        ACE_DEBUG ((LM_DEBUG, " {Invalid Profile}\n"));
        return;
      }

    TAO_Endpoint* endpoint = profile->endpoint ();
    if (!endpoint)
      {
        ACE_DEBUG ((LM_DEBUG, " {Invalid Endpoint}\n"));
        return;
      }

    // Display protocol
    CORBA::ULong tag = endpoint->tag ();
    ACE_CString tag_name;
    get_tag_name (tag, tag_name);

    ACE_DEBUG ((LM_DEBUG, "\n"));
    int count;
    for (count= 0; count < level; ++count)
      ACE_DEBUG ((LM_DEBUG, "%s ", myTree));
    for (count= 0; count < sizeMyNode; ++count)
      ACE_DEBUG ((LM_DEBUG, " "));
    ACE_DEBUG ((LM_DEBUG, " Protocol: %s\n",
               tag_name.c_str()));

    // Display Endpoint
    for (count= 0; count < level; ++count)
      ACE_DEBUG ((LM_DEBUG, "%s ", myTree));
    for (count= 0; count < sizeMyNode; ++count)
      ACE_DEBUG ((LM_DEBUG, " "));
    char buf[256]= {'\0'};
    if (endpoint->addr_to_string (buf, sizeof(buf)-1u) < 0)
      ACE_OS::strcpy( buf, "{Endpoint too long}" );
    ACE_DEBUG ((LM_DEBUG, " Endpoint: %s\n", buf));
  }

  //==========================================================================
  // Display NS entries from a finite list.
  void
  show_chunk (const CosNaming::NamingContext_ptr nc,
              const CosNaming::BindingList &bl,
              int level
              ACE_ENV_ARG_DECL)
  {
    for (CORBA::ULong i = 0;
         i < bl.length ();
         ++i)
      {
        int count;
        for (count= 0; count < level-1; ++count)
          ACE_DEBUG ((LM_DEBUG, "%s ", myTree));
        ACE_DEBUG ((LM_DEBUG, "%s %s", myNode,
                   bl[i].binding_name[0].id.in ()));

        if (bl[i].binding_name[0].kind[0])
          ACE_DEBUG ((LM_DEBUG,
                     " (Kind: %s)",
                     bl[i].binding_name[0].kind.in ()));

        CosNaming::Name Name;
        Name.length (1);
        Name[0].id =
          CORBA::string_dup (bl[i].binding_name[0].id);
        Name[0].kind =
          CORBA::string_dup (bl[i].binding_name[0].kind);

        CORBA::Object_var obj = nc->resolve (Name ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        // If this is a context node, follow it down to the next level...
        if (bl[i].binding_type == CosNaming::ncontext)
          {
            ACE_DEBUG ((LM_DEBUG, ": Naming context"));

            CosNaming::NamingContext_var xc;
            ACE_TRY_EX (inner)
              {
                xc = CosNaming::NamingContext::_narrow (obj.in ()
                                                        ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK_EX (inner);
              }
            ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
              {
                 ACE_UNUSED_ARG (not_used);
                 xc= 0;
                ACE_DEBUG ((LM_DEBUG, " {Destroyed}"));
              }
            ACE_ENDTRY;
            ACE_CHECK;

            if (const int backwards= NestedNamingContexts::hasBeenSeen (xc.in ()))
              {
                ACE_DEBUG ((LM_DEBUG, " (Binding Loop)\n"));
                if (!noLoops)
                  {
                    int count;
                    for (count= 0; count < (level - backwards); ++count)
                      ACE_DEBUG ((LM_DEBUG, "%s ", myTree));
                    ACE_DEBUG ((LM_DEBUG, "^"));
                    int chars;
                    while (++count < level)
                      for (chars= 0; chars <= sizeMyTree; ++chars)
                        ACE_DEBUG ((LM_DEBUG, "-"));
                    for (chars= 0; chars < sizeMyNode; ++chars)
                      ACE_DEBUG ((LM_DEBUG, "-"));
                    ACE_DEBUG ((LM_DEBUG, "^\n"));
                  }
              }
            else
              {
                if (showCtxIOR)
                  {
                    CORBA::String_var str =
                      orb->object_to_string (obj.in ()
                                             ACE_ENV_ARG_PARAMETER);
                    ACE_CHECK;
                    ACE_DEBUG ((LM_DEBUG, ": %s", str.in ()));
                  }

                if (maxDepth != level)
                  {
                    ACE_DEBUG ((LM_DEBUG, "\n"));
                    if (xc.in ())
                      {
                        list_context (xc.in (), level + 1 ACE_ENV_ARG_PARAMETER);
                        ACE_CHECK;
                      }
                  }
                else
                  ACE_DEBUG ((LM_DEBUG, " {Max depth reached}\n"));
              }
          }
        // Mark this node as a reference
        else
          {
            ACE_DEBUG ((LM_DEBUG, ": Object Reference"));
            if (CORBA::is_nil (obj.in ()))
              ACE_DEBUG ((LM_DEBUG, " {Null}"));

            if (showIOR)
              {
                CORBA::String_var str =
                  orb->object_to_string (obj.in ()
                                         ACE_ENV_ARG_PARAMETER);
                ACE_CHECK;
                ACE_DEBUG ((LM_DEBUG, ": %s\n", str.in ()));
              }
            else if (CORBA::is_nil (obj.in ()))
              ACE_DEBUG ((LM_DEBUG, "\n"));
            else
              display_endpoint_info (obj.in(), level);
          }
      }
  }

  //==========================================================================
  void
  list_context (const CosNaming::NamingContext_ptr nc,
                const int level
                ACE_ENV_ARG_DECL)
  {
    CosNaming::BindingIterator_var it;
    CosNaming::BindingList_var bl;
    const CORBA::ULong CHUNK = 100;

    NestedNamingContexts::add (nc);
    nc->list (CHUNK, bl, it ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    show_chunk (nc, bl.in (), level ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (!CORBA::is_nil (it.in ()))
      {
        CORBA::Boolean more;

        do
          {
            more = it->next_n (CHUNK, bl);
            show_chunk (nc, bl.in (), level ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
          } while (more);

        it->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }

    NestedNamingContexts::remove ();
  }
} // end of local unnamed namespace

//============================================================================
int
ACE_TMAIN (int argcw, ACE_TCHAR *argvw[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Contact the orb
      ACE_Argv_Type_Converter argcon (argcw, argvw);
      orb = CORBA::ORB_init (argcon.get_argc (), argcon.get_ASCII_argv (),
                             "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Scan through the command line options
      bool
        failed = false,
        showNSonly = false;
      int
        argc = argcon.get_argc ();
      ACE_TCHAR
        **argv = argcon.get_TCHAR_argv ();
      char
        kindsep= '.',
        ctxsep[]= "/",
        *name = 0;
      const char
        *const pname = ACE_TEXT_ALWAYS_CHAR (argv[0]),
        *nameService = 0;

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
                      else if (showNSonly)
                        {
                          ACE_DEBUG ((LM_DEBUG,
                                     "Error: --nsior and --ns "
                                     "are both specified\n"));
                          failed = true;
                        }
                      else
                        nameService = ACE_TEXT_ALWAYS_CHAR (*argv);
                    }
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--nsior")))
                {
                  if (showIOR || showCtxIOR || noLoops
                      || nameService || name || maxDepth)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --nsior given "
                                 "with other options\n"));
                      failed = true;
                    }
                  else
                    showNSonly = true;
                }
              else if (0 == ACE_OS::strcmp(*argv, ACE_TEXT ("--ior")))
                {
                  if (showNSonly)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --nsior and --ior are "
                                 "both specified\n"));
                      failed = true;
                    }
                  else
                    showIOR = true;
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--ctxior")))
                {
                  if (showNSonly)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --nsior and --ctxior are "
                                 "both specified\n"));
                      failed = true;
                    }
                  else
                    showCtxIOR = true;
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--tree")))
                {
                  if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --tree requires an argument\n"));
                      failed= true;
                    }
                  else
                    {
                      ++argv;
                      if (showNSonly)
                        {
                          ACE_DEBUG ((LM_DEBUG,
                                     "Error: --nsior and --tree are "
                                     "both specified\n"));
                          failed = true;
                        }
                      else
                        myTree = ACE_TEXT_ALWAYS_CHAR (*argv);
                    }
                }
              else if (0 == ACE_OS::strcmp (*argv, ACE_TEXT ("--node")))
                {
                  if (!--argc)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --node requires an argument\n"));
                      failed= true;
                    }
                  else
                    {
                      ++argv;
                      if (showNSonly)
                        {
                          ACE_DEBUG ((LM_DEBUG,
                                     "Error: --nsior and --node are "
                                     "both specified\n"));
                          failed = true;
                        }
                      else
                        myNode = ACE_TEXT_ALWAYS_CHAR (*argv);
                    }
                }
              else if (0 == strcmp(*argv, ACE_TEXT ("--noloops")))
                {
                  if (showNSonly)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --nsior and --noloops are "
                                 "both specified\n"));
                      failed = true;
                    }
                  else
                    noLoops = true;
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
                    {
                      ++argv;
                      if (showNSonly)
                        {
                          ACE_DEBUG ((LM_DEBUG,
                                     "Error: --nsior and --name are "
                                     "both specified\n"));
                          failed = true;
                        }
                      else
                        name = ACE_TEXT_ALWAYS_CHAR (*argv);
                    }
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
                  else if (showNSonly)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --nsior and --ctxsep are "
                                 "both specified\n"));
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
                  else if (showNSonly)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --nsior and --kindsep are "
                                 "both specified\n"));
                      failed = true;
                    }
                  else
                    kindsep = ACE_TEXT_ALWAYS_CHAR (*argv)[0];
                }
              else if (0 == strcmp(*argv, ACE_TEXT ("--max")))
                {
                  if (maxDepth)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --max given more than once\n"));
                      failed = true;
                    }
                  else if (!--argc || !isdigit (ACE_TEXT_ALWAYS_CHAR (*++argv)[0]))
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --max requires a number\n"));
                      failed = true;
                    }
                  else if (showNSonly)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                 "Error: --nsior and --max are "
                                 "both specified\n"));
                      failed = true;
                    }
                  else
                     maxDepth= ACE_OS::atoi (ACE_TEXT_ALWAYS_CHAR (*argv));
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                             "Unknown option %s\n", ACE_TEXT_ALWAYS_CHAR (*argv)));
                  failed = true;
                }
            }
        }

      if (failed)
        {
          ACE_DEBUG ((LM_DEBUG, "\n%s options:\n"
            "  --nsior               {Display the naming service IOR and exit}\n"
            "or:\n"
            "  --ns <ior>            {Defaults to standard NameService}\n"
            "  --ior                 {Display ior for end points}\n"
            "  --ctxior              {Display ior for naming contexts}\n"
            "  --tree \"xx\"           {Defaults to | for drawing tramlines}\n"
            "  --node \"xx\"           {Defaults to + for drawing nodes}\n"
            "  --noloops             {Inhibit drawing of naming context loops}\n"
            "  --name <name>         {Lists sub-set, defaults to root}\n"
            "  --ctxsep  <character> {<name> Context separation character, default /}\n"
            "  --kindsep <character> {<name> ID/Kind separation character, default .}\n"
            "  --max <number>        {If given, limits displayed sub-context depth}\n",
            pname));
          orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          return 1;
        }

      // Initialise the lengths of the myTree and myNode draw strings.
      sizeMyTree= ACE_OS::strlen (myTree);
      sizeMyNode= ACE_OS::strlen (myNode);

      // Contact the name service
      CORBA::Object_var obj;
      if (nameService)
        obj = orb->string_to_object (nameService ACE_ENV_ARG_PARAMETER);
      else
        obj = orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var root_nc =
        CosNaming::NamingContext::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (root_nc.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error: nil root naming context\n"));
          orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          return 1;
        }
      if (name)
        {
          // Assemble the name from the user string given
          CosNaming::Name the_name (0);
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

          // Now find this sub-context and replace the root with it.
          obj = root_nc->resolve( the_name ACE_ENV_ARG_PARAMETER );
          ACE_TRY_CHECK;
          root_nc =
            CosNaming::NamingContext::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (CORBA::is_nil (root_nc.in ()))
            {
              ACE_DEBUG ((LM_DEBUG,
                         "Error: Can't find naming context\n    %s\n", name));
              orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
              return 1;
            }
        }

      CORBA::String_var str =
        orb->object_to_string (root_nc.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (showNSonly)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "The NameService is located via:\n%s\n", str.in ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Naming Service: %s\n---------------\n",
                     ((showCtxIOR)? str.in () : "")));
          list_context (root_nc.in (), 1 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in nslist");
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      return -1;
    }
  ACE_ENDTRY;

  orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}
