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


  be_type * const discriminant_type =
    be_type::narrow_from_decl (node->disc_type ());

  ACE_ASSERT (discriminant_type != 0);

  if (this->gen_case_typecodes (node) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO::be_visitor_union_typecode::visit_union - "
                       "Unable to generate union field "
                       "TypeCodes.\n"),
                      -1);

  if (this->visit_cases (node) != 0)
    return -1;

  // Generate the TypeCode instantiation.
  os
    << "static TAO::TypeCode::Union<char const *," << be_nl
    << "                            TAO::TypeCode::Case<char const *>"
    << " const * const *," << be_nl
    << "                            TAO::Null_RefCount_Policy>"
    << be_idt_nl
    << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
    << "\"" << node->repoID () << "\"," << be_nl
    << "\"" << node->original_local_name () << "\"," << be_nl
    << "&" << discriminant_type->tc_name () << "," << be_nl
    << "_tao_cases_" << node->flat_name () << "," << be_nl
    << node->nfields () << ", "
    << node->default_index () << ");" << be_uidt_nl
    << be_uidt_nl;

  return
    this->gen_typecode_ptr (be_type::narrow_from_decl (node));
}

int
TAO::be_visitor_union_typecode::gen_case_typecodes (be_union * node)
{
  AST_Field ** member_ptr = 0;

  size_t const count = node->nfields ();

  for (size_t i = 0; i < count; ++i)
    {
      node->field (member_ptr, i);

      be_interface * const intf =
        be_interface::narrow_from_decl ((*member_ptr)->field_type ());

      if (intf && intf->is_defined ())
        {
          // Only generate TypeCodes for interfaces and valuetypes if
          // they are forward declared.
          continue;
        }

      be_type * const member_type =
        be_type::narrow_from_decl ((*member_ptr)->field_type ());

      member_type->accept (this);
    }

  return 0;
}

int
TAO::be_visitor_union_typecode::visit_cases (be_union * node)
{
  // Generate array containing union case/branch characterstics.

  be_type * const discriminant_type =
    be_type::narrow_from_decl (node->disc_type ());

  std::string const fields_name (std::string ("_tao_cases_")
                                 + node->flat_name ());

  TAO_OutStream & os = *this->ctx_->stream ();

  // Start with static instances of each array element-to-be.
  size_t const count = node->nfields ();

  for (size_t i = 0; i < count; ++i)
    {
      AST_Field ** member_ptr = 0;

      node->field (member_ptr, i);

      be_type * const type =
        be_type::narrow_from_decl ((*member_ptr)->field_type ());

      be_union_branch * const branch =
        be_union_branch::narrow_from_decl (*member_ptr);

      ACE_ASSERT (branch != 0);

      if (branch->label ()->label_kind () == AST_UnionLabel::UL_label)
        {
          // Non-default case.

          os << "static TAO::TypeCode::Non_Default_Case<"
             << discriminant_type->full_name () << ", "
             << "char const *> const "
             << fields_name.c_str () << "_" << i <<" (";

          // Generate the label value.  Only the first label value is
          // used in the case where multiple labels are used for the
          // same union branch/case.
          branch->gen_label_value (&os, 0);

          os << ", ";
        }
      else
        {
          // Default case.

          ACE_ASSERT (branch->label ()->label_kind ()
                      == AST_UnionLabel::UL_default);

          os << "static TAO::TypeCode::Default_Case<char const *> const "
             << fields_name.c_str () << "_" << i << "(";
        }

      os << "\"" << branch->original_local_name () << "\", "
         << "&"  << type->tc_name ()
         << ");" << be_nl;
    }

  // Now generate the TAO::TypeCode::Case array.
  os << be_nl
     << "static TAO::TypeCode::Case<char const *> const * const "
     << fields_name.c_str ()
     << "[] =" << be_idt_nl
     << "{" << be_idt_nl;

  for (size_t n = 0; n < count; ++n)
    {
      os << "&" << fields_name.c_str () << "_" << n;

      if (n < count - 1)
        os << ",";

      os << be_nl;
    }

  os << be_uidt_nl
     << "};" << be_uidt_nl << be_nl;

  return 0;
}
