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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_root.h"


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
  // first open the client-side header file for writing
  if (tao_cg->start_client_header (idl_global->be_get_client_hdr_fname ())
      == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_visitor_root_ch::init - "
                  "Error opening client header file\n"));
      return -1;
    }

  // init the stream
  this->ctx_->stream (tao_cg->client_header ());
  return 0;
}
