//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_exception_holder_operation_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, ami_exception_holder_operation_cs, "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_ami_exception_holder_operation_cs::be_visitor_operation_ami_exception_holder_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_exception_holder_operation_cs::~be_visitor_operation_ami_exception_holder_operation_cs (void)
{
}

int
be_visitor_operation_ami_exception_holder_operation_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  // Start with the current indentation level.
  os->indent ();

  // Init the return type variable.
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_exception_holder_operation_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Generate the return type. Return type is simply void.
  *os << "void " << be_nl;

  // Get the scope name.
  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  if (parent == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_exception_holder_operation_cs::"
                         "visit_operation - "
                         "scopeless operation :-<\n"),
                        -1);
    }

  be_interface *parent_interface = be_interface::narrow_from_decl (parent);
  if (parent_interface->is_nested () &&
      parent_interface->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    *os << "OBV_";

  // Here we do not have our overridden be_interface methods,
  // so the interface type strategy does not work here.
  // We have to go by foot.
  // Genereate scope name.
  *os << parent->compute_name ("_tao_", "");

  // Generate the operation name.
  *os << "::";

  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "set_";
      else
        *os << "get_";
    }
  *os << node->local_name () << "(";

  if (!idl_global->exception_support ())
    *os << "CORBA::Environment &ACE_TRY_ENV";

  *os << ")" << be_uidt;

  // now generate the throw specs
  if (this->gen_throw_spec (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%N:%l) be_visitor_operation_ami_exception_holder_operation_cs::")
                         ASYS_TEXT ("::visit_operation - ")
                         ASYS_TEXT ("Failed to generate throw spec\n")),
                        -1);
    }

  *os << be_nl << "{" << be_idt_nl;

  *os << this->gen_environment_var () << be_nl;


  if (node->exceptions ())
    {
      os->indent ();
      *os << "static TAO_Exception_Data "
          << "exceptions_data [] = " << be_nl;
      *os << "{" << be_idt_nl;
      // initialize an iterator to iterate thru the exception list
      UTL_ExceptlistActiveIterator *ei;
      ACE_NEW_RETURN (ei,
                      UTL_ExceptlistActiveIterator (node->exceptions ()),
                      -1);
      int excep_count = 0;
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
          excep_count++;

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

      os->indent ();
      *os << "CORBA::ULong exceptions_count = "
          << excep_count << ";";
    }



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
      << "    // CORBA::MARSHAL" << be_nl
      << "    ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE," << be_nl
      << "                               CORBA::COMPLETED_YES));" << be_nl
      << "  }" << be_nl << be_nl;

  *os << "if (this->is_system_exception ())" << be_idt_nl
      << "{" << be_idt_nl
      << "CORBA::ULong minor = 0;" << be_nl
      << "CORBA::ULong completion = 0;" << be_nl
      << "if ((_tao_in >> minor) == 0 ||" << be_nl
      << "  (_tao_in >> completion) == 0)" << be_idt_nl
      << "ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE," << be_idt_nl
      << "CORBA::COMPLETED_MAYBE));" << be_uidt << be_uidt_nl;

  *os << "CORBA::SystemException* exception =" << be_idt_nl
      << "TAO_Exceptions::create_system_exception (type_id.in ()," << be_idt_nl
      << "ACE_TRY_ENV);" << be_uidt_nl << be_uidt_nl
      << "ACE_CHECK;" << be_nl << be_nl;
  *os << "if (exception == 0)" << be_idt_nl
      << " {" << be_idt_nl
      << "// @@ We should raise a CORBA::NO_MEMORY, but we ran out" << be_nl
      << "//    of memory already. We need a pre-allocated, TSS," << be_nl
      << "//    CORBA::NO_MEMORY instance" << be_nl
      << "ACE_NEW (exception, CORBA::UNKNOWN);" << be_uidt_nl
      << "}"<< be_uidt_nl
      << "exception->minor (minor);" << be_nl
      << "exception->completed (CORBA::CompletionStatus (completion));" << be_nl

      << "// @@ There should be a better way to raise this exception!" << be_nl
      << "//    This code works for both native and emulated exceptions," << be_nl
      << "//    but it is ugly." << be_nl
      << "ACE_TRY_ENV.exception (exception);" << be_nl
      << "// We can not use ACE_THROW here." << be_nl
      << "return;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl;

  if (node->exceptions())
    {
      *os << be_idt_nl
          << "else  // it must be user exception" << be_idt_nl
          << "{" << be_idt_nl
          << "// Match the exception interface repository id with the" << be_nl
          << "// exception in the exception list." << be_nl
          << "// This is important to decode the exception." << be_nl

          << "for (CORBA::ULong i = 0;" << be_nl
          << "     i < exceptions_count;" << be_nl
          << "     i++)" << be_idt_nl
          << "{" << be_idt_nl
          << "CORBA::TypeCode_ptr tcp = exceptions_data[i].tc;" << be_nl;

      *os << "const char *except_id = tcp->id (ACE_TRY_ENV);" << be_nl
          << "ACE_CHECK;" << be_nl

          << "if (ACE_OS::strcmp (type_id.in (), except_id) != 0)" << be_idt_nl
          << "continue;" << be_uidt_nl << be_nl

          << "// match" << be_nl
          << "CORBA::Exception_ptr exception = exceptions_data[i].alloc ();"
          << be_nl << be_nl

          << "if (exception == 0)" << be_idt_nl
          << "ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE," << be_nl
          << "                             CORBA::COMPLETED_YES));" << be_uidt_nl

          << "exception->_tao_decode (_tao_in, ACE_TRY_ENV);"
          << be_nl << be_nl;

      *os << "// @@ There should be a better way to raise this exception!" << be_nl
          << "//    This code works for both native and emulated exceptions," << be_nl
          << "//    but it is ugly." << be_nl
          << "ACE_TRY_ENV.exception (exception); // We can not use ACE_THROW here." << be_nl
          << "return;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl

          << "// If we couldn't find the right exception, report it as" << be_nl
          << "// CORBA::UNKNOWN." << be_nl << be_nl;

      *os << "// @@ It would seem like if the remote exception is a" << be_nl
          << "//    UserException we can assume that the request was" << be_nl
          << "//    completed." << be_nl
          << "ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE," << be_nl
          << "                           CORBA::COMPLETED_YES));" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }

  *os << "};\n\n";
  return 0;
}
