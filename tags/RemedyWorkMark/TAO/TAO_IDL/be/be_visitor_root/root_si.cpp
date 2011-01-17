
//=============================================================================
/**
 *  @file    root_si.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the Root in the server inline file
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ***********************************************
// Root visitor for server inline
// ***********************************************

be_visitor_root_si::be_visitor_root_si (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_si::~be_visitor_root_si (void)
{
}

int
be_visitor_root_si::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_si::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_si::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  (void) tao_cg->end_server_inline ();

  return  0;
}

int
be_visitor_root_si::init (void)
{
  /// First open the server-side file for writing
  int status =
    tao_cg->start_server_inline (
      be_global->be_get_server_inline_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_si::init - ")
                         ACE_TEXT ("Error opening server inline file\n")),
                        -1);
    }

  /// Initialize the stream.
  this->ctx_->stream (tao_cg->server_inline ());
  return 0;
}
