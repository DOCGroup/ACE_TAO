//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_arglist.cpp
//
// = DESCRIPTION
//    Visitor that generates the parameters in an operation of the AMI
//    Reply Handler.
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>  
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, ami_handler_arglist, "$Id$")


// ************************************************************
// Visitor for parameter list in AMI Handler call back declarations
// and definitions.
// ************************************************************

be_visitor_args_ami_handler_arglist::be_visitor_args_ami_handler_arglist (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_ami_handler_arglist::~be_visitor_args_ami_handler_arglist (void)
{
}

int
be_visitor_args_ami_handler_arglist::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_arglist::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent (); // start with current indentation level

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  int result = bt->accept (this);
  if (result == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_arglist::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }
  // Print the variable name only if the type was printed already. 
  if (result)
    *os << " " << node->local_name () << ",\n";

  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "const " << this->type_name (node);
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << this->type_name (node);
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_ptr");
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  
  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_ptr");
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << this->type_name (node);
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << "const " << this->type_name (node) << " &";
          return 1;
      /* NOT REACHED */
        } // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_ptr");
          return 1;
          /* NOT REACHED */
        } // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << this->type_name (node);
          return 1;
          /* NOT REACHED */
        } // end switch direction
    } // end of else

  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "const " << this->type_name (node) << " &";
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "const char *";
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "const " << this->type_name (node) << " &";
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "const " << this->type_name (node) << " &";
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_arglist::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}


#ifdef IDL_HAS_VALUETYPE

int
be_visitor_args_ami_handler_arglist::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << this->type_name (node) << " *";
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

int
be_visitor_args_ami_handler_arglist::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:      
      *os << "const " << this->type_name (node) << " *";
      return 1;
      /* NOT REACHED */
    }
  return 0;
}

#endif /* IDL_HAS_VALUETYPE */
