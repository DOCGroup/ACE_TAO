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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

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
  *os << node->name () << "_ptr " << node->name ()
      << "::_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << node->name () << "::_nil ();" << be_uidt_nl
      << "CORBA::Boolean is_a = obj->_is_a (\""
      << node->repoID () << "\", ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK_RETURN (" << node->name () << "::_nil ());" << be_nl
      << "if (is_a == 0)" << be_idt_nl
      << "return " << node->name () << "::_nil ();" << be_uidt_nl;

  *os << "return " << node->name ()
      << "::_unchecked_narrow (obj, ACE_TRY_ENV);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // This may be necessary to work around a GCC compiler bug!
//  const char *skel_name = node->full_skel_name (); // unused at this time
//    const char *coll_name = node->full_coll_name ();
//    assert (coll_name != 0);

  // The _unchecked_narrow method
  *os << node->name () << "_ptr " << node->name ()
      << "::_unchecked_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << node->name () << "::_nil ();" << be_uidt_nl;

  *os << "TAO_Stub* stub = obj->_stubobj ();" << be_nl
      << "stub->_incr_refcnt ();" << be_nl;
  *os << "void* servant = 0;" << be_nl;
  *os << "if (obj->_is_collocated () "
      << "&& obj->_servant() != 0)" << be_idt_nl
      << "servant = obj->_servant()->_downcast (\""
      << "IDL:omg.org/CORBA/Object:1.0\");" << be_uidt_nl;

  *os << "if (servant != 0)" << be_idt_nl << "{" << be_idt_nl
    // The collocated object reference factory is not working right (yet)
      << node->name () << "_ptr retv = ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << node->name () << "_ptr," << be_nl
      << "ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << "PortableServer::Servant," << be_nl
      << "servant" << be_uidt_nl
      << ")" << be_uidt_nl
      << "->_create_collocated_objref (" << be_idt << be_idt_nl
      << "\"" << node->repoID () << "\"," << be_nl
      << "TAO_ORB_Core::ORB_CONTROL," << be_nl
      << "stub" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << ");" << be_uidt_nl
      << "if (retv != 0)" << be_idt_nl
      << "return retv;" << be_uidt
    // So we are still using the old way to create collocated objref.
      << be_uidt_nl
      << "}" << be_uidt_nl;

  *os << "return new " << node->name () << "(stub);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The _duplicate method
  *os << node->name () << "_ptr " << be_nl
      << node->name () << "::_duplicate ("
      << node->name () << "_ptr obj)" << be_nl
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
  *os << "CORBA::Boolean " << node->name () << "::_is_a (" <<
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
  *os << "const char* " << node->name ()
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
                         "TypeCode definition failed\n"
                         ), -1);
    }


  return 0;
}
