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

ACE_RCSID (be_visitor_structure, 
           structure_ch, 
           "$Id$")

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

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the _var and _out typedefs.
  node->gen_common_varout (os);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl
      << "struct " << be_global->stub_export_macro () << " "
      << node->local_name () << be_nl
      << "{" << be_idt_nl;

  // Generate the _ptr_type and _var_type typedefs.
  *os << "typedef " << node->local_name () << "_var _var_type;"
      << be_nl << be_nl;

  if (be_global->any_support ())
    {
      *os << "static void _tao_any_destructor (void *);";
    }

  // Generate code for field members.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_ch::"
                         "visit_structure - "
                         "codegen for scope failed\n"),
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
                             "(%N:%l) be_visitor_structure_ch::"
                             "visit_structure - "
                             "TypeCode declaration failed\n"),
                            -1);
        }
    }

  node->cli_hdr_gen (I_TRUE);
  return 0;
}
