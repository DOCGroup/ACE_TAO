//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_union.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Union
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

// generic struct visitor
be_visitor_union::be_visitor_union (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_union::~be_visitor_union (void)
{
}

// visit the Union node and its scope
int
be_visitor_union::visit_union (be_union *node)
{
  return -1; // must be overriden
}

int
be_visitor_union::visit_union_branch (be_union_branch *node)
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
    case TAO_CodeGen::TAO_UNION_PUBLIC_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CH);
      break;
    case TAO_CodeGen::TAO_UNION_PRIVATE_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_PRIVATE_CH);
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CI);
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_CS);
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_union::"
                           "visit_union_branch - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::"
                         "visit_union_branch - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::"
                         "visit_union_branch - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// ******************************************************
// for client header
// ******************************************************

be_visitor_union_ch::be_visitor_union_ch (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_ch::~be_visitor_union_ch (void)
{
}

// visit the Union node and its scope
int be_visitor_union_ch::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same


  if (!node->cli_hdr_gen () && !node->imported ()) // not already generated and
                                                   // not imported
    {
      os = this->ctx_->stream ();

      // generate the ifdefined macro for the union type
      os->gen_ifdef_macro (node->flatname ());
      os->indent (); // start with the current indentation level
      *os << "class " << node->local_name () << be_nl
          << "{" << be_nl
          << "public:" << be_idt_nl

        // generate default and copy constructors
          << node->local_name () << " (void); // default constructor" << be_nl
          << node->local_name () << " (const " << node->local_name ()
          << " &); // copy constructor" << be_nl
        // generate destructor
          << "~" << node->local_name () << " (void); // destructor" << be_nl
        // generate assignment operator
          << node->local_name () << " &operator= (const "
          << node->local_name () << " &); // copy constructor\n\n";

      // retrieve the disriminant type
      bt = be_type::narrow_from_decl (node->disc_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "bad disciminant type\n"), -1);
        }

      // the discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement.

      ctx.state (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CH); // set current code
                                                          // gen state
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "bad visitor\n"), -1);
        }

      if (bt->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             " visit_union - "
                             "codegen for discriminant failed\n"), -1);
        }
      delete visitor;

      // now generate the public defn for the union branch members. For this,
      // set our state to reflect what we are aiming to do
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CH); // set current code
                                                            // gen state
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "codegen for public defn of union members\n"),
                            -1);
        }

      // now generate the private data members of the union
      os->decr_indent ();
      *os << "private:\n";
      os->incr_indent ();
      *os << bt->nested_type_name (node) << " disc_;" << be_nl; // emit the
                                                 // ACE_NESTED_CLASS macro

      // the members are inside of a union
      *os << "union" << be_nl;
      *os << "{\n";
      os->incr_indent (0);
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PRIVATE_CH); // set current
                                                             // code gen state
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "codegen for private members of union\n"), -1);
        }

      os->decr_indent ();
      *os << "}; // end of union \n";

      os->decr_indent ();
      *os << "}; // " << node->name () << "\n\n";

      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "TypeCode declaration failed\n"
                             ), -1);
        }


      os->gen_endif ();

      // generate the ifdefined macro for the _var type
      os->gen_ifdef_macro (node->flatname (), "_var");
      // generate var defn
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "codegen for _var\n"), -1);
        }
      os->gen_endif ();

      // generate the ifdefined macro for the array type
      os->gen_ifdef_macro (node->flatname (), "_out");
      // a class is generated for an out defn only for a variable length struct
      if (node->size_type () == be_decl::VARIABLE)
        {
          if (node->gen_out_defn () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_union_ch::"
                                 "visit_union - "
                                 "codegen for _out\n"), -1);
            }
        }
      else
        {
          os->indent ();
          *os << "typedef " << node->local_name () << " &" << node->local_name
            () << "_out;\n\n";
        }
      os->gen_endif ();

      node->cli_hdr_gen (I_TRUE);
    }

  return 0;
}

// ******************************************************
// for client inline
// ******************************************************

