// $Id$

// ============================================================================
//
// = LIBRARY
//    utils
//
// = FILENAME
//    nslist.cpp
//
// = DESCRIPTION
//    Naming Service listing utility
//
// = AUTHOR
//     Written 1999-06-03 by Thomas Lockhart, NASA/JPL <Thomas.Lockhart@jpl.nasa.gov>
//
// ============================================================================

#include "orbsvcs/CosNamingC.h"

CORBA::ORB_var orb;
int showIOR = 0;
int showNSonly = 0;

static void list_context (CosNaming::NamingContext_ptr nc, int level);

// Display NS entries from a finite list.

static void
show_chunk (CosNaming::NamingContext_ptr nc,
            const CosNaming::BindingList &bl,
            int level)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  for (CORBA::ULong i = 0;
       i < bl.length ();
       i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%*s%s",
                  2 * level,
                  "",
                  bl[i].binding_name[0].id.in ()));

      if (ACE_OS::strlen(bl[i].binding_name[0].kind) > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%s)",
                    bl[i].binding_name[0].kind.in ()));

      CosNaming::Name Name;
      Name.length (1);
      Name[0].id =
        CORBA::string_dup (bl[i].binding_name[0].id);
      Name[0].kind =
        CORBA::string_dup (bl[i].binding_name[0].kind);

      CORBA::Object_var obj = nc->resolve (Name);

      // If this is a context node, follow it down to the next
      // level...
      if (bl[i].binding_type == CosNaming::ncontext)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ": context\n"));

          CosNaming::NamingContext_var xc =
            CosNaming::NamingContext::_narrow (obj.in ());
          list_context (xc.in (), level + 1);
        }
      // Mark this node as a reference
      else
        {
          if (showIOR)
            {
              CORBA::String_var str =
                orb->object_to_string (obj.in (),
                                       ACE_TRY_ENV);
              ACE_DEBUG ((LM_DEBUG,
                          ": <%s>\n",
                          str.in ()));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ": reference\n"));
            }
        }
    }
}

static void
list_context (CosNaming::NamingContext_ptr nc,
              int level)
{
  CosNaming::BindingIterator_var it;
  CosNaming::BindingList_var bl;
  const CORBA::ULong CHUNK = 100;

  nc->list (CHUNK, bl, it);
  show_chunk (nc, bl.in (), level);

  if (!CORBA::is_nil (it.in ()))
    {
      CORBA::Boolean more;

      do
        {
          more = it->next_n (CHUNK, bl);
          show_chunk (nc, bl.in (), level);
        }
      while (more);

      it->destroy();
    }
}

int
main (int argc, char *argv[])
{
  showIOR = 0;
  showNSonly = 0;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      orb = CORBA::ORB_init (argc, argv);

      char *pname = argv[0];

      while (argc > 0)
        {
          if (strcmp(*argv, "--ior") == 0)
            {
              if (showNSonly)
                {
                   ACE_DEBUG ((LM_DEBUG, "Error: --nsior and --ior are both specified\n"));
                   return 1;
                }
              showIOR = 1;
            }
          else if (strcmp(*argv, "--nsior") == 0)
            {
              if (showIOR)
                {
                   ACE_DEBUG ((LM_DEBUG, "Error: --nsior and --ior are both specified\n"));
                   return 1;
                }
                showNSonly = 1;
            }
          else if (strncmp(*argv, "--", 2) == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Usage: %s [ --ior | --nsior ]\n", pname));
              return 1;
            }
          argc--;
          argv++;
        }

      CORBA::Object_var obj =
        orb->resolve_initial_references ("NameService", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var root_nc =
        CosNaming::NamingContext::_narrow (obj.in ());
      ACE_TRY_CHECK;

      CORBA::String_var str =
        orb->object_to_string (root_nc.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (obj.in ()) || CORBA::is_nil (root_nc.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Naming Service not found"),
                          -1);

      if (showNSonly)
        {
          // ACE_DEBUG ((LM_DEBUG, "%s", str.in ()));
          printf( "%s", str.in());
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

          list_context (root_nc.in (), 1);
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
