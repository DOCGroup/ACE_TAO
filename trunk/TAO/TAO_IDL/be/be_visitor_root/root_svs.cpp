//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_svs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the CIAO servant source
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_root,
           root_svs,
           "$Id$")

// ********************************
// Root visitor for CIAO servant source
// ********************************

be_visitor_root_svs::be_visitor_root_svs (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_svs::~be_visitor_root_svs (void)
{
}

int
be_visitor_root_svs::init (void)
{
  // First open the client-side header file for writing.
  int status =
    tao_cg->start_ciao_svnt_source (
      be_global->be_get_ciao_svnt_src_fname ());
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_svs::init - ")
                         ACE_TEXT ("Error opening CIAO servant ")
                         ACE_TEXT ("source file\n")),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_svnt_source ());

  return 0;
}
