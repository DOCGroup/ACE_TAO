//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ctor_assign.cpp
//
// = DESCRIPTION
//    Visitor generating code for the special CTOR and assignment operator for
//    Exceptions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_exception.h"

ACE_RCSID(be_visitor_exception, ctor_assign, "$Id$")


// ************************************************************************
// used for the body of the assignment operator and the copy constructor
// ************************************************************************

be_visitor_exception_ctor_assign::be_visitor_exception_ctor_assign (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_exception_ctor_assign::~be_visitor_exception_ctor_assign (void)
{
}

int be_visitor_exception_ctor_assign::visit_exception (be_exception *node)
{
  this->ctx_->node (node); // save the argument node

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor_assign::"
                         "visit_exception - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int be_visitor_exception_ctor_assign::visit_field (be_field *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor_assign::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor_assign::"
                         "visit_field - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_exception_ctor_assign::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  if (this->ctx_->exception ()) // special ctor
    {
      *os << "this->" << bd->local_name () << " = _tao_" << bd->local_name ()
          << ";\n";
    }
  else
    {
      *os << "this->" << bd->local_name () << " = _tao_excp." << bd->local_name ()
          << ";\n";
    }
  return 0;
}

int be_visitor_exception_ctor_assign::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  if (this->ctx_->exception ()) // special ctor
    {
      *os << "this->" << bd->local_name () << " = _tao_" << bd->local_name ()
          << ";\n";
    }
  else
    {
      *os << "this->" << bd->local_name () << " = _tao_excp." << bd->local_name ()
          << ";\n";
    }
  return 0;
}

int be_visitor_exception_ctor_assign::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  if (this->ctx_->exception ()) // special ctor
    {
      *os << "this->" << bd->local_name () << " = " << node->name ()
          << "::_duplicate (_tao_" << bd->local_name () << ");\n";
    }
  else
    {
      *os << "this->" << bd->local_name () << " = " << node->name ()
          << "::_duplicate (_tao_excp." << bd->local_name () << ".in ());\n";
    }
  return 0;
}

int be_visitor_exception_ctor_assign::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  if (this->ctx_->exception ()) // special ctor
    {
      *os << "this->" << bd->local_name () << " = " << node->name ()
          << "::_duplicate (_tao_" << bd->local_name () << ");\n";
    }
  else
    {
      *os << "this->" << bd->local_name () << " = " << node->name ()
          << "::_duplicate (_tao_excp." << bd->local_name () << ".in ());\n";
    }
  return 0;
}

int be_visitor_exception_ctor_assign::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      if (this->ctx_->exception ()) // special ctor
        {
          *os << "this->" << bd->local_name () << " = _tao_"
              << bd->local_name () << ";\n";
        }
      else
        {
          *os << "this->" << bd->local_name () << " = _tao_excp."
              << bd->local_name () << ";\n";
        }
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      if (this->ctx_->exception ()) // special ctor
        {
          *os << "this->" << bd->local_name () << " = "
              << node->name () << "::_duplicate (_tao_"
              << bd->local_name () << ");\n";
        }
      else
        {
          *os << "this->" << bd->local_name () << " = "
              << node->name () << "::_duplicate (_tao_excp."
              << bd->local_name () << ".in ());\n";
        }
    } // end else if
  else // simple predefined types
    {
      if (this->ctx_->exception ()) // special ctor
        {
          *os << "this->" << bd->local_name () << " = _tao_"
              << bd->local_name () << ";\n";
        }
      else
        {
          *os << "this->" << bd->local_name () << " = _tao_excp."
              << bd->local_name () << ";\n";
        }
    } // end of else

  return 0;
}

int be_visitor_exception_ctor_assign::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  if (this->ctx_->exception ()) // special ctor
    {
      *os << "this->" << bd->local_name () << " = _tao_" << bd->local_name ()
          << ";\n";
    }
  else
    {
      *os << "this->" << bd->local_name () << " = _tao_excp." << bd->local_name ()
          << ";\n";
    }
  return 0;
}

int be_visitor_exception_ctor_assign::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  if (this->ctx_->exception ()) // special ctor
    {
      *os << "this->" << bd->local_name () << " = CORBA::string_dup (_tao_"
          << bd->local_name () << ");\n";
    }
  else
    {
      *os << "this->" << bd->local_name ()
          << " = CORBA::string_dup (_tao_excp." << bd->local_name ()
          << ".in ());\n";
    }
  return 0;
}

int be_visitor_exception_ctor_assign::visit_structure (be_structure *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  if (this->ctx_->exception ()) // special ctor
    {
      *os << "this->" << bd->local_name () << " = _tao_" << bd->local_name ()
          << ";\n";
    }
  else
    {
      *os << "this->" << bd->local_name () << " = _tao_excp." << bd->local_name ()
          << ";\n";
    }
  return 0;
}

int be_visitor_exception_ctor_assign::visit_union (be_union *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_decl *bd = this->ctx_->node ();

  os->indent ();
  if (this->ctx_->exception ()) // special ctor
    {
      *os << "this->" << bd->local_name () << " = _tao_" << bd->local_name ()
          << ";\n";
    }
  else
    {
      *os << "this->" << bd->local_name () << " = _tao_excp." << bd->local_name ()
          << ";\n";
    }
  return 0;
}

int be_visitor_exception_ctor_assign::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor_assign::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
