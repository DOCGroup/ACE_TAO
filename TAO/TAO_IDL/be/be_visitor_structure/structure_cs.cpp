//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    structure_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Structures in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_structure.h"

ACE_RCSID(be_visitor_structure, structure_cs, "$Id$")


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

// visit the structure node and its scope
int be_visitor_structure_cs::visit_structure (be_structure *node)
{
  if (!node->cli_stub_gen () && !node->imported () && !node->is_local ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor *visitor;
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
      ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_cs::"
                             "visit_structure - "
                             "TypeCode definition failed\n"
                             ), -1);
        }

      TAO_OutStream *os = this->ctx_->stream ();
      os->indent ();
      if (!node->is_local ())
        *os << "void "
            << node->name ()
            << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
            << "{" << be_idt_nl
            << node->local_name () << " *tmp = ACE_static_cast ("
            << node->local_name () << "*, _tao_void_pointer);" << be_nl
            << "delete tmp;" << be_uidt_nl
            << "}\n\n";

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
