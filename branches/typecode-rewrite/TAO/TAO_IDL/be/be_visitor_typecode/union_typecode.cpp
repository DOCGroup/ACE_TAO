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
#include <iostream>


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

  be_union_branch * default_case = 0;
  if (this->visit_cases (node, default_case) != 0)
    return -1;

  os << be_uidt_nl
     << "};" << be_uidt_nl << be_nl;

  // Generate the TypeCode instantiation.
  os
    << "static TAO::TypeCode::Union<char const *," << be_nl
    << "                            " << case_array_type.c_str ()
    << " const *," << be_nl
    << "                            TAO::Null_RefCount_Policy> const"
    << be_idt_nl
    << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
    << "\"" << node->repoID () << "\"," << be_nl
    << "\"" << node->original_local_name () << "\"," << be_nl
    << discriminant_type->tc_name () << "," << be_nl
    << "_tao_cases_" << node->flat_name () << "," << be_nl
    << node->nfields () << ","
    << node->default_index () << "," << be_nl;

  if (default_case)
    {
      be_type * const type =
        be_type::narrow_from_decl (default_case->field_type ());

      os << "\"" << default_case->original_local_name () << "\", "
         << "&"  << type->tc_name ();
    }
  else
    {
      os << "0, 0"; // No default_case.
    }

  os << ");" << be_uidt_nl
     << be_uidt_nl;

  return
    this->gen_typecode_ptr (be_type::narrow_from_decl (node));
}

int
TAO::be_visitor_union_typecode::visit_cases (be_union * node,
                                             be_union_branch *& default_case)
{
  AST_Field ** member_ptr = 0;

  size_t const count = node->nfields ();

  TAO_OutStream & os = *this->ctx_->stream ();

  default_case = 0;

  be_type * const discriminant_type =
    be_type::narrow_from_decl (node->disc_type ());

  for (size_t i = 0; i < count; ++i)
    {
      node->field (member_ptr, i);

      be_union_branch * const branch =
        be_union_branch::narrow_from_decl (*member_ptr);

      ACE_ASSERT (branch != 0);

      if (branch->label ()->label_kind () == AST_UnionLabel::UL_label)
        {
          // Only deal with non-default cases.  The default case has
          // special handling in the union TypeCode implementation.

          os << "TAO::TypeCode::Non_Default_Case<"
             << discriminant_type->full_name () << ", "
             << "char const *> (";

          // Generate the label value.  Only the first label value is
          // used in the case where multiple labels are used for the
          // same union branch/case.
          branch->gen_label_value (&os, 0);

          os << ", ";

          be_type * const type =
            be_type::narrow_from_decl ((*member_ptr)->field_type ());

          os << "\"" << branch->original_local_name () << "\", "
             << "&"  << type->tc_name ()
             << ")";

          if (i < count - (node->default_index () != -1 ? 2 : 1))
            os << ",";

          os << be_nl;
        }
      else
        {
          default_case = branch;
        }
    }

  return 0;
}
