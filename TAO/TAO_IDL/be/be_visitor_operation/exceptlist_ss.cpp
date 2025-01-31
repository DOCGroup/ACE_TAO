
//=============================================================================
/**
 *  @file    exceptlist_ss.cpp
 *
 *  Visitor generating code for the list of exceptions that an operation
 *  raises (skeleton/server side).
 *
 *  @author Ossama Othman
 */
//=============================================================================

#include "operation.h"

// ****************************************************************************
// visitor to generate the exception list for operations (skeleton/server_side)
// ****************************************************************************

be_visitor_operation_exceptlist_ss::be_visitor_operation_exceptlist_ss (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_exceptlist_ss::~be_visitor_operation_exceptlist_ss ()
{
}

int
be_visitor_operation_exceptlist_ss::visit_operation (be_operation * node)
{
  TAO_OutStream * const os = this->ctx_->stream ();

  // When there are exceptions and typecode support is enabled, then generate
  // the typecodes of the user exceptions this operation can throw.
  if (node->exceptions () && be_global->tc_support ())
    {
      *os << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl;

      *os << "static ::CORBA::TypeCode_ptr const exceptions[] = " << be_idt_nl;
      *os << "{" << be_idt_nl;

      // Initialize an iterator to iterate thru the exception list.
      // Continue until each element is visited.
      // Iterator must be advanced explicitly inside the loop.
      for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
           !ei.is_done ();)
        {
          be_exception * ex = dynamic_cast<be_exception*> (ei.item ());

          *os << ex->tc_name ();

          ei.next ();

          if (!ei.is_done ())
            {
              *os << "," << be_nl;
            }
        }

      *os << be_uidt_nl << "};" << be_uidt_nl;

      *os << "\n#endif /* TAO_HAS_INTERCEPTORS */" << be_nl_2;
    }

  return 0;
}
