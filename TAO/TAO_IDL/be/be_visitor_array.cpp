//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_array.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Array
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_array.h"

// ************************************************************************
//  visitor for array declaration in client header
// ************************************************************************

be_visitor_array_ch::be_visitor_array_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_ch::~be_visitor_array_ch (void)
{
}

int be_visitor_array_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_type *bt;  // base type
  be_decl *scope = this->ctx_->scope (); // scope in which it is used

  // nothing to do if we are imported or code is already generated
  if (node->imported () || (node->cli_hdr_gen ()))
    return 0;

  this->ctx_->node (node); // save the array node

  // retrieve the type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  // generate the ifdefined macro
  os->gen_ifdef_macro (node->flatname ());

  os->indent ();
  if (this->ctx_->tdef ())
    {
      // this is a typedef to an array node
      *os << "typedef " << bt->nested_type_name (scope) << " "
          << node->local_name ();
      if (node->gen_dimensions (os) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_ch::"
                             "visit_array - "
                             "gen dimensions failed\n"),
                            -1);
        }
      *os << ";" << be_nl;
      *os << "typedef " << bt->nested_type_name (scope) << " "
          << node->local_name () << "_slice";
      if (node->gen_dimensions (os, 1) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_ch::"
                             "visit_array - "
                             "gen slice dimensions failed\n"),
                            -1);
        }
      *os << ";" << be_nl;
    }
  else
    {
    }
  // typedef the _var, _out, and _forany types
  if (node->gen_var_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_argument - "
                         "var_defn failed\n"),
                        -1);
    }
  if (node->gen_out_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_argument - "
                         "out_defn failed\n"),
                        -1);
    }
  if (node->gen_forany_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_argument - "
                         "forany_defn failed\n"),
                        -1);
    }
  // the _alloc, _dup, copy, and free methods. If the node is nested, the
  // methods become static
  *os << "static " << node->nested_type_name (scope, "_slice") << " *"
      << node->nested_type_name (scope, "_alloc") << " (void);" << be_nl;
  *os << "static " << node->nested_type_name (scope, "_slice") << " *"
      << node->nested_type_name (scope, "_dup") << " (const "
      << node->nested_type_name (scope, "_slice") << " *_tao_slice);" << be_nl;
  *os << "static void " << node->nested_type_name (scope, "_copy") << " ("
      << node->nested_type_name (scope, "_slice") << " *_tao_to, const "
      << node->nested_type_name (scope, "_slice") << " *_tao_from);" << be_nl;
  *os << "static void " << node->nested_type_name (scope, "_free") << " ("
      << node->nested_type_name (scope, "_slice") << " *_tao_slice);" << be_nl;

  // is this a typedefined array? if so, then let the typedef deal with
  // generation of the typecode
  if (!this->ctx_->tdef ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor_context ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "TypeCode declaration failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // generate the endif macro
  os->gen_endif ();

  node->cli_hdr_gen (1);
  return 0;
}

// ************************************************************************
//  visitor for array declaration in client inline
// ************************************************************************

be_visitor_array_ci::be_visitor_array_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_ci::~be_visitor_array_ci (void)
{
}

int be_visitor_array_ci::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_type *bt;  // base type
  be_decl *scope = this->ctx_->scope (); // scope in which it is used

  // nothing to do if we are imported or code is already generated
  if (node->imported () || (node->cli_inline_gen ()))
    return 0;

  this->ctx_->node (node); // save the array node

  // retrieve the type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  // alloc method
  os->indent (); // start from current indentation
  *os << "ACE_INLINE " << node->name () << "_slice *" << be_nl;
  *os << node->name () << "_alloc (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return new " << bt->name ();
  if (node->gen_dimensions (os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "gen_client_inline - "
                         "dimensions codegen failed\n"),
                        -1);
    }

  *os << ";" << be_uidt_nl;
  *os << "}\n\n";

  // free method
  os->indent ();
  *os << "ACE_INLINE void" << be_nl
      << node->name () << "_free (" << node->name ()
      << "_slice *_tao_slice)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "delete [] _tao_slice;" << be_uidt_nl;
  *os << "}\n\n";

  // generate code for the _var, _out, and _forany types
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "var_defn failed\n"),
                        -1);
    }
  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "out_defn failed\n"),
                        -1);
    }
  if (node->gen_forany_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "forany_defn failed\n"),
                        -1);
    }

  // is this a typedefined array? if so, then let the typedef deal with
  // generation of the typecode
  if (!this->ctx_->tdef ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor_context ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cs::"
                             "visit_array - "
                             "TypeCode definition failed\n"
                             ), -1);
        }
      delete visitor;
    }

  node->cli_inline_gen (1);
  return 0;
}

// ************************************************************************
//  visitor for array declaration in client stubs
// ************************************************************************

be_visitor_array_cs::be_visitor_array_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_cs::~be_visitor_array_cs (void)
{
}

