
//=============================================================================
/**
 *  @file    operation_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the CIAO executor file.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_operation_exs::be_visitor_operation_exs (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    os_ (*ctx->stream ()),
    scope_ (0),
    your_code_here_ ("/* Your code here. */"),
    class_extension_ ("_exec_i")
{
}

be_visitor_operation_exs::~be_visitor_operation_exs (void)
{
}

int
be_visitor_operation_exs::visit_operation (be_operation *node)
{
  // Impl classes shouldn't have implied AMI operations.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

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
      << this->ctx_->port_prefix ().c_str ()
      << this->scope_->original_local_name ()->get_string ()
      << this->class_extension_.c_str () << "::"
      << node->local_name ();

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

void
be_visitor_operation_exs::scope (be_decl *node)
{
  this->scope_ = node;
}

void
be_visitor_operation_exs::class_extension (const char *extension)
{
  this->class_extension_ = extension;
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

