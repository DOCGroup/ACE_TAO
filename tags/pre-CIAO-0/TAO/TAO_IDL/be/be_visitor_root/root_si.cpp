//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_si.cpp
//
// = DESCRIPTION
//    Visitor generating code for the Root in the server inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_root.h"

ACE_RCSID(be_visitor_root, root_si, "$Id$")


// ***********************************************
// Root visitor for server inline
// ***********************************************

be_visitor_root_si::be_visitor_root_si (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_si::~be_visitor_root_si (void)
{
}

int
be_visitor_root_si::init (void)
{
  // first open the file for writing
  if (tao_cg->start_server_inline (be_global->be_get_server_inline_fname ())
        == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_si::init - "
                         "server inline open failed\n"), -1);
    }

  if (be_global->gen_tie_classes ())
    {
      if (tao_cg->start_server_template_inline (
              be_global->be_get_server_template_inline_fname ()
            )
          == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root_si::init - "
                             "Error opening server template inline file\n"),
                            -1);
        }
    }

  // init stream
  this->ctx_->stream (tao_cg->server_inline ());
  return 0;
}
