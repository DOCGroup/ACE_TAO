// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_ih.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the server implementation header
//
// = AUTHOR
//   Yamuna  Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_root.h"

ACE_RCSID(be_visitor_root, root_ih, "$Id$")


// ***********************************
// Root visitor for implementation header
// ***********************************

be_visitor_root_ih::be_visitor_root_ih (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ih::~be_visitor_root_ih (void)
{
}

int
be_visitor_root_ih::init (void)
{
  // open the file
  if (tao_cg->start_implementation_header (
          be_global->be_get_implementation_hdr_fname ()
        )
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_ih::init - "
                         "Error :%p: Unable to open implementation header file : %s\n",
                         "start_implementation_header",
                         be_global->be_get_implementation_hdr_fname ()),
                        -1);
    }


  // set the stream and the next state
  this->ctx_->stream (tao_cg->implementation_header ());
  return 0;
}
