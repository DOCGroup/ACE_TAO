//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_cs, "$Id$")


// ************************************************************
// Interface visitor for client stubs
// ************************************************************

be_visitor_interface_cs::be_visitor_interface_cs (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cs::~be_visitor_interface_cs (void)
{
}

int
be_visitor_interface_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (node->cli_stub_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  os->indent (); // start with whatever indentation level we are at

  // first generate the code for the static methods

  // The _narrow method
  *os << node->full_name () << "_ptr " << node->full_name ()
      << "::_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << node->full_name () << "::_nil ();" << be_uidt_nl
      << "CORBA::Boolean is_a = obj->_is_a (\""
      << node->repoID () << "\", ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK_RETURN (" << node->full_name () << "::_nil ());" << be_nl
      << "if (is_a == 0)" << be_idt_nl
      << "return " << node->full_name () << "::_nil ();" << be_uidt_nl;

  *os << "return " << node->full_name ()
      << "::_unchecked_narrow (obj, ACE_TRY_ENV);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << node->full_name () << "_ptr " << node->full_name ()
      << "::_unchecked_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &";
  if (idl_global->gen_locality_constraint ())
    *os << " ACE_TRY_ENV";

  *os << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  // This may be necessary to work around a GCC compiler bug!
  //  const char *skel_name = node->full_skel_name (); // unused at this time
  //    const char *coll_name = node->full_coll_name ();
  //    assert (coll_name != 0);

  // The _unchecked_narrow method
  if (!idl_global->gen_locality_constraint ())
    {
      *os << "if (CORBA::is_nil (obj))" << be_idt_nl
          << "return " << node->full_name () << "::_nil ();" << be_uidt_nl;

      *os << "TAO_Stub* stub = obj->_stubobj ();" << be_nl
          << "stub->_incr_refcnt ();" << be_nl;

      *os << "if (obj->_is_collocated () && _TAO_collocation_"
          << node->flat_name () << "_Stub_Factory_function_pointer != 0)"
          << be_idt_nl << "{" << be_idt_nl << node->local_name ()
          << "_ptr retv = _TAO_collocation_"
          << node->flat_name () << "_Stub_Factory_function_pointer (obj);"
          << be_nl << "if (retv != 0)" << be_idt_nl << "return retv;"
          << be_uidt << be_uidt_nl << "}" << be_uidt_nl;

      *os << node->full_name () << "_ptr retval = 0;" << be_nl
          << "ACE_NEW_RETURN (retval, " << node->full_name ()
          << " (stub), 0);" << be_nl
          << "return retval;" <<  be_uidt_nl;
    }
  else
    {
      // _narrow implementation for locality constraint object.
      *os << "void *servant = 0;" << be_nl
          << "if (!obj->_is_collocated ()" << be_idt << be_idt << be_idt_nl
          << "|| !obj->_servant ()" << be_nl
          << "|| (servant = obj->_servant()->_downcast (\""
          << node->repoID () << "\")) == 0" << be_uidt_nl
          << ")" << be_uidt_nl
          << "ACE_THROW_RETURN (CORBA::MARSHAL (), "
          << node->full_name () << "::_nil ());" << be_uidt_nl;

      // Locality constraint objects alway use "direct" collocated
      // implementation.
      *os << node->full_name () << "_ptr retval = 0;" << be_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "retval," << be_nl
          << node->full_coll_name (be_interface::DIRECT) << " ("
          << "ACE_reinterpret_cast (POA_" << node->full_name ()
          << "_ptr, servant), 0)," << be_nl
          << "0" << be_uidt_nl << ");" << be_uidt_nl
          << "return retval;" << be_uidt_nl;
    }
  *os << "}\n" << be_nl;


  // The _duplicate method
  *os << node->full_name () << "_ptr " << be_nl
      << node->full_name () << "::_duplicate ("
      << node->full_name () << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "if (!CORBA::is_nil (obj))" << be_idt_nl
      << "obj->_incr_refcnt ();" << be_uidt_nl
      << "return obj;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // generate code for the elements of the interface
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  // generate the is_a method
  os->indent ();
  *os << "CORBA::Boolean " << node->full_name () << "::_is_a (" <<
    "const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "if (\n";
  os->incr_indent (0);
  if (node->traverse_inheritance_graph (be_interface::is_a_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "_is_a method codegen failed\n"), -1);
    }

  os->indent ();
  *os << "(!ACE_OS::strcmp ((char *)value, \"IDL:omg.org/CORBA/Object:1.0\")))\n";
  *os << "  return 1; // success using local knowledge\n";
  os->decr_indent ();
  *os << "else" << be_nl;
  *os << "  return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "const char* " << node->full_name ()
      << "::_interface_repository_id (void) const"
      << be_nl
      << "{" << be_idt_nl
      << "return \"" << node->repoID () << "\";" << be_uidt_nl
      << "}\n\n";

  // by using a visitor to declare and define the TypeCode, we have the
  // added advantage to conditionally not generate any code. This will be
  // based on the command line options. This is still TO-DO
  be_visitor *visitor;
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
  ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "TypeCode definition failed\n"),
                        -1);
    }

  // AMI

  // Generate code for the AMI Reply Handler.
  if (idl_global->ami_call_back () == I_TRUE)
    {
      // Generate the exception holder
      be_interface_type_strategy *old_strategy =
        node->set_strategy (new be_interface_ami_exception_holder_strategy (node));

      // Set the context.
      be_visitor_context ctx (*this->ctx_);
      // Set the state.
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_INTERFACE_CS);

      // Create the visitor.
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cs::"
                             "visit_interface - "
                             "Bad visitor\n"),
                            -1);
        }

      // Call the visitor on this interface.
      if (node->accept (visitor) == -1)
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cs::"
                             "visit_interface - "
                             "code gen for ami handler default stub failed\n"),
                            -1);
        }
      delete visitor;
      visitor = 0;

      // = Generate the default stub code for Handler.

      // Remove the exception holder strategy, delete it,
      // and set the ami handler strategy
      delete node->set_strategy (new be_interface_ami_handler_strategy (node));

      // = Generate the default stub code for Handler.

      // Set the context.
      ctx = *this->ctx_;

      // Set the state.
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_INTERFACE_CS);

      // Create the visitor.
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cs::"
                             "visit_interface - "
                             "Bad visitor\n"),
                            -1);
        }

      // Call the visitor on this interface.
      if (node->accept (visitor) == -1)
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cs::"
                             "visit_interface - "
                             "code gen for ami handler default stub failed\n"),
                            -1);
        }
      delete visitor;

      delete node->set_strategy (old_strategy);
    }

  return 0;
}
