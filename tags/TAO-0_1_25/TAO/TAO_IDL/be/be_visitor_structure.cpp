//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_structure.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Structure
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_structure.h"

// generic struct visitor
be_visitor_structure::be_visitor_structure (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_structure::~be_visitor_structure (void)
{
}

// visit the Structure node and its scope
int
be_visitor_structure::visit_structure (be_structure *node)
{
  return -1; // must be overriden
}

int
be_visitor_structure::visit_field (be_field *node)
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
    case TAO_CodeGen::TAO_STRUCT_CH:
      ctx.state (TAO_CodeGen::TAO_FIELD_CH);
      break;
    case TAO_CodeGen::TAO_STRUCT_CI:
      ctx.state (TAO_CodeGen::TAO_FIELD_CI);
      break;
    case TAO_CodeGen::TAO_STRUCT_CS:
      ctx.state (TAO_CodeGen::TAO_FIELD_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_structure::"
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
                         "(%N:%l) be_visitor_structure::"
                         "visit_field - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::"
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

be_visitor_structure_ch::be_visitor_structure_ch (be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_ch::~be_visitor_structure_ch (void)
{
}

// visit the Structure node and its scope
int be_visitor_structure_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_hdr_gen () && !node->imported ()) // not already generated and
                                                   // not imported
    {
      os = this->ctx_->stream ();

      os->indent (); // start from whatever indentation level we were at
      *os << "struct " << node->local_name () << be_nl
          << "{" << be_idt << "\n";

      // generate code for field members
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ch::"
                             "visit_structure - "
                             "codegen for scope failed\n"), -1);
        }

      os->decr_indent ();
      *os << "};\n\n";

      // generate var defn
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ch::"
                             "visit_structure - "
                             "codegen for _var failed\n"), -1);
        }

      // a class is generated for an out defn only for a variable length struct
      if (node->size_type () == be_decl::VARIABLE)
        {
          if (node->gen_out_defn () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_structure_ch::"
                                 "visit_structure - "
                                 "codegen for _out failed\n"), -1);
            }
        }
      else
        {
          os->indent ();
          *os << "typedef " << node->local_name () << " &" << node->local_name
            () << "_out;\n\n";
        }

      // generate the typecode decl
      if (node->is_nested ())
        {
          // we have a scoped name
          os->indent ();
          *os << "static CORBA::TypeCode_ptr " << node->tc_name
            ()->last_component () << ";\n\n";
        }
      else
        {
          // we are in the ROOT scope
          os->indent ();
          *os << "extern CORBA::TypeCode_ptr " << node->tc_name
            ()->last_component () << ";\n\n";
        }
      node->cli_hdr_gen (I_TRUE);
    }
  return 0;
}

// ******************************************************
// for client inline
// ******************************************************

be_visitor_structure_ci::be_visitor_structure_ci (be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_ci::~be_visitor_structure_ci (void)
{
}

// visit the Structure node and its scope
int be_visitor_structure_ci::visit_structure (be_structure *node)
{
  if (!node->cli_inline_gen () && !node->imported ())
    {
      if (node->gen_var_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ci::"
                             "visit_structure - "
                             "codegen for _var failed\n"), -1);
        }
      if (node->size_type () == be_decl::VARIABLE &&
          node->gen_out_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ci::"
                             "visit_structure - "
                             "codegen for _out failed\n"), -1);
        }
      // all we have to do is to visit the scope
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ci::"
                             "visit_structure - "
                             "codegen for scope failed\n"), -1);
        }
      node->cli_inline_gen (I_TRUE);
    }
  return 0;
}

// ***************************************************************************
// for client stubs
// ***************************************************************************
be_visitor_structure_cs::be_visitor_structure_cs (be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_cs::~be_visitor_structure_cs (void)
{
}

// visit the Structure_Cs node and its scope
int be_visitor_structure_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_stub_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

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
                             "(%N:%l) be_visitor_structure_cs::"
                             "visit_structure - "
                             "codegen for typecode failed\n"), -1);
        }
      os->decr_indent ();
      *os << "};\n\n";

      os->indent ();
      *os << "static CORBA::TypeCode _tc__tc_" << node->flatname () <<
        " (CORBA::tk_struct, sizeof (_oc_" <<  node->flatname () <<
        "), (char *) &_oc_" << node->flatname () <<
        ", CORBA::B_FALSE);" << be_nl;
      *os << "CORBA::TypeCode_ptr " << node->tc_name () << " = &_tc__tc_" <<
        node->flatname () << ";\n\n";

      // do any code generation required for the scope members
      // all we have to do is to visit the scope
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_cs::"
                             "visit_structure - "
                             "codegen for scope failed\n"), -1);
        }
      node->cli_stub_gen (I_TRUE);
    }

  return 0;
}

// ***************************************************************************
// Structure visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_structure_any_op_ch::be_visitor_structure_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_any_op_ch::~be_visitor_structure_any_op_ch (void)
{
}

int
be_visitor_structure_any_op_ch::visit_structure (be_structure *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_header ();

  // generate the Any <<= and >>= operator declarations
  os->indent ();
  *os << "void operator<<= (CORBA::Any &, const " << node->name ()
      << " &); // copying version" << be_nl;
  *os << "void operator<<= (CORBA::Any &, " << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << "CORBA::Boolean operator>>= (const CORBA::Any &, "
      << node->name () << " *&);\n";


  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::visit_structure - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_structure_any_op_ch::visit_field (be_field *node)
{
  be_type *bt; // field's type

  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_ch::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}

// ***************************************************************************
// Structure visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_structure_any_op_cs::be_visitor_structure_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_any_op_cs::~be_visitor_structure_any_op_cs (void)
{
}

int
be_visitor_structure_any_op_cs::visit_structure (be_structure *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_stubs ();

  // generate the Any <<= and >>= operator declarations
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


  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::visit_structure - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}

int
be_visitor_structure_any_op_cs::visit_field (be_field *node)
{
  be_type *bt; // field's type

  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_cs::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}
