// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ih.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the implementation header
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_ih, "$Id$")

// ************************************************************
// Operation visitor for implementation header
// ************************************************************

be_visitor_operation_ih::be_visitor_operation_ih (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ih::~be_visitor_operation_ih (void)
{
}

int
be_visitor_operation_ih::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node representing the return type

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *os << "virtual ";

  // STEP I: generate the return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // grab the right visitor to generate the return type
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ih::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ih::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  // STEP 2: generate the operation name
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_IH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ih::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ih::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  //generate the exceptions that are thrown by the operation
  gen_raise_exception (node);

  *os << ";\n\n";

  return 0;
}


//Method to generate the exceptions that are thrown by the operation
int
be_visitor_operation_ih::gen_raise_exception (be_operation *node)
{

  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

   if (node->exceptions ())
    {
      os->indent ();
     
      // initialize an iterator to iterate thru the exception list
      UTL_ExceptlistActiveIterator *ei;
      ACE_NEW_RETURN (ei,
                      UTL_ExceptlistActiveIterator (node->exceptions ()),
                      -1);
      *os << be_idt_nl << "TAO_THROW_SPEC ((";
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
   

          // allocator method
          *os << excp->name ();
          ei->next ();
          if (!ei->is_done ())
            {
              *os << "," <<be_nl<<"\t\t";
              //os->indent ();
            }
          // except the last one is processed?

        } // end of while loop
      delete ei;
      *os << "))"<<be_uidt;
    } // end of if

   return 0;

}
