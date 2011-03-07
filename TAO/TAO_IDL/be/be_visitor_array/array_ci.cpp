// $Id$

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
  if (node->imported () || node->cli_inline_gen ())
    {
      return 0;
    }

  this->ctx_->node (node); // save the array node

  // If we contain an anonymous sequence, generate code for it here.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_ci::"
                         "visit_array - "
                         "bad base type\n"),
                        -1);
    }

  AST_Decl::NodeType nt = bt->node_type ();

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
      && bt->is_child (this->ctx_->scope ()->decl ()))
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

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the array traits specialization definitions,
  // guarded by #ifdef on unaliased array element type and length.

  ACE_CString unique;

  if (nt == AST_Decl::NT_typedef)
    {
      be_typedef *td = be_typedef::narrow_from_decl (bt);
      unique = td->primitive_base_type ()->flat_name ();
    }
  else
    {
      unique = bt->flat_name ();
    }

  char buf[NAMEBUFSIZE];
  ACE_CDR::ULong i;

  for (i = 0UL; i < node->n_dims (); ++i)
    {
      ACE_OS::memset (buf,
                      '\0',
                      NAMEBUFSIZE);
      ACE_OS::sprintf (buf,
                       "_%d",
                       node->dims ()[i]->ev ()->u.ulval);
      unique += buf;
    }

  unique += "_traits";

  *os << be_nl_2
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << "TAO::Array_Traits<" << fname << "_forany>::free ("
      << be_idt << be_idt_nl
      << fname << "_slice * _tao_slice" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << fname << "_free (_tao_slice);" << be_uidt_nl
      << "}";

  *os << be_nl_2
      << "ACE_INLINE" << be_nl
      << fname << "_slice *" << be_nl
      << "TAO::Array_Traits<" << fname << "_forany>::dup ("
      << be_idt << be_idt_nl
      << "const " << fname << "_slice * _tao_slice" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << fname << "_dup (_tao_slice);" << be_uidt_nl
      << "}";

  *os << be_nl_2
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << "TAO::Array_Traits<" << fname << "_forany>::copy ("
      << be_idt << be_idt_nl
      << fname << "_slice * _tao_to," << be_nl
      << "const " << fname << "_slice * _tao_from" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << fname << "_copy (_tao_to, _tao_from);" << be_uidt_nl
      << "}";

  *os << be_nl_2
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << "TAO::Array_Traits<" << fname << "_forany>::zero ("
      << be_idt << be_idt_nl
      << fname << "_slice * _tao_slice" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  ACE_CDR::ULong ndims = node->n_dims ();
  be_array *primitive_type = 0;

  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      // Base type of the array node is a typedef. We need to make sure that
      // this typedef is not to another array type. If it is, then we cannot
      // assign an array to another. We will have to invoke the underlying
      // array type's copy method for every array dimension.

      // There may be more than one level of typedef.
      be_type *tmp = bt;

      while (tmp->node_type () == AST_Decl::NT_typedef)
        {
          be_typedef *tdef = be_typedef::narrow_from_decl (tmp);
          tmp = be_type::narrow_from_decl (tdef->base_type ());
        }

      primitive_type = be_array::narrow_from_decl (tmp);
    }

  *os << "// Zero each individual element." << be_nl;

  // Generate nested loops for as many dimensions as there are.
  for (i = 0; i < ndims; ++i)
    {
      // Retrieve the ith dimension value.
      AST_Expression *expr = node->dims ()[i];

      if ((expr == 0) || ((expr != 0) && (expr->ev () == 0)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "(%N:%l) be_visitor_array_cs::"
                              "visit_array - "
                              "bad array dimension\n"),
                            -1);
        }

      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          // Generate a loop for each dimension.
          *os << "for ( ::CORBA::ULong i" << i << " = 0; i" << i << " < "
              << expr->ev ()->u.ulval << "; ++i" << i << ")" << be_idt_nl
              << "{" << be_idt_nl;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "(%N:%l) be_visitor_array_cs::"
                              "visit_array - "
                              "bad array dimension value\n"),
                            -1);
        }
    }

  if (primitive_type)
    {
      // The base type is a typedef to another array type, so
      // we use the base type's copy method.
      *os << "// call the underlying _zero" << be_nl;

      * os << "TAO::Array_Traits< ";

      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "be_visitor_array_cs::"
                              "visit_array - "
                              "base type decl failed\n"),
                            -1);
        }

      * os << "_forany";

      * os << ">::";

      *os << "zero (_tao_slice";

      for (i = 0; i < ndims; ++i)
        {
          *os << "[i" << i << "]";
        }

      *os << ");";
    }
  else
    {
      // The base type is not a typedef to possibly another array type. In
      // such a case, assign each element.

      *os << "_tao_slice";

      for (i = 0; i < ndims; ++i)
        {
          *os << "[i" << i << "]";
        }

      *os << " = ";

      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "be_visitor_array_ch::"
                              "visit_array - "
                              "base type decl failed\n"),
                            -1);
        }

      *os << " ();";
    }

  for (i = 0; i < ndims; ++i)
    {
      // Add closing braces as many times as the number of dimensions.
      *os << be_uidt_nl << "}" << be_uidt;
    }
  *os << be_uidt_nl << "}";

  *os << be_nl_2
      << "ACE_INLINE" << be_nl
      << fname << "_slice *" << be_nl
     << "TAO::Array_Traits<" << fname << "_forany>::alloc (void)"
      << be_idt << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << fname << "_alloc ();" << be_uidt_nl
      << "}";

  *os << be_nl;

  node->cli_inline_gen (true);
  return 0;
}
