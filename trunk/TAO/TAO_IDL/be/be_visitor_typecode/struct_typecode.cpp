// -*- C++ -*-

//=============================================================================
/**
 *  @file  struct_typecode.cpp
 *
 *  $Id$
 *
 *  Structure TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#include <string>


TAO::be_visitor_struct_typecode::be_visitor_struct_typecode (
  be_visitor_context * ctx,
  bool is_exception)
  : be_visitor_typecode_defn (ctx),
    is_exception_ (is_exception)
{
}

int
TAO::be_visitor_struct_typecode::visit_structure (AST_Structure * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl << be_nl
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;


  std::string const fields_name (std::string ("_tao_fields_")
                                 + node->flat_name ());

  // Generate array containing struct field characteristics.
  os << "static TAO::TypeCode::Struct_Field<char const *> const "
     << fields_name.c_str ()
     << "[] =" << be_idt_nl
     << "{" << be_idt_nl;

  if (this->visit_members (node) != 0)
    return -1;

  os << be_uidt_nl
     << "};" << be_uidt_nl << be_nl;

  // Generate the TypeCode instantiation.
  os
    << "static TAO::TypeCode::Struct<char const *," << be_nl
    << "                             TAO::TypeCode::Struct_Field<char const *> const *," << be_nl
    << "                             CORBA::tk_"
    << (this->is_exception_ ? "except" : "struct") << "," << be_nl
    << "                             TAO::Null_RefCount_Policy>"
    << be_idt_nl
    << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
    << "\"" << node->repoID () << "\"," << be_nl
    << "\"" << node->original_local_name () << "\"," << be_nl
    << "_tao_fields_" << node->flat_name () << "," << be_nl
    << node->nfields () << ");" << be_uidt_nl
    << be_uidt_nl;

  return
    this->gen_typecode_ptr (be_type::narrow_from_decl (node));
}

int
TAO::be_visitor_struct_typecode::visit_members (AST_Structure * node)
{
  AST_Field ** member_ptr = 0;

  size_t const count = node->nfields ();

  TAO_OutStream & os = *this->ctx_->stream ();

  for (size_t i = 0; i < count; ++i)
    {
      node->field (member_ptr, i);

      be_decl * const member_decl =
        be_decl::narrow_from_decl (*member_ptr);

      be_type * const member_type =
        be_type::narrow_from_decl ((*member_ptr)->field_type ());

      os << "{ "
         << "\"" << member_decl->original_local_name () << "\", "
         << "&"  << member_type->tc_name ()
         << " }";

      if (i < count - 1)
        os << ",";

      os << be_nl;
    }

  return 0;
}
