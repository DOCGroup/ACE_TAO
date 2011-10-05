
//=============================================================================
/**
 *  @file    interface_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the client header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ******************************************************
// Interface visitor for client header
// ******************************************************

be_visitor_interface_ch::be_visitor_interface_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ch::~be_visitor_interface_ch (void)
{
}

int
be_visitor_interface_ch::visit_interface (be_interface *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // This will be a no-op if it has already been done by a forward
  // declaration.
  node->gen_var_out_seq_decls ();

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  AST_Component *c = AST_Component::narrow_from_decl (node);

  // Now generate the class definition.
  *os << be_nl_2
      << "class " << be_global->stub_export_macro ()
      << " " << node->local_name () << be_idt_nl
      << ": ";

  node->gen_stub_inheritance (os);

  // Generate the body.

  *os << be_nl
      << "{" << be_nl
      << "public:" << be_idt;

  if (!node->is_local ())
    {
      bool abs = node->is_abstract ();

      *os << be_nl
          << "friend class TAO::"
          << (abs ? "AbstractBase_" : "") << "Narrow_Utils<"
          << node->local_name () << ">;";
    }

  node->gen_stub_decls (os);

  // Generate the static _duplicate, _narrow, _unchecked_narrow and
  // _nil operations.
  *os << be_nl_2
      << "// The static operations." << be_nl
      << "static " << node->local_name () << "_ptr " << "_duplicate ("
      << node->local_name () << "_ptr obj);" << be_nl_2;

  if (c == 0)
    {
      *os << "static void _tao_release ("
          << node->local_name () << "_ptr obj);"
          << be_nl_2;
    }

  if (! this->gen_xxx_narrow ("_narrow", node, os))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Error in ")
                         ACE_TEXT ("be_visitor_interface_ch::")
                         ACE_TEXT ("visit_interface while generating ")
                         ACE_TEXT ("_narrow () declaration\n")),
                        -1);
    }

  if (c == 0)
    {
      if (! this->gen_xxx_narrow ("_unchecked_narrow", node, os))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Error in ")
                             ACE_TEXT ("be_visitor_interface_ch::")
                             ACE_TEXT ("visit_interface while generating ")
                             ACE_TEXT ("_unchecked_narrow () declaration\n")),
                            -1);
        }
    }

  *os << "static " << node->local_name () << "_ptr _nil (void);";

  // Generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ch::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  if (node->is_local ())
    {
      if (node->convert_parent_ops (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("be_visitor_interface_ch::")
                            ACE_TEXT ("visit_interface - ")
                            ACE_TEXT ("converting parent ops failed\n")),
                            -1);
        }
    }

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  // If we inherit from both CORBA::Object and CORBA::AbstractBase,
  // we have to override _add_ref() to avoid ambiguity.
  if (node->has_mixed_parentage ())
    {
      *os << "virtual void _add_ref (void);" << be_nl_2;
    }

  // The _is_a method
  *os << "virtual ::CORBA::Boolean _is_a (const char *type_id);"
      << be_nl;

  // The _interface_repository_id method.
  *os << "virtual const char* _interface_repository_id "
      << "(void) const;";

  // The virtual marshal method, to prevent marshal of local iterfaces.
  *os << be_nl
      << "virtual ::CORBA::Boolean marshal "
      << "(TAO_OutputCDR &cdr);";

  if (c == 0 && be_global->gen_ostream_operators ())
    {
      *os << be_nl
          << "virtual std::ostream &_tao_stream_v (std::ostream &) const;";
    }

  *os << be_uidt_nl << be_nl
      << "protected:" << be_idt_nl;

  if (! node->is_local ())
    {
      // Generate the "protected" constructor so that users cannot
      // instantiate us.

      if (! node->is_abstract ())
        {
          *os << "// Concrete interface only." << be_nl
              << node->local_name () << " (void);"
              << be_nl_2;
        }
    }

  if (node->is_abstract () || node->is_local ())
    {
      // Protected default constructor for abstract interfaces.
      *os << "// Abstract or local interface only." << be_nl
          << node->local_name () << " (void);" << be_nl_2;
    }

  if (node->is_abstract ())
    {
      // Protected copy constructor for abstract interfaces.
      *os << "// Protected for abstract interfaces." << be_nl
          << node->local_name () << " (const "
          << node->local_name () << " &);" << be_nl_2;
    }

  // Local interfaces don't support stub objects.
  if (! node->is_local ())
    {
      if (! node->is_abstract ())
        {
          *os << "// Concrete non-local interface only." << be_nl
              << node->local_name () << " (" << be_idt << be_idt_nl
              << "::IOP::IOR *ior," << be_nl
              << "TAO_ORB_Core *orb_core);" << be_uidt
              << be_uidt_nl << be_nl;
        }

      *os << "// Non-local interface only." << be_nl
          << node->local_name () << " (" << be_idt << be_idt_nl
          << "TAO_Stub *objref," << be_nl
          << "::CORBA::Boolean _tao_collocated = false," << be_nl
          << "TAO_Abstract_ServantBase *servant = 0," <<  be_nl
          << "TAO_ORB_Core *orb_core = 0);"
          << be_uidt << be_uidt;
    }

  if (c != 0)
    {
      // Friends declarations, component only.
      *os << be_nl_2
          << "friend class " << node->direct_proxy_impl_name ()
          << ";";
    }

  // Protected destructor.
  *os << be_nl_2
      << "virtual ~" << node->local_name () << " (void);";

  // Private copy constructor and assignment operator. These are not
  // allowed, hence they are private.
  *os << be_uidt_nl << be_nl
      << "private:" << be_idt_nl;

  // Abstract interfaces have a *protected* copy constructor.
  if (! node->is_abstract ())
    {
      *os << "// Private and unimplemented for concrete interfaces." << be_nl
          << node->local_name () << " (const "
          << node->local_name () << " &);" << be_nl_2;
    }

  *os << "void operator= (const " << node->local_name () << " &);";

  *os << be_uidt_nl
      << "};";

  be_visitor_context ctx (*this->ctx_);

  // Don't support smart proxies for local interfaces.
  if (! node->is_local ())
    {
      if (be_global->gen_smart_proxies ())
        {
          *os << be_nl_2;

          // Smart Proxy related classes.
          ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH);
          be_visitor_interface_smart_proxy_ch sp_visitor (&ctx);

          if (node->accept (&sp_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_interface_ch::")
                                 ACE_TEXT ("visit_interface - ")
                                 ACE_TEXT ("codegen for smart ")
                                 ACE_TEXT ("proxy classes failed\n")),
                                -1);
            }
        }
    }

  if (be_global->tc_support ())
    {
      be_visitor_typecode_decl td_visitor (&ctx);

      if (node->accept (&td_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_ch::")
                             ACE_TEXT ("visit_interface - ")
                             ACE_TEXT ("TypeCode declaration failed\n")),
                            -1);
        }
    }

  node->cli_hdr_gen (true);
  return 0;
}

int
be_visitor_interface_ch::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_ch::visit_connector (be_connector *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_ch::visit_extended_port (be_extended_port *node)
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
be_visitor_interface_ch::visit_mirror_port (be_mirror_port *node)
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
be_visitor_interface_ch::gen_abstract_ops_helper (be_interface *node,
                                                  be_interface *base,
                                                  TAO_OutStream *os)
{
  if (!base->is_abstract ())
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
                             ACE_TEXT ("be_interface::")
                             ACE_TEXT ("gen_abstract_ops_helper - ")
                             ACE_TEXT ("bad node in this scope\n")),
                            -1);
        }

      if (d->node_type () == AST_Decl::NT_op)
        {

          be_operation *op = be_operation::narrow_from_decl (d);
          op->set_local (node->is_local ());
          ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
          be_visitor_operation_ch op_visitor (&ctx);
          op_visitor.visit_operation (op);
          op->set_local (base->is_local ());
        }
    }

  return 0;
}

bool
be_visitor_interface_ch::gen_xxx_narrow (const char *nar,
                                         be_interface *node,
                                         TAO_OutStream *os)
{
  *os << "static " << node->local_name () << "_ptr "
      << nar << " (";

  if (node->is_abstract ())
    {
      *os << "::CORBA::AbstractBase_ptr obj";
    }
  else
    {
      *os << "::CORBA::Object_ptr obj";
    }

  *os << ");" << be_nl;

  return true;
}
