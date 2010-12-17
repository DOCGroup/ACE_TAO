/* -*- c++ -*- */
// $Id$

#include "be_identifier_helper.h"
#include "be_type.h"

#include "utl_identifier.h"
#include "utl_string.h"
#include "fe_private.h"
#include "global_extern.h"

ACE_CString IdentifierHelper::tmp_retval_;

const char *
IdentifierHelper::type_name (be_type *t, be_visitor *visitor)
{
  AST_PredefinedType *pdt = 0;

  switch (t->node_type ())
    {
      case AST_Decl::NT_sequence:
      case AST_Decl::NT_wstring:
      case AST_Decl::NT_string:
        (void) t->accept (visitor);
        return "";
      case AST_Decl::NT_pre_defined:
        pdt = AST_PredefinedType::narrow_from_decl (t);

        switch (pdt->pt ())
          {
            case AST_PredefinedType::PT_pseudo:
              return t->full_name ();
            case AST_PredefinedType::PT_object:
              return "Object";
            case AST_PredefinedType::PT_any:
              return "any";
            case AST_PredefinedType::PT_long:
              return "long";
            case AST_PredefinedType::PT_ulong:
              return "unsigned long";
            case AST_PredefinedType::PT_longlong:
              return "long long";
            case AST_PredefinedType::PT_ulonglong:
              return "unsigned long long";
            case AST_PredefinedType::PT_short:
              return "short";
            case AST_PredefinedType::PT_ushort:
              return "unsigned short";
            case AST_PredefinedType::PT_float:
              return "float";
            case AST_PredefinedType::PT_double:
              return "double";
            case AST_PredefinedType::PT_longdouble:
              return "long double";
            case AST_PredefinedType::PT_char:
              return "char";
            case AST_PredefinedType::PT_wchar:
              return "wchar";
            case AST_PredefinedType::PT_boolean:
              return "boolean";
            case AST_PredefinedType::PT_octet:
              return "octet";
            case AST_PredefinedType::PT_void:
              return "void";
            default:
              break;
          }
      default:
        IdentifierHelper::tmp_retval_ = "::";
        IdentifierHelper::tmp_retval_ +=
          IdentifierHelper::orig_sn (t->name ()).c_str ();
        return IdentifierHelper::tmp_retval_.c_str ();
    }
}

Identifier *
IdentifierHelper::original_local_name (Identifier * local_name)
{
  Identifier * id = 0;
  const char *lname = local_name->get_string ();

  // Remove _cxx_ if:
  // 1. it occurs and
  // 2. it occurs at the beginning of the string and
  // 3. the rest of the string is a C++ keyword
  if (ACE_OS::strstr (lname, "_cxx_") == lname)
    {
      TAO_IDL_CPP_Keyword_Table cpp_key_tbl;

      unsigned int len =
        static_cast<unsigned int> (ACE_OS::strlen (lname + 5));

      const TAO_IDL_CPP_Keyword_Entry *entry =
        cpp_key_tbl.lookup (lname + 5, len);

      if (entry != 0)
        {
          // Remove _cxx_ and assign to the Identifier variable.
          ACE_NEW_RETURN (id,
                          Identifier (lname + 5),
                          0);
        }
    }

  if (id == 0)
    {
      id = local_name->copy ();
    }

  return id;
}

ACE_CString
IdentifierHelper::orig_sn (UTL_IdList * sn, bool for_idl)
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

      id =
        for_idl
          ? IdentifierHelper::original_local_name (i.item ())
          : i.item ()->copy ();

      i.next ();

      // Append the identifier.
      retval +=
        for_idl
          ? IdentifierHelper::try_escape (id).c_str ()
          : id->get_string ();

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
  // Convert the identifier string into a
  // canonical (uppercase) form as a ACE_CString
  char *tmp = local_name->get_string ();
  ACE_CString ext_id (tmp);
  UTL_String::get_canonical_rep (ext_id);

  return !idl_global->idl_keywords ().find (ext_id);
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
