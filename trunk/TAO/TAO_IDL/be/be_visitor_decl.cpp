//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_decl.cpp
//
// = DESCRIPTION
//    Visitor for the base be_decl node. This serves to maintain the current
//    state (context) of code generation for the derived visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_decl.h"

ACE_RCSID(be, be_visitor_decl, "$Id$")


// Root visitor for client header
be_visitor_decl::be_visitor_decl (be_visitor_context *ctx)
  : ctx_ (ctx)
{
}

be_visitor_decl::~be_visitor_decl (void)
{
  delete this->ctx_;
}
