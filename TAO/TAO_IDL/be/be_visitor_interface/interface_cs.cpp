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

  be_type *bt;

  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

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
      << "return " << bt->nested_type_name (this->ctx_->scope ()) 
      << "::_nil ();" << be_uidt_nl
      << "CORBA::Boolean is_a = obj->_is_a (\""
      << node->repoID () << "\", ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK_RETURN (" << bt->nested_type_name (this->ctx_->scope ()) 
      << "::_nil ());" << be_nl
      << "if (is_a == 0)" << be_idt_nl
      << "return " << bt->nested_type_name (this->ctx_->scope ()) 
      << "::_nil ();" << be_uidt_nl;

  *os << "return " << bt->nested_type_name (this->ctx_->scope ())
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

  // The _unchecked_narrow method
  if (!idl_global->gen_locality_constraint ())
    {
      *os << "if (CORBA::is_nil (obj))" << be_idt_nl
          << "return " << bt->nested_type_name (this->ctx_->scope ()) 
          << "::_nil ();" << be_uidt_nl;

      *os << "TAO_Stub* stub = obj->_stubobj ();" << be_nl
          << "stub->_incr_refcnt ();" << be_nl
          // Declare the default proxy.
          << bt->nested_type_name (this->ctx_->scope ()) 
          << "_ptr default_proxy = " 
          << bt->nested_type_name (this->ctx_->scope ()) 
          <<"::_nil ();" << be_nl;

      // If the policy didtates that the proxy be collocated, use the
      // function to create one.

      *os << "if (obj->_is_collocated () && _TAO_collocation_"
          << node->flat_name () << "_Stub_Factory_function_pointer != 0)"
          << be_idt_nl << "{"<<be_idt_nl
          << "default_proxy = _TAO_collocation_"<< node->flat_name ()
          << "_Stub_Factory_function_pointer (obj);"
          << be_uidt_nl<<"}"<<be_uidt_nl;

      // The default proxy will either be returned else be transformed to 
      // a smart one!
      *os << "if (CORBA::is_nil (default_proxy))" << be_idt_nl
          << "ACE_NEW_RETURN (default_proxy, " 
          << bt->nested_type_name (this->ctx_->scope ()) 
          << " (stub), " << bt->nested_type_name (this->ctx_->scope ()) 
          << "::_nil ());"<< be_uidt_nl
          << "return TAO_" << node->flat_name () 
          << "_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);"
          << be_uidt_nl;
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
          << bt->nested_type_name (this->ctx_->scope ()) 
          << "::_nil ());" << be_uidt_nl;

      // Locality constraint objects alway use "direct" collocated
      // implementation.
      *os << node->full_name () << "_ptr retval = 0;" << be_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "retval," << be_nl
          << node->full_coll_name (be_interface::DIRECT) << " ("
          << "ACE_reinterpret_cast (POA_" 
          << bt->nested_type_name (this->ctx_->scope ())
          << "_ptr, servant), 0)," << be_nl
          << "0" << be_uidt_nl << ");" << be_uidt_nl
          << "return retval;" << be_uidt_nl;
    }
  *os << "}\n" << be_nl;


  // The _duplicate method
  *os << node->full_name () << "_ptr " << be_nl
      << node->full_name () << "::_duplicate ("
      << bt->nested_type_name (this->ctx_->scope ()) 
      << "_ptr obj)" << be_nl
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

  // Smart Proxy classes
  be_visitor_context ctx (*this->ctx_);
  be_visitor *visitor = 0;
  
  ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for smart proxy classes failed\n"),
                            -1);
    }
  delete visitor;
  visitor = 0;
  
  // by using a visitor to declare and define the TypeCode, we have the
  // added advantage to conditionally not generate any code. This will be
  // based on the command line options. This is still TO-DO
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

 
  return 0;
}
