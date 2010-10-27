
//=============================================================================
/**
 *  @file    serializer_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TAO::DCPS::Serializer operators for structures
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#include "utl_identifier.h"

be_visitor_structure_serializer_op_cs::be_visitor_structure_serializer_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_serializer_op_cs::~be_visitor_structure_serializer_op_cs (void)
{
}

int
be_visitor_structure_serializer_op_cs::visit_structure (be_structure *node)
{
  if (node->cli_stub_serializer_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_serializer_op_cs::"
                         "visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  IDL_GlobalData::DCPS_Data_Type_Info* info;
  if (0 != (info =  idl_global->is_dcps_type(node->name())))
    {
      *os << "::CORBA::Boolean _dcps_has_key (const "
          << node->name() << "& ) { " << be_idt_nl;
      if (info->key_list_.is_empty())
        *os << "return false;";
      else
        *os << "return true;";
      *os << be_uidt_nl << "}" << be_nl;
    }

  //- _dcps_max_marshaled_size(type) method
  this->ctx_->sub_state (TAO_CodeGen::TAO_MAX_MARSHALED_SIZE);
  *os << "size_t "
      << "_dcps_max_marshaled_size (const "
      << node->name() << "& _tao_aggregate)" << be_nl
      << "{" << be_idt_nl
      << "ACE_UNUSED_ARG (_tao_aggregate);"
      << " // sometimes not used - avoid warning" << be_nl;

  {
    be_visitor_context new_ctx (*this->ctx_);
    be_visitor_serializer_op_field_decl field_decl (&new_ctx);

    if (field_decl.visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_serializer_op_cs::"
                             "visit_structure - "
                             "codegen for field decl scope failed\n"),
                            -1);
        }

#ifdef DCPS_DEBUG_IDL
    *os << "size_t result = 0;" << be_nl;
#else
    *os << "return " << be_idt_nl;
#endif

    if (this->visit_scope (node) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) be_visitor_structure_serializer_op_cs::"
                          "visit_structure - "
                          "codegen for scope failed\n"),
                          -1);
      }
  }
  *os << ";" << be_uidt << be_uidt_nl;
#ifdef DCPS_DEBUG_IDL
  *os << "return result;" <<  be_uidt_nl;
#endif
  *os << "}" << be_nl_2;


  //- _tao_is_bounded_size(type) method
  this->ctx_->sub_state (TAO_CodeGen::TAO_IS_BOUNDED_SIZE);
  *os << "::CORBA::Boolean _tao_is_bounded_size (const "
      << node->name() << "& _tao_aggregate)" << be_nl
      << "{" << be_idt_nl;

  {
    be_visitor_context new_ctx (*this->ctx_);
    be_visitor_serializer_op_field_decl field_decl (&new_ctx);
    field_decl.visit_scope (node);

    *os << "ACE_UNUSED_ARG (_tao_aggregate);"
        << " // often not used - avoid warning"  << be_nl
        << "return" << be_idt_nl;

    if (this->visit_scope (node) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) be_visitor_structure_serializer_op_cs::"
                          "visit_structure - "
                          "codegen for scope failed\n"),
                          -1);
      }
  }
  *os << ";" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  //- _dcps_find_size(type) method -- like max_marshaled_size but use len - not max.
  this->ctx_->sub_state (TAO_CodeGen::TAO_FIND_SIZE);
  *os << "size_t "
      << "_dcps_find_size(const "
      << node->name() << "& _tao_aggregate)" << be_nl
      << "{" << be_idt_nl
      << "ACE_UNUSED_ARG(_tao_aggregate);"
      << " // sometimes not used - avoid warning" << be_nl;

  {
    be_visitor_context new_ctx (*this->ctx_);
    be_visitor_serializer_op_field_decl field_decl (&new_ctx);
    field_decl.visit_scope (node);
#ifdef DCPS_DEBUG_IDL
    *os << "size_t result = 0;" << be_nl;
#else
    *os << "return " << be_idt_nl;
#endif

    if (this->visit_scope (node) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) be_visitor_structure_serializer_op_cs::"
                          "visit_structure - "
                          "codegen for scope failed\n"),
                          -1);
      }
  }
  *os << ";" << be_uidt << be_uidt_nl;
#ifdef DCPS_DEBUG_IDL
  *os << "return result;" <<  be_uidt_nl;
#endif
  *os << "}" << be_nl_2;


  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO::DCPS::Serializer &strm," << be_nl
      << "const " << node->name () << " &_tao_aggregate" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  be_visitor_context new_ctx (*this->ctx_);
  be_visitor_serializer_op_field_decl field_decl (&new_ctx);
  field_decl.visit_scope (node);

  *os << "return" << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_serializer_op_cs::"
                         "visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << ";" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_INPUT);

  *os << "::CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO::DCPS::Serializer &";

  if (! node->is_local ())
    {
      *os << "strm";
    }

  *os << "," << be_nl
      << node->name () << " &";

  if (! node->is_local ())
    {
      *os << "_tao_aggregate";
    }

  *os << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->is_local ())
    {
      *os << "return 0;";
    }
  else
    {
      new_ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      field_decl.visit_scope (node);

      *os << "return" << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_serializer_op_cs"
                             "::visit_structure - "
                             "codegen for scope failed\n"),
                            -1);
        }

      *os << ";" << be_uidt << be_uidt;
    }

  *os << be_uidt_nl << "}";


  // QueryCondition support

  *os << be_nl_2 << "#ifdef DDS_USE_QUERY_CONDITION_COMPARATOR" << be_nl
      << "namespace OpenDDS" << be_nl
      << "{" << be_idt_nl
      << "namespace DCPS" << be_nl
      << "{" << be_idt_nl
      << "ComparatorBase::Ptr create_qc_comparator (" << node->name ()
      << " *, const char *field, ComparatorBase::Ptr next)" << be_nl
      << "{" << be_idt_nl;

  size_t nfields = node->nfields ();
  bool used_args (false);
  for (size_t i = 0; i < nfields; ++i)
    {
      AST_Field **f;
      node->field (f, i);
      char *fname = (*f)->local_name ()->get_string ();
      ACE_UINT32 fname_len = ACE_OS::strlen (fname) + 1;
      AST_Type *ftype = (*f)->field_type ();
      UTL_ScopedName *ftype_name = ftype->name ();
      switch (ftype->node_type ())
        {
        case AST_Decl::NT_pre_defined:
        case AST_Decl::NT_string:
        case AST_Decl::NT_enum:
          used_args = true;
          *os << "if (ACE_OS::strcmp(\"" << fname << "\", field) == 0)"
              << be_idt_nl
              << "{" << be_idt_nl
              << "return make_field_cmp(&" << node->name () << "::" << fname
              << ", next);" << be_uidt_nl
              << "}" << be_uidt_nl;
          break;
        case AST_Decl::NT_struct:
          used_args = true;
          *os << "if (ACE_OS::strncmp(\"" << fname << ".\", field, "
              << fname_len << ") == 0)"
              << be_idt_nl
              << "{" << be_idt_nl
              << "return make_struct_cmp(&" << node->name () << "::" << fname
              << ", create_qc_comparator(static_cast<" << ftype_name
              << " *>(0), field + " << fname_len << ", 0), next);"
              << be_uidt_nl
              << "}" << be_uidt_nl;
          break;
        default: ;
        }
    }

  if (!used_args)
    {
      *os << "ACE_UNUSED_ARG (field);" << be_nl
          << "ACE_UNUSED_ARG (next);" << be_nl;
    }

  *os << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "#endif" << be_nl;

  node->cli_stub_serializer_op_gen (true);
  return 0;
}

int
be_visitor_structure_serializer_op_cs::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_OUTPUT:
    case TAO_CodeGen::TAO_CDR_INPUT:
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      if (!this->last_node (bd))
        {
          *os << " &&" << be_nl;
        }
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
    case TAO_CodeGen::TAO_FIND_SIZE:
#ifdef DCPS_DEBUG_IDL
      *os << ";" << be_nl;
#else
      if (!this->last_node (bd))
        {
          *os << " +" << be_nl;
        }
#endif
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
    default:
      break;
    };

  return 0;
}

int
be_visitor_structure_serializer_op_cs::pre_process (be_decl *bd)
{
#ifdef DCPS_DEBUG_IDL
  TAO_OutStream *os = this->ctx_->stream ();
#else
  ACE_UNUSED_ARG(bd);
#endif

      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
          break;
        case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
        case TAO_CodeGen::TAO_FIND_SIZE:
#ifdef DCPS_DEBUG_IDL
          *os << "result += ";
#endif
          break;
        case TAO_CodeGen::TAO_CDR_OUTPUT:
        case TAO_CodeGen::TAO_CDR_INPUT:
        case TAO_CodeGen::TAO_CDR_SCOPE:
        default:
          break;
        };
  return 0;
}
