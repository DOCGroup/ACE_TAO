
//=============================================================================
/**
 *  @file    root_cnh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the CIAO
 *  connector impl header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ********************************
// Root visitor for CIAO connector impl header
// ********************************

be_visitor_root_cnh::be_visitor_root_cnh (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_cnh::~be_visitor_root_cnh (void)
{
}

int
be_visitor_root_cnh::init (void)
{
  // First open the client-side header file for writing.
  int status =
    tao_cg->start_ciao_conn_header (
      be_global->be_get_ciao_conn_hdr_fname ());
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cnh::init - ")
                         ACE_TEXT ("Error opening CIAO conn impl ")
                         ACE_TEXT ("header file\n")),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_conn_header ());

  return 0;
}
