//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for interfaces
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface,
           cdr_op_cs,
           "$Id$")

be_visitor_interface_cdr_op_cs::be_visitor_interface_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cdr_op_cs::~be_visitor_interface_cdr_op_cs (void)
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

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  *os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->full_name () << "_ptr _tao_objref)" << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->is_abstract ())
    {
      *os << "::CORBA::AbstractBase_ptr";
    }
  else if (node->node_type () == AST_Decl::NT_component)
    {
      *os << "Components::CCMObject_ptr";
    }
  else
    {
      *os << "::CORBA::Object_ptr";
    }

  *os << " _tao_corba_obj = _tao_objref;" << be_nl;
  *os << "return (strm << _tao_corba_obj);" << be_uidt_nl
      << "}" << be_nl << be_nl;

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
  else if (node->node_type () == AST_Decl::NT_component)
    {
      *os << "Components::CCMObject_var obj;";
    }
  else
    {
      *os << "::CORBA::Object_var obj;";
    }

  *os << be_nl << be_nl
      << "if (!(strm >> obj.inout ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "typedef ::" << node->name () << " RHS_SCOPED_NAME;"
      << be_nl << be_nl
      << "// Narrow to the right type." << be_nl;

  *os << "_tao_objref =" << be_idt_nl;

  if (!node->is_abstract ())
    {
      *os << "TAO::Narrow_Utils<RHS_SCOPED_NAME>::unchecked_narrow (";
    }
  else
    {
      *os << "TAO::AbstractBase_Narrow_Utils<RHS_SCOPED_NAME>::unchecked_narrow (";
    }

  *os << be_idt << be_idt_nl
      << "obj.in ()," << be_nl;

  if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
    {
      *os << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer" << be_uidt_nl;
    }
  else
    {
      *os << "0" << be_uidt_nl;
    }

  *os << ");" << be_uidt_nl << be_uidt_nl;

  *os << "return true;" << be_uidt_nl
      << "}" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      *os << be_nl;

      node->gen_ostream_operator (os);
    }

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_stub_cdr_op_gen (1);
  return 0;
}
