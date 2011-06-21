
//=============================================================================
/**
 *  @file    root_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the client stubs file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ***********************************
// Root visitor for client stubs
// ***********************************

be_visitor_root_cs::be_visitor_root_cs (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_cs::~be_visitor_root_cs (void)
{
}

int
be_visitor_root_cs::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cs::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }


  if (this->gen_arg_traits (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cs::")
                         ACE_TEXT ("visit_root - failed to ")
                         ACE_TEXT ("generate stub arg traits\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cs::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }


  if (this->gen_obv_defns (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cs::")
                         ACE_TEXT ("visit_root - ")
                         ACE_TEXT ("failed to generate OBV_ defns\n")),
                        -1);
    }


  if (this->gen_any_ops (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cs::")
                         ACE_TEXT ("visit_root - failed to ")
                         ACE_TEXT ("generate Any operators\n")),
                        -1);
    }


  if (this->gen_cdr_ops (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cs::")
                         ACE_TEXT ("visit_root - failed to ")
                         ACE_TEXT ("generate CDR operators\n")),
                        -1);
    }


  (void) tao_cg->end_client_stubs ();

  return 0;
}

int
be_visitor_root_cs::init (void)
{
  /// First open the client-side file for writing
  int status =
    tao_cg->start_client_stubs (
      be_global->be_get_client_stub_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cs::init - ")
                         ACE_TEXT ("Error opening client source file\n")),
                        -1);
    }

  /// Initialize the stream.
  this->ctx_->stream (tao_cg->client_stubs ());
  return 0;
}

int
be_visitor_root_cs::gen_arg_traits (be_root *node)
{
  be_visitor_context ctx = *this->ctx_;
  be_visitor_arg_traits arg_visitor ("", &ctx);
  return node->accept (&arg_visitor);
}

int
be_visitor_root_cs::gen_obv_defns (be_root *node)
{
  be_visitor_context ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CS);
  be_visitor_obv_module obv_visitor (&ctx);
  return obv_visitor.visit_scope (node);
}

int
be_visitor_root_cs::gen_any_ops (be_root *node)
{
  int status = 0;

  if (be_global->any_support ())
    {
      be_visitor_context ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_ROOT_ANY_OP_CS);
      be_visitor_root_any_op any_op_visitor (&ctx);
      status = node->accept (&any_op_visitor);
    }

  /// Conditional switch to the *A.cpp stream is done
  /// in the visitor constructor.
  if (be_global->gen_anyop_files ())
    {
      (void) tao_cg->end_anyop_source ();
    }

  return status;
}

int
be_visitor_root_cs::gen_cdr_ops (be_root *node)
{
  be_visitor_context ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_ROOT_CDR_OP_CS);
  be_visitor_root_cdr_op visitor (&ctx);
  return node->accept (&visitor);
}


