//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_exception.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Exception
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_exception.h"

// generic struct visitor
be_visitor_exception::be_visitor_exception (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_exception::~be_visitor_exception (void)
{
}

// visit the Exception node and its scope
int
be_visitor_exception::visit_exception (be_exception *node)
{
  return -1; // must be overriden
}

int
be_visitor_exception::visit_field (be_field *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_EXCEPTION_CH:
      ctx.state (TAO_CodeGen::TAO_FIELD_CH);
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CI:
      ctx.state (TAO_CodeGen::TAO_FIELD_CI);
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CS:
      ctx.state (TAO_CodeGen::TAO_FIELD_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_exception::"
                           "visit_field - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::"
                         "visit_field - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::"
                         "visit_field - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// ******************************************************
// for client header
// ******************************************************

be_visitor_exception_ch::be_visitor_exception_ch (be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_ch::~be_visitor_exception_ch (void)
{
}

// visit the Exception node and its scope
int be_visitor_exception_ch::visit_exception (be_exception *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_hdr_gen () && !node->imported ()) // not already generated and
                                                   // not imported
    {
      os = this->ctx_->stream ();

      os->gen_ifdef_macro (node->flatname (), "_ptr");

      os->indent ();
      *os << "class " << node->local_name () << ";" << be_nl;
      // generate the _ptr declaration
      *os << "typedef " << node->local_name () << " *"
	  << node->local_name () << "_ptr;" << be_nl;
      os->gen_endif ();

      os->gen_ifdef_macro (node->flatname ());

      os->indent ();
      *os << "class " << idl_global->export_macro ()
	  << " " << node->local_name ()
	  << " : public CORBA::UserException" << be_nl;
      *os << "{" << be_nl
	  << "public:\n";
      os->incr_indent ();
      // constructors and destructor
      *os << node->local_name () << " (void); // default ctor" << be_nl;
      *os << node->local_name () << " (const " << node->local_name ()
          << " &); // copy ctor" << be_nl;
      *os << "~" << node->local_name () << " (void); // dtor" << be_nl;

      // generate constructor that takes each member as a parameter. We need a
      // new state. Such a constructor exists if we have members
      if (node->member_count () > 0)
        {
          be_visitor_context ctx (*this->ctx_);
          ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_CH);
          be_visitor *visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception::"
                                 "visit_exception - "
                                 "codegen for ctor failed\n"), -1);
            }
          delete visitor;
        }

      os->indent ();
      // assignment operator
      *os << node->local_name () << " &operator= (const "
          << node->local_name () << " &);" << be_nl;
      // the static _narrow method
      *os << "static " << node->local_name ()
          << " *_narrow (CORBA::Exception *);\n";

      // generate code for field members
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_ch::"
                             "visit_exception - "
                             "codegen for scope failed\n"), -1);
        }

      os->decr_indent ();
      *os << "};" << be_nl;

      // generate the Any <<= and >>= operators
      os->indent ();
      if (node->is_nested ())
        *os << "friend ";
      *os << "void operator<<= (CORBA::Any &, const " << node->local_name ()
          << " &); // copying version" << be_nl;
      if (node->is_nested ())
        *os << "friend ";
      *os << "void operator<<= (CORBA::Any &, " << node->local_name ()
          << "*); // noncopying version" << be_nl;
      if (node->is_nested ())
        *os << "friend ";
      *os << "CORBA::Boolean operator>>= (const CORBA::Any &, "
          << node->local_name () << " *&);\n";

      // generate the typecode decl
      if (node->is_nested ())
        {
          // we have a scoped name
          *os << "static CORBA::TypeCode_ptr "
              << node->tc_name ()->last_component () << ";\n\n";
        }
      else
        {
          // we are in the ROOT scope
          *os << "extern "
	      << idl_global->export_macro ()
	      << " CORBA::TypeCode_ptr "
	      << node->tc_name ()->last_component () << ";\n\n";
        }
      os->gen_endif ();

      node->cli_hdr_gen (1);

    }
  return 0;
}

// ******************************************************
// for client inline
// ******************************************************

be_visitor_exception_ci::be_visitor_exception_ci (be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_ci::~be_visitor_exception_ci (void)
{
}

// visit the Exception node and its scope
int be_visitor_exception_ci::visit_exception (be_exception *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  if (!node->cli_inline_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();
      os->indent ();
      *os << "// *************************************************************"
          << be_nl;
      *os << "// Inline operations for exception " << node->name () << be_nl;
      *os << "// *************************************************************\n\n";

      // generate inline code  required of any anonymous types of members
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception::"
                             "visit_exception -"
                             "code for inline failed\n"), -1);
        }

      node->cli_inline_gen (I_TRUE);
    }
  return 0;
}

