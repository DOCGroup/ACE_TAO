//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_root.h"

ACE_RCSID(be_visitor_root, root_cs, "$Id$")


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
  if (tao_cg->start_client_stubs (idl_global->be_get_client_stub_fname ())
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_cs - "
                         "Error opening client stub file\n"), -1);
    }

  // init stream
  this->ctx_->stream (tao_cg->client_stubs ());
  return 0;
}
