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

/* $Log$
 * Revision 1.1  1999/06/09 19:43:38  schmidt
 * .
 *
 * Revision 1.3  1999/06/08 18:15:50  lockhart
 * Clean up code and add a few comments in preparation for contributing to
 *  the TAO distribution.
 *
 * Revision 1.2  1999/06/07 21:02:39  lockhart
 * Check for non-existant Naming Service. If none found, then exit with msg.
 * Indent multi-level names.
 *
 * Revision 1.1  1999/06/03 00:21:19  lockhart
 * First cut at a Naming Service utility which prints out current NS entries.
 * Based on code in ACP, but can travel down trees rather than just showing
 *  the highest level entries.
 */

#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"

static void list_context (CosNaming::NamingContext_ptr nc, int level);

// Display NS entries from a finite list.

static void 
show_chunk (CosNaming::NamingContext_ptr nc,
            const CosNaming::BindingList &bl,
            int level)
{
  for (CORBA::ULong i = 0;
       i < bl.length ();
       i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%*s",
                  2 * level,
                  bl[i].binding_name[0].id));

      if (ACE_OS::strlen(bl[i].binding_name[0].kind) > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%s)",
                    bl[i].binding_name[0].kind));

      // If this is a context node, follow it down to the next
      // level...
      if (bl[i].binding_type == CosNaming::ncontext)
	{
          ACE_DEBUG ((LM_DEBUG,
                      ": context\n"));

	  CosNaming::Name Name;
	  Name.length (1);
	  Name[0].id =
            CORBA::string_dup (bl[i].binding_name[0].id);

	  CORBA::Object_var obj = nc->resolve (Name);

	  CosNaming::NamingContext_var xc =
            CosNaming::NamingContext::_narrow (obj);
	  list_context (xc, level + 1);
	}
      // Mark this node as a reference
      else
        // The next version should resolve and show the IOR...
        ACE_DEBUG ((LM_DEBUG,
                    ": reference\n"));
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
  show_chunk (nc, bl, level);

  if (!CORBA::is_nil (it.in ()))
    {
      CORBA::Boolean more;

      do
	{
	  more = it->next_n (CHUNK, bl);
	  show_chunk (nc, bl, level);
	} 
      while (more);

      it->destroy();
    }
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj;
      obj = orb->resolve_initial_references ("NameService");
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var root_nc =
        CosNaming::NamingContext::_narrow (obj);
      ACE_TRY_CHECK;

      CORBA::String_var str =
	orb->object_to_string (root_nc.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (obj))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Naming Service not found"),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  "Naming Service: <%s> ---------\n",
                  str));

      list_context (root_nc, 1);
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

