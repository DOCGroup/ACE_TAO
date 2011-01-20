
//=============================================================================
/**
 *  @file    exceptlist_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the list of exceptions that an operation
 *  raises.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// ****************************************************************************
// visitor to generate the exception list for operations
// ****************************************************************************

be_visitor_operation_exceptlist_cs::be_visitor_operation_exceptlist_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_exceptlist_cs::~be_visitor_operation_exceptlist_cs (void)
{
}

int
be_visitor_operation_exceptlist_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Don't do anything if the exception list is empty.
  if (node->exceptions ())
    {
      *os << be_nl_2
          << "static TAO::Exception_Data" << be_nl
          << "_tao_" << node->flat_name ()
          << "_exceptiondata [] = " << be_idt_nl;
      *os << "{" << be_idt_nl;

      be_exception *ex = 0;

      // Initialize an iterator to iterate thru the exception list.
      // Continue until each element is visited.
      // Iterator must be advanced explicitly inside the loop.
      for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
           !ei.is_done ();)
        {
          AST_Decl *d = ei.item ();
          ex = be_exception::narrow_from_decl (d);

          *os << "{" << be_idt_nl
              << "\"" << ex->repoID () << "\"," << be_nl
              << ex->name () << "::_alloc"
              << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl;

          if (be_global->tc_support ())
            {
              *os << ", " << ex->tc_name ();
            }
          else
            {
              *os << ", 0";
            }

          *os << "\n#endif /* TAO_HAS_INTERCEPTORS */" << be_uidt_nl
              << "}";

          ei.next ();

          if (!ei.is_done ())
            {
              *os << "," << be_nl_2;
            }
        }

      *os << be_uidt_nl << "};" << be_uidt;
    }

  return 0;
}
