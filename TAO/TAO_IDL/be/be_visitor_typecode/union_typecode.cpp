// -*- C++ -*-

//=============================================================================
/**
 *  @file  union_typecode.cpp
 *
 *  $Id$
 *
 *  Union TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#include <string>


TAO::be_visitor_union_typecode::be_visitor_union_typecode (
  be_visitor_context * ctx)
  : be_visitor_typecode_defn (ctx)
{
}

int
TAO::be_visitor_union_typecode::visit_union (be_union * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl << be_nl
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;


  std::string const fields_name (std::string ("_tao_cases_")
                                 + node->flat_name ());


  be_type * const discriminant_type =
    be_type::narrow_from_decl (node->disc_type ());

  ACE_ASSERT (discriminant_type != 0);

  std::string const case_array_type (
    std::string ("TAO::TypeCode::Non_Default_Case<")
    + std::string (discriminant_type->full_name ())
    + std::string (", char const *>"));

  // Generate array containing union non-default case
  // characteristics.
  os << "static " << case_array_type.c_str () << " const "
     << fields_name.c_str ()
     << "[] =" << be_idt_nl
     << "{" << be_idt_nl;

  if (this->visit_cases (node) != 0)
    return -1;

  os << be_uidt_nl
     << "};" << be_uidt_nl << be_nl;

  // Generate the TypeCode instantiation.
  os
    << "static TAO::TypeCode::Union<char const *," << be_nl
    << "                            " << case_array_type.c_str () << " const *," << be_nl
    << "                            TAO::Null_RefCount_Policy> const"
    << be_idt_nl
    << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
    << "\"" << node->repoID () << "\"," << be_nl
    << "\"" << node->original_local_name () << "\"," << be_nl
    << discriminant_type->tc_name () << "," << be_nl
    << "_tao_cases_" << node->flat_name () << "," << be_nl
    << node->nfields () << ","
    << node->default_index () << "," << be_nl
//     << default_member_name,
//     << default_member_type
    << ");" << be_uidt_nl
    << be_uidt_nl;

  return
    this->gen_typecode_ptr (be_type::narrow_from_decl (node));
}

int
TAO::be_visitor_union_typecode::visit_cases (be_union * node)
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
