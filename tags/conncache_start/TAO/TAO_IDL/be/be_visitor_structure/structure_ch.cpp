//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    structure_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Structure in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_structure.h"

ACE_RCSID(be_visitor_structure, structure_ch, "$Id$")


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
      *os << "struct " << idl_global->export_macro () << " "
          << node->local_name () << be_nl
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

      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor *visitor;
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ch::"
                             "visit_structure - "
                             "TypeCode declaration failed\n"
                             ), -1);
        }


      node->cli_hdr_gen (I_TRUE);
    }
  return 0;
}
