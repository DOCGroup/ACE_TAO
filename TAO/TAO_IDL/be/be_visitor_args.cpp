//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_args.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Argument
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_args.h"

be_visitor_args::be_visitor_args (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_args::~be_visitor_args (void)
{
}

int be_visitor_args::visit_argument (be_argument *node)
{
  return -1;
}

// helper that returns the type name either as a nested type name (for header
// files) or as a fully scoped name. In addition, we make sure that if the type
// is an alias, we use that name
const char *
be_visitor_args::type_name (be_type *node, const char *suffix)
{
  static char namebuf [NAMEBUFSIZE];
  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  be_type *bt; // type to use

  // use the typedefed name if that is the one used in the IDL defn
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // generate the NESTED_CLASS macro i.e., a relative path name to satisfy the
  // MSVC++ compiler
  switch (this->ctx_->stream ()->stream_type ())
    {
    case TAO_OutStream::TAO_CLI_HDR:
      ACE_OS::sprintf (namebuf, "%s", bt->nested_type_name
                       (this->ctx_->scope (), suffix));
      break;
    default: // fullname for all other cases
      ACE_OS::sprintf (namebuf, "%s", bt->fullname ());
      if (suffix)
        ACE_OS::strcat (namebuf, suffix);
    }

  return namebuf;
}

// helper that returns the direction type of the argument
AST_Argument::Direction
be_visitor_args::direction (void)
{
  // grab the argument node. We know that our context has stored the right
  // argument node
  be_argument *arg = this->ctx_->be_node_as_argument ();

  ACE_ASSERT (arg != 0);
  return arg->direction ();
}

// ************************************************************
// be_visitor_args_arglist for parameter list in method declarations and
// definitions
// ************************************************************

be_visitor_args_arglist::be_visitor_args_arglist (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_arglist::~be_visitor_args_arglist (void)
{
}

int be_visitor_args_arglist::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

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

int be_visitor_args_arglist::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_INOUT: // inout
      *os << this->type_name (node, "_ptr") << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_INOUT: // inout
      *os << this->type_name (node, "_ptr") << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node) << " &";
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  *os << "const " << this->type_name (node) << " &";
	  break;
	case AST_Argument::dir_INOUT:
	  *os << this->type_name (node) << " &";
	  break;
	case AST_Argument::dir_OUT:
	  *os << this->type_name (node, "_out");
	  break;
	} // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  *os << this->type_name (node, "_ptr");
	  break;
	case AST_Argument::dir_INOUT:
	  *os << this->type_name (node, "_ptr") << " &";
	  break;
	case AST_Argument::dir_OUT:
	  *os << this->type_name (node, "_out");
	  break;
	} // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  *os << this->type_name (node);
	  break;
	case AST_Argument::dir_INOUT:
	  *os << this->type_name (node) << " &";
	  break;
	case AST_Argument::dir_OUT:
	  *os << this->type_name (node, "_out");
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_arglist::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const char *";
      break;
    case AST_Argument::dir_INOUT:
      *os << "char *&";
      break;
    case AST_Argument::dir_OUT:
      *os << "CORBA::String_out";
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// *************************************************************************
// visitor for argument to do any pre docall processing. Not all types need
// this. Only those that have an _out type need this.  In addition, interfaces
// need it because we ned to convert from the interface type to the base Object
// type and vice versa.
// *************************************************************************

