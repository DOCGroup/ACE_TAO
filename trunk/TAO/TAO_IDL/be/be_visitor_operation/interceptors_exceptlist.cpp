//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_exceptlist.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, interceptors_exceptlist, "$Id$")


// ************************************************************
// Operation visitor for exception list
// ************************************************************

be_visitor_operation_interceptors_exceptlist::be_visitor_operation_interceptors_exceptlist (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_exceptlist::~be_visitor_operation_interceptors_exceptlist (void)
{
}

int
be_visitor_operation_interceptors_exceptlist::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  //  be_type *bt;       // type node

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  // Start with the current indentation level.
  os->indent ();

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTLIST)
    {
      return this->gen_exceptlist (node);
    }

  ACE_ERROR_RETURN ((LM_ERROR,
                     "(%N:%l) be_visitor_interceptors_exceptlist::"
                     "visit_operation - "
                     "Bad context\n"),
                    -1);
}

int
be_visitor_operation_interceptors_exceptlist::gen_exceptlist (be_operation *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  // Generate the exception data array.
  *os << be_nl
      << "static TAO_Exception_Data " << "_tao_" << node->flat_name ()
      << "_exceptiondata[] = " << be_nl;
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
  *os << be_uidt_nl << "};" << be_nl;

  long excp_count = (node->exceptions())->length ();
  *os << be_nl << "CORBA::ULong len = 0;" << be_nl
      << "for (CORBA::ULong i = 0; i < " << excp_count << "; ++i)"
      << be_idt_nl
      << "{" << be_idt_nl
      << "CORBA::TypeCode_ptr tcp = _tao_" << node->flat_name ()
      << "_exceptiondata[i].tc;" << be_nl
      << "len = exception_list->length ();" << be_nl
      << "exception_list->length (len + 1);"<<be_nl
      << "TAO_Pseudo_Object_Manager<CORBA::TypeCode,CORBA::TypeCode_var> tcp_object (&tcp, 1);" << be_nl
      << "(*exception_list)[len] = tcp_object;" << be_uidt_nl
      << "}\n" << be_uidt;

return 0;
}