be_visitor_union_ci::be_visitor_union_ci (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_ci::~be_visitor_union_ci (void)
{
}

// visit the Union node and its scope
int be_visitor_union_ci::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same


  if (!node->cli_inline_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      *os << "// *************************************************************"
          << be_nl;
      *os << "// Inline operations for union " << node->name () << be_nl;
      *os << "// *************************************************************\n\n";

      // generate the default constructor and the destructor here
      os->indent ();
      *os << "// default constructor" << be_nl
          << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name () << " (void)" << be_nl
          << "{" << be_nl
          << "}" << be_nl << be_nl;

      *os << "// destructor" << be_nl
          << "ACE_INLINE" << be_nl
          << node->name () << "::~" << node->local_name () << " (void)" << be_nl
          << "{" << be_nl
          << "}\n\n";

      // the discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement.

      bt = be_type::narrow_from_decl (node->disc_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "bad discriminant type\n"), -1);
        }

      ctx.state (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CI); // set current code
                                                          // gen state
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "bad visitor\n"), -1);
        }
      // generate code for the discriminant
      if (bt->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit union - "
                             "codegen for discrminant failed\n"), -1);
        }

      // now generate the implementation of the access methods for the
      // union. For this set our state
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CI);
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for scope failed\n"), -1);
        }

      // generate the ifdefined macro for the array type
      os->gen_ifdef_macro (node->flatname (), "_var");
      if (node->gen_var_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for _var failed\n"), -1);
        }
      os->gen_endif ();

      // generate the ifdefined macro for the array type then generate the _out
      // impl
      os->gen_ifdef_macro (node->flatname (), "_out");
      if (node->size_type () == be_decl::VARIABLE
          && node->gen_out_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ci::"
                             "visit_union - "
                             "codegen for _out failed\n"), -1);
        }
      os->gen_endif ();

      node->cli_inline_gen (I_TRUE);
    }
  return 0;
}

// ******************************************************
// for client stubs
// ******************************************************

be_visitor_union_cs::be_visitor_union_cs (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_cs::~be_visitor_union_cs (void)
{
}

// visit the Union_cs node and its scope
int be_visitor_union_cs::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt; // for discriminant type

  if (!node->cli_stub_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      be_visitor_context ctx (*this->ctx_);
      // the discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement. We need to generate its
      // typecode

      bt = be_type::narrow_from_decl (node->disc_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs::"
                             "visit_union - "
                             "bad discriminant type\n"), -1);
        }

      ctx.state (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CS); // set current code
                                                          // gen state
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs::"
                             "visit_union - "
                             "bad visitor\n"), -1);
        }
      // generate code for the discriminant
      if (bt->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs::"
                             "visit union - "
                             "codegen for discrminant failed\n"), -1);
        }

      // first generate code for any of the members (if required, e.g.,
      // anonymous sequences, structs, unions, arrays)
      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CS); // set current code
                                                            // gen state
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs"
                             "visit_union - "
                             "codegen for scope failed\n"), -1);
        }

      // now generate the operations on the union such as the copy constructor
      // and the assignment operator

      *os << "// *************************************************************"
          << be_nl;
      *os << "// Operations for union " << node->name () << be_nl;
      *os << "// *************************************************************\n\n";

      this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS);

      // generate the copy constructor and the assignment operator here
      os->indent ();
      *os << "// copy constructor" << be_nl;
      *os << node->name () << "::" << node->local_name () << " (const " <<
        node->name () << " &u)" << be_nl;
      *os << "{\n";
      os->incr_indent ();
      // first set the discriminant
      *os << "this->disc_ = u.disc_;" << be_nl;
      // now switch based on the disc value
      *os << "switch (this->disc_)" << be_nl;
      *os << "{\n";
      os->incr_indent (0);
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs"
                             "visit_union - "
                             "codegen for copy ctor failed\n"), -1);
        }

      os->decr_indent ();
      *os << "}\n";
      os->decr_indent ();
      *os << "}\n\n";

      // assignment operator
      os->indent ();
      *os << "// assignment operator" << be_nl;
      *os << node->name () << " &" << be_nl; // return type
      *os << node->name () << "::operator= (const " <<
        node->name () << " &u)" << be_nl;
      *os << "{\n";
      os->incr_indent ();
      // first set the discriminant
      *os << "this->disc_ = u.disc_;" << be_nl;
      // now switch based on the disc value
      *os << "switch (this->disc_)" << be_nl;
      *os << "{\n";
      os->incr_indent (0);
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs"
                             "visit_union - "
                             "codegen for assign op failed\n"), -1);
        }

      os->decr_indent ();
      *os << "}" << be_nl;
      *os << "return *this;\n";
      os->decr_indent ();
      *os << "}\n\n";

      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_cs::"
                             "visit_union - "
                             "TypeCode definition failed\n"
                             ), -1);
        }


      node->cli_stub_gen (I_TRUE);
    }
  return 0;
}


// *************************************************************************
// be_visitor_discriminant_ch - visitor for discriminant in client header file
// *************************************************************************

be_visitor_union_discriminant_ch::be_visitor_union_discriminant_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_ch::~be_visitor_union_discriminant_ch (void)
{
}

