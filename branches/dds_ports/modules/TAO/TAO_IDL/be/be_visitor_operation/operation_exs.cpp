//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    operation_exs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the CIAO executor file.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_operation,
           operation_exs,
           "$Id$")

// ************************************************************
// Operation visitor for CIAO executor implementations
// ************************************************************

be_visitor_operation_exs::be_visitor_operation_exs (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    os_ (*ctx->stream ()),
    scope_ (0),
    your_code_here_ ("/* Your code here. */")
{
}

be_visitor_operation_exs::~be_visitor_operation_exs (void)
{
}

int
be_visitor_operation_exs::visit_operation (be_operation *node)
{
  this->ctx_->node (node);

  os_ << be_nl << be_nl;
     
  // Retrieve the operation return type.
  be_type *rt = be_type::narrow_from_decl (node->return_type ());

  if (rt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_exs::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("Bad return type\n")),
                        -1);
    }

  // Generate the return type mapping (same as in the header file)
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_rettype rt_visitor (&ctx);

  if (rt->accept (&rt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_exs::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("codegen for return ")
                         ACE_TEXT ("type failed\n")),
                        -1);
    }

  // Generate the operation name
  os_ << be_nl
      << scope_->local_name () << "_exec_i::" << node->local_name ();
  
  // Generate the argument list, which will use our overrridden 
  // visit_argument().
  be_visitor_operation_arglist al_visitor (this->ctx_);
  al_visitor.unused (true);

  if (node->accept (&al_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_exs::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("codegen for argument ")
                         ACE_TEXT ("list failed\n")),
                        -1);
    }

  // Must set this again, it's been changed by traversals above.
  this->ctx_->node (node);
  
  return this->gen_op_body (rt);
}

int
be_visitor_operation_exs::visit_argument (be_argument *node)
{
  os_ << "/* " << node->local_name () << " */";
  
  return 0;
}

int
be_visitor_operation_exs::post_process (be_decl *bd)
{
  if (this->last_node (bd))
    {
      os_ << ");" << be_uidt;
    }
  else
    {
      os_ << "," << be_nl;
    }

  return 0;
}

void
be_visitor_operation_exs::scope (be_interface *node)
{
  this->scope_ = node;
}

int
be_visitor_operation_exs::gen_op_body (be_type *return_type)
{
  os_ << be_nl
      << "{" << be_idt_nl
      << your_code_here_;
      
  be_operation *op =
    be_operation::narrow_from_decl (this->ctx_->node ());
  
  if (! op->void_return_type ())
    {
      os_ << be_nl;
    
      be_null_return_emitter emitter (this->ctx_);
      
      if (emitter.emit (return_type) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_operation_exs::")
                             ACE_TEXT ("gen_op_body - ")
                             ACE_TEXT ("be_null_return_emitter::")
                             ACE_TEXT ("emit() failed\n")),
                            -1);
        }
    }
       
  os_ << be_uidt_nl
      << "}";
      
  return 0;
}

