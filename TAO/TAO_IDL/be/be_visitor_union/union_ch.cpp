//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Unions in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, union_ch, "$Id$")


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
      *os << "class " << idl_global->export_macro () << " "
          << node->local_name () << ": public TAO_Base_Union " << be_nl
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

      // now check if we need to generate the _default () method
      be_union::DefaultValue dv;
      if (node->default_value (dv) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "computing default value failed\n"),
                            -1);
        }
      if ((dv.computed_ != 0) && (node->default_index () == -1))
        {
          // only if all cases are not covered AND there is no explicit
          // default, we get the _default () method
          os->indent ();
          *os << "void _default ();\n";
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
      *os << "} u_; // end of union" << be_nl;

      // the reset method (TAO extension)
      *os << "// TAO extensions" << be_nl;
      *os << "void _reset (" << bt->nested_type_name (node)
          << ", CORBA::Boolean);" << be_nl;
      *os << "// Frees any allocated storage" << be_nl << be_nl;
      // the virtual overloaded _discriminant method
      *os << "virtual void *_discriminant (void);" << be_nl;
      *os << "// returns pointer to the discriminant" << be_nl << be_nl;
      // the overloaded virtual reset method
      *os << "virtual void _reset (void);" << be_nl;
      *os << "// calls the above reset with finalize=1" << be_nl << be_nl;
      // the _access method
      *os << "virtual void *_access (CORBA::Boolean flag);" << be_nl;
      *os << "// accesses the right data member. "
          << "Also will allocate on TRUE flag" << be_nl << be_uidt_nl;
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
