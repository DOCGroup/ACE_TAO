
//=============================================================================
/**
 *  @file    tie_ss.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for operations for the TIE class.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ************************************************************
// Operation visitor for server inline for TIE class operations.
// ************************************************************

be_visitor_operation_tie_ss::be_visitor_operation_tie_ss (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_tie_ss::~be_visitor_operation_tie_ss (void)
{
}

int be_visitor_operation_tie_ss::visit_operation (be_operation *node)
{
  /// These implied IDL operations are not to be processed on
  /// the skeleton side.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_interface *intf = this->ctx_->interface ();

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_tie_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Retrieve the operation return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_ss::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Although unlikely it is possible that the 'T' in 'template class<T>' will
  // conflict with an argument name...
  ACE_CString template_name ("T");
  bool template_name_ok = false;

  while (!template_name_ok)
    {
      template_name_ok = true;

      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
          ! si.is_done () && template_name_ok;
          si.next ())
        {
          // Check for conflicts between the arg name and the proposed template
          // class identifier
          AST_Argument *arg =
            AST_Argument::narrow_from_decl (si.item ());

          if (! ACE_OS::strcmp (arg->local_name ()->get_string (),
                                template_name.c_str ()))
            {
              // clash !
              template_name_ok = false;
            }
        }

      if (! template_name_ok)
        {
          // We had a clash - postfix an underscore and try again
          template_name += "_";
        }
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "template <class " << template_name.c_str () << ">" << be_nl;

  // Generate the return type mapping (same as in the header file).
  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype oro_visitor (&ctx);

  if (bt->accept (&oro_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_ss::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  *os << " " << intf->full_skel_name () << "_tie<"
      << template_name.c_str () << ">::"
      << this->ctx_->port_prefix ().c_str ()
      << node->local_name () << " ";

  // STEP 4: generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  be_visitor_operation_arglist oao_visitor (&ctx);

  if (node->accept (&oao_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  *os << be_nl << "{" << be_idt_nl;

  be_predefined_type *pdt = be_predefined_type::narrow_from_decl (bt);

  if (pdt == 0 || pdt->pt () != AST_PredefinedType::PT_void)
    {
      *os << "return ";
    }

  *os << "this->ptr_->" << node->local_name () << " (" << be_idt;

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS);
  be_visitor_operation_argument ocau_visitor (&ctx);

  if (node->accept (&ocau_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }

  *os << be_uidt_nl;
  *os << ");" << be_uidt_nl;
  *os << "}";

  return 0;
}