int
be_visitor_union_discriminant_ch::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_union *bu =
    this->ctx_->be_node_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  os->indent ();
  // the set method
  *os << "void _d (" << bt->nested_type_name (bu) << ");" << be_nl;
  // the get method
  *os << bt->nested_type_name (bu) << " _d (void) const;\n\n";
  return 0;
}

int
be_visitor_union_discriminant_ch::visit_predefined_type (be_predefined_type
                                                         *node)
{
  TAO_OutStream *os; // output stream
  be_union *bu =
    this->ctx_->be_node_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os = this->ctx_->stream ();

  os->indent ();
  // the set method
  *os << "void _d (" << bt->nested_type_name (bu) << ");" << be_nl;
  // the get method
  *os << bt->nested_type_name (bu) << " _d (void) const;\n\n";
  return 0;
}

int
be_visitor_union_discriminant_ch::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_discriminant_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// *************************************************************************
// be_visitor_discriminant_ci - visitor for discriminant in client inline file
// *************************************************************************

be_visitor_union_discriminant_ci::be_visitor_union_discriminant_ci
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_ci::~be_visitor_union_discriminant_ci (void)
{
}

int
be_visitor_union_discriminant_ci::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_union *bu =
    this->ctx_->be_node_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os = this->ctx_->stream ();

  os->indent ();
  // the set method
  *os << "// accessor to set the discriminant" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::_d (" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl
      << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl;

  // the get method
  *os << "// accessor to get the discriminant" << be_nl
      << "ACE_INLINE " << bt->name () << be_nl
      << bu->name () << "::_d (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->disc_;" << be_uidt_nl
      << "}\n\n";
  return 0;
}

int
be_visitor_union_discriminant_ci::visit_predefined_type (be_predefined_type
                                                         *node)
{
  TAO_OutStream *os; // output stream
  be_union *bu =
    this->ctx_->be_node_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os = this->ctx_->stream ();

  os->indent ();
  // the set method
  *os << "// accessor to set the discriminant" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::_d (" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl
      << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl;

  // the get method
  *os << "// accessor to get the discriminant" << be_nl
      << "ACE_INLINE " << bt->name () << be_nl
      << bu->name () << "::_d (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->disc_;" << be_uidt_nl
      << "}\n\n";
  return 0;
}

int
be_visitor_union_discriminant_ci::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_discriminant_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }
  this->ctx_->alias (0);

  return 0;
}

// *************************************************************************
// be_visitor_discriminant_cs - visitor for discriminant in client stubs
// *************************************************************************

be_visitor_union_discriminant_cs::be_visitor_union_discriminant_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_cs::~be_visitor_union_discriminant_cs (void)
{
}

int
be_visitor_union_discriminant_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_union *bu =
    this->ctx_->be_node_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // generate the typecode for the enum
      ctx.state (TAO_CodeGen::TAO_ENUM_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_cs::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_cs::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  return 0;
}

// ***************************************************************************
// Union visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_union_any_op_ch::be_visitor_union_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_any_op_ch::~be_visitor_union_any_op_ch (void)
{
}

int
be_visitor_union_any_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_header ();

  // generate the Any <<= and >>= operator declarations
  os->indent ();
  *os << "void " << idl_global->export_macro ()
      << " operator<<= (CORBA::Any &, const " << node->name ()
      << " &); // copying version" << be_nl;
  *os << "void " << idl_global->export_macro ()
      << " operator<<= (CORBA::Any &, " << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator>>= (const CORBA::Any &, "
      << node->name () << " *&);\n";


  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::visit_union - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_union_any_op_ch::visit_union_branch (be_union_branch *node)
{
  be_type *bt; // field's type

  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_union_branch - "
                         "Bad field type\n"
                         ), -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}

// ***************************************************************************
// Union visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_union_any_op_cs::be_visitor_union_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_any_op_cs::~be_visitor_union_any_op_cs (void)
{
}

int
be_visitor_union_any_op_cs::visit_union (be_union *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_stubs ();

  // generate the Any <<= and >>= operator declarations
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, const "
      << node->name () << " &_tao_elem) // copying" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", new "
      << node->name () << "(_tao_elem), 1, _tao_env);" << be_uidt_nl
      << "}" << be_nl;

  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << " *_tao_elem) // non copying" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_elem, 1, _tao_env); // consume it" << be_uidt_nl
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
                         "(%N:%l) be_visitor_union::visit_union - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}

int
be_visitor_union_any_op_cs::visit_union_branch (be_union_branch *node)
{
  be_type *bt; // field's type

  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_cs::"
                         "visit_union_branch - "
                         "Bad field type\n"
                         ), -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_cs::"
                         "visit_union_branch - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}
