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
      << "CORBA::Environment &env" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << node->name () << "::_nil ();" << be_uidt_nl
      << "if (!obj->_is_a (\"" << node->repoID () << "\", env))"
      << be_idt_nl
      << "return " << node->name () << "::_nil ();" << be_uidt_nl;

  *os << "return " << node->name ()
      << "::_unchecked_narrow (obj, env);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // This may be necessary to work around a GCC compiler bug!
  const char *skel_name = node->full_skel_name ();
  const char *coll_name = node->full_coll_name ();
  assert (coll_name != 0);

  // The _unchecked_narrow method
  *os << node->name () << "_ptr " << node->name ()
      << "::_unchecked_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &env" << be_uidt_nl
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
      << node->repoID () << "\");" << be_uidt_nl;

  *os << "if (servant == 0)" << be_idt_nl
      << "return new " << node->name () << "(stub);" << be_uidt_nl;

  *os << "return new "
      << coll_name << "(" << be_idt << be_idt_nl
      << "ACE_reinterpret_cast(" << skel_name
      << "_ptr, servant)," << be_nl
      << "stub" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
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
    "const CORBA::Char *value, CORBA::Environment &env)" << be_nl;
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
  *os << "(!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (env))))\n";
  *os << "  return 1; // success using local knowledge\n";
  os->decr_indent ();
  *os << "else" << be_nl;
  *os << "  return this->CORBA_Object::_is_a (value, env);\n";
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
