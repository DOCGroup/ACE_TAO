//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_exception_holder_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for excpetion holders.
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           ami_exception_holder_cs, 
           "$Id$")

// ************************************************************
// Interface visitor for client stubs
// ************************************************************

be_visitor_valuetype_ami_exception_holder_cs::
be_visitor_valuetype_ami_exception_holder_cs (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_ami_exception_holder_cs::
~be_visitor_valuetype_ami_exception_holder_cs (void)
{
}

int
be_visitor_valuetype_ami_exception_holder_cs::visit_valuetype (
    be_valuetype *node
  )
{
 TAO_OutStream *os = this->ctx_->stream ();

  if (node->is_nested () 
      && node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      *os << "OBV_";
    }

  *os << node->compute_name ("_tao_", "") << "::"
      << node->compute_local_name ("_tao_", "") << " () { }" << be_nl << be_nl;

  if (node->is_nested () 
      && node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      *os << "OBV_";
    }

  *os << node->compute_name ("_tao_", "") << "::~"
      << node->compute_local_name ("_tao_", "") << " () { }" << be_nl << be_nl;

  // Generate code for the elements of the interface
  if (this->visit_valuetype_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ami_exception_holder_cs::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  return 0;
}


int
be_visitor_valuetype_ami_exception_holder_cs::visit_operation (be_operation *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_RAISE_OPERATION_CS);
  be_visitor_operation_ami_exception_holder_operation_cs visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ami_exception_holder_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  return 0;
}
