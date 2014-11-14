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

#include "typecode.h"

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
  bool in_recursion = node->in_recursion (recursion_queue);
  if (qnode && in_recursion)
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

  // as this was no nested visit mark this typecode as recursive
  this->is_recursive_ = in_recursion;

  this->is_nested_ = true;

  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl_2
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

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
     << "sizeof (_tao_cases_" << node->flat_name () << ")/"
        "sizeof (_tao_cases_" << node->flat_name () << "[0])," << be_nl
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

  ACE_CDR::ULong const count = node->nfields ();

  for (ACE_CDR::ULong i = 0u; i < count; ++i)
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
  ACE_CDR::ULong countLabels = 0u;

  // Start with static instances of each array element-to-be.
  ACE_CDR::ULong const countFields = node->nfields ();
  for (ACE_CDR::ULong fieldNumber = 0u; fieldNumber < countFields; ++fieldNumber)
    {
      AST_Field ** member_ptr = 0;
      node->field (member_ptr, fieldNumber);

      be_type * const type =
        be_type::narrow_from_decl ((*member_ptr)->field_type ());

      be_union_branch * const branch =
        be_union_branch::narrow_from_decl (*member_ptr);

      ACE_ASSERT (branch != 0);
      ACE_CDR::ULong numberOfLabels = branch->label_list_length ();

      // Check for a default branch for the current union field.
      ACE_CDR::ULong labelNumber;
      for (labelNumber= 0u; labelNumber < numberOfLabels; ++labelNumber)
        if (branch->label (labelNumber)->label_kind ()
            == AST_UnionLabel::UL_default)
          {
            // Generate the default branch ONLY for the current union field.
            os << "static TAO::TypeCode::Case_T<"
               << discriminant_type->full_name () << ", "
               << "char const *, ::CORBA::TypeCode_ptr const *> const "
               << fields_name_str << "__" << countLabels++ << " (";

            branch->gen_default_label_value (&os, node);

            os << ", \"" << branch->original_local_name () << "\", "
               << "&"  << type->tc_name ()
               << ");" << be_nl;

            // Since we have just generated the default case, there is no
            // point in generating the other alternative cases as they
            // are all covered by this default anyway and any other
            // cases covered by the default will take up extra static
            // memory space for no reason.
            numberOfLabels = 0u;
            break;
          }

      // Now we have handled the default case for this type (if any),
      // generate all the other alternate case labels for the type.
      for (labelNumber= 0u; labelNumber < numberOfLabels; ++labelNumber)
        {
          ACE_ASSERT (branch->label (labelNumber)->label_kind ()
                      == AST_UnionLabel::UL_label);

          os << "static TAO::TypeCode::Case_T<"
             << discriminant_type->full_name () << ", "
             << "char const *, ::CORBA::TypeCode_ptr const *> const "
             << fields_name_str << "__" << countLabels++ << " (";

          branch->gen_label_value (&os, labelNumber);

          os << ", \"" << branch->original_local_name () << "\", "
             << "&"  << type->tc_name ()
             << ");" << be_nl;
        }
    }

  // Now generate the TAO::TypeCode::Case array.
  os << be_nl
     << "static TAO::TypeCode::Case<char const *, "
     << "::CORBA::TypeCode_ptr const *> const * const "
     << fields_name_str
     << "[] =" << be_idt_nl
     << "{" << be_idt_nl;

  for (ACE_CDR::ULong n = 0u; n < countLabels; ++n)
    {
      os << "&" << fields_name_str << "__" << n;

      if (n < countLabels - 1u)
        {
          os << "," << be_nl;
        }
      else
        {
          os << be_uidt_nl;
        }
    }

  os << "};" << be_uidt_nl << be_nl;

  return 0;
}
