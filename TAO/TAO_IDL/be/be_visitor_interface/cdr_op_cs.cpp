
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  Visitor generating code for CDR operators for interfaces
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_cdr_op_cs::be_visitor_interface_cdr_op_cs (
    be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cdr_op_cs::~be_visitor_interface_cdr_op_cs ()
{
}

int
be_visitor_interface_cdr_op_cs::visit_interface (be_interface *node)
{
  // No CDR operations for local interfaces.
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  // Visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cdr_op_cs::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  TAO_INSERT_COMMENT (os);

  *os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->full_name () << "_ptr _tao_objref)" << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl;

  AST_Decl::NodeType nt = node->node_type ();

  if (node->is_abstract ())
    {
      *os << "::CORBA::AbstractBase_ptr";
    }
  else if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      *os << "Components::CCMObject_ptr";
    }
  else
    {
      *os << "::CORBA::Object_ptr";
    }

  *os << " _tao_corba_obj = _tao_objref;" << be_nl;
  *os << "return (strm << _tao_corba_obj);" << be_uidt_nl
      << "}" << be_nl_2;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_INPUT);

  *os << "::CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->full_name () << "_ptr &_tao_objref)" << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->is_abstract ())
    {
      *os << "::CORBA::AbstractBase_var obj;";
    }
  else if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      *os << "Components::CCMObject_var obj;";
    }
  else
    {
      *os << "::CORBA::Object_var obj;";
    }

  *os << be_nl_2
      << "if (!(strm >> obj.inout ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl
      << be_nl
      << "// Narrow to the right type." << be_nl;

  *os << "_tao_objref = ";

  if (!node->is_abstract ())
    {
      *os << "TAO::Narrow_Utils< ::" << node->name () << ">::unchecked_narrow ("
          << "obj.in ());";
    }
  else
    {
      *os << "TAO::AbstractBase_Narrow_Utils< ::" << node->name () << ">::unchecked_narrow ("
          << "obj.in ());";
    }

  *os << be_nl_2;

  *os << "return true;" << be_uidt_nl
      << "}" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      *os << be_nl;

      node->gen_ostream_operator (os, false);
    }

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_stub_cdr_op_gen (true);
  return 0;
}

int
be_visitor_interface_cdr_op_cs::visit_component (
  be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_cdr_op_cs::visit_connector (
  be_connector *node)
{
  return this->visit_interface (node);
}

