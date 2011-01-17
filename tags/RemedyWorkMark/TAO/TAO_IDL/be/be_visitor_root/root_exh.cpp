
//=============================================================================
/**
 *  @file    root_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the CIAO
 *  exec impl header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

// ********************************
// Root visitor for CIAO exec impl header
// ********************************

be_visitor_root_exh::be_visitor_root_exh (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_exh::~be_visitor_root_exh (void)
{
}

int
be_visitor_root_exh::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_exh::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_exh::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  (void) tao_cg->end_ciao_exec_header ();

  return 0;
}

int
be_visitor_root_exh::init (void)
{
  // First open the client-side header file for writing.
  int status =
    tao_cg->start_ciao_exec_header (
      be_global->be_get_ciao_exec_hdr_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_exh::init - ")
                         ACE_TEXT ("Error opening CIAO exec impl ")
                         ACE_TEXT ("header file\n")),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_exec_header ());

  return 0;
}