// ***************************************************************************
// for client stubs
// ***************************************************************************
be_visitor_exception_cs::be_visitor_exception_cs (be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_cs::~be_visitor_exception_cs (void)
{
}

// visit the Exception_Cs node and its scope
int be_visitor_exception_cs::visit_exception (be_exception *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_stub_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      // default constructor
      os->indent ();
      *os << "// default constructor" << be_nl;
      *os << node->name () << "::" << node->local_name () << " (void)" << be_nl;
      *os << "  : CORBA_UserException (CORBA::TypeCode::_duplicate ("
          << node->tc_name () << "))\n";
      *os << "{" << be_nl;
      *os << "}\n\n";

      // destructor
      os->indent ();
      *os << "// destructor - all members are of self managing types" << be_nl;
      *os << node->name () << "::~" << node->local_name () << " (void)" << be_nl;
      *os << "{" << be_nl;
      *os << "}\n\n";

      // copy constructor
      os->indent ();
      *os << "// copy constructor" << be_nl;
      *os << node->name () << "::" << node->local_name () << " (const " <<
        node->name () << " &_tao_excp)" << be_nl;
      *os << "  :CORBA_UserException (" <<
        "CORBA::TypeCode::_duplicate (_tao_excp._type ()))" << be_nl;
      *os << "{\n";
      os->incr_indent ();
      // assign each individual member
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cs::"
                             "visit_exception -"
                             "codegen for scope failed\n"), -1);
        }
      delete visitor;
      os->decr_indent ();
      *os << "}\n\n";

      // assignment operator
      os->indent ();
      *os << "// assignment operator" << be_nl;
      *os << node->name () << "&" << be_nl;
      *os << node->name () << "::operator= (const " <<
        node->name () << " &_tao_excp)" << be_nl;
      *os << "{\n";
      os->incr_indent ();
      *os << "this->type_ = " <<
        "CORBA::TypeCode::_duplicate (_tao_excp._type ());\n";
      // assign each individual member
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cs::"
                             "visit_exception -"
                             "codegen for scope failed\n"), -1);
        }
      delete visitor;
      os->indent ();
      *os << "return *this;\n";
      os->decr_indent ();
      *os << "}\n\n";

      // constructor taking all members. It exists only if there are any
      // members
      if (node->member_count () > 0)
        {
          // generate the signature
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_CS);
          visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception::"
                                 "visit_exception - "
                                 "codegen for ctor failed\n"), -1);
            }
          delete visitor;

          *os << "  : CORBA_UserException "
              << " (CORBA::TypeCode::_duplicate (" << node->tc_name ()
              << "))" << be_nl;
          *os << "{\n";
          os->incr_indent ();
          // assign each individual member. We need yet another state
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);
          ctx.exception (1); // indicate that the special ctor is being
                             // generated
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception_cs::"
                                 "visit_exception -"
                                 "codegen for scope failed\n"), -1);
            }
          delete visitor;
          os->decr_indent ();
          *os << "}\n\n";
        }

      // narrow method
      os->indent ();
      *os << "// narrow" << be_nl;
      *os << node->name () << "_ptr " << be_nl;
      *os << node->name () << "::_narrow (CORBA::Exception *exc)" << be_nl;
      *os << "{\n";
      os->incr_indent ();
      *os << "if (!ACE_OS::strcmp (\"" << node->repoID () <<
        "\", exc->_id ())) // same type" << be_nl;
      *os << "  return ACE_dynamic_cast (" << node->name () << "_ptr, exc);" <<
        be_nl;
      *os << "else" << be_nl;
      *os << "  return 0;\n";
      os->decr_indent ();
      *os << "}\n\n";

      // Any <<= and >>= operators
      os->indent ();
      *os << "void operator<<= (CORBA::Any &_tao_any, const "
          << node->name () << " &_tao_elem) // copying" << be_nl
          << "{" << be_idt_nl
          << "CORBA::Environment _tao_env;" << be_nl
          << "_tao_any.replace (" << node->tc_name () << ", &"
          << "_tao_elem, 1, _tao_env);" << be_uidt_nl
          << "}" << be_nl;

      *os << "void operator<<= (CORBA::Any &_tao_any, "
          << node->name () << " *_tao_elem) // non copying" << be_nl
          << "{" << be_idt_nl
          << "CORBA::Environment _tao_env;" << be_nl
          << "_tao_any.replace (" << node->tc_name () << ", "
          << "_tao_elem, 0, _tao_env);" << be_uidt_nl
          << "}" << be_nl;

      *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
          << node->name () << " *&_tao_elem)" << be_nl
          << "{" << be_idt_nl
          << "CORBA::Environment _tao_env;" << be_nl
          << "if (!_tao_any.type ()->equal (" << node->tc_name ()
          << ", _tao_env)) return 0; // not equal" << be_nl
          << "if (_tao_any.any_owns_data ())" << be_nl
          << "{" << be_idt_nl
          << "ACE_NEW_RETURN (_tao_elem, " << node->name () << ", 0);"
          << be_nl
          << "TAO_InputCDR stream ((ACE_Message_Block *)_tao_any.value ());"
          << be_nl
          << "if (stream.decode (" << node->tc_name ()
          << ", _tao_elem, 0, _tao_env)" << be_nl
          << "  == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
          << "{" << be_idt_nl
          << "((CORBA::Any *)&_tao_any)->replace (_tao_any.type (), "
          << "_tao_elem, 1, _tao_env);"
          << be_nl
          << "  return 1;" << be_uidt_nl
          << "}" << be_nl
          << "else" << be_nl  // decode failed
          << "{" << be_idt_nl
          << "delete _tao_elem;" << be_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}" << be_nl
          << "else" << be_nl  // else any does not own the data
          << "{" << be_idt_nl
          << "_tao_elem = (" << node->name () << " *)_tao_any.value ();"
          << be_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}\n\n";

      os->indent ();
      // generate the typecode information here
      os->indent (); // start from current indentation level
      *os << "static const CORBA::Long _oc_" << node->flatname () << "[] =" <<
        be_nl;
      *os << "{\n";
      os->incr_indent (0);
      // note that we just need the parameters here and hence we generate the
      // encapsulation for the parameters
      if (node->gen_encapsulation () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_structure::gen_client_stubs -"
                             "codegen for scope failed\n"), -1);
        }
      os->decr_indent ();
      *os << "};" << be_nl;

      *os << "static CORBA::TypeCode _tc__tc_" << node->flatname () <<
        " (CORBA::tk_except, sizeof (_oc_" <<  node->flatname () <<
        "), (char *) &_oc_" << node->flatname () <<
        ", CORBA::B_FALSE);" << be_nl;
      *os << "CORBA::TypeCode_ptr " << node->tc_name () << " = &_tc__tc_" <<
        node->flatname () << ";\n\n";

      node->cli_stub_gen (I_TRUE);
    }

  return 0;
}

