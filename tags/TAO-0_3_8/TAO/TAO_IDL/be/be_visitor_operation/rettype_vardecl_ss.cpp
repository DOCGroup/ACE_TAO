//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_vardecl_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for return type variable declaration in the
//    skeleton file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, rettype_vardecl_ss, "$Id$")


// ********************************************************************************
//    be_visitor_operation_rettype_vardecl_ss
//    This visitor generates code for variable declaration and initialization
//    of the return type.
// ********************************************************************************

be_visitor_operation_rettype_vardecl_ss::be_visitor_operation_rettype_vardecl_ss
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_vardecl_ss::
~be_visitor_operation_rettype_vardecl_ss (void)
{
}

int
be_visitor_operation_rettype_vardecl_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
#if 0 /* ASG */
  *os << bt->name () << "_var _tao_retval;" << be_nl;
  *os << bt->name () << "_slice *&_tao_ptr_retval = _tao_retval.out ();\n";
#endif
  *os << bt->name () << "_var _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << " _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  // due to the virtual inheritance and the fact that we will be passing the
  // address of the objref to the marshaling routine, we use the base
  // CORBA::Object_ptr as the type for the return value even though the actual
  // return type may be some derived class
  *os << "CORBA::Object_var _tao_retval = CORBA::Object::_nil ();\n";
#if 0 /* ASG */
  *os << "CORBA::Object_var _tao_retval = CORBA::Object::_nil ();" << be_nl;
  *os << "CORBA::Object_ptr &_tao_ptr_retval = _tao_retval.out ();\n";
#endif
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::
visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  // due to the virtual inheritance and the fact that we will be passing the
  // address of the objref to the marshaling routine, we use the base
  // CORBA::Object_ptr as the type for the return value even though the actual
  // return type may be some derived class
#if 0 /* ASG */
  *os << "CORBA::Object_var _tao_retval = CORBA::Object::_nil ();" << be_nl;
  *os << "CORBA::Object_ptr &_tao_ptr_retval = _tao_retval.out ();\n";
#endif
  *os << "CORBA::Object_var _tao_retval = CORBA::Object::_nil ();" << be_nl;
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      os->indent ();
#if 0 /* ASG */
      *os << bt->name () << "_var _tao_retval;" << be_nl;
      *os << bt->name () << "_ptr &_tao_ptr_retval = _tao_retval.out ();\n";
#endif
      *os << bt->name () << "_var _tao_retval;\n";
      break;
    case AST_PredefinedType::PT_any:
      os->indent ();
#if 0 /* ASG */
      *os << bt->name () << "_var _tao_retval;" << be_nl;
      *os << bt->name () << "_ptr &_tao_ptr_retval = _tao_retval.out ();\n";
#endif
      *os << bt->name () << "_var _tao_retval;\n";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      os->indent ();
      *os << bt->name () << " _tao_retval = 0;\n";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_sequence (be_sequence *node)
{
  // we should never directly be here because anonymous sequence return types
  // are not allowed
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
#if 0 /* ASg */
  *os << bt->name () << "_var _tao_retval;" << be_nl;
  *os << bt->name () << " *&_tao_ptr_retval = _tao_retval.out ();\n";
#endif
  *os << bt->name () << "_var _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_string (be_string * /* node*/)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
#if 0 /* ASG */
  *os << "CORBA::String_var _tao_retval;" << be_nl;
  *os << "char *&_tao_ptr_retval = _tao_retval.out ();\n";
#endif
  *os << "CORBA::String_var _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    {
#if 0 /* ASG */
      *os << bt->name () << "_var _tao_retval;" << be_nl;
      *os << bt->name () << " *&_tao_ptr_retval = _tao_retval.out ();\n";
#endif
      *os << bt->name () << "_var _tao_retval;\n";
    }
  else
    *os << bt->name () << " _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_vardecl_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    {
#if 0 /* ASG */
      *os << bt->name () << "_var _tao_retval;" << be_nl;
      *os << bt->name () << " *&_tao_ptr_retval = _tao_retval.out ();\n";
#endif
      *os << bt->name () << "_var _tao_retval;" << be_nl;
    }
  else
    *os << bt->name () << " _tao_retval;\n";
  return 0;
}


// ********************************************************************************
//    be_visitor_operation_rettype_vardecl_ss
//    This visitor generates code for variable declaration and initialization
//    of the return type.
// ********************************************************************************

be_compiled_visitor_operation_rettype_vardecl_ss
::be_compiled_visitor_operation_rettype_vardecl_ss
(be_visitor_context *ctx)
  : be_visitor_operation_rettype_vardecl_ss (ctx)
{
}

be_compiled_visitor_operation_rettype_vardecl_ss::
~be_compiled_visitor_operation_rettype_vardecl_ss (void)
{
}

int
be_compiled_visitor_operation_rettype_vardecl_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << node->name () << "_var _tao_retval;\n";
  return 0;
}

int
be_compiled_visitor_operation_rettype_vardecl_ss::
visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << node->name () << "_var _tao_retval;\n";
  return 0;
}

