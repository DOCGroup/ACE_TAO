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

ACE_RCSID (be_visitor_operation, 
           interceptors_exceptlist, 
           "$Id$")

// ************************************************************
// Operation visitor for exception list
// ************************************************************

be_visitor_operation_interceptors_exceptlist::
be_visitor_operation_interceptors_exceptlist (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_exceptlist::
~be_visitor_operation_interceptors_exceptlist (void)
{
}

int
be_visitor_operation_interceptors_exceptlist::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // Start with the current indentation level.
  os->indent ();
  TAO_CodeGen::CG_STATE state = this->ctx_->state ();

  if (state == TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTLIST)
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
be_visitor_operation_interceptors_exceptlist::gen_exceptlist (
    be_operation *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // Generate the exception data array.
  *os << be_nl
      << "static CORBA::TypeCode_ptr " << "_tao_" << node->flat_name ()
      << "_exceptiondata[] = " << be_nl;
  *os << "{" << be_idt_nl;

  be_exception *excp = 0;

  // Initialize an iterator to iterate thru the exception list.
  // Continue until each element is visited.
  // Iterator must be advanced explicitly inside the loop.
  for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
       !ei.is_done ();)
    {
      excp = be_exception::narrow_from_decl (ei.item ());

      if (excp == 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_interceptors_exceptlist"
              "gen_exceptlist - "
              "be_exception::narrow_from_decl failed\n"),
            -1
          );
        }

      *os << excp->tc_name ();
      ei.next ();

      if (!ei.is_done ())
        {
          *os << "," << be_nl;
        }
    }

  *os << be_uidt_nl << "};" << be_nl;

  long excp_count = (node->exceptions())->length ();
  *os << be_nl
      << "exception_list->length (" << excp_count << ");" << be_nl
      << "for (CORBA::ULong i = 0; i < " << excp_count << "; ++i)"
      << be_idt_nl
      << "{" << be_idt_nl
      << "CORBA::TypeCode_ptr tcp = _tao_" << node->flat_name ()
      << "_exceptiondata[i];" << be_nl
      << "TAO_Pseudo_Object_Manager<CORBA::TypeCode, "
      << "CORBA::TypeCode_var> tcp_object (&tcp, 1);" << be_nl
      << "(*exception_list)[i] = tcp_object;" << be_uidt_nl
      << "}\n" << be_uidt;

return 0;
}
