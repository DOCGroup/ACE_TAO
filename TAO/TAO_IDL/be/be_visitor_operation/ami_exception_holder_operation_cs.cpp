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

  if (!be_global->exception_support ())
    *os << "TAO_ENV_SINGLE_ARG_DECL";

  *os << ")" << be_uidt;

  // now generate the throw specs
  if (this->gen_throw_spec (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_operation_ami_")
                         ACE_TEXT ("exception_holder_operation_cs::")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("Failed to generate throw spec\n")),
                        -1);
    }

  *os << be_nl << "{" << be_idt_nl;

  *os << this->gen_environment_var () << be_nl;

  const char *exception_data_arg = "0";
  const char *exception_count_arg = "0";

  // Don't do anything if the exception list is empty.
  if (node->exceptions ())
    {
      *os << "static TAO_Exception_Data " << "exceptions_data [] = " << be_nl;
      *os << "{" << be_idt_nl;

      // Initialize an iterator to iterate thru the exception list.
      UTL_ExceptlistActiveIterator ei (node->exceptions ());
      int excep_count = 0;

      AST_Decl *d = 0;

      // Continue until each element is visited.
      while (!ei.is_done ())
        {
          d = ei.item ();

          *os << "{" << be_idt_nl
              << "\"" << d->repoID () << "\"," << be_nl;
          // Allocator method.
          *os << d->name () << "::_alloc" << be_uidt_nl
              << "}";

          excep_count++;
          ei.next ();

          if (!ei.is_done ())
            {
              *os << "," << be_nl;
            }

        }

      *os << be_uidt_nl << "};\n\n";

      os->indent ();
      *os << "CORBA::ULong exceptions_count = "
          << excep_count << ";\n" << be_nl;

      exception_data_arg = "exceptions_data";
      exception_count_arg = "exceptions_count";
    }

  *os << "TAO_Messaging_Helper::exception_holder_raise (" << be_idt_nl
      << exception_data_arg << "," << be_nl
      << exception_count_arg << "," << be_nl
      << "this->marshaled_exception ().get_buffer ()," << be_nl
      << "this->marshaled_exception ().length ()," << be_nl
      << "this->byte_order ()," << be_nl
      << "this->is_system_exception ()" << be_nl
      << "TAO_ENV_ARG_PARAMETER);" << be_uidt << be_uidt_nl;

  *os << "}\n\n";
  return 0;
}
