
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
 */
//=============================================================================


#include "ace/SString.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/Endpoint.h"
#include "tao/Profile.h"
#include "tao/Stub.h"

CORBA::ORB_var orb;
int showIOR = 0;
int showNSonly = 0;

static void list_context (CosNaming::NamingContext_ptr nc,
                          int level
                          TAO_ENV_ARG_DECL);

static void
get_tag_name (CORBA::ULong tag, ACE_CString& tag_string)
{
  switch(tag)
    {
    case TAO_TAG_IIOP_PROFILE:
      tag_string = "IIOP";
      break;
    case TAO_TAG_UIOP_PROFILE:
      tag_string = "UIOP";
      break;
    case TAO_TAG_SHMEM_PROFILE:
      tag_string = "SHMEM";
      break;
#ifdef TAO_TAG_UDP_PROFILE
    case TAO_TAG_UDP_PROFILE:
      tag_string = "GIOP over UDP";
#endif /* TAO_TAG_UDP_PROFILE */
    default:
      tag_string = "Unknown tag: " + tag;
      break;
    }
}


static void
display_endpoint_info (CORBA::Object_ptr obj)
{

  TAO_Stub *stub = obj->_stubobj ();
  if (!stub)
    {
      ACE_DEBUG ((LM_DEBUG, "Invalid stub\n"));
      return;
    }

  TAO_Profile* profile = stub->profile_in_use ();
  if (!profile)
    {
      ACE_DEBUG ((LM_DEBUG, "Invalid profile\n"));
      return;
    }


  TAO_Endpoint* endpoint = profile->endpoint ();
  if (!endpoint)
    {
      ACE_DEBUG ((LM_DEBUG, "Invalid profile\n"));
      return;
    }

  CORBA::ULong tag = endpoint->tag ();
  ACE_CString tag_name;
  get_tag_name (tag, tag_name);

  char buf[255];
  if (endpoint->addr_to_string (buf, 255) < 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Could not put endpoint address in string.\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Protocol: %s,   Endpoint: %s\n",
              tag_name.c_str(),
              buf));
}

// Display NS entries from a finite list.

static void
show_chunk (CosNaming::NamingContext_ptr nc,
            const CosNaming::BindingList &bl,
            int level
            TAO_ENV_ARG_DECL)
{
  for (CORBA::ULong i = 0;
       i < bl.length ();
       i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%*s%s",
                  2 * level,
                  "",
                  bl[i].binding_name[0].id.in ()));

      if (ACE_OS::strlen (bl[i].binding_name[0].kind) > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%s)",
                    bl[i].binding_name[0].kind.in ()));

      CosNaming::Name Name;
      Name.length (1);
      Name[0].id =
        CORBA::string_dup (bl[i].binding_name[0].id);
      Name[0].kind =
        CORBA::string_dup (bl[i].binding_name[0].kind);

      CORBA::Object_var obj = nc->resolve (Name TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // If this is a context node, follow it down to the next
      // level...
      if (bl[i].binding_type == CosNaming::ncontext)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ": naming context\n"));

          CosNaming::NamingContext_var xc =
            CosNaming::NamingContext::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;

          list_context (xc.in (), level + 1 TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      // Mark this node as a reference
      else
        {
          if (showIOR)
            {
              CORBA::String_var str =
                orb->object_to_string (obj.in ()
                                       TAO_ENV_ARG_PARAMETER);
              ACE_CHECK;
              ACE_DEBUG ((LM_DEBUG,
                          ": <%s>\n",
                          str.in ()));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ": object reference:   "));
              display_endpoint_info (obj.in());
            }
        }
    }
}

static void
list_context (CosNaming::NamingContext_ptr nc,
              int level
              TAO_ENV_ARG_DECL)
{
  CosNaming::BindingIterator_var it;
  CosNaming::BindingList_var bl;
  const CORBA::ULong CHUNK = 100;

  nc->list (CHUNK, bl, it TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  show_chunk (nc, bl.in (), level TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (it.in ()))
    {
      CORBA::Boolean more;

      do
        {
          more = it->next_n (CHUNK, bl);
          show_chunk (nc, bl.in (), level TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      while (more);

      it->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

int
main (int argc, char *argv[])
{
  showIOR = 0;
  showNSonly = 0;

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      orb = CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      char *pname = argv[0];

      while (argc > 0)
        {
          if (strcmp(*argv, "--ior") == 0)
            {
              if (showNSonly)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Error: --nsior and --ior are "
                              "both specified\n"));
                  return 1;
                }
              showIOR = 1;
            }
          else if (ACE_OS::strcmp (*argv, "--nsior") == 0)
            {
              if (showIOR)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Error: --nsior and --ior "
                              "are both specified\n"));
                  return 1;
                }
              showNSonly = 1;
            }
          else if (ACE_OS::strncmp (*argv, "--", 2) == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Usage: %s [ --ior | --nsior ]\n", pname));
              return 1;
            }
          argc--;
          argv++;
        }

      CORBA::Object_var obj =
        orb->resolve_initial_references ("NameService" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var root_nc =
        CosNaming::NamingContext::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        orb->object_to_string (root_nc.in ()
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (obj.in ()) || CORBA::is_nil (root_nc.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Naming Service not found"),
                          -1);

      if (showNSonly)
        {
          // ACE_DEBUG ((LM_DEBUG, "%s", str.in ()));
          ACE_OS::printf ("%s", str.in());
        }
      else
        {
          if (showIOR)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Naming Service: <%s>\n---------\n",
                          str.in ()));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Naming Service:\n---------\n"));
            }

          list_context (root_nc.in (), 1 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in nslist");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
