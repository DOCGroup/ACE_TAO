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
  TAO_OutStream *os;

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      *os << "class " << node->local_name () << "_var;" << be_nl << be_nl;

      *os << "struct " << be_global->stub_export_macro () << " "
          << node->local_name () << be_nl
          << "{" << be_idt << be_nl;

      // Generate the _ptr_type and _var_type typedefs
      // but we must protect against certain versions of g++
      *os << "\n#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)"
          << be_nl;
      *os << "typedef " << node->local_name () << "_var _var_type;\n"
          << "#endif /* ! __GNUC__ || g++ >= 2.8 */\n" << be_nl;

      if (!node->is_local ())
        {
          *os << "static void _tao_any_destructor (void*);"
              << be_nl << be_nl;
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
      *os << "};" << be_nl << be_nl;

      // Generate var definition.
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ch::"
                             "visit_structure - "
                             "codegen for _var failed\n"),
                            -1);
        }

      // A class is generated for an out defn only for a variable
      // length struct.
      if (node->size_type () == AST_Type::VARIABLE)
        {
          if (node->gen_out_defn () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_structure_ch::"
                                 "visit_structure - "
                                 "codegen for _out failed\n"),
                                -1);
            }
        }
      else
        {
          *os << "typedef " << node->local_name () << " &"
              << node->local_name () << "_out;" << be_nl << be_nl;
        }

      if (be_global->tc_support ())
        {
          be_visitor_context ctx (*this->ctx_);
          ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
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
    }

  return 0;
}
