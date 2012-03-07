//=============================================================================
/**
 *  @file    interface_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the client stubs file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "interface.h"

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
  if (node->imported () || node->cli_stub_gen ())
    {
      return 0;
    }

  be_type *bt = 0;

  // Set the right type.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  AST_Component *c = AST_Component::narrow_from_decl (node);
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->is_defined () && be_global->gen_arg_traits ())
    {
      *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      *os << be_nl_2
          << "// Traits specializations for " << node->name () << ".";

      *os << be_nl_2
          << node->name () << "_ptr" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::duplicate ("
          << be_idt << be_idt_nl
          << node->name () << "_ptr p)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "return " << node->name () << "::_duplicate (p);" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "void" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::release ("
          << be_idt << be_idt_nl
          << node->name () << "_ptr p)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl;

      // Workaround for broken HP V7.4-004 on OpenVMS IA83
      if (node->has_mixed_parentage ())
        {
          *os << "::CORBA::AbstractBase_ptr abs = p;" << be_nl
              << "::CORBA::release (abs);" << be_uidt_nl;
        }
      else
        {
          *os << "::CORBA::release (p);" << be_uidt_nl;
        }

      *os << "}";

      *os << be_nl_2
          << node->name () << "_ptr" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::nil (void)"
          << be_nl
          << "{" << be_idt_nl
          << "return " << node->name () << "::_nil ();" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "::CORBA::Boolean" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::marshal ("
          << be_idt << be_idt_nl
          << "const " << node->name () << "_ptr p," << be_nl
          << "TAO_OutputCDR & cdr)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "return ";


      if (node->is_abstract () || c != 0)
        {
          *os << "cdr << p;";
        }
      else
        {
          *os << "::CORBA::Object::marshal (p, cdr);";
        }

      *os << be_uidt_nl
          << "}";
    }

  // If we are generating CORBA Policy we need to add some more methods
  if (ACE_OS::strcmp (node->full_name (), "CORBA::Policy") == 0)
    {
      *os << be_nl
          << "CORBA::Boolean" << be_nl
          << "CORBA::Policy::_tao_encode (TAO_OutputCDR &)" << be_nl
          << "{" << be_nl
          << "  return false;" << be_nl
          << "}" << be_nl << be_nl
          << "CORBA::Boolean" << be_nl
          << "CORBA::Policy::_tao_decode (TAO_InputCDR &)" << be_nl
          << "{" << be_nl
          << "  return false;" << be_nl
          << "}" << be_nl << be_nl
          << "TAO_Cached_Policy_Type" << be_nl
          << "CORBA::Policy::_tao_cached_type (void) const" << be_nl
          << "{" << be_nl
            << "return TAO_CACHED_POLICY_UNCACHED;" << be_nl
          << "}" << be_nl << be_nl
          << "TAO_Policy_Scope" << be_nl
          << "CORBA::Policy::_tao_scope (void) const" << be_nl
          << "{" << be_nl
          << "  return TAO_POLICY_DEFAULT_SCOPE;" << be_nl
          << "}" << be_nl;
    }

  if (c == 0 && be_global->gen_ostream_operators ())
    {
      *os << be_nl_2
          << "std::ostream &" << be_nl
          << node->name () << "::_tao_stream_v (std::ostream &strm) const"
          << be_nl
          << "{" << be_idt_nl
          << "return strm << \"\\\"" << node->repoID () << "\\\"\";"
          << be_uidt_nl
          << "}";
    }

  if (node->has_mixed_parentage ())
    {
      *os << be_nl_2
          << "void" << be_nl
          << "CORBA::release ("
          << node->name ()
          << "_ptr p)" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::AbstractBase_ptr abs = p;" << be_nl
          << "::CORBA::release (abs);" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "::CORBA::Boolean" << be_nl
          << "CORBA::is_nil ("
          << node->name ()
          << "_ptr p)" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::Object_ptr obj = p;" << be_nl
          << "return ::CORBA::is_nil (obj);" << be_uidt_nl
          << "}";
    }

  // Generate code for the elements of the interface.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_cs::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  if (node->is_local ())
    {
      *os << be_nl_2
          << node->name () << "::" << node->local_name ()
          << " (void)" << be_nl
          << "{}";
    }

  if (! node->is_abstract () && ! node->is_local ())
    {
      *os << be_nl_2
          << node->name () << "::" << node->local_name ()
          << " (void)" << be_nl;

      *os << "{" << be_nl;

      *os << "}";
    }

  *os << be_nl_2
      << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl;
  *os << "{" << be_nl << "}" << be_nl_2;

  bool gen_any_destructor =
    be_global->any_support ()
    && (!node->is_local ()
        || be_global->gen_local_iface_anyops ());

  if (gen_any_destructor)
    {
      *os << "void" << be_nl
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *_tao_tmp_pointer =" << be_idt_nl
          << "static_cast<"
          << node->local_name () << " *> (_tao_void_pointer);" << be_uidt_nl
          << "::CORBA::release (_tao_tmp_pointer);" << be_uidt_nl
          << "}" << be_nl_2;
    }

  if (node->has_mixed_parentage ())
    {
      *os << "void" << be_nl
          << node->name () << "::_add_ref (void)" << be_nl
          << "{" << be_idt_nl
          << "this->::CORBA::Object::_add_ref ();"
          << be_uidt_nl
          << "}" << be_nl_2;
    }

  // The _narrow method
  if (! this->gen_xxx_narrow ("narrow", node))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_cs::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("_narrow () method codegen failed\n")),
                        -1);
    }

  // The _unchecked_narrow method, not for components.
  if (c == 0 && ! this->gen_xxx_narrow ("unchecked_narrow", node))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_cs::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("_unchecked_narrow () method codegen")
                         ACE_TEXT (" failed\n")),
                        -1);
    }

  // The _nil method
  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_nil (void)"
      << be_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl_2;

  // The _duplicate method
  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_duplicate ("
      << bt->local_name ()
      << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "obj->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt_nl
      << "return obj;" << be_uidt_nl
      << "}" << be_nl_2;

  // The _tao_release method
  if (c == 0)
    {
      *os << "void" << be_nl
          << node->full_name () << "::_tao_release ("
          << bt->local_name ()
          << "_ptr obj)" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::release (obj);" << be_uidt_nl
          << "}" << be_nl_2;
    }

  *os << "::CORBA::Boolean" << be_nl
      << node->full_name () << "::_is_a (const char *value)" << be_nl;

  *os << "{" << be_idt_nl
      << "if (" << be_idt << be_idt_nl;

  int status = node->gen_is_a_ancestors (os);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_cs::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("gen_is_a_ancestors() failed\n")),
                        -1);
    }

  *os << ")" << be_nl
      << "{" << be_idt_nl
      << "return true; // success using local knowledge" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl;

  if (node->is_abstract () || node->is_local ())
    {
      *os << "return false;" << be_uidt_nl;
    }
  else
    {
      *os << "return this->::CORBA::Object::_is_a (value);" << be_uidt_nl;
    }

  *os << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "const char* " << node->full_name ()
      << "::_interface_repository_id (void) const"
      << be_nl
      << "{" << be_idt_nl
      << "return \"" << node->repoID ()
      << "\";" << be_uidt_nl
      << "}";

  bool is_loc = node->is_local ();

  *os << be_nl_2
      << "::CORBA::Boolean" << be_nl
      << node->name () << "::marshal (TAO_OutputCDR &"
      << (is_loc ? " /* " : "") << "cdr"
      << (is_loc ? " */" : "") << ")" << be_nl
      << "{" << be_idt_nl
      << "return "
      << (is_loc ? "false" : "(cdr << this)")
      << ";" << be_uidt_nl
      << "}";

  if (! node->is_abstract ())
    {
      // Smart Proxy classes.
      if (! node->is_local () && be_global->gen_smart_proxies ())
        {
          be_visitor_context ctx (*this->ctx_);

          ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS);
          be_visitor_interface_smart_proxy_cs isp_visitor (&ctx);

          if (node->accept (&isp_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_interface_cs::")
                                 ACE_TEXT ("visit_interface - ")
                                 ACE_TEXT ("codegen for smart ")
                                 ACE_TEXT ("proxy classes failed\n")),
                                -1);
            }
        }
    }

  if (be_global->tc_support ())
    {

      be_visitor_context ctx = *this->ctx_;
      TAO::be_visitor_objref_typecode tc_visitor (&ctx);

      if (node->accept (&tc_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_cs::")
                             ACE_TEXT ("visit_interface - ")
                             ACE_TEXT ("TypeCode definition failed\n")),
                            -1);
        }
    }

  return 0;
}

