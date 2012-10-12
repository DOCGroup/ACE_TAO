
//=============================================================================
/**
 *  @file    root_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the Root in the client inline
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "root.h"

be_visitor_root_ci::be_visitor_root_ci (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ci::~be_visitor_root_ci (void)
{
}

int
be_visitor_root_ci::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ci::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ci::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }


  if (this->gen_obv_defns (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ci::")
                         ACE_TEXT ("visit_root - ")
                         ACE_TEXT ("failed to generate OBV_ defns\n")),
                        -1);
    }

  (void) tao_cg->end_client_inline ();

  return 0;
}

int
be_visitor_root_ci::init (void)
{
  /// First open the client-side file for writing
  int status =
    tao_cg->start_client_inline (
      be_global->be_get_client_inline_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ci::init - ")
                         ACE_TEXT ("Error opening client inline file\n")),
                        -1);
    }

  /// Initialize the stream.
  this->ctx_->stream (tao_cg->client_inline ());
  return 0;
}

int
be_visitor_root_ci::gen_obv_defns (be_root *node)
{
  be_visitor_context ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CI);
  be_visitor_obv_module obv_visitor (&ctx);
  return obv_visitor.visit_scope (node);
}
