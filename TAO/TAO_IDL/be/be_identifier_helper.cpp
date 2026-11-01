/* -*- c++ -*- */
#include "be_identifier_helper.h"
#include "be_type.h"

#include "utl_string.h"
#include "fe_private.h"
#include "global_extern.h"

#include "ace/SString.h"

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

        // Not reached.
        return "";
        break;
      default:
        {
          std::string retval ("::");
          retval += IdentifierHelper::orig_sn (t->name ());
          return retval;
        }
    }
}

std::string
IdentifierHelper::original_local_name (std::string const& local_name)
{
  // Remove _cxx_ if:
  // 1. it occurs and
  // 2. it occurs at the beginning of the string and
  // 3. the rest of the string is a C++ keyword
  if (local_name.compare (0, 5, "_cxx_") == 0)
    {
      TAO_IDL_CPP_Keyword_Table cpp_key_tbl;

      unsigned int const len =
        static_cast<unsigned int> (local_name.size () - 5);
      std::string const keyword = local_name.substr (5);

      const TAO_IDL_CPP_Keyword_Entry *entry =
        cpp_key_tbl.lookup (keyword.data (), len);

      if (entry != nullptr)
        {
          return keyword;
        }
    }

  return local_name;
}

std::string
IdentifierHelper::orig_sn (UTL_IdList * sn, bool for_idl)
{
  std::string retval;
  bool first = true;
  bool second = false;

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

      std::string const id =
        for_idl
          ? IdentifierHelper::original_local_name (i.item ()->get_string ())
          : i.item ()->get_string ();

      i.next ();

      // Append the identifier.
      if (for_idl)
        {
          retval += IdentifierHelper::try_escape (id);
        }
      else
        {
          retval += id;
        }

      if (first)
        {
          if (!id.empty ())
            {
              // Does not start with a "".
              first = false;
            }
          else
            {
              second = true;
            }
        }
    }

  return retval;
}

bool
IdentifierHelper::is_idl_keyword (std::string const& local_name)
{
  std::string const ext_id = UTL_String::get_canonical_rep (local_name);

  return idl_global->idl_keywords ().find (ext_id)
    != idl_global->idl_keywords ().end ();
}

std::string
IdentifierHelper::try_escape (std::string const& local_name)
{
  if (IdentifierHelper::is_idl_keyword (local_name))
    {
      std::string retval ("_");
      retval += local_name;
      return retval;
    }
  else
    {
      return local_name;
    }
}
