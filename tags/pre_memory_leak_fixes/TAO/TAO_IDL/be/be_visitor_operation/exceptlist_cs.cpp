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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, exceptlist_cs, "$Id$")


// ****************************************************************************
// visitor to generate the exception list for operations
// ****************************************************************************

be_visitor_operation_exceptlist_cs::be_visitor_operation_exceptlist_cs (be_visitor_context
                                                            *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_exceptlist_cs::~be_visitor_operation_exceptlist_cs (void)
{
}

int
be_visitor_operation_exceptlist_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  // don't do anything if the exception list is empty
  if (node->exceptions ())
    {
      os->indent ();
      *os << "static TAO_Exception_Data " << "_tao_" << node->flatname ()
          << "_exceptiondata [] = " << be_nl;
      *os << "{" << be_idt_nl;
      // initialize an iterator to iterate thru the exception list
      UTL_ExceptlistActiveIterator *ei;
      ACE_NEW_RETURN (ei,
                      UTL_ExceptlistActiveIterator (node->exceptions ()),
                      -1);
      // continue until each element is visited
      while (!ei->is_done ())
        {
          be_exception *excp = be_exception::narrow_from_decl (ei->item ());

          if (excp == 0)
            {
              delete ei;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_exceptlist_cs"
                                 "visit_operation - "
                                 "codegen for scope failed\n"), -1);

            }
          *os << "{";
          // the typecode name
          *os << excp->tc_name ();
          *os << ", ";
          // allocator method
          *os << excp->name () << "::_alloc}";
          ei->next ();
          if (!ei->is_done ())
            {
              *os << ",\n";
              os->indent ();
            }
          // except the last one is processed?

        } // end of while loop
      delete ei;
      *os << be_uidt_nl << "};\n\n";
    } // end of if
  return 0;
}
