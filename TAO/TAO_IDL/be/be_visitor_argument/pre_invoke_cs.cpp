//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    pre_invoke_cs.cpp
//
// = DESCRIPTION
//    Visitor that generates code (if any) for pre-processing prior to call to
//    do_static_call in the client stub
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, pre_invoke_cs, "$Id$")


// *************************************************************************
// visitor for argument to do any pre invoke processing. Not all types need
// this. Only those that have an _out type need this.  This ne is for compiled
// marshaling and overrides osme methods of the base class (which does the job
// for interpretive marshaling)
// *************************************************************************

be_visitor_args_pre_invoke_cs::be_visitor_args_pre_invoke_cs
(be_visitor_context *ctx)
  : be_visitor_args_pre_docall_cs (ctx)
{
}

be_visitor_args_pre_invoke_cs::~be_visitor_args_pre_invoke_cs (void)
{
}

int
be_visitor_args_pre_invoke_cs::visit_interface (be_interface *)
{
  // overriding action
  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

