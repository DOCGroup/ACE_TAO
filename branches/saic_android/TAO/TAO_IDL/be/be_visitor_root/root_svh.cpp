
//=============================================================================
/**
 *  @file    root_svh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the CIAO servant header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

// ********************************
// Root visitor for CIAO servant header
// ********************************

be_visitor_root_svh::be_visitor_root_svh (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_svh::~be_visitor_root_svh (void)
{
}

int
be_visitor_root_svh::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_svh::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_svh::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  (void) tao_cg->end_ciao_svnt_header ();

  return 0;
}

int
be_visitor_root_svh::init (void)
{
  /// First open the file for writing.
  int status =
    tao_cg->start_ciao_svnt_header (
      be_global->be_get_ciao_svnt_hdr_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_svh::init - ")
                         ACE_TEXT ("Error opening CIAO servant ")
                         ACE_TEXT ("header file\n")),
                        -1);
    }

  /// Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_svnt_header ());

  return 0;
}