int be_visitor_array_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_type *bt;  // base type
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  unsigned long i;

  // nothing to do if we are imported or code is already generated
  if (node->imported () || (node->cli_stub_gen ()))
    return 0;

  this->ctx_->node (node); // save the array node

  // retrieve the type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  // dup method
  os->indent ();
  *os << node->name () << "_slice *" << be_nl
      << node->name () << "_dup (const " << node->name ()
      << "_slice *_tao_src_array)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << node->name () << "_slice *_tao_dup_array = " << node->name ()
      << "_alloc ();" << be_nl;
  *os << "if (!_tao_dup_array) return (" << node->name ()
      << "_slice *)0;" << be_nl;
  *os << node->name () << "_copy (_tao_dup_array, _tao_src_array);" << be_nl;
  *os << "return _tao_dup_array;" << be_uidt_nl;
  *os << "}\n\n";

  // copy method
  *os << "void " << be_nl;
  *os << node->name () << "_copy (" << node->name () << "_slice * _tao_to, "
      << "const " << node->name () << "_slice *_tao_from)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "// copy each individual elements" << be_nl;

  // generate nested loops for as many dimensions as there are
  for (i = 0; i < node->n_dims (); i++)
    {
      // retrieve the ith dimension value
      AST_Expression *expr = node->dims ()[i];
      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cs::"
                             "visit_array - "
                             "bad array dimension\n"),
                            -1);
        }
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          // generate a loop for each dimension
          *os << "for (CORBA::ULong i" << i << " = 0; i" << i << " < "
              << expr->ev ()->u.ulval << "; i" << i << "++)" << be_idt_nl;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cs::"
                             "visit_array - "
                             "bad array dimension value\n"),
                            -1);
        }
    }

  // now generate code such that every element of the array gets assigned
  // inside the innermost level of the  nested loops generated above
  be_array *primitive_type = 0;
  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      // base type of the array node is a typedef. We need to make sure that
      // this typedef is not to another array type. If it is, then we cannot
      // assign an array to another. We will have to invoke the underlying
      // array type's copy method for every array dimension.
      be_typedef *tdef = be_typedef::narrow_from_decl (bt);
      // check if the base type of the typedef node is an Array node
      primitive_type = be_array::narrow_from_decl (tdef->base_type ());
    }
  if (primitive_type)
    {
      // the base type is not a typedef to a possibly another array type. In
      // such a case, assign each element.

      *os << "// call the underlying _copy" << be_nl;
      *os << bt->name () << "_copy (_tao_to";
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      *os << ", ";
      *os << "_tao_from";  // generate the rvalue
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      *os << ");";
    }
  else
    {
      // the base type is not a typedef to a possibly another array type. In
      // such a case, assign each element.

      *os << "_tao_to"; // generate the lvalue
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      *os << " = ";
      *os << "_tao_from";  // generate the rvalue
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      *os << ";";
    }
  for (i = 0; i < node->n_dims (); i++)
    {
      // decrement indentation as many times as the number of dimensions
      *os << be_uidt;
    }
  *os << be_uidt_nl << "}\n\n";

  node->cli_stub_gen (1);

  return 0;
}

// ***************************************************************************
// Array visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_array_any_op_ch::be_visitor_array_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_any_op_ch::~be_visitor_array_any_op_ch (void)
{
}

int
be_visitor_array_any_op_ch::visit_array (be_array *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_header ();

  // generate the Any <<= and >>= operator declarations
  os->indent ();
  *os << "void " << idl_global->export_macro ()
      << " operator<<= (CORBA::Any &, const " << node->name ()
      << "_forany &);" << be_nl;
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator>>= (const CORBA::Any &, "
      << node->name () << "_forany &);\n";

  node->cli_hdr_any_op_gen (1);
  return 0;
}

// ***************************************************************************
// Array visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_array_any_op_cs::be_visitor_array_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_any_op_cs::~be_visitor_array_any_op_cs (void)
{
}

int
be_visitor_array_any_op_cs::visit_array (be_array *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_stubs ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, const "
      << node->name () << "_forany &_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "if (_tao_elem.nocopy ()) // no copy" << be_idt_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_elem.ptr (), 1, _tao_env); // consume it" << be_uidt_nl
      << "else // copy" << be_idt_nl
      << "_tao_any.replace (" << node->tc_name () << ", " << node->name ()
      << "_dup (_tao_elem.ptr ()), 1, _tao_env);" << be_uidt_nl
      << be_uidt_nl << "}" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << "_forany &_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "if (!_tao_any.type ()->equal (" << node->tc_name ()
      << ", _tao_env)) return 0; // not equal" << be_nl
      << "if (_tao_any.any_owns_data ())" << be_nl
      << "{" << be_idt_nl
      << node->name () << "_slice *&_tao_elem_ptr = _tao_elem.out ();" << be_nl
      << "_tao_elem_ptr = " << node->name () << "_alloc ();" << be_nl
      << "if (!_tao_elem_ptr) return 0;" << be_nl
      << "TAO_InputCDR stream ((ACE_Message_Block *)_tao_any.value ());"
      << be_nl
      << "if (stream.decode (" << node->tc_name ()
      << ", _tao_elem_ptr, 0, _tao_env)" << be_nl
      << "  == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl
      << "((CORBA::Any *)&_tao_any)->replace (_tao_any.type (), "
      << "_tao_elem_ptr, 1, _tao_env);"
      << be_nl
      << "  return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // decode failed
      << "{" << be_idt_nl
      << node->name () << "_free (_tao_elem_ptr);" << be_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // else Any does not own the data
      << "{" << be_idt_nl
      << "_tao_elem = (" << node->name () << "_slice *)_tao_any.value ();"
      << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}\n\n";

  node->cli_stub_any_op_gen (1);
  return 0;
}
