//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for AMI Handler class for an Interface.
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu> 
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface,  ami_handler_cs, "$Id$")


// ************************************************************
// Interface visitor for client stubs
// ************************************************************

be_visitor_ ami_handler_cs::be_visitor_ ami_handler_cs (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_ ami_handler_cs::~be_visitor_ ami_handler_cs (void)
{
}

int
be_visitor_ ami_handler_cs::visit_interface (be_interface *node)
{
  // output stream.
  TAO_OutStream *os;

  if (node->cli_stub_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  // Start with whatever indentation level we are at.
  os->indent ();

  // Create the full name for the AMI_<Interface name>_Handler
  // interface.  
  char *full_name = 0;

  // Full name. 
  // If there exists a scope name for this, then generate
  // "Scope::AMI_<Local Name>_Handler". 

  size_t scope_len = 0;

  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  
  if (parent != 0 &&
      parent->full_name () != 0 &&
      ACE_OS::strlen (parent->full_name ()))
    scope_len = ACE_OS::strlen (parent->full_name ()) + ACE_OS::strlen ("::");
  
  ACE_NEW_RETURN (full_name,
                  char [scope_len +
                       ACE_OS::strlen ("AMI_") +
                       ACE_OS::strlen (node->local_name ()->get_string ()) +
                       ACE_OS::strlen ("_Handler") +
                       1],
                  -1);
  
  if (parent != 0 &&
      parent->full_name () != 0 &&
      ACE_OS::strlen (parent->full_name ()))
    ACE_OS::sprintf (full_name,
                     "%s::AMI_%s_Handler",
                     parent->full_name (),
                     node->local_name ()->get_string ());
  else
    ACE_OS::sprintf (full_name,
                     "AMI_%s_Handler",
                     node->local_name ()->get_string ());

  // First generate the code for the static methods.
  
  // The _narrow method
  *os << full_name << "_ptr " << full_name
      << "::_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &env" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << full_name << "::_nil ();" << be_uidt_nl
      << "if (!obj->_is_a (\"" << node->repoID () << "\", env))"
      << be_idt_nl
      << "return " << full_name << "::_nil ();" << be_uidt_nl;

  *os << "return " << full_name
      << "::_unchecked_narrow (obj, env);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // This may be necessary to work around a GCC compiler bug!
  //  const char *skel_name = node->full_skel_name (); // unused at this time
  const char *coll_name = node->full_coll_name ();
  assert (coll_name != 0);

  // The _unchecked_narrow method
  *os << full_name << "_ptr " << full_name
      << "::_unchecked_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "ACE_UNUSED_ARG (ACE_TRY_ENV);" << be_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << full_name << "::_nil ();" << be_uidt_nl;

  *os << "TAO_Stub* stub = obj->_stubobj ();" << be_nl
      << "stub->_incr_refcnt ();" << be_nl;
  *os << "void* servant = 0;" << be_nl;
  *os << "if (obj->_is_collocated () "
      << "&& obj->_servant() != 0)" << be_idt_nl
      << "servant = obj->_servant()->_downcast (\""
      << "IDL:omg.org/CORBA/Object:1.0\");" << be_uidt_nl;

  *os << "if (servant != 0)" << be_idt_nl << "{" << be_idt_nl
    // The collocated object reference factory is not working right (yet)
      << full_name << "_ptr retv = ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << full_name << "_ptr," << be_nl
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

  *os << "return new " << full_name << "(stub);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The _duplicate method
  *os << full_name << "_ptr " << be_nl
      << full_name << "::_duplicate ("
      << full_name << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "if (!CORBA::is_nil (obj))" << be_idt_nl
      << "obj->_incr_refcnt ();" << be_uidt_nl
      << "return obj;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // generate code for the elements of the interface
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ ami_handler_cs::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  // generate the is_a method
  os->indent ();
  *os << "CORBA::Boolean " << full_name << "::_is_a (" <<
    "const CORBA::Char *value, CORBA::Environment &env)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "if (\n";
  os->incr_indent (0);
  if (node->traverse_inheritance_graph (be_interface::is_a_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ ami_handler_cs::"
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
  *os << "const char* " << full_name
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
                         "(%N:%l) be_visitor_ ami_handler_cs::"
                         "visit_interface - "
                         "TypeCode definition failed\n"
                         ), -1);
    }
  
  // @@ Alex: Make sure you have done the following in the other ami
  //    visitors. (Alex).
  delete full_name;
  full_name = 0;

  return 0;
}
