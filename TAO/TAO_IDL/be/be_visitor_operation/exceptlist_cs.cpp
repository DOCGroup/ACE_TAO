//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    exceptlist_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for the list of exceptions that an operation
//    raises.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           exceptlist_cs, 
           "$Id$")

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
      *os << "static TAO_Exception_Data " << "_tao_" << node->flat_name ()
          << "_exceptiondata [] = " << be_idt_nl;
      *os << "{" << be_idt_nl;

      AST_Decl *d = 0;

      // Initialize an iterator to iterate thru the exception list.
      // Continue until each element is visited.
      // Iterator must be advanced explicitly inside the loop.
      for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
           !ei.is_done ();)
        {
          d = ei.item ();

          *os << "{" << be_idt_nl
              << "\"" << d->repoID () << "\"," << be_nl;
          // Allocator method.
          *os << d->name () << "::_alloc" << be_uidt_nl
              << "}";

          ei.next ();

          if (!ei.is_done ())
            {
              *os << "," << be_nl << be_nl;
            }
        }

      *os << be_uidt_nl << "};" << be_uidt_nl << be_nl;
    }

  return 0;
}
