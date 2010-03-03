
//=============================================================================
/**
 *  @file    root_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the client stubs file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// ***********************************
// Root visitor for client stubs
// ***********************************

be_visitor_root_cs::be_visitor_root_cs (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_cs::~be_visitor_root_cs (void)
{
}

int
be_visitor_root_cs::init (void)
{
  // first open the file
  if (tao_cg->start_client_stubs (be_global->be_get_client_stub_fname ())
        == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_cs - "
                         "Error opening client stub file\n"), 
                        -1);
    }

  // init stream
  this->ctx_->stream (tao_cg->client_stubs ());
  return 0;
}