// ************************************************************************
//  code to generate the special ctor
// ************************************************************************

be_visitor_exception_ctor::be_visitor_exception_ctor
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_exception_ctor::~be_visitor_exception_ctor (void)
{
}

int
be_visitor_exception_ctor::post_process (void)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << ",\n";
  return 0;
}

int be_visitor_exception_ctor::visit_exception (be_exception *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << node->local_name ();
  else
    *os << node->name () << "::" << node->local_name ();
  *os << "(" << be_idt << "\n";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor_assign::"
                         "visit_exception - "
                         "codegen for scope failed\n"),
                        -1);
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << be_uidt << ");\n\n";
  else
    *os << be_uidt << ")\n\n";

  return 0;
}

int be_visitor_exception_ctor::visit_field (be_field *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor::"
                         "visit_exception - "
                         "Bad field type\n"),
                        -1);
    }

  os->indent ();
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor::"
                         "visit_field - "
                         "cannot accept visitor\n"),
                        -1);
    }

  *os << " _tao_" << node->local_name ();
  return 0;
}

int be_visitor_exception_ctor::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // set the right type;
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << "const " << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << "const " << bt->name ();
  return 0;
}

int be_visitor_exception_ctor::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // set the right type;
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();
  return 0;
}

int be_visitor_exception_ctor::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // set the right type;
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << "const " << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << "const " << bt->name ();
  *os << "_ptr ";
  return 0;
}

int be_visitor_exception_ctor::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // set the right type;
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << "const " << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << "const " << bt->name ();
  *os << "_ptr ";
  return 0;
}

int be_visitor_exception_ctor::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // set the right type;
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
        *os << "const " << bt->nested_type_name (this->ctx_->scope ());
      else
        *os << "const " << bt->name ();
      *os << " &";
      return 0;
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
        *os << "const " << bt->nested_type_name (this->ctx_->scope ());
      else
        *os << "const " << bt->name ();
      *os << "_ptr ";
    } // end else if
  else // simple predefined types
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
        *os << bt->nested_type_name (this->ctx_->scope ());
      else
        *os << bt->name ();
    } // end of else

  return 0;
}

int be_visitor_exception_ctor::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // set the right type;
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << "const " << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << "const " << bt->name ();
  *os << " &";
  return 0;
}

int be_visitor_exception_ctor::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "const char *";
  return 0;
}

int be_visitor_exception_ctor::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // set the right type;
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << "const " << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << "const " << bt->name ();
  *os << " &";
  return 0;
}

int be_visitor_exception_ctor::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // set the right type;
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    *os << "const " << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << "const " << bt->name ();
  *os << " &";
  return 0;
}

int be_visitor_exception_ctor::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

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
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
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
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
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
  return 0;
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
