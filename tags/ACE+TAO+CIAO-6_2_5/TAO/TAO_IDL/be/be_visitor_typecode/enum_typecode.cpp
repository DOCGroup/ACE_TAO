// -*- C++ -*-

//=============================================================================
/**
 *  @file  enum_typecode.cpp
 *
 *  $Id$
 *
 *  Enumeration TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#include "typecode.h"
#include "utl_scope.h"

TAO::be_visitor_enum_typecode::be_visitor_enum_typecode (
  be_visitor_context * ctx)
  : be_visitor_typecode_defn (ctx)
{
}

int
TAO::be_visitor_enum_typecode::visit_enum (be_enum * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl_2
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  ACE_CString const tao_enumerators ("_tao_enumerators_");
  ACE_CString const enumerators_name (tao_enumerators
                                      + node->flat_name ());

  // Generate array containing enum field characteristics.
  os << "static char const * const "
     << enumerators_name.c_str ()
     << "[] =" << be_idt_nl
     << "{" << be_idt_nl;

  if (this->visit_members (node) != 0)
    return -1;

  os << be_uidt_nl
     << "};" << be_uidt_nl << be_nl;

  // Generate the TypeCode instantiation.
  os
    << "static TAO::TypeCode::Enum<char const *," << be_nl
    << "                           char const * const *," << be_nl
    << "                           TAO::Null_RefCount_Policy>"
    << be_idt_nl
    << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
    << "\"" << node->repoID () << "\"," << be_nl
    << "\"" << node->original_local_name () << "\"," << be_nl
    << "_tao_enumerators_" << node->flat_name () << "," << be_nl
    << node->member_count () << ");" << be_uidt_nl
    << be_uidt_nl;

  if (this->gen_typecode_ptr (node) != 0)
    return -1;

  return 0;
}

int
TAO::be_visitor_enum_typecode::visit_members (be_enum * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  size_t const count = node->member_count ();
  size_t n = 0;

  for (UTL_ScopeActiveIterator i (node, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl * const d = i.item  ();
      AST_EnumVal * const item = AST_EnumVal::narrow_from_decl (d);

      // os << item->name ();
      os << "\"" << item->original_local_name () << "\"";

      if (n < count - 1)
        os << ",";

      os << be_nl;

      ++n;
    }

  return 0;
}
