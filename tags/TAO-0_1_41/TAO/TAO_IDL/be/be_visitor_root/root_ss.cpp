//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for the Root in the server skeletons file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_root.h"


// ******************************************************
// Root visitor for server skeletons
// ******************************************************

be_visitor_root_ss::be_visitor_root_ss (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ss::~be_visitor_root_ss (void)
{
}

int
be_visitor_root_ss::init (void)
{
  // first open the file for writing
  if (tao_cg->start_server_skeletons (idl_global
                                      ->be_get_server_skeleton_fname ())
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_ss::init - "
                         "Error opening server skeletons file\n"), -1);
    }

  if (tao_cg->start_server_template_skeletons
      (idl_global->be_get_server_template_skeleton_fname ())
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_ss::init - "
                         "Error opening server template skeleton file\n"),
                        -1);
    }

  // set stream
  this->ctx_->stream (tao_cg->server_skeletons ());
  return 0;
}
