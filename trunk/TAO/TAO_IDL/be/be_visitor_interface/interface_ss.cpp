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

  if (node->srv_skel_gen () || node->imported ())
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

  // Collocation function pointer initializer.
  *os << node->name () << "_ptr _TAO_collocation_POA_"
      << node->flatname () << "_Stub_Factory (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_uidt_nl
      << ")\n";

  os->incr_indent (0);
  *os << "{" << be_idt_nl
      << "TAO_Stub *stub = obj->_stubobj ();" << be_nl << be_nl
      << "switch (stub->servant_orb_var ()->orb_core"
      << " ()->get_collocation_strategy ())" << be_idt_nl
      << "{" << be_nl << "case TAO_ORB_Core::THRU_POA:" << be_idt_nl;

  if (idl_global->gen_thru_poa_collocation ())
    *os << "return new " << node->full_coll_name (be_interface::THRU_POA)
        << " (stub);" << be_uidt_nl;
  else
    *os << "break;" << be_uidt_nl;

  *os << "case TAO_ORB_Core::DIRECT:" << be_idt_nl;

  if (idl_global->gen_direct_collocation ())
    *os << "{" << be_idt_nl
        << "void *servant = ACE_reinterpret_cast (void*, obj->_servant ());" << be_nl
        << "if (servant != 0)" << be_idt_nl
        << "return new " << node->full_coll_name (be_interface::DIRECT)
        << " (ACE_reinterpret_cast (" << node->full_skel_name () << "*, servant)"
        << ", stub);" << be_uidt << be_uidt_nl << "}" << be_uidt_nl;

  *os << "break;" << be_uidt_nl << "default:" << be_idt_nl << "break;" << be_uidt_nl
        << "}" << be_uidt_nl << "return 0;" << be_uidt_nl << "}\n\n";

  *os << "int _TAO_collocation_POA_" << node->flatname ()
      << "_Stub_Factory_Initializer"
      << " (void *dummy)" << be_nl
      << "{" << be_idt_nl
      << "ACE_UNUSED_ARG (dummy);" << be_nl << be_nl
      << "_TAO_collocation_" << node->flatname ()
      << "_Stub_Factory_function_pointer = " << be_idt_nl
      << "_TAO_collocation_POA_" << node->flatname ()
      << "_Stub_Factory;" << be_uidt_nl << be_nl
      << "return 0;" << be_uidt_nl << "}\n\n";

  *os << "int _TAO_collocation_POA_" << node->flatname ()
      << "_Stub_Factory_Initializer_Scarecrow = " << be_idt_nl
      << "_TAO_collocation_POA_" << node->flatname ()
      << "_Stub_Factory_Initializer (ACE_reinterpret_cast (void*, _TAO_collocation_POA_"
      << node->flatname () << "_Stub_Factory_Initializer));" << be_uidt_nl;

  os->incr_indent (0);

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

  *os << "{" << be_idt_nl
      << "this->optable_ = &tao_" << node->flatname ()
      << "_optable;" << be_uidt_nl
      << "}\n\n";

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
      << "{}\n" << be_nl;

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
  *os << "{\n";
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
      << "CORBA::ServerRequest &_tao_server_request, " << be_nl
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
  *os << "if (!((_tao_in >> value.out ())))" << be_idt_nl;
  *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl << be_nl;
  *os << "_tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);" << be_nl;
  *os << "ACE_CHECK;" << be_nl << be_nl;
  *os << "_tao_server_request.init_reply (ACE_TRY_ENV);" << be_nl;
  *os << "ACE_CHECK;" << be_nl;
  *os << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();" << be_nl;
  *os << "if (!((_tao_out << CORBA::Any::from_boolean (_tao_retval))))" << be_idt_nl;
  *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt << be_uidt_nl;
  *os << "}\n\n";


  // generate code for the _non_existent skeleton
  os->indent ();
  *os << "void " << node->full_skel_name ()
      << "::_non_existent_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_object_reference," << be_nl
      << "void * /* context */," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << node->full_skel_name () << " *_tao_impl = ("
      << node->full_skel_name () << " *) _tao_object_reference;" << be_nl;
  *os << "CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);" << be_nl;
  *os << "ACE_CHECK;" << be_nl << be_nl;
  *os << "_tao_server_request.init_reply (ACE_TRY_ENV);" << be_nl;
  *os << "ACE_CHECK;" << be_nl;
  *os << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();" << be_nl;
  *os << "if (!((_tao_out << CORBA::Any::from_boolean (_tao_retval))))" << be_idt_nl;
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
      << "}\n\n";

  // the downcast method.
  os->indent ();
  *os << "void* " << node->full_skel_name ()
      << "::_downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->traverse_inheritance_graph (be_interface::downcast_helper, os) == -1)
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
      << "}\n\n";


  // now the dispatch method
  os->indent ();
  *os << "void " << node->full_skel_name () <<
    "::_dispatch (CORBA::ServerRequest &req, " <<
    "void *context, CORBA::Environment &ACE_TRY_ENV)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "TAO_Skeleton skel; // pointer to skeleton for operation" << be_nl;
  *os << "const char *opname = req.operation (); // retrieve operation name"
      << be_nl;
  *os << "// find the skeleton corresponding to this opname" << be_nl;
  *os << "if (this->_find (opname, skel, req.operation_length ()) == -1)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "ACE_ERROR ((LM_ERROR, \"Bad operation <%s>\\n\", opname));" << be_nl;
  *os << "ACE_THROW (CORBA_BAD_OPERATION ());"
    //<< "ACE_TRY_ENV);" << be_uidt_nl;
      << be_uidt_nl;
    //  *os << "env.exception (new CORBA_BAD_OPERATION ());" << be_nl;
  *os << "}" << be_nl;
  *os << "else" << be_idt_nl;
  *os << "skel (req, this, context, ACE_TRY_ENV);" << be_uidt << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  *os << "const char* " << node->full_skel_name ()
      << "::_interface_repository_id (void) const"
      << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return \"" << node->repoID () << "\";\n";
  os->decr_indent ();
  *os << "}\n\n";

  *os << "\n";

  // the _this () operation
  *os << node->name () << "*" << be_nl
      << node->full_skel_name ()
      << "::_this (CORBA_Environment &ACE_TRY_ENV)" << be_nl
      << "{" << be_idt_nl
      << "TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK_RETURN (0);" << be_nl
      << "switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())" << be_idt_nl
      << "{" << be_nl
      << "case TAO_ORB_Core::THRU_POA:" << be_idt_nl;

  // Thru POA stub
  if (idl_global->gen_thru_poa_collocation ())
    *os << "return new "
        << node->full_coll_name (be_interface::THRU_POA) << " (stub);" << be_uidt_nl;
  else
    *os << "ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);" << be_uidt_nl;

  // Direct stub
  *os << "case TAO_ORB_Core::DIRECT:" << be_idt_nl;
  if (idl_global->gen_direct_collocation ())
    *os << "return new "
        << node->full_coll_name (be_interface::DIRECT) << " (this, stub);" << be_uidt_nl;
  else
    *os << "ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);" << be_uidt_nl;

  *os << "default:" << be_idt_nl
      << "ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}\n\n";

  // the _create_collocated_objref method.  If the idl compiler does
  // not generate the type of collocated stub but the orb is asking
  // for it, simply return null so a remote stub will be used.
  os->indent ();

  // generate the collocated class impl
  if (idl_global->gen_thru_poa_collocation ())
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

  if (idl_global->gen_direct_collocation ())
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

  *os << "\n\n";

  return 0;
}
