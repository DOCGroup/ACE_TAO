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

ACE_RCSID (be_visitor_structure, 
           structure_cs, 
           "$Id$")

// ***************************************************************************
// For client stubs.
// ***************************************************************************

be_visitor_structure_cs::be_visitor_structure_cs (be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_cs::~be_visitor_structure_cs (void)
{
}

int 
be_visitor_structure_cs::visit_structure (be_structure *node)
{
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      be_visitor_typecode_defn visitor (&ctx);

      if (visitor.visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_cs::"
                             "visit_structure - "
                             "TypeCode definition failed\n"), 
                            -1);
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl;
  *os << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  if (be_global->any_support ())
    {
      *os << "void "
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *tmp = ACE_static_cast ("
          << node->local_name () << "*, _tao_void_pointer);" << be_nl
          << "delete tmp;" << be_uidt_nl
          << "}";
    }

  // Do any code generation required for the scope members
  // all we have to do is to visit the scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_cs::"
                         "visit_structure - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  node->cli_stub_gen (I_TRUE);
  return 0;
}
