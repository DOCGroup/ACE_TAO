
//=============================================================================
/**
 *  @file    structure_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Structures in the client stubs file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


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
      TAO::be_visitor_struct_typecode visitor (&ctx);

      if (visitor.visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_cs::"
                             "visit_structure - "
                             "TypeCode definition failed\n"),
                            -1);
        }
    }

  if (be_global->any_support ())
    {
      TAO_OutStream *os = this->ctx_->stream ();

      *os << be_nl_2;
      *os << "// TAO_IDL - Generated from " << be_nl
          << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

      *os << "void " << be_nl
          << node->name ()
          << "::_tao_any_destructor (" << be_idt << be_idt_nl
          << "void *_tao_void_pointer" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << node->local_name () << " *_tao_tmp_pointer =" << be_idt_nl
          << "static_cast<" << node->local_name ()
          << " *> (_tao_void_pointer);" << be_uidt_nl
          << "delete _tao_tmp_pointer;" << be_uidt_nl
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

  node->cli_stub_gen (true);
  return 0;
}