be_visitor_args_pre_docall_cs::be_visitor_args_pre_docall_cs
(be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_pre_docall_cs::~be_visitor_args_pre_docall_cs (void)
{
}

int
be_visitor_args_pre_docall_cs::void_return_type (void)
{
  // is the operation return type void?
  be_argument *arg = this->ctx_->be_node_as_argument ();
  ACE_ASSERT (arg != 0);
  be_operation *op = be_operation::narrow_from_scope (arg->defined_in ());
  ACE_ASSERT (arg != 0);

  be_type *bt = be_type::narrow_from_decl (op->return_type ());
  if (bt->node_type () == AST_Decl::NT_pre_defined
      && (be_predefined_type::narrow_from_decl (bt)->pt ()
          == AST_PredefinedType::PT_void))
    return 1;
  else
    return 0;
}

int be_visitor_args_pre_docall_cs::visit_argument (be_argument *node)
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
                         "be_visitor_args_pre_docall_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_args_pre_docall_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << bt->name () << "_slice *&_tao_base_" << arg->local_name ()
          << " = " << arg->local_name () << ".ptr ();\n";
      if (!this->void_return_type ())
        {
          *os << "ACE_NEW_RETURN (_tao_base_" << arg->local_name ()
              << ", " << bt->name () << "_slice, _tao_retval);\n";
        }
      else
        {
          *os << "ACE_NEW (_tao_base_" << arg->local_name ()
              << ", " << bt->name () << "_slice);\n";
        }
      break;
    default:
      break;
    }
  return 0;

}

int
be_visitor_args_pre_docall_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << "CORBA::Object_ptr _tao_base_" << arg->local_name ()
          << " = " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "CORBA::Object_ptr _tao_base_" << arg->local_name () << ";\n";
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_args_pre_docall_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << "CORBA::Object_ptr _tao_base_" << arg->local_name ()
          << " = " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "CORBA::Object_ptr _tao_base_" << arg->local_name () << ";\n";
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_args_pre_docall_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // pre do_static_call processing is valid only for pseudo objects and for Any
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      {
      switch (this->direction ())
        {
        case AST_Argument::dir_OUT:
          os->indent ();
          *os << bt->name () << "_ptr &_tao_base_" << arg->local_name ()
              << " = " << arg->local_name () << ".ptr ();\n";
          break;
        default:
          break;
        }
      }
      break;
    case AST_PredefinedType::PT_any:
      {
      switch (this->direction ())
        {
        case AST_Argument::dir_OUT:
          os->indent ();
          *os << bt->name () << " *&_tao_base_" << arg->local_name ()
              << " = " << arg->local_name () << ".ptr ();" << be_nl;
          if (!this->void_return_type ())
            {
              *os << "ACE_NEW_RETURN (_tao_base_" << arg->local_name ()
                  << ", CORBA::Any, _tao_retval);\n";
            }
          else
            {
              *os << "ACE_NEW (_tao_base_" << arg->local_name ()
                  << ", CORBA::Any);\n";
            }
          break;
        default:
          break;
        }
      }
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_args_pre_docall_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_OUT:
      // caller must have allocated the pointer
      os->indent ();
      *os << bt->name () << " *&_tao_base_" << arg->local_name ()
          << " = " << arg->local_name () << ".ptr ();" << be_nl;
      if (!this->void_return_type ())
        {
          *os << "ACE_NEW_RETURN (_tao_base_" << arg->local_name ()
              << ", " << bt->name () << ", _tao_retval);\n";
        }
      else
        {
          *os << "ACE_NEW (_tao_base_" << arg->local_name ()
              << ", " << bt->name () << ");\n";
        }
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_args_pre_docall_cs::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_OUT:
      // caller should have allocated the pointer
      os->indent ();
      *os << "char *&_tao_base_" << arg->local_name ()
          << " = " << arg->local_name () << ".ptr ();\n";
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_args_pre_docall_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (node->size_type () == be_type::VARIABLE)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_OUT:
          // caller must have allocated the pointer
          os->indent ();
          *os << bt->name () << " *&_tao_base_" << arg->local_name ()
              << " = " << arg->local_name () << ".ptr ();" << be_nl;
          if (!this->void_return_type ())
            {
              *os << "ACE_NEW_RETURN (_tao_base_" << arg->local_name ()
                  << ", " << bt->name () << ", _tao_retval);\n";
            }
          else
            {
              *os << "ACE_NEW (_tao_base_" << arg->local_name ()
                  << ", " << bt->name () << ");\n";
            }
          break;
        default:
          break;
        }
    }
  return 0;
}

