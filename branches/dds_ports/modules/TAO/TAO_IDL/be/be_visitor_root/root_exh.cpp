//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    root_exh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the CIAO exec impl header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_root,
           root_exh,
           "$Id$")

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
                         ACE_TEXT ("Error opening CIAO exewc impl ")
                         ACE_TEXT ("header file\n")),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_exec_header ());

  return 0;
}