bool
be_visitor_interface_cs::gen_xxx_narrow (const char *pre,
                                         be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_" << pre << " ("
      << be_idt << be_idt_nl;

  if (node->is_abstract ())
    {
      *os << "::CORBA::AbstractBase_ptr";
    }
  else
    {
      *os << "::CORBA::Object_ptr";
    }

  *os << " _tao_objref)"
      << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->is_local ())
    {
      *os << "return " << node->local_name ()
          << "::_duplicate (" << be_idt << be_idt_nl
          << "dynamic_cast<" << node->local_name ()
          << "_ptr> (_tao_objref)"
          << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_nl_2;

      return true;
    }
  else if (be_global->gen_smart_proxies () &&
           !node->is_abstract ())
    {
      *os << node->full_name () << " *proxy = 0;"
      << be_nl_2
          << "proxy = TAO::Narrow_Utils<"
          << node->local_name () << ">::" << pre << " (";

      *os << be_idt << be_idt_nl;

      *os << "_tao_objref";
      if (ACE_OS::strcmp (pre, "narrow") == 0)
        {
          *os << "," << be_nl
              << "\"" << node->repoID () << "\"";
        }

      *os << ");";

      *os << be_uidt << be_uidt << be_nl
          << "return TAO_" << node->flat_name ()
          << "_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (proxy);"
          << be_uidt << be_uidt_nl
          << "}" << be_nl_2;
    }
  else
    {
      *os << "return" << be_idt_nl;

      if (!node->is_abstract ())
        {
          *os << "TAO::Narrow_Utils<"
              << node->local_name () << ">::" << pre << " (";
        }
      else
        {
          *os << "TAO::AbstractBase_Narrow_Utils<"
              << node->local_name () << ">::" << pre <<" (";
        }

      *os << be_idt << be_idt_nl
          << "_tao_objref";

      if (ACE_OS::strcmp (pre, "narrow") == 0)
        {
          *os << "," << be_nl
              << "\"" << node->repoID () << "\"";
        }

      *os << ");";

      *os << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "}" << be_nl_2;
    }

  return true;
}