int
be_visitor_args_pre_docall_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (node->size_type () == be_type::VARIABLE)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_OUT:
          os->indent ();
          *os << bt->name () << " *&_tao_base_" << arg->local_name ()
              << " = " << arg->local_name () << ".ptr ();" << be_nl;
          if (!this->void_return_type ())
            {
              *os << "ACE_NEW_RETURN (_tao_base_" << arg->local_name ()
                  << ", " << bt->name () << ", _tao_retval);\n";
            }
          else
            {
              *os << "ACE_NEW (_tao_base_" << arg->local_name ()
                  << ", " << bt->name () << ");\n";
            }
          break;
        default:
          break;
        }
    }
  return 0;
}

int
be_visitor_args_pre_docall_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_pre_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ****************************************************************************
// visitor for argument passing to do_static_call. The do_static_call method takes a variable
// number of parameters. The total number of parameters is determined by the
// "calldata" parameter that must be passed before the variable list
// starts. Each argument to the do_static_call is considered to be of type "void *".
// Hence we pass the address of each argument. The case for _out is a bit
// tricky where we must first retrieve the pointer, allocate memory and pass it
// to do_static_call. This is done in the "pre_do_static_call" processing.
// ****************************************************************************

be_visitor_args_docall_cs::be_visitor_args_docall_cs (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_docall_cs::~be_visitor_args_docall_cs (void)
{
}

int be_visitor_args_docall_cs::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type of the argument
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_docall_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_docall_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_docall_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      // pass reference to the pointer to slice
      *os << "_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      // pass the address. Storage is already allocated
      *os << "&" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "&_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "&_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get argument node

  os->indent ();
  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
	  *os << "&" << arg->local_name ();
	  break;
	case AST_Argument::dir_OUT:
	  *os << "_tao_base_" << arg->local_name ();
	  break;
	} // end switch direction
    } // end of if any
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g.,
                                                         // CORBA::Object,
                                                         // CORBA::TypeCode
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
          *os << "&" << arg->local_name ();
          break;
	case AST_Argument::dir_OUT:
          *os << "&_tao_base_" << arg->local_name ();
	  break;
	} // end switch direction
    } // end else if pseudo
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
	case AST_Argument::dir_OUT:
	  *os << "&" << arg->local_name ();
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_docall_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      *os << "_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      *os << "&_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      {
        // check if the size type is VARIABLE
        if (node->size_type () == be_type::VARIABLE)
          *os << "_tao_base_" << arg->local_name ();
        else
          *os << "&" << arg->local_name ();
      }
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      {
        // check if the size type is VARIABLE
        if (node->size_type () == be_type::VARIABLE)
          *os << "_tao_base_" << arg->local_name ();
        else
          *os << "&" << arg->local_name ();
      }
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// *************************************************************************
// visitor for argument to do any pre docall processing. Not all types need
// this. Only those that have an _out type need this.  In addition, interfaces
// need it because we ned to convert from the interface type to the base Object
// type and vice versa.
// *************************************************************************

be_visitor_args_post_docall_cs::be_visitor_args_post_docall_cs
(be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_post_docall_cs::~be_visitor_args_post_docall_cs (void)
{
}

int
be_visitor_args_post_docall_cs::visit_argument (be_argument *node)
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
                         "be_visitor_args_pre_docall_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_args_post_docall_cs::visit_interface (be_interface *node)
{
  // we must narrow the out object reference to the appropriate type
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      {
        os->indent ();
        // assign the narrowed obj reference
        *os << arg->local_name () << " = " << node->name ()
            << "::_narrow (_tao_base_" << arg->local_name ()
            << ", _tao_environment);" << be_nl;
        *os << "CORBA::release (_tao_base_" << arg->local_name ()
            << ");\n";
      }
      break;
    }
  return 0;
}

