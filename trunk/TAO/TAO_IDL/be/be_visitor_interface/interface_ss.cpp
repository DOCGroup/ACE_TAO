//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the server skeletons file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_ss, "$Id$")


// ************************************************************
// Interface visitor for server skeletons
// ************************************************************

be_visitor_interface_ss::be_visitor_interface_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ss::~be_visitor_interface_ss (void)
{
}

int
be_visitor_interface_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (node->srv_skel_gen () || node->imported () || node->is_local ())
    return 0;

  os = this->ctx_->stream ();

  // generate the skeleton class name

  os->indent (); // start with whatever indentation level we are at

  if (node->gen_operation_table () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for operation table failed\n"),
                        -1);
    }

  // Strategized Proxy Broker Implementation
  be_visitor *visitor = 0;
  be_visitor_context ctx;

  if (be_global->gen_thru_poa_collocation () ||
      be_global->gen_direct_collocation ())
    {
      ctx =  (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_cs::"
                             "visit_interface - "
                             "codegen for Base Proxy Broker class failed\n"),
                            -1);
        }
      delete visitor;

      // Proxy Broker  Factory Function.
      *os << be_nl
          << node->full_base_proxy_broker_name () << " *" << be_nl
          << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
          << "_Factory_function (CORBA::Object_ptr obj)" << be_nl
          << "{" << be_idt_nl // idt = 1
          << "ACE_UNUSED_ARG (obj);" << be_nl
          << "return "
          << node->full_strategized_proxy_broker_name ()
          << "::" <<"the"
          << node->strategized_proxy_broker_name ()
          << "();" << be_uidt_nl // idt = 0
          << "}" << be_nl << be_nl;

      // Proxy Broker Function Pointer Initializer.
      *os << "int" << be_nl
          << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
          << "_Factory_Initializer (long _dummy_)" << be_nl
          << "{" << be_idt_nl // idt = 1
          << "ACE_UNUSED_ARG (_dummy_);" << be_nl << be_nl
          << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
          << "_Factory_function_pointer = "
          << be_idt_nl  // idt = 2
          << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
          << "_Factory_function;"
          << be_uidt_nl // idt = 1
          << be_nl
          << "return 0;" << be_uidt_nl // idt = 0
          << "}" << be_nl << be_nl;


      *os << "static int " <<  node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Stub_Factory_Initializer_Scarecrow = " << be_idt_nl
          << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
          << "_Factory_Initializer (ACE_reinterpret_cast (long, "
          << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
          << "_Factory_Initializer));"
          << be_uidt_nl << be_nl;
    }


  // Proxy Impl Implementations.
  if (be_global->gen_thru_poa_collocation ())
    {
      visitor = 0;
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_cs::"
                             "visit_interface - "
                             "codegen for Base Proxy Broker class failed\n"),
                            -1);
        }
      delete visitor;
    }
  if (be_global->gen_direct_collocation ())
    {
      visitor = 0;
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_cs::"
                             "visit_interface - "
                             "codegen for Base Proxy Broker class failed\n"),
                            -1);
        }
      delete visitor;
    }

  // constructor
  *os << "// skeleton constructor" << be_nl;
  // find if we are at the top scope or inside some module
  if (!node->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *os << node->full_skel_name () << "::POA_" << node->local_name () <<
        " (void)" << be_nl;
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *os << node->full_skel_name () << "::" << node->local_name () <<
        " (void)" << be_nl;
    }

  // Generate optable
  *os << "{" << be_idt_nl
      << "this->optable_ = &tao_" << node->flat_name ()
      << "_optable;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "// copy ctor" << be_nl;
  // find if we are at the top scope or inside some module
  if (!node->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *os << node->full_skel_name () << "::POA_"
          << node->local_name () << " ("
          << "const POA_" << node->local_name () << "& rhs)";
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *os << node->full_skel_name () << "::"
          << node->local_name () << " (const "
          << node->local_name () << "& rhs)";
    }
  *os << be_idt_nl
      << ": ";
  if (node->traverse_inheritance_graph
      (be_interface::copy_ctor_helper, os) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "be_visitor_interface_ss::visit_interface - "
                       " copy ctor generation failed\n"), -1);
  *os << "  TAO_ServantBase (rhs)" << be_uidt_nl
      << "{}" << be_nl << be_nl;

  *os << "// skeleton destructor" << be_nl;

  if (!node->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *os << node->full_skel_name () << "::~POA_" << node->local_name () <<
        " (void)" << be_nl;
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *os << node->full_skel_name () << "::~" << node->local_name () <<
        " (void)" << be_nl;
    }
  *os << "{" << be_nl;
  *os << "}\n\n";


  // generate code for elements in the scope (e.g., operations)
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // generate code for the _is_a skeleton
  os->indent ();
  *os << "void " << node->full_skel_name ()
      << "::_is_a_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_object_reference," << be_nl
      << "void * /* context */," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << "TAO_InputCDR &_tao_in = _tao_server_request.incoming ();" << be_nl;
  *os << node->full_skel_name () << " *_tao_impl = ("
      << node->full_skel_name () << " *) _tao_object_reference;" << be_nl;
  *os << "CORBA::Boolean _tao_retval = 0;" << be_nl;
  *os << "CORBA::String_var value;" << be_nl;
  *os << "if (!(_tao_in >> value.out ()))" << be_idt_nl;
  if (be_global->use_raw_throw ())
    *os << "throw CORBA::MARSHAL ();" << be_uidt_nl << be_nl;
  else
    *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl << be_nl;
  *os << "_tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);" << be_nl;
  *os << "ACE_CHECK;" << be_nl << be_nl;
  *os << "_tao_server_request.init_reply ();" << be_nl;
  *os << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();" << be_nl;
  *os << "if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))" << be_idt_nl;
  if (be_global->use_raw_throw ())
    *os << "throw CORBA::MARSHAL ();" << be_uidt << be_uidt_nl;
  else
    *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt << be_uidt_nl;
  *os << "}" << be_nl << be_nl;


      // generate code for the _non_existent skeleton
  *os << "void " << node->full_skel_name ()
      << "::_non_existent_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_object_reference," << be_nl
      << "void * /* context */," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << node->full_skel_name () << " *_tao_impl = ("
      << node->full_skel_name () << " *) _tao_object_reference;" << be_nl;
  *os << "CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);" << be_nl;
  *os << "ACE_CHECK;" << be_nl << be_nl;
  *os << "_tao_server_request.init_reply ();" << be_nl;
  *os << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();" << be_nl;
  *os << "if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))" << be_idt_nl;
  if (be_global->use_raw_throw ())
    *os << "throw CORBA::MARSHAL ();" << be_uidt << be_uidt_nl;
  else
    *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  *os << "CORBA::Boolean " << node->full_skel_name ()
      << "::_is_a (" << be_idt << be_idt_nl
      << "const char* value," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (\n" << be_idt;
  if (node->traverse_inheritance_graph (be_interface::is_a_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }

  os->indent ();
  *os << "(!ACE_OS::strcmp ((char *)value, "
      << "CORBA::_tc_Object->id (ACE_TRY_ENV))))"
      << be_idt_nl << "return 1;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return 0;" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // the downcast method.
  *os << "void* " << node->full_skel_name ()
      << "::_downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->traverse_inheritance_graph (be_interface::downcast_helper,
                                        os)
       == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }

  *os << "if (ACE_OS::strcmp (logical_type_id, "
      << "\"IDL:omg.org/CORBA/Object:1.0\") == 0)" << be_idt_nl
      << "return ACE_static_cast(PortableServer::Servant, this);"
      << be_uidt_nl;

  *os << "return 0;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // now the dispatch method
  *os << "void " << node->full_skel_name () <<
    "::_dispatch (TAO_ServerRequest &req, " <<
    "void *context, CORBA::Environment &ACE_TRY_ENV)" << be_nl;
  *os << "{" << be_idt_nl;
  //BRT
  *os << "this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);" << be_uidt_nl;
