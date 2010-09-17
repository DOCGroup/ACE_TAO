
//=============================================================================
/**
 *  @file    structure_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Structure in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

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

// Visit the Structure node and its scope.
int be_visitor_structure_ch::visit_structure (be_structure *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // Evaluate the member in time for the decision to generate
  // the recursive typecode include in the stub source file.
  ACE_Unbounded_Queue<AST_Type *> list;
  (void) node->in_recursion (list);

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the _var and _out typedefs.
  node->gen_common_varout (os);

  *os << be_nl << be_nl;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl
      << "struct " << be_global->stub_export_macro () << " "
      << node->local_name () << be_nl
      << "{" << be_idt;

  node->gen_stub_decls (os);

  *os << be_nl;

  // Generate code for field members.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_structure_ch::")
                         ACE_TEXT ("visit_structure - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  *os << be_uidt_nl;
  *os << "};";

  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      be_visitor_typecode_decl visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_structure_ch::")
                             ACE_TEXT ("visit_structure - ")
                             ACE_TEXT ("TypeCode declaration failed\n")),
                            -1);
        }
    }

  node->cli_hdr_gen (true);
  return 0;
}
