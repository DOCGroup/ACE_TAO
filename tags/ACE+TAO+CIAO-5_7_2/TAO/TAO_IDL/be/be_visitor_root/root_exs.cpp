//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    root_exs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the CIAO
//    exec impl source
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_root,
           root_exs,
           "$Id$")

// ********************************
// Root visitor for CIAO exec impl source
// ********************************

be_visitor_root_exs::be_visitor_root_exs (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_exs::~be_visitor_root_exs (void)
{
}

int
be_visitor_root_exs::init (void)
{
  // First open the client-side header file for writing.
  int status =
    tao_cg->start_ciao_exec_source (
      be_global->be_get_ciao_exec_src_fname ());
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_exs::init - ")
                         ACE_TEXT ("Error opening CIAO exec impl ")
                         ACE_TEXT ("source file\n")),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_exec_source ());

  return 0;
}