//  *os << "TAO_Skeleton skel; // pointer to skeleton for operation" << be_nl;
//  *os << "const char *opname = req.operation (); // retrieve operation name"
//      << be_nl;
//  *os << "// find the skeleton corresponding to this opname" << be_nl;
//  *os << "if (this->_find (opname, skel, req.operation_length ()) == -1)" << be_nl;
//  *os << "{" << be_idt_nl;
//  *os << "ACE_ERROR ((LM_ERROR, \"Bad operation <%s>\\n\", opname));" << be_nl;
//  if (idl_global->use_raw_throw ())
//    *os << "throw CORBA_BAD_OPERATION ();";
//  else
//    *os << "ACE_THROW (CORBA_BAD_OPERATION ());";
//  *os << be_uidt_nl;
//  *os << "}" << be_nl;
//  *os << "else" << be_idt_nl;
//  *os << "skel (req, this, context, ACE_TRY_ENV);" << be_uidt << be_uidt_nl;
  *os << "}" << be_nl << be_nl;

  *os << "const char* " << node->full_skel_name ()
      << "::_interface_repository_id (void) const"
      << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return \"" << node->repoID () << "\";" << be_uidt_nl;
  *os << "}" << be_nl << be_nl;

  // the _this () operation
  *os << node->full_name () << "*" << be_nl
      << node->full_skel_name ()
      << "::_this (CORBA_Environment &ACE_TRY_ENV)" << be_nl
      << "{" << be_idt_nl // idt = 1
      << "TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK_RETURN (0);" << be_nl << be_nl;

  *os << "CORBA::Object_ptr tmp = CORBA::Object::_nil ();" << be_nl
      << be_nl
      << "if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())"
      << be_idt_nl // idt = 2
      << "ACE_NEW_RETURN (tmp, CORBA::Object (stub, 1, this), 0);"
      << be_uidt_nl // idt = 1
      << "else"
      << be_idt_nl // idt = 2
      << "ACE_NEW_RETURN (tmp, CORBA::Object (stub, 0, this), 0);"
      << be_uidt_nl << be_nl // idt = 1
      << "CORBA::Object_var obj = tmp;" << be_nl
      << "return " << "::" << node->full_name () << "::_unchecked_narrow (obj.in ());"
      << be_uidt_nl // idt = 0
      << "}" << be_nl;

  // the _create_collocated_objref method.  If the idl compiler does
  // not generate the type of collocated stub but the orb is asking
  // for it, simply return null so a remote stub will be used.
  // generate the collocated class impl

  /*
  if (be_global->gen_thru_poa_collocation ())
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ss::"
                             "visit_interface - "
                             "Bad visitor for thru_poa collocated class\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ss::"
                             "visit_interface - "
                             "codegen for thru_poa collocated class failed\n"),
                            -1);
        }
      delete visitor;
    }

  if (be_global->gen_direct_collocation ())
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ss::"
                             "visit_interface - "
                             "Bad visitor for direct collocated class\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ss::"
                             "visit_interface - "
                             "codegen for direct collocated class failed\n"),
                            -1);
        }
      delete visitor;
    }
  */
  *os << "\n\n";

  // Interceptor classes

  ctx = *this->ctx_;
  visitor = 0;

  ctx.state (TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for interceptors classes failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;

  return 0;
}
