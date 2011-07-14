//=============================================================================
/**
 *  @file    array_ch.cpp
 *
 *  $Id$

 *
 *  Visitor for Array code generation in client header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ************************************************************************
//  visitor for array declaration in client header
// ************************************************************************

be_visitor_array_ch::be_visitor_array_ch (be_visitor_context *ctx)
  : be_visitor_array (ctx)
{
}

be_visitor_array_ch::~be_visitor_array_ch (void)
{
}

int be_visitor_array_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_decl *scope = this->ctx_->scope ()->decl ();

  // Nothing to do if we are imported or code is already generated.
  if (node->imported () || node->cli_hdr_gen ())
    {
      return 0;
    }

  this->ctx_->node (node);

  // Retrieve the type.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_ch::"
                         "visit_array - "
                         "bad base type\n"),
                        -1);
    }

  AST_Decl::NodeType nt = bt->node_type ();

  *os << be_nl_2 << "// TAO_IDL - Generated from " << be_nl
               << "// " __FILE__ << ":" << __LINE__;

  // If we contain an anonymous sequence,
  // generate code for the sequence here.
  if (nt == AST_Decl::NT_sequence)
    {
      if (this->gen_anonymous_base_type (bt,
                                         TAO_CodeGen::TAO_ROOT_CH)
          == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  // If the array is an anonymous member and if its element type
  // is a declaration (not a reference), we must generate code for
  // the declaration.
  if (this->ctx_->alias () == 0 // Not a typedef.
      && bt->is_child (this->ctx_->scope ()->decl ()))
    {
      int status = 0;
      be_visitor_context ctx (*this->ctx_);

      switch (nt)
      {
        case AST_Decl::NT_enum:
          {
            be_visitor_enum_ch ec_visitor (&ctx);
            status = bt->accept (&ec_visitor);
            break;
          }
        case AST_Decl::NT_struct:
          {
            be_visitor_structure_ch sc_visitor (&ctx);
            status = bt->accept (&sc_visitor);
            break;
          }
        case AST_Decl::NT_union:
          {
            be_visitor_union_ch uc_visitor (&ctx);
            status = bt->accept (&uc_visitor);
            break;
          }
        default:
          break;
      }

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "array base type codegen failed\n"),
                            -1);
        }
    }

  *os << be_nl_2
      << "typedef ";

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "base type decl failed\n"),
                        -1);
    }

  *os << " ";

  be_typedef *td = this->ctx_->tdef ();

  if (td == 0)
    {
      // We are dealing with an anonymous array case. Generate a typedef with
      // an _ prepended to the name.
      *os << "_";
    }

  *os << node->local_name ();

  if (node->gen_dimensions (os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "gen dimensions failed\n"),
                        -1);
    }

  *os << ";" << be_nl;

  // Now define the slice type and other required operations
  *os << "typedef ";

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "base type decl failed\n"),
                        -1);
    }

  *os << " ";

  char anon_p [2];
  ACE_OS::memset (anon_p,
                  '\0',
                  2);

  if (this->ctx_->tdef () != 0)
    {
      anon_p[0] = '\0';
    }
  else
    {
      ACE_OS::sprintf (anon_p,
                       "_");
    }

  *os << anon_p << node->local_name () << "_slice";

  if (node->gen_dimensions (os, 1) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_array_ch::")
                         ACE_TEXT ("visit_array - ")
                         ACE_TEXT ("gen slice dimensions failed\n")),
                        -1);
    }

  *os << ";";

  *os << be_nl
      << "struct " << anon_p << node->nested_type_name (scope, "_tag")
      << " {};" << be_nl;

  // No _var or _out class for an anonymous (non-typedef'd) array.
  if (td != 0)
    {
      // Generate _var class decl.
      // An _out decl is generated only for a variable size array.
      if (node->size_type () == AST_Type::VARIABLE)
        {
          *os << be_nl_2
              << "typedef" << be_idt_nl
              << "TAO_VarArray_Var_T<" << be_idt << be_idt_nl
              << node->local_name () << "," << be_nl
              << node->local_name () << "_slice," << be_nl
              << node->local_name () << "_tag" << be_uidt_nl
              << ">" << be_uidt_nl
              << node->local_name () << "_var;" << be_uidt;

          *os << be_nl_2
              << "typedef" << be_idt_nl
              << "TAO_Array_Out_T<" << be_idt << be_idt_nl
              << node->local_name () << "," << be_nl
              << node->local_name () << "_var," << be_nl
              << node->local_name () << "_slice," << be_nl
              << node->local_name () << "_tag" << be_uidt_nl
              << ">" << be_uidt_nl
              << node->local_name () << "_out;" << be_uidt;
        }
      else
        {
          *os << be_nl_2
              << "typedef" << be_idt_nl
              << "TAO_FixedArray_Var_T<" << be_idt << be_idt_nl
              << node->local_name () << "," << be_nl
              << node->local_name () << "_slice," << be_nl
              << node->local_name () << "_tag" << be_uidt_nl
              << ">" << be_uidt_nl
              << node->local_name () << "_var;" << be_uidt;

          *os << be_nl_2
              << "typedef" << be_idt_nl << node->local_name () << be_nl
              << node->local_name () << "_out;" << be_uidt;
        }
    }

  // Generate _forany decl.
  *os << be_nl_2
      << "typedef" << be_idt_nl
      << "TAO_Array_Forany_T<" << be_idt << be_idt_nl
      << anon_p << node->local_name () << "," << be_nl
      << anon_p << node->local_name () << "_slice," << be_nl
      << anon_p << node->local_name () << "_tag" << be_uidt_nl
      << ">" << be_uidt_nl
      << anon_p << node->local_name () << "_forany;" << be_uidt;

  *os << be_nl_2;

  // The _alloc, _dup, copy, and free methods. If the node is nested, the
  // methods become static
  const char *storage_class = 0;

  if (node->is_nested ())
    {
      if (scope->node_type () != AST_Decl::NT_module)
        {
          storage_class = "static ";
        }
      else
        {
          storage_class = "TAO_NAMESPACE_STORAGE_CLASS ";
        }
    }
  else
    {
      storage_class = "TAO_EXPORT_MACRO ";
    }

  if (td != 0)
    {
      // Typedefed array.
      *os << storage_class << node->nested_type_name (scope, "_slice")
          << " *" << be_nl;
      *os << node->nested_type_name (scope, "_alloc") << " (void);"
          << be_nl_2;
      *os << storage_class << "void" << be_nl
          << node->nested_type_name (scope, "_free")
          << " (" << be_idt << be_idt_nl;
      *os << node->nested_type_name (scope, "_slice")
          << " *_tao_slice);" << be_uidt
          << be_uidt_nl << be_nl;
      *os << storage_class << node->nested_type_name (scope, "_slice")
          << " *" << be_nl;
      *os << node->nested_type_name (scope, "_dup")
          << " (" << be_idt << be_idt_nl
          << "const ";
      *os << node->nested_type_name (scope, "_slice")
          << " *_tao_slice);" << be_uidt
          << be_uidt_nl << be_nl;
      *os << storage_class << "void" << be_nl
          << node->nested_type_name (scope, "_copy")
          << " (" << be_idt << be_idt_nl;
      *os << node->nested_type_name (scope, "_slice") << " *_tao_to," << be_nl
          << "const ";
      *os << node->nested_type_name (scope, "_slice")
          << " *_tao_from);" << be_uidt
          << be_uidt;
    }
  else
    {
      // Anonymous array.
      *os << storage_class << node->nested_type_name (scope, "_slice", "_")
          << " *" << be_nl;
      *os << node->nested_type_name (scope, "_alloc", "_")
          << " (void);" << be_nl_2;
      *os << storage_class << "void" << be_nl
          << node->nested_type_name (scope, "_free", "_")
          << " (" << be_idt << be_idt_nl;
      *os << node->nested_type_name (scope, "_slice", "_")
          << " *_tao_slice);" << be_uidt
          << be_uidt_nl << be_nl;
      *os << storage_class << node->nested_type_name (scope, "_slice", "_")
          << " *" << be_nl;
      *os << node->nested_type_name (scope, "_dup", "_")
          << " (" << be_idt << be_idt_nl
          << "const ";
      *os << node->nested_type_name (scope, "_slice", "_")
          << " *_tao_slice);" << be_uidt
          << be_uidt_nl << be_nl;
      *os << storage_class << "void" << be_nl
          << node->nested_type_name (scope, "_copy", "_")
          << " (" << be_idt << be_idt_nl;
      *os << node->nested_type_name (scope, "_slice", "_")
          << " *_tao_to," << be_nl
          << "const ";
      *os << node->nested_type_name (scope, "_slice", "_")
          << " *_tao_from" << be_uidt_nl
          << ");" << be_uidt;
    }

  node->cli_hdr_gen (true);
  return 0;
}
