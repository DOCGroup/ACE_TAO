/* -*- c++ -*- */
#include "be_identifier_helper.h"
#include "be_type.h"

#include "utl_identifier.h"
#include "utl_string.h"
#include "fe_private.h"
#include "global_extern.h"

#include "ace/SString.h"

#include <string_view>

std::string
IdentifierHelper::type_name (be_type *t, be_visitor *visitor)
{
  AST_PredefinedType *pdt = nullptr;

  switch (t->node_type ())
    {
      case AST_Decl::NT_sequence:
      case AST_Decl::NT_wstring:
      case AST_Decl::NT_string:
        (void) t->accept (visitor);
        return "";
      case AST_Decl::NT_pre_defined:
        pdt = dynamic_cast<AST_PredefinedType*> (t);

        switch (pdt->pt ())
          {
            case AST_PredefinedType::PT_pseudo:
              return std::string (t->full_name ());
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

        // Not reached.
        return "";
        break;
      default:
        return std::string ("::") + IdentifierHelper::orig_sn (t->name ());
    }
}

Identifier *
IdentifierHelper::original_local_name (Identifier const *local_name)
{
  Identifier * id = nullptr;
  std::string_view const lname =
    local_name->get_string_view_or_empty ();

  // Remove _cxx_ if:
  // 1. it occurs and
  // 2. it occurs at the beginning of the string and
  // 3. the rest of the string is a C++ keyword
  if (lname.substr (0, 5) == "_cxx_")
    {
      TAO_IDL_CPP_Keyword_Table cpp_key_tbl;

      unsigned int len =
        static_cast<unsigned int> (lname.size () - 5);
      std::string_view const keyword = lname.substr (5);

      const TAO_IDL_CPP_Keyword_Entry *entry =
        cpp_key_tbl.lookup (keyword.data (), len);

      if (entry != nullptr)
        {
          // Remove _cxx_ and assign to the Identifier variable.
          ACE_NEW_RETURN (id,
                          Identifier (keyword),
                          nullptr);
        }
    }

  if (id == nullptr)
    {
      id = local_name->copy ();
    }

  return id;
}

std::string
IdentifierHelper::orig_sn (UTL_IdList * sn, bool for_idl)
{
  std::string retval;
  bool first = true;
  bool second = false;
  Identifier *id = nullptr;

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
          ? IdentifierHelper::try_escape (id)
          : id->get_string_view_or_empty ();

       if (first)
        {
          if (!id->get_string_view_or_empty ().empty ())
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
      id = nullptr;
    }

  return retval;
}

bool
IdentifierHelper::is_idl_keyword (Identifier const *local_name)
{
  std::string_view const name =
    local_name->get_string_view_or_empty ();
  std::string const ext_id = UTL_String::get_canonical_rep (name);

  return idl_global->idl_keywords ().find (ext_id)
    != idl_global->idl_keywords ().end ();
}

std::string
IdentifierHelper::try_escape (Identifier const *local_name)
{
  std::string s_local_name (
    local_name->get_string_view_or_empty ());

  if (IdentifierHelper::is_idl_keyword (local_name))
    {
      return "_" + s_local_name;
    }
  else
    {
      return s_local_name;
    }
}