int
be_visitor_args_post_docall_cs::visit_interface_fwd (be_interface_fwd *node)
{
  // we must narrow the out object reference to the appropriate type
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      {
        os->indent ();
        // assign the narrowed obj reference
        *os << arg->local_name () << " = " << node->name ()
            << "::_narrow (_tao_base_" << arg->local_name ()
            << ", _tao_environment);" << be_nl;
        *os << "CORBA::release (_tao_base_" << arg->local_name ()
            << ");\n";
      }
      break;
    }
  return 0;
}

int
be_visitor_args_post_docall_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ************************************************************************
// Visitor to generate code for argument variable declaration
// ************************************************************************

be_visitor_args_vardecl_ss::be_visitor_args_vardecl_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_vardecl_ss::~be_visitor_args_vardecl_ss (void)
{
}

int be_visitor_args_vardecl_ss::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << bt->name () << "_var _tao_var_"
	      << arg->local_name () << ";" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_var_" << arg->local_name () << ".out ());\n";
        }
      else
        *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << "_ptr " << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_ptr _tao_base_" << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "CORBA::Object_ptr _tao_base_"
	  << arg->local_name () << ";" << be_nl;
      *os << bt->name () << "_var _tao_var_"
	  << arg->local_name () << ";" << be_nl;
      *os << bt->name () << "_out " << arg->local_name ()
          << " (_tao_var_" << arg->local_name () << ".out ());\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << "_ptr " << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_ptr _tao_base_" << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "CORBA::Object_ptr _tao_base_"
	  << arg->local_name () << ";" << be_nl;
      *os << bt->name () << "_var _tao_var_"
	  << arg->local_name () << ";" << be_nl;
      *os << bt->name () << "_out " << arg->local_name ()
          << " (_tao_var_" << arg->local_name () << ".out ());\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
          os->indent ();
          *os << bt->name () << " " << arg->local_name () << ";\n";
	  break;
	case AST_Argument::dir_OUT:
          os->indent ();
          *os << bt->name () << "_var _tao_var_"
	      << arg->local_name () << ";" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_var_" << arg->local_name () << ".out ());\n";
	  break;
	} // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
	{
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          os->indent ();
          *os << bt->name () << "_ptr " << arg->local_name () << ";\n";
          break;
        case AST_Argument::dir_OUT:
          os->indent ();
          *os << bt->name () << "_var _tao_var_"
	      << arg->local_name () << ";" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_var_" << arg->local_name () << ".out ());\n";
          break;
	} // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
	case AST_Argument::dir_OUT:
          os->indent ();
          *os << bt->name () << " " << arg->local_name () << ";\n";
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_vardecl_ss::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << bt->name () << "_var _tao_var_"
	  << arg->local_name () << ";" << be_nl;
      *os << bt->name () << "_out " << arg->local_name ()
          << " (_tao_var_" << arg->local_name () << ".out () );\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << "char *" << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "CORBA::String_var _tao_var_"
	  << arg->local_name () << ";" << be_nl;
      *os << "CORBA::String_out " << arg->local_name ()
          << " (_tao_var_" << arg->local_name () << ".out ());\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      // check if it is variable sized
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << bt->name () << "_var _tao_var_"
	      << arg->local_name () << ";" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_var_" << arg->local_name () << ".out ());\n";
        }
      else
        *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      // check if it is variable sized
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << bt->name () << "_var _tao_var_"
	      << arg->local_name () << ";" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_var_" << arg->local_name () << ". ptr());\n";
        }
      else
        *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ************************************************************************
// Visitor to generate code for passing argument to the marshal/demarshal
// routines
// ************************************************************************

be_visitor_args_marshal_ss::be_visitor_args_marshal_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_marshal_ss::~be_visitor_args_marshal_ss (void)
{
}

int be_visitor_args_marshal_ss::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_marshal_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_marshal_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "_tao_var_" << arg->local_name () << ".ptr ()";
      break;
    }
  return 0;
}

