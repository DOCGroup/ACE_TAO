//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_root.h"


// ***********************************
// Root visitor for server header
// ***********************************

be_visitor_root_sh::be_visitor_root_sh (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_sh::~be_visitor_root_sh (void)
{
}

int
be_visitor_root_sh::init (void)
{
  // open the file
  if (tao_cg->start_server_header (idl_global->be_get_server_hdr_fname ())
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_sh::init - "
                         "Error opening server header file\n"), -1);
    }

  if (tao_cg->start_server_template_header
      (idl_global->be_get_server_template_hdr_fname ())
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_sh::init - "
                         "Error opening server template header file\n"),
                        -1);
    }

  // set the stream and the next state
  this->ctx_->stream (tao_cg->server_header ());
  return 0;
}
