
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  $Id$
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for the CDR operators.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru> based on code from Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> based on code from Aniruddha Gokhale
 */
//=============================================================================

#include "valuetype.h"

be_visitor_valuetype_cdr_op_cs::be_visitor_valuetype_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_cdr_op_cs::~be_visitor_valuetype_cdr_op_cs (void)
{
}

int
be_visitor_valuetype_cdr_op_cs::visit_valuetype (be_valuetype *node)
{
  // Already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || ! node->is_defined ())
    {
      return 0;
    }

  // Generate helper functions implementation.
  if (node->gen_helper_stubs () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_cs::"
                         "visit_valuetype - "
                         "codegen for helper functions failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();
  node->cli_stub_cdr_op_gen (true);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci"
                         "::visit_valuetype - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean" << be_nl
      << "operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->full_name ()
      << " *_tao_valuetype" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;
  *os << "return" << be_idt_nl
      << "::CORBA::ValueBase::_tao_marshal (" << be_idt << be_idt_nl
      << "strm," << be_nl
      << "_tao_valuetype," << be_nl
      << "reinterpret_cast<ptrdiff_t> (&"
      << node->full_name () << "::_downcast)"
      << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "::CORBA::Boolean" << be_nl
      << "operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->full_name ()
      << " *&_tao_valuetype" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;
  *os << "return " << node->full_name ()
      << "::_tao_unmarshal (strm, _tao_valuetype);"
      << be_uidt_nl
      << "}" << be_nl_2;

  if (be_global->gen_ostream_operators ())
    {
      node->gen_ostream_operator (os, false);
    }

  *os << be_global->core_versioning_end () << be_nl;

  if (!node->is_abstract ())
    {
      // Functions that marshal state.
      be_visitor_context new_ctx (*this->ctx_);
      be_visitor_valuetype_marshal_cs visitor (&new_ctx);
      visitor.visit_valuetype (node);
    }

  return 0;
}

int
be_visitor_valuetype_cdr_op_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

// @@@ (JP) The following three methods are a hack to get CDR
// operators generated for anonymous array and sequence
// valuetype members. This should be done like it is in structs,
// but part of that mechanism is used by valuetypes for
// generating code to marshal the state. Someday this should
// be untangled and made consistent.

int
be_visitor_valuetype_cdr_op_cs::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_ci::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  // Save the node.
  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_cdr_op_cs::visit_sequence (be_sequence *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_sequence_cdr_op_cs visitor (&ctx);
  return node->accept (&visitor);
}