int be_visitor_args_marshal_ss::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "&" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_marshal_ss::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "&_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_marshal_ss::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "&_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_marshal_ss::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
          os->indent ();
          *os << "&" << arg->local_name ();
	  break;
	case AST_Argument::dir_OUT:
          os->indent ();
          *os << "&" << arg->local_name () << ".ptr ()";
	  break;
	} // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
          os->indent ();
          *os << "&" << arg->local_name ();
	  break;
	case AST_Argument::dir_OUT:
          os->indent ();
          *os << "_tao_var_" << arg->local_name () << ".ptr ()";
	  break;
	} // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
	case AST_Argument::dir_OUT:
          os->indent ();
          *os << "&" << arg->local_name ();
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_marshal_ss::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "_tao_var_" << arg->local_name () << ".ptr ()";
      break;
    }
  return 0;
}

int be_visitor_args_marshal_ss::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "&" << arg->local_name () << ".ptr ()";
      break;
    }
  return 0;
}

int be_visitor_args_marshal_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      if (node->size_type () == be_type::VARIABLE)
        *os << "_tao_var_" << arg->local_name () << ".ptr ()";
      else
        *os << "&" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_marshal_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      if (node->size_type () == be_type::VARIABLE)
        *os << "_tao_var_" << arg->local_name () << ".ptr ()";
      else
        *os << "&" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_marshal_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_marshal_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ************************************************************************
// visitor for passing arguments to the upcall
// ************************************************************************

be_visitor_args_pre_upcall_ss::be_visitor_args_pre_upcall_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_pre_upcall_ss::~be_visitor_args_pre_upcall_ss (void)
{
}

int be_visitor_args_pre_upcall_ss::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_pre_upcall_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_pre_upcall_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT: // inout
      os->indent ();
      *os << arg->local_name () << " = " << node->name ()
          << "::_narrow (_tao_base_" << arg->local_name ()
          << ", _tao_environment);\n";
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT: // inout
      os->indent ();
      *os << arg->local_name () << " = " << node->name ()
          << "::_narrow (_tao_base_" << arg->local_name ()
          << "_tao_environment);\n";
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_pre_upcall_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_pre_upcall_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ************************************************************************
// visitor for passing arguments to the upcall
// ************************************************************************

be_visitor_args_upcall_ss::be_visitor_args_upcall_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_upcall_ss::~be_visitor_args_upcall_ss (void)
{
}

int be_visitor_args_upcall_ss::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << node->local_name ();
      break;
    }
  return 0;

}

// ************************************************************************
//  visitor for doing any post-processing after the upcall is made
// ************************************************************************

be_visitor_args_post_upcall_ss::be_visitor_args_post_upcall_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_post_upcall_ss::~be_visitor_args_post_upcall_ss (void)
{
}

int be_visitor_args_post_upcall_ss::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_upcall_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_upcall_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_post_upcall_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT: // inout
      os->indent ();
      *os << "CORBA::release (_tao_base_" << arg->local_name () << ");"
          << be_nl;
      *os << "_tao_base_" << arg->local_name ()
	  << " = " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "_tao_base_" << arg->local_name () << " = _tao_var_"
          << arg->local_name () << ".in ();\n";
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT: // inout
      os->indent ();
      *os << "CORBA::release (_tao_base_" << arg->local_name () << ");"
          << be_nl;
      *os << "_tao_base_" << arg->local_name () << " = " << arg->local_name ()
          << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "_tao_base_" << arg->local_name () << " = _tao_var_"
          << arg->local_name () << ".in ();\n";
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_post_upcall_ss::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_upcall_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ************************************************************************
//  visitor for doing any post-processing after the marshaling is done
// ************************************************************************

be_visitor_args_post_marshal_ss::be_visitor_args_post_marshal_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_post_marshal_ss::~be_visitor_args_post_marshal_ss (void)
{
}

int be_visitor_args_post_marshal_ss::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_marshal_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_marshal_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_post_marshal_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT: // inout
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "CORBA::release (_tao_base_" << arg->local_name () << ");\n";
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT: // inout
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << "CORBA::release (_tao_base_" << arg->local_name () << ");\n";
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_post_marshal_ss::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_marshal_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
