
//=============================================================================
/**
 *  @file    root_cns.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the CIAO
 *  connector impl source
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ********************************
// Root visitor for CIAO connector impl source
// ********************************

be_visitor_root_cns::be_visitor_root_cns (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_cns::~be_visitor_root_cns (void)
{
}

int
be_visitor_root_cns::init (void)
{
  // First open the client-side header file for writing.
  int status =
    tao_cg->start_ciao_conn_source (
      be_global->be_get_ciao_conn_src_fname ());
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cns::init - ")
                         ACE_TEXT ("Error opening CIAO conn impl ")
                         ACE_TEXT ("source file\n")),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_conn_source ());

  return 0;
}
