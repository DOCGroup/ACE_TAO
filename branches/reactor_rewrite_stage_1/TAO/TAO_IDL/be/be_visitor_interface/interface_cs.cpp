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
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  be_type *bt;

  // Set the right type.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Initialize the static narrrowing helper variable.
  *os << "int " << node->full_name () << "::_tao_class_id = 0;"
      << be_nl << be_nl;

  // Global functions to allow non-defined forward declared interfaces
  // access to some methods in the full definition.
  *os << node->full_name () << "_ptr" << be_nl
      << "tao_" << node->flat_name ()
      << "_duplicate (" << be_idt << be_idt_nl
      << node->full_name () << "_ptr p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name ()
      << "::_duplicate (p);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "void" << be_nl
      << "tao_" << node->flat_name ()
      << "_release (" << be_idt << be_idt_nl
      << node->full_name () << "_ptr p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::release (p);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << node->full_name () <<  "_ptr" << be_nl
      << "tao_" << node->flat_name ()
      << "_nil (" << be_idt << be_idt_nl
      << "void" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name ()
      << "::_nil ();" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << node->full_name () << "_ptr" << be_nl
      << "tao_" << node->flat_name ()
      << "_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object *p" << be_nl
      << "TAO_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name ()
      << "::_narrow (p TAO_ENV_ARG_PARAMETER);"
      << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "CORBA::Object *" << be_nl
      << "tao_" << node->flat_name ()
      << "_upcast (" << be_idt << be_idt_nl
      << "void *src" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << node->full_name () << " **tmp =" << be_idt_nl
      << "ACE_static_cast (" << node->full_name ()
      << " **, src);" << be_uidt_nl
      << "return *tmp;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generate the _var class.
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for _var failed\n"), -1);
    }

  // Generate the _out class.
  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for _out failed\n"), -1);
    }

  be_visitor *visitor = 0;
  be_visitor_context ctx;

  // Interceptor classes.  The interceptors helper classes must be
  // defined before the interface operations because they are used in
  // the implementation of said operations.

  ctx = (*this->ctx_);
  visitor = 0;

  ctx.state (TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS);
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

  if (!node->is_local ())
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS);
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

      visitor = 0;
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS);
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

   // Generate the destructor and default constructor.
  *os << be_nl;
  *os << "// default constructor" << be_nl;
  *os << node->name () << "::" << node->local_name ();
  if (!node->is_local ())
    {
      *os << " (int collocated)" << be_nl
          << "{" << be_idt_nl
          << "this->" << node->flat_name ()
          << "_setup_collocation (collocated);" << be_uidt_nl
          << be_uidt << "}" << be_nl << be_nl;
    }
  else
    {
      *os << " (void)" << be_nl
          << "{}" << be_idt_nl;
    }

  *os << "// destructor" << be_nl;
  *os << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl;
  *os << "{}" << be_nl << be_nl;

  if (!node->is_local ())
    {
      // Collocation setup method.
      *os << "void" << be_nl
          << node->name () << "::" << node->flat_name ()
          << "_setup_collocation (int collocated)" << be_nl
          << "{" << be_idt_nl
          << "if (collocated)" << be_idt_nl
          << "this->the" << node->base_proxy_broker_name ()
          << "_ =" << be_idt_nl
          << "::" << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer (this);"
          << be_uidt << be_uidt_nl
          << "else" << be_idt_nl
          << "this->the" << node->base_proxy_broker_name ()
          << "_ =" << be_idt_nl
          << "::" << node->full_remote_proxy_broker_name ()
          << "::the" << node->remote_proxy_broker_name ()
          << " ();" << be_uidt << be_uidt;

      // Now we setup the immediate parents.
      int n_parents = node->n_inherits ();

      if (n_parents > 0)
        {
          *os << be_nl;

          for (int i = 0; i < n_parents; i++)
            {
              be_interface *inherited =
                be_interface::narrow_from_decl (node->inherits ()[i]);

              *os << be_nl
                  << "this->" << inherited->flat_name ()
                  << "_setup_collocation" << " (collocated);";

              if (i == n_parents - 1)
                {
                  *os << be_uidt_nl;
                }
              else
                {
                  *os << be_nl;
                }
            }
        }
      else
        {
          *os << be_uidt_nl;
        }

      *os << "}" << be_nl << be_nl;
    }

  // Then generate the code for the static methods
  // Local interfaces don't have any operators.
  if (! node->is_local ())
    {
      *os << "void "
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *tmp = ACE_static_cast ("
          << node->local_name () << "*, _tao_void_pointer);" << be_nl
          << "CORBA::release (tmp);" << be_uidt_nl
          << "}\n" << be_nl;
    }

  // The _narrow method

  *os << node->full_name () << "_ptr " << node->full_name ()
      << "::_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_nl
      << "TAO_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  // Local interfaces have slightly different _narrow implementation.
  if (! node->is_local ())
    {
      // Remote _narrow implementation.
      *os << "if (CORBA::is_nil (obj))" << be_idt_nl
          << "return " << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ();" << be_uidt_nl;

      *os << "if (! obj->_is_local ())" << be_idt_nl
          << "{" << be_idt_nl
          << "CORBA::Boolean is_a = obj->_is_a (\""
          << node->repoID () << "\" TAO_ENV_ARG_PARAMETER);" << be_nl
          << "ACE_CHECK_RETURN (" << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ());" << be_nl
          << "if (is_a == 0)" << be_idt_nl
          << "return " << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ();" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl;
    }

  *os << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_unchecked_narrow (obj TAO_ENV_ARG_PARAMETER);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The _unchecked_narrow method

  *os << node->full_name () << "_ptr " << node->full_name ()
      << "::_unchecked_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_nl
      << "TAO_ENV_ARG_DECL_NOT_USED" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ();" << be_uidt_nl;


  if (! node->is_local ())
    {
      // Remote _uncheck_narrow implementation.
      *os << "if (! obj->_is_local ())" << be_idt_nl
          << "{" << be_idt_nl
          << "TAO_Stub* stub = obj->_stubobj ();" << be_nl
          << "if (stub)" << be_idt_nl
          << "stub->_incr_refcnt ();" << be_uidt_nl
          // Declare the default proxy.
          << bt->nested_type_name (this->ctx_->scope ())
          << "_ptr default_proxy = "
          << bt->nested_type_name (this->ctx_->scope ())
          <<"::_nil ();\n\n";

      // If the policy didtates that the proxy be collocated, use the
      // function to create one.
      os->indent ();
      *os << "if (" << be_idt << be_idt_nl // 2 idt
          << "!CORBA::is_nil (stub->servant_orb_var ().ptr ()) &&" << be_nl
          << "stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects () &&"
          << be_nl
          << "obj->_is_collocated () &&" << be_nl
          << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
          << "_Factory_function_pointer != 0" << be_uidt_nl << ")"  // 1 idt
          << be_nl << "{" // 0 idt
          << be_idt_nl   // 1 idt
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl  // 2 idt
          << "default_proxy," << be_nl
          << "::" <<  bt->name ()
          << " (" << be_idt << be_idt_nl  // 3 idt
          << "stub," << be_nl
          << "1," << be_nl
          << "obj->_servant ()" << be_uidt_nl << ")," << be_uidt_nl
          <<  bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ()" << be_uidt_nl << ");"
          << be_uidt << be_uidt_nl   // 1 idt
          << "}" << be_uidt_nl << be_nl; // 0 idt


      // The default proxy will either be returned else be transformed to
      // a smart one!
      *os << "if (CORBA::is_nil (default_proxy))" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "default_proxy," << be_nl
          << "::" << bt->name () << " (" << be_idt << be_idt_nl
          << "stub," << be_nl
          << "0," << be_nl
          << "obj->_servant ()" << be_uidt_nl
          << ")," << be_uidt_nl
          << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ()" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      if (be_global->gen_smart_proxies ())
        {
          *os << "return TAO_" << node->flat_name ()
              << "_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);"
              << be_uidt_nl;
        }
      else
        {
          *os << "return default_proxy;" << be_uidt_nl;
        }

      *os << "}" << be_uidt_nl
          << "else " << be_idt_nl;
    }
  else
    {
      *os << be_idt;
    }

  *os << "return" << be_idt_nl
      << "ACE_reinterpret_cast" << be_idt_nl
      <<"(" << be_idt_nl
      << node->local_name () << "_ptr," << be_idt_nl
      << "obj->_tao_QueryInterface" << be_idt_nl
      << "(" << be_idt_nl
      << "ACE_reinterpret_cast" << be_idt_nl
      << "(" << be_idt_nl
      << "ptr_arith_t," << be_nl
      << "&" << node->local_name ()
      << "::_tao_class_id" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << ")" << be_uidt << be_uidt << be_uidt_nl
      << ");\n" << be_uidt << be_uidt << be_uidt << be_uidt << be_uidt;

  *os << "}\n" << be_nl;


  // The _duplicate method
  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_duplicate ("
      << bt->nested_type_name (this->ctx_->scope ())
      << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "if (!CORBA::is_nil (obj))" << be_idt_nl
      << "obj->_add_ref ();" << be_uidt_nl
      << "return obj;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // generate the is_a method.  _is_a is not supported on local objects.
  if (! node->is_local ())
    {
      os->indent ();
      *os << "CORBA::Boolean " << node->full_name ()
          << "::_is_a ("
          << "const CORBA::Char *value TAO_ENV_ARG_DECL)"
          << be_nl
          << "{\n";

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
      *os << "  return this->CORBA_Object::_is_a (value TAO_ENV_ARG_PARAMETER);\n";
      os->decr_indent ();
      *os << "}\n\n";
    }

  // Generating _tao_QueryInterface method.
  os->indent ();
  *os << "void *" << node->full_name ()
      << "::_tao_QueryInterface (ptr_arith_t type)" << be_nl
      << "{" << be_idt_nl
      << "void *retv = 0;" << be_nl
      << "if ";

  if (node->traverse_inheritance_graph (be_interface::queryinterface_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "_tao_QueryInterface method codegen failed\n"), -1);
    }

  *os << "(type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_tao_class_id))"
      << be_idt_nl << "retv = ACE_reinterpret_cast (void *," << be_idt_nl
      << "ACE_static_cast (CORBA::Object_ptr, this));" << be_uidt_nl << be_uidt_nl
      << "if (retv)" << be_idt_nl
      << "this->_add_ref ();" << be_uidt_nl
      << "return retv;" << be_uidt_nl
      << "}\n\n";

  os->indent ();
  *os << "const char* " << node->full_name ()
      << "::_interface_repository_id (void) const"
      << be_nl
      << "{" << be_idt_nl
      << "return \"" << node->repoID ()
      << "\";" << be_uidt_nl
      << "}\n\n";

  os->decr_indent (0);

  // generate code for the elements of the interface
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }


  // Smart Proxy classes.
  if (! node->is_local ())
    {
      be_visitor_context ctx (*this->ctx_);
      be_visitor *visitor = 0;

      ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
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

      delete visitor;
    }

  return 0;
}
