//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_post_docall_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for return type post processing following a
//    do_static_call.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, rettype_post_docall_cs, "$Id$")


// *******************************************************************************
//    be_visitor_operation_rettype_post_docall_cs
//
//    This visitor generates code that passes the return type variable to the
//    do_static_call method
// ********************************************************************************

be_visitor_operation_rettype_post_docall_cs::
be_visitor_operation_rettype_post_docall_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_post_docall_cs::
~be_visitor_operation_rettype_post_docall_cs (void)
{
}

int
be_visitor_operation_rettype_post_docall_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << "_tao_retval = " << bt->name ()
      << "::_narrow (_tao_base_retval, ACE_TRY_ENV);" << be_nl;
  *os << "CORBA::release (_tao_base_retval);\n";
  return 0;
}

int
be_visitor_operation_rettype_post_docall_cs::
visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << "_tao_retval = " << bt->name ()
      << "::_narrow (_tao_base_retval, ACE_TRY_ENV);" << be_nl;
  *os << "CORBA::release (_tao_base_retval);\n";
  return 0;
}
