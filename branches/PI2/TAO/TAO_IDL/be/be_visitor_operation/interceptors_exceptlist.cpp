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
  be_type *bt;       // type node

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  // Start with the current indentation level.
  os->indent ();

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTLIST:
      return this->gen_exceptlist (node);
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTION_ID:
      return this->gen_exception_id (node);
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interceptors_exceptlist::"
                           "visit_operation - "
                           "Bad context\n"),
                          -1);
      }

    }

  return 0;
}

int
be_visitor_operation_interceptors_exceptlist::gen_exceptlist (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  // Start with the current indentation level.
  os->indent ();

  *os <<" CORBA::ULong length = this->exception_list_.length ();"<<be_nl
      << "for (CORBA::Ulong i = 0;; ++i)" << be_nl
      << " {" << be_idt_nl
      << " CORBA::TypeCode_ptr tcp = _tao_" << node->flat_name ()
      << "_exceptiondata [i].tc;" << be_nl
      << "// We stop add to the exception list once we strike a void entry."<<be_nl
      << " if (tcp->kind () == CORBA::_tc_null)"<< be_idt_nl
      << "break;"<<be_uidt_nl
      << "this->exception_list_.length (length + 1);"<<be_nl
      << "this->exception_list_[length] = *tcp;" << be_uidt_nl
      << " }\n";

return 0;
}

int
be_visitor_operation_interceptors_exceptlist::gen_exception_id (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  // Start with the current indentation level.
  os->indent ();

  // For received_exception id
  *os << "// we have the is_system_exception boolean" << be_nl
      << "// the byte_order boolean and" << be_nl
      << "// the marshaled_exception sequence of octet" << be_nl
      << " TAO_InputCDR _tao_in ((const char*) "
      << "this->marshaled_exception ().get_buffer ()," << be_nl
      << "  this->marshaled_exception ().length ()," << be_nl
      << "  this->byte_order ());" << be_nl << be_nl
      << "CORBA::String_var type_id;" << be_nl << be_nl
      << "if ((_tao_in >> type_id.inout ()) == 0)" << be_nl
      << "  {" << be_nl
      << "    // Could not demarshal the exception id, raise an local" << be_nl
      << "    // CORBA::MARSHAL" << be_nl;

  if (idl_global->use_raw_throw ())
    *os << "    throw (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE," << be_nl;
  else
    *os << "    ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE," << be_nl;

  *os << "                               CORBA::COMPLETED_YES));" << be_nl
      << "  }" << be_nl 
      << "else "<< be_nl
      << "  {"<<be_nl
      << "return type_id._retn ()"<< be_nl
      << "  }\n\n";

  return 0;
  
}
