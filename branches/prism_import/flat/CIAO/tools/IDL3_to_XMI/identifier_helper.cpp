/* -*- c++ -*- */
// $Id$

#include "identifier_helper.h"
#include "utl_identifier.h"
#include "utl_string.h"
#include "global_extern.h"

Identifier *
IdentifierHelper::original_local_name (Identifier * local_name)
{
  Identifier * id = 0;

  // Remove _cxx_ if it is present.
  if (ACE_OS::strstr (local_name->get_string (),
                       "_cxx_")
        == local_name->get_string ())
    {
      // CString class is good to do this stuff.
      ACE_CString name_str (local_name->get_string ());

      // Remove _cxx_.
      name_str = name_str.substr (ACE_OS::strlen ("_cxx_"));

      // Assign to the Identifier variable.
      ACE_NEW_RETURN (id,
                      Identifier (name_str.c_str ()),
                      0);
    }
  else
    {
      id = local_name->copy ();
    }

  return id;
}


ACE_CString
IdentifierHelper::orig_sn (UTL_IdList * sn, bool appended_to)
{
  ACE_CString retval;
  bool first = true;
  bool second = false;
  Identifier *id = 0;

  for (UTL_IdListActiveIterator i (sn); !i.is_done ();)
    {
      if (!first)
        {
          retval += "::";
        }
      else if (second)
        {
          first = second = false;
        }

      id = IdentifierHelper::original_local_name (i.item ());
      i.next ();

      // Append the identifier.
      retval +=
        appended_to && i.is_done ()
          ? id->get_string ()
          : IdentifierHelper::try_escape (id).c_str ();

       if (first)
        {
          if (ACE_OS::strcmp (id->get_string (), "") != 0)
            {
              // Does not start with a "".
              first = false;
            }
          else
            {
              second = true;
            }
        }

     id->destroy ();
      delete id;
      id = 0;
    }

  return retval;
}

bool
IdentifierHelper::is_idl_keyword (Identifier * local_name)
{
  UTL_String utl_tmp (local_name->get_string ());
  ACE_CString ext_id (utl_tmp.get_canonical_rep (),
                      0,
                      false);

  int status = idl_global->idl_keywords ().find (ext_id);
  utl_tmp.destroy ();

  return status == 0;
}

ACE_CString
IdentifierHelper::try_escape (Identifier * local_name)
{
  ACE_CString s_local_name (local_name->get_string ());

  if (IdentifierHelper::is_idl_keyword (local_name))
    {
      return "_" + s_local_name;
    }
  else
    {
      return s_local_name;
    }
}
