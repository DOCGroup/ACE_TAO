// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Arrays in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_array,
           array_ci,
           "$Id$")


// ************************************************************************
//  visitor for array declaration in client inline
// ************************************************************************

be_visitor_array_ci::be_visitor_array_ci (be_visitor_context *ctx)
  : be_visitor_array (ctx)
{
}

be_visitor_array_ci::~be_visitor_array_ci (void)
{
}

int be_visitor_array_ci::visit_array (be_array *node)
{
  // Nothing to do if we are imported or code is already generated.
  if (node->imported () || (node->cli_inline_gen ()))
    {
      return 0;
    }

  this->ctx_->node (node); // save the array node

  // If we contain an anonymous sequence, generate code for it here.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  AST_Decl::NodeType nt = bt->node_type ();

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_ci::"
                         "visit_array - "
                         "bad base type\n"),
                        -1);
    }

  if (nt == AST_Decl::NT_sequence)
    {
      if (this->gen_anonymous_base_type (bt,
                                         TAO_CodeGen::TAO_ROOT_CI)
          == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ci::"
                             "visit_array - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  // If the array is an anonymous member and if its element type
  // is a declaration (not a reference), we must generate code for
  // the declaration.
  if (this->ctx_->alias () == 0 // Not a typedef.
      && bt->is_child (this->ctx_->scope ()))
    {
      int status = 0;
      be_visitor_context ctx (*this->ctx_);

      switch (nt)
      {
        case AST_Decl::NT_struct:
          {
            be_visitor_structure_ci sc_visitor (&ctx);
            status = bt->accept (&sc_visitor);
            break;
          }
        case AST_Decl::NT_union:
          {
            be_visitor_union_ci uc_visitor (&ctx);
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

  // To hold the full and local.
  char fname [NAMEBUFSIZE];
  char lname [NAMEBUFSIZE];
  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (lname,
                  '\0',
                  NAMEBUFSIZE);

  if (this->ctx_->tdef ())
    {
      // Typedefed node.
      ACE_OS::sprintf (fname, "%s",
                       node->full_name ());
      ACE_OS::sprintf (lname, "%s",
                       node->local_name ()->get_string ());
    }
  else
    {
      // For anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name.
      if (node->is_nested ())
        {
          be_decl *parent =
            be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (fname,
                           "%s::_%s",
                           parent->full_name (),
                           node->local_name ()->get_string ());
          ACE_OS::sprintf (lname,
                           "_%s",
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname,
                           "_%s",
                           node->full_name ());
          ACE_OS::sprintf (lname,
                           "_%s",
                           node->local_name ()->get_string ());
        }
    }

  // Generate _life struct static member definitions.
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << "TAO::Array_Traits<" << node->name () << ", " 
      << node->name () << "_slice>::tao_free ("
      << be_idt << be_idt_nl
      << fname << "_slice * _tao_slice" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << fname << "_free (_tao_slice);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "ACE_INLINE" << be_nl
      << fname << "_slice *" << be_nl
      << "TAO::Array_Traits<" << node->name () << ", " 
      << node->name () << "_slice>::tao_dup ("
      << be_idt << be_idt_nl
      << "const " << fname << "_slice * _tao_slice" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << fname << "_dup (_tao_slice);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << "TAO::Array_Traits<" << node->name () << ", " 
      << node->name () << "_slice>::tao_copy ("
      << be_idt << be_idt_nl
      << fname << "_slice * _tao_to," << be_nl
      << "const " << fname << "_slice * _tao_from" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << fname << "_copy (_tao_to, _tao_from);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "ACE_INLINE" << be_nl
      << fname << "_slice *" << be_nl
      << "TAO::Array_Traits<" << node->name () << ", " 
      << node->name () << "_slice>::tao_alloc (void)" << be_nl
      << "{" << be_idt_nl
      << "return " << fname << "_alloc ();" << be_uidt_nl
      << "}";

// ===================================================

  *os << be_nl << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << node->fwd_helper_name () << "_life::tao_free ("
      << be_idt << be_idt_nl
      << fname << "_slice * _tao_slice" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << fname << "_free (_tao_slice);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "ACE_INLINE" << be_nl
      << fname << "_slice *" << be_nl
      << node->fwd_helper_name () << "_life::tao_dup ("
      << be_idt << be_idt_nl
      << "const " << fname << "_slice * _tao_slice" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << fname << "_dup (_tao_slice);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << node->fwd_helper_name () << "_life::tao_copy ("
      << be_idt << be_idt_nl
      << fname << "_slice * _tao_to," << be_nl
      << "const " << fname << "_slice * _tao_from" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << fname << "_copy (_tao_to, _tao_from);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "ACE_INLINE" << be_nl
      << fname << "_slice *" << be_nl
      << node->fwd_helper_name () << "_life::tao_alloc (void)" << be_nl
      << "{" << be_idt_nl
      << "return " << fname << "_alloc ();" << be_uidt_nl
      << "}";

  node->cli_inline_gen (1);
  return 0;
}
