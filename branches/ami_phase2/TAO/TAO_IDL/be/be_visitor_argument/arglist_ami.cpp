//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    arglist.cpp
//
// = DESCRIPTION
//    Visitor that generates the parameters in an Operation signature
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, arglist_ami, "$Id$")


// ************************************************************
// be_visitor_args_arglist for parameter list in method declarations and
// definitions for AMI stubs.
// ************************************************************

be_visitor_args_arglist_ami::be_visitor_args_arglist_ami (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_arglist_ami::~be_visitor_args_arglist_ami (void)
{
}

int
be_visitor_args_arglist_ami::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node);                   // save the argument node 

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent (); // start with current indentation level

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  *os << " " << node->local_name () << ",\n";
  return 0;
}

int
be_visitor_args_arglist_ami::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  
  switch (this->direction ())
    {
      // No out Parameters in the AMI stub:
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node);
      break;
    }
  return 0;
}
