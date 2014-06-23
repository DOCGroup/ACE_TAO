// -*- C++ -*-

//=============================================================================
/**
 *  @file  value_typecode.cpp
 *
 *  $Id$
 *
 *  Structure TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#include "typecode.h"

TAO::be_visitor_value_typecode::be_visitor_value_typecode (
  be_visitor_context * ctx)
  : be_visitor_typecode_defn (ctx)
  , is_recursive_ (false)
  , is_nested_ (false)
{
}

int
TAO::be_visitor_value_typecode::visit_valuetype (be_valuetype * node)
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
                         "(%N:%l) be_visitor_value_typecode::"
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

  if (this->gen_member_typecodes (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "TAO::be_visitor_value_typecode::visit_valuetype - "
                        "Unable to generate valuetype/eventtype field "
                        "TypeCodes.\n"),
                        -1);
    }

  ACE_CDR::ULong const count =
    node->data_members_count (AST_Field::vis_PUBLIC)
    + node->data_members_count (AST_Field::vis_PRIVATE);

  ACE_CString const fields_name (ACE_CString ("_tao_fields_")
                                 + node->flat_name ());

  // Generate array containing value field characteristics.
  os << "static TAO::TypeCode::Value_Field<char const *, "
     << "::CORBA::TypeCode_ptr const *> const ";

  if (count == 0)
    {
      os << "* const " << fields_name.c_str () << " = 0;" << be_nl;
    }
  else
    {
      os << fields_name.c_str () << "[] =" << be_idt_nl
         << "{" << be_idt_nl;

      if (this->visit_members (node) != 0)
        {
          return -1;
        }

      os << be_uidt_nl
         << "};" << be_uidt_nl;
    }

  // Generate the TypeCode instantiation.

  static char const StringType[]      = "char const *";
  static char const TypeCodeType[]    = "::CORBA::TypeCode_ptr const *";
  static char const MemberArrayType[] =
    "TAO::TypeCode::Value_Field<char const *, "
    "::CORBA::TypeCode_ptr const *> const *";

  os << "static ";

  if (this->is_recursive_)
    {
      os << "TAO::TypeCode::Recursive_Type<" << be_idt_nl;
    }

  // -- TypeCodeBase --
  os
    << "TAO::TypeCode::Value<" << StringType << "," << be_nl
    << "                     " << TypeCodeType << "," << be_nl
    << "                     " << MemberArrayType << "," << be_nl
    << "                     TAO::Null_RefCount_Policy>";

  if (this->is_recursive_)
    {
      os << "," << be_nl
         << TypeCodeType << "," << be_nl
         << MemberArrayType << " >" << be_uidt_nl;
    }

  os << be_idt_nl
     << "_tao_tc_" << node->flat_name () << " (" << be_idt_nl
     << "::CORBA::tk_"
     << (dynamic_cast<be_eventtype *> (node) ? "event" : "value")
     << "," << be_nl
     << "\"" << node->repoID () << "\"," << be_nl
     << "\"" << node->original_local_name () << "\"," << be_nl;

  // ValueModifier
  //
  // TAO doesn't support CUSTOM valuetypes.  Go
  // with VM_TRUNCATABLE, VM_NONE or VM_ABSTRACT.
  os << "::CORBA::"
     << (node->is_abstract () ? "VM_ABSTRACT" :
        (node->truncatable () ? "VM_TRUNCATABLE" :
        "VM_NONE")) << "," << be_nl;

  // Concrete base type.
  AST_Type * const concrete_base =
    node->inherits_concrete ();

  if (concrete_base)
    {
      be_type * const base_type =
        be_type::narrow_from_decl (concrete_base);

      ACE_ASSERT (base_type);

      os << "&" << base_type->tc_name () << "," << be_nl;
    }
  else
    {
      // No concrete base.
      os << "&::CORBA::_tc_null," << be_nl;
    }

  // Fields
  os << "_tao_fields_" << node->flat_name () << "," << be_nl
     << count << ");" << be_uidt_nl
     << be_uidt_nl;

  if (this->gen_typecode_ptr (be_type::narrow_from_decl (node)) != 0)
    {
      return -1;
    }

  return 0;
}

int
TAO::be_visitor_value_typecode::gen_member_typecodes (be_valuetype * node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl * const d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_value_typecode::"
                             "gen_member_typecodes - "
                             "bad node in this scope\n"),
                            0);
        }

      AST_Field * const field = AST_Field::narrow_from_decl (d);

      if (!field
          || field->visibility () == AST_Field::vis_NA)
        {
          continue;
        }

      be_type * const member_type =
        be_type::narrow_from_decl (field->field_type ());

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
        && member_type->in_recursion (recursion_queue))
        {
          this->is_recursive_ = true;
        }
*/
    }

  return 0;
}

int
TAO::be_visitor_value_typecode::visit_members (be_valuetype * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  size_t const count =
    node->data_members_count (AST_Field::vis_PUBLIC)
    + node->data_members_count (AST_Field::vis_PRIVATE);

  size_t i = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl * const d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_value_typecode::"
                             "visit_members - "
                             "bad node in this scope\n"), 0);
        }

      AST_Field * const field = AST_Field::narrow_from_decl (d);

      if (!field)
        {
          continue;
        }

      AST_Field::Visibility const vis = field->visibility ();

      if (vis == AST_Field::vis_NA)
        {
          continue;
        }

      be_decl * const member_decl =
        be_decl::narrow_from_decl (field);

      be_type * const member_type =
        be_type::narrow_from_decl (field->field_type ());

      os << "{ "
         << "\"" << member_decl->original_local_name () << "\", "
         << "&"  << member_type->tc_name () << ", ";

      switch (vis)
        {
        case AST_Field::vis_PUBLIC:
          os << "::CORBA::PUBLIC_MEMBER";
          break;

        case AST_Field::vis_PRIVATE:
          os << "::CORBA::PRIVATE_MEMBER";
          break;

        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_value_typecode::"
                             "visit_members - "
                             "Unknown valuetype member visibility: %d.\n",
                             vis),
                            -1);
        };

      os << " }";

      if (i < count - 1)
        {
          os << ",";
        }

      os << be_nl;

      ++i;
    }

  return 0;
}