int
be_visitor_interface_cs::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_cs::visit_connector (be_connector *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_cs::visit_extended_port (be_extended_port *node)
{
  this->ctx_->port_prefix () = node->local_name ()->get_string ();
  this->ctx_->port_prefix () += '_';

  /// If the port visit traverses any attributes defined in the
  /// original porttype, this is a way for visitors down the
  /// line to tell what scope we are actually in.
  this->ctx_->interface (
    be_interface::narrow_from_scope (node->defined_in ()));

  /// Will ignore everything but porttype attributes.
  int status = this->visit_scope (node->port_type ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ch")
                         ACE_TEXT ("::visit_extended_port - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  /// Reset port prefix string.
  this->ctx_->port_prefix () = "";
  return 0;
}

int
be_visitor_interface_cs::visit_mirror_port (be_mirror_port *node)
{
  this->ctx_->port_prefix () = node->local_name ()->get_string ();
  this->ctx_->port_prefix () += '_';

  /// If the port visit traverses any attributes defined in the
  /// original porttype, this is a way for visitors down the
  /// line to tell what scope we are actually in.
  this->ctx_->interface (
    be_interface::narrow_from_scope (node->defined_in ()));

  /// Will ignore everything but porttype attributes.
  int status = this->visit_scope (node->port_type ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ch")
                         ACE_TEXT ("::visit_mirror_port - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  /// Reset port prefix string.
  this->ctx_->port_prefix () = "";
  return 0;
}

int
be_visitor_interface_cs::gen_abstract_ops_helper (be_interface *node,
                                                  be_interface *base,
                                                  TAO_OutStream *os)
{
  // If the derived interface is local, the abstract parent's operation
  // was generated as pure virtual.
  if (!base->is_abstract () || node->is_local ())
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cs::"
                             "gen_abstract_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      if (d->node_type () == AST_Decl::NT_op)
        {
          UTL_ScopedName *item_new_name  = 0;
          ACE_NEW_RETURN (item_new_name,
                          UTL_ScopedName (d->local_name ()->copy (),
                                          0),
                          -1);

          UTL_ScopedName *new_op_name =
            (UTL_ScopedName *)node->name ()->copy ();
          new_op_name->nconc (item_new_name);

          be_operation *op = be_operation::narrow_from_decl (d);
          UTL_ScopedName *old_op_name =
            (UTL_ScopedName *) op->name ()->copy ();
          op->set_name (new_op_name);
          op->set_defined_in (node);
          op->is_abstract (node->is_abstract ());

          ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
          be_visitor_operation_cs op_visitor (&ctx);
          op_visitor.visit_operation (op);

          op->set_name (old_op_name);
          op->set_defined_in (base);
          op->is_abstract (base->is_abstract ());
        }
    }

  return 0;
}
