//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    post_invoke_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for post-processing of arguments following an
//    invocation.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, post_invoke_cs, "$Id$")


// *************************************************************************
// visitor for argument to do any post invocation processing. Not all types need
// this. Only those that have an _out type need this.  In addition, interfaces
// need it because we need to convert from the interface type to the base Object
// type and vice versa.
// *************************************************************************

be_visitor_args_post_invoke_cs::be_visitor_args_post_invoke_cs
(be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_post_invoke_cs::~be_visitor_args_post_invoke_cs (void)
{
}

int
be_visitor_args_post_invoke_cs::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type of the argument
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_pre_docall_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_docall_compiled_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_args_post_invoke_cs::visit_interface (be_interface *node)
{
  // we must narrow the out object reference to the appropriate type
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
      {
        // Assign the narrowed obj reference.
        if (node->is_defined ())
          {
            *os << "CORBA::release (" << arg->local_name () << ");" << be_nl;
          }
        else
          {
            *os << "tao_" << node->flat_name () << "_release ("
                << arg->local_name () << ");" << be_nl;
          }
      }
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_args_post_invoke_cs::visit_interface_fwd (be_interface_fwd *node)
{
  // we must narrow the out object reference to the appropriate type
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
      {
        // Assign the narrowed obj reference.
        if (node->full_definition ()->is_defined ())
          {
            *os << "CORBA::release (" << arg->local_name () << ");" << be_nl;
          }
        else
          {
            *os << "tao_" << node->flat_name () << "_release ("
                << arg->local_name () << ");" << be_nl;
          }
      }
      break;
    default:
      break;
    }
  return 0;
}

#ifdef IDL_HAS_VALUETYPE

int
be_visitor_args_post_invoke_cs::visit_valuetype (be_valuetype *)
{
  // we must narrow the out object reference to the appropriate type
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
      {
        os->indent ();
        *os << "CORBA::remove_ref (" << arg->local_name ()
            << ");\n";
      }
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_args_post_invoke_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  // we must narrow the out object reference to the appropriate type
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
      {
        os->indent ();
        *os << "CORBA::remove_ref (" << arg->local_name ()
            << ");\n";
      }
      break;
    default:
      break;
    }
  return 0;
}

#endif /* IDL_HAS_VALUETYPE */

int
be_visitor_args_post_invoke_cs::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      os->indent ();

      if (node->width () == (long) sizeof (char))
        {
          *os << "CORBA::string_free (" << arg->local_name ()
              << ");" << be_nl;
        }
      else
        {
          *os << "CORBA::wstring_free (" << arg->local_name ()
              << ");" << be_nl;
        }

      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int
be_visitor_args_post_invoke_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_docall_compiled_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
