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
  , is_recursive_ (false)
  , is_nested_ (false)
{
}

int
TAO::be_visitor_union_typecode::visit_union (be_union * node)
{
  if (!node->is_defined ())
    {
      return this->gen_forward_declared_typecode (node);
    }

  // Check if we are repeated.
  be_visitor_typecode_defn::QNode const * const qnode =
    this->queue_lookup (this->tc_queue_, node);

  ACE_Unbounded_Queue<AST_Type *> recursion_queue;
  this->is_recursive_ = node->in_recursion (recursion_queue);
  if (qnode && this->is_recursive_)
    {
    // we're repeated and we're recursive so just leave
      return 0;
    }
  else if (this->queue_insert (this->tc_queue_, node, 0) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_typecode::"
                         "visit_type - "
                         "queue insert failed\n"),
                        -1);
    }

  if (this->recursion_detect_ || this->is_nested_)
    {
      return 0;
    }

  this->is_nested_ = true;

  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl << be_nl
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  be_type * const discriminant_type =
    be_type::narrow_from_decl (node->disc_type ());

  ACE_ASSERT (discriminant_type != 0);

  if (this->gen_case_typecodes (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO::be_visitor_union_typecode::visit_union - "
                         "Unable to generate union field "
                         "TypeCodes.\n"),
                        -1);
    }

  if (this->visit_cases (node) != 0)
    {
      return -1;
    }

  static char const StringType[]      = "char const *";
  static char const TypeCodeType[]    = "::CORBA::TypeCode_ptr const *";
  static char const MemberArrayType[] =
    "TAO::TypeCode::Case<char const *, "
    "::CORBA::TypeCode_ptr const *> const * const *";

  // Generate the TypeCode instantiation.
  os << "static ";

  if (this->is_recursive_)
    {
      os << "TAO::TypeCode::Recursive_Type<" << be_idt_nl;
    }

  // -- TypeCodeBase --
  os << "TAO::TypeCode::Union<" << StringType << "," << be_nl
     << "                            " << TypeCodeType << "," << be_nl
     << "                            " << MemberArrayType << "," << be_nl
     << "                            TAO::Null_RefCount_Policy>";

  if (this->is_recursive_)
    {
      os << "," << be_nl
         << TypeCodeType << "," << be_nl
         << MemberArrayType << " >" << be_uidt_nl;
    }

  os << be_idt_nl
     << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
     << "\"" << node->repoID () << "\"," << be_nl
     << "\"" << node->original_local_name () << "\"," << be_nl
     << "&" << discriminant_type->tc_name () << "," << be_nl
     << "_tao_cases_" << node->flat_name () << "," << be_nl
     << node->nfields () << ", "
     << node->default_index () << ");" << be_uidt_nl
     << be_uidt_nl;

  if (this->gen_typecode_ptr (be_type::narrow_from_decl (node)) != 0)
    {
      return -1;
    }

  return 0;
}

int
TAO::be_visitor_union_typecode::gen_case_typecodes (be_union * node)
{
  AST_Field ** member_ptr = 0;

  size_t const count = node->nfields ();

  for (size_t i = 0; i < count; ++i)
    {
      node->field (member_ptr, i);

      be_type * const member_type =
        be_type::narrow_from_decl ((*member_ptr)->field_type ());

      if (this->is_typecode_generation_required (member_type)
          && member_type->accept (this) != 0)
        {
          return -1;
        }

/*  MCO@20101020 - this is unnecessary since the check for recursiveness
 *  of *this* typecode has already been done before calling this method

      be_visitor_typecode_defn::QNode const * const qnode =
        this->queue_lookup (this->tc_queue_, node);

      ACE_Unbounded_Queue<AST_Type *> recursion_queue;
      recursion_queue.enqueue_tail (node);
      if (qnode
        &&
          member_type->in_recursion (recursion_queue))
        {
          this->is_recursive_ = true;
        }
*/
    }

  return 0;
}

int
TAO::be_visitor_union_typecode::visit_cases (be_union * node)
{
  // Generate array containing union case/branch characterstics.

  be_type * const discriminant_type =
    be_type::narrow_from_decl (node->disc_type ());

  ACE_CString tao_cases ("_tao_cases_");
  ACE_CString const fields_name (tao_cases
                                 + node->flat_name ());
  const char *fields_name_str = fields_name.c_str ();

  TAO_OutStream & os = *this->ctx_->stream ();

  // Start with static instances of each array element-to-be.
  ACE_CDR::ULong const count = node->nfields ();

  for (ACE_CDR::ULong i = 0; i < count; ++i)
    {
      AST_Field ** member_ptr = 0;

      node->field (member_ptr, i);

      be_type * const type =
        be_type::narrow_from_decl ((*member_ptr)->field_type ());

      be_union_branch * const branch =
        be_union_branch::narrow_from_decl (*member_ptr);

      ACE_ASSERT (branch != 0);

      os << "static TAO::TypeCode::Case_T<"
         << discriminant_type->full_name () << ", "
         << "char const *, ::CORBA::TypeCode_ptr const *> const "
         << fields_name_str << "__" << i <<" (";

      if (branch->label ()->label_kind () == AST_UnionLabel::UL_label)
        {
          // Non-default case.

          // Generate the label value.  Only the first label value is
          // used in the case where multiple labels are used for the
          // same union branch/case.
          branch->gen_label_value (&os, 0);
        }
      else
        {
          // Default case.

          ACE_ASSERT (branch->label ()->label_kind ()
                      == AST_UnionLabel::UL_default);

          branch->gen_default_label_value (&os, node);
        }

      os << ", \"" << branch->original_local_name () << "\", "
         << "&"  << type->tc_name ()
         << ");" << be_nl;
    }

  // Now generate the TAO::TypeCode::Case array.
  os << be_nl
     << "static TAO::TypeCode::Case<char const *, "
     << "::CORBA::TypeCode_ptr const *> const * const "
     << fields_name_str
     << "[] =" << be_idt_nl
     << "{" << be_idt_nl;

  for (ACE_CDR::ULong n = 0; n < count; ++n)
    {
      os << "&" << fields_name_str << "__" << n;

      if (n < count - 1)
        {
          os << ",";
        }

      os << be_nl;
    }

  os << be_uidt_nl
     << "};" << be_uidt_nl << be_nl;

  return 0;
}
