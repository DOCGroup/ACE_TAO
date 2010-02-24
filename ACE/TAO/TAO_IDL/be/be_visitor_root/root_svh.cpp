//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_svh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the CIAO servant header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_root,
           root_svh,
           "$Id$")

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
be_visitor_root_svh::init (void)
{
  // First open the client-side header file for writing.
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

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_svnt_header ());

  return 0;
}
