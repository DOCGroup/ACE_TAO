// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_is.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the implementation skeleton
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"
#include        "ace/SString.h" 

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_is, "$Id$")

// ************************************************************
// Operation visitor for implementation skeleton
// ************************************************************

be_visitor_operation_is::be_visitor_operation_is (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_is::~be_visitor_operation_is (void)
{
}

int
be_visitor_operation_is::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node representing the return type

  //cout<<"Within visit_operation "<<endl;
  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level




  // STEP I: generate the return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_is::"
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
                         "be_visitor_operation_is::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_is::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;


  ACE_CString str(node->flatname ());


  int lnmlength = ACE_OS::strlen (node->local_name ()->get_string ());

  int fnmlength = ACE_OS::strlen (node->flatname ());
  fnmlength--;

  char * classname =  str.substr (0,(fnmlength-lnmlength) ).rep ();
  
  // STEP 2: generate the operation name
  *os << " " << idl_global->impl_class_prefix () << classname << idl_global->impl_class_suffix () << "::" << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_IS);
  visitor = tao_cg->make_visitor (&ctx);


  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_is::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }


  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_is::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  //generate the excetions thrown by the operation
  this->gen_raise_exception (node);

  *os <<be_idt_nl << "{"<<be_idt_nl;
  *os << "//Add your implementation here"<<be_uidt_nl;

  //Code to generate teh return statement in the operations.....
  //Can be uncommented when required

  /*
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_IS);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_is::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }
  

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_is::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;
  */

  *os << "}" << be_nl << be_uidt_nl;

  
  return 0;
}


//method to generate the exceptions throw by the operation
int
be_visitor_operation_is::gen_raise_exception (be_operation *node)
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
