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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"


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
	  << "POA_" << node->local_name () << "& rhs)";
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *os << node->full_skel_name () << "::"
	  << node->local_name () << " ("
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
      << "{}\n";

  // destructor
  os->indent ();
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
  *os << "}\n";


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
      << "void * /*context*/," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "static const TAO_Param_Data_Skel " << node->flatname ()
      << "_is_a_paramdata [] = " << be_nl;
  *os << "{" << be_idt_nl;
  *os << "{CORBA::_tc_boolean, 0, 0}," << be_nl;
  *os << "{CORBA::_tc_string, CORBA::ARG_IN, 0}" << be_uidt_nl;
  *os << "};" << be_nl;
  *os << "static const TAO_Call_Data_Skel " << node->flatname ()
      << "_is_a_calldata = " << be_nl;
  *os << "{\"_is_a\", 1, 2, " << node->flatname () << "_is_a_paramdata};"
      << be_nl;
  *os << "CORBA::Environment _tao_skel_environment;" << be_nl;
  *os << node->full_skel_name () << "_ptr  _tao_impl = ("
      << node->full_skel_name () << "_ptr) _tao_object_reference;"
      << be_nl;
  *os << "CORBA::Boolean _tao_retval;" << be_nl;
  *os << "char *_tao_value = 0;" << be_nl;
  *os << "_tao_server_request.demarshal (" << be_idt_nl
      << "_tao_environment, " << be_nl
      << "&" << node->flatname () << "_is_a_calldata, " << be_nl
      << "&_tao_retval, " << be_nl
      << "&_tao_value" << be_uidt_nl
      << ");" << be_nl;
  *os << "if (_tao_environment.exception () != 0) return;" << be_nl;
  *os << "_tao_retval = _tao_impl->_is_a (_tao_value, "
      << "_tao_skel_environment);" << be_nl;
  *os << "_tao_server_request.marshal (" << be_idt_nl
      << "_tao_environment, " << be_nl
      << "_tao_skel_environment," << be_nl
      << "&" << node->flatname () << "_is_a_calldata, " << be_nl
      << "&_tao_retval, " << be_nl
      << "&_tao_value" << be_uidt_nl
      << ");" << be_nl;
  *os << "CORBA::string_free (_tao_value);" << be_uidt_nl;
  *os << "}\n\n";


  os->indent ();
  *os << "CORBA::Boolean " << node->full_skel_name ()
      << "::_is_a (" << be_idt << be_idt_nl
      << "const char* value," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
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
      << "CORBA::_tc_Object->id (_tao_environment))))"
      << be_idt_nl << "return CORBA::B_TRUE;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return CORBA::B_FALSE;" << be_uidt << be_uidt << be_uidt_nl
      << "}\n\n";

  // generate code for the _non_existent skeleton
  os->indent ();
  *os << "void " << node->full_skel_name ()
      << "::_non_existent_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_object_reference," << be_nl
      << "void * /*context*/," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << "static const TAO_Param_Data_Skel " << node->flatname ()
      << "_non_existent_paramdata [] = " << be_nl
      << "{" << be_idt_nl
      << "{CORBA::_tc_boolean, 0, 0}" << be_uidt_nl
      << "};" << be_nl;
  *os << "static const TAO_Call_Data_Skel " << node->flatname ()
      << "_non_existent_calldata = " << be_nl
      << "{\"_non_existent\", 1, 1, " << node->flatname ()
      << "_non_existent_paramdata};" << be_nl;
  *os << "CORBA::Environment _tao_skel_environment;" << be_nl;
  *os << "CORBA::Boolean _tao_retval = CORBA::B_FALSE;" << be_nl;
  *os << "_tao_server_request.marshal (" << be_idt_nl
      << "_tao_environment, " << be_nl
      << "_tao_skel_environment," << be_nl
      << "&" << node->flatname () << "_non_existent_calldata, " << be_nl
      << "&_tao_retval " << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << "}\n\n";

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
    "void *context, CORBA::Environment &env)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "TAO_Skeleton skel; // pointer to skeleton for operation" << be_nl;
  *os << "const char *opname = req.operation (); // retrieve operation name"
      << be_nl;
  *os << "// find the skeleton corresponding to this opname" << be_nl;
  *os << "if (this->_find (opname, skel) == -1)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "env.exception (new CORBA_BAD_OPERATION (CORBA::COMPLETED_NO));"
      << be_nl;
  *os << "ACE_ERROR ((LM_ERROR, \"Bad operation <%s>\\n\", opname));\n";
  os->decr_indent ();
  *os << "}\n";
  *os << "else" << be_nl;
  *os << "  skel (req, this, context, env);\n";
  os->decr_indent ();
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

  // generate the collocated class impl
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "Bad visitor for collocated class\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for collocated class failed\n"),
                        -1);
    }
  delete visitor;

  *os << "\n";

  // the _this () operation
  *os << node->name () << "*" << be_nl
      << node->full_skel_name ()
      << "::_this (CORBA_Environment &_env)" << be_nl
      << "{" << be_idt_nl
      << "STUB_Object *stub = this->_create_stub (_env);" << be_nl
      << "if (_env.exception () != 0)" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "return new " << node->full_coll_name ()
      << " (this, stub);" << be_uidt << be_nl;

  *os << "}\n\n";

   return 0;
}
