//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_root, 
           root_ch, 
           "$Id$")

// ********************************
// Root visitor for client header
// ********************************

be_visitor_root_ch::be_visitor_root_ch (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ch::~be_visitor_root_ch (void)
{
}

int
be_visitor_root_ch::init (void)
{
  // First open the client-side header file for writing
  if (tao_cg->start_client_header (be_global->be_get_client_hdr_fname ())
        == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_ch::init - "
                         "Error opening client header file\n"),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->client_header ());

  return 0;
}
