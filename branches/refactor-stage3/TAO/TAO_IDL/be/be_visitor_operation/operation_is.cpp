// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_is.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the implementation skeleton
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           operation_is, 
           "$Id$")

// ************************************************************
// Operation visitor for implementation skeleton
// ************************************************************

be_visitor_operation_is::be_visitor_operation_is (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_is::~be_visitor_operation_is (void)
{
}

int
be_visitor_operation_is::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_interface *intf = this->ctx_->interface ();

  this->ctx_->node (node); // save the node

  // STEP I: generate the return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_is::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype oro_visitor (&ctx);

  if (bt->accept (&oro_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_is::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  const char *classname = 0;

  if (intf)
    {
      // If derived class/
      classname = intf->flat_name ();
    }
  else
    {
      classname = ScopeAsDecl (node->defined_in ())->flat_name ();
    }

  // STEP 2: generate the operation name
  *os << " " << be_global->impl_class_prefix () << classname
      << be_global->impl_class_suffix () << "::" << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_IS);
  be_visitor_operation_arglist oa_visitor (&ctx);

  if (node->accept (&oa_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_is::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  *os << be_nl << "{" << be_idt_nl;
  *os << "// Add your implementation here" << be_uidt_nl;
  *os << "}" << be_nl << be_nl;

  return 0;
}

