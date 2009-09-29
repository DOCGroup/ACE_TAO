//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_svh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components in the servant header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component,
           component_svh,
           "$Id$")

// ******************************************************
// Component visitor for server header
// ******************************************************

be_visitor_component_svh::be_visitor_component_svh (be_visitor_context *ctx)
  : be_visitor_component (ctx),
    node_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->svnt_export_macro ()),
    in_ancestor_ (false)
{
  /// All existing CIAO examples set the servant export values in the CIDL
  /// compiler to equal the IDL compiler's skel export values. Below is a
  /// partial effort to decouple them, should be completely decoupled
  /// sometime. See comment in codegen.cpp, line 1173.
  if (export_macro_ == "")
    {
      export_macro_ = be_global->skel_export_macro ();
    }
}

be_visitor_component_svh::~be_visitor_component_svh (void)
{
}

int
be_visitor_component_svh::visit_component (be_component *node)
{
  node_ = node;
  TAO_OutStream &os_  = *this->ctx_->stream ();

  if (this->gen_facets () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("gen_facets() failed\n")),
                        -1);
    }

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node_->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  this->gen_context_class ();

  if (this->gen_servant_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("gen_servant_class() failed\n")),
                        -1);
    }

  this->gen_entrypoint ();

  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_component_svh::visit_operation (be_operation *node)
{
  be_visitor_operation_ch v (this->ctx_);
  return v.visit_operation (node);
}

int
be_visitor_component_svh::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}

int
be_visitor_component_svh::gen_facets (void)
{
  for (UTL_ScopeActiveIterator si (node_, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d->node_type () != AST_Decl::NT_provides)
        {
          continue;
        }

      AST_Provides *p =
        AST_Provides::narrow_from_decl (d);

      be_type *impl =
        be_type::narrow_from_decl (p->provides_type ());

      if (impl->svnt_hdr_facet_gen ())
        {
          continue;
        }

      // No '_cxx_' prefix>
      const char *lname =
        impl->original_local_name ()->get_string ();

      be_decl *scope =
        be_scope::narrow_from_scope (impl->defined_in ())->decl ();
      ACE_CString suffix (scope->flat_name ());

      if (suffix != "")
        {
          suffix = ACE_CString ("_") + suffix;
        }

      os_ << be_nl << be_nl
          << "namespace CIAO_FACET" << suffix.c_str () << be_nl
          << "{" << be_idt_nl;

      const char *impl_name = "::CORBA::Object";
      bool is_intf = impl->node_type () == AST_Decl::NT_interface;

      if (is_intf)
        {
          impl_name =
            be_interface::narrow_from_decl (impl)->full_skel_name ();
        }

      os_ << "class " << lname << "_Servant" << be_idt_nl
          << ": public virtual " << impl_name << be_uidt_nl
          << "{" << be_nl
          << "public:" << be_idt_nl;

      AST_Decl *s = ScopeAsDecl (impl->defined_in ());
      ACE_CString sname_str (s->full_name ());
      const char *sname = sname_str.c_str ();
      const char *global = (sname_str == "" ? "" : "::");

      os_ << lname << "_Servant (" << be_idt_nl
          << global << sname << "::CCM_"
          << lname << "_ptr executor," << be_nl
          << "::Components::CCMContext_ptr ctx);" << be_uidt_nl << be_nl;

      os_ << "virtual ~" << lname << "_Servant (void);";

      if (is_intf)
        {
          be_interface *intf =
            be_interface::narrow_from_decl (impl);

          if (this->gen_facet_ops_attrs (intf) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_component_svh::gen_facet - "
                                 "gen_facet_ops_attrs() failed\n"),
                                -1);
            }
        }

      os_ << be_nl << be_nl << "// Get component implementation." << be_nl
          << "virtual CORBA::Object_ptr _get_component (void);"
          << be_uidt_nl << be_nl;

      os_ << "protected:" << be_idt_nl;

      os_ << "// Facet executor." << be_nl
          << global << sname << "::CCM_"
          << lname << "_var executor_;" << be_nl << be_nl;

      os_ << "// Context object." << be_nl
          << "::Components::CCMContext_var ctx_;" << be_uidt_nl;

      os_ << "};" << be_nl << be_uidt_nl;

      os_ << "}";

      impl->svnt_hdr_facet_gen (true);
    }

  return 0;
}

int
be_visitor_component_svh::gen_facet_ops_attrs (be_interface *node)
{
  int status =
    node->traverse_inheritance_graph (
      be_visitor_component_svh::op_attr_decl_helper,
      &os_);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svh::")
                         ACE_TEXT ("gen_facet_ops_attrs - ")
                         ACE_TEXT ("traverse_inheritance_graph() failed\n")),
                        -1);
    }

  return 0;
}

void
be_visitor_component_svh::gen_context_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // No '_cxx_' prefix.
  const char *lname =
    node_->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");
  bool swapping = be_global->gen_component_swapping ();

  os_ << be_nl
      << "class " << lname << "_Servant;"
      << be_nl << be_nl;

  os_ << "class " << export_macro_.c_str () << " " << lname
      << "_Context" << be_idt_nl
      << ": public virtual ::CIAO::"
      << (swapping ? "Upgradeable_" : "")
      << "Context_Impl<" << be_idt << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_Context," << be_nl
      << lname << "_Servant," << be_nl
      << "::" << node_->name () << ">" << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  os_ << "// Allow the servant to access our state." << be_nl
      << "friend class " << lname << "_Servant;"
      << be_nl << be_nl;

  os_ << "// Some useful typedefs." << be_nl<< be_nl
      << "typedef" << be_nl
      << "::CIAO::Context_Impl<" << be_idt << be_idt_nl
      << global << sname << "::CCM_"
      << lname << "_Context," << be_nl
      << lname << "_Servant," << be_nl
      << "::" << node_->name () << ">" << be_uidt_nl
      << "base_type;" << be_uidt_nl << be_nl;

  os_ << "typedef base_type::context_type context_type;" << be_nl
      << "typedef base_type::servant_type servant_type;" << be_nl
      << "typedef base_type::component_type component_type;"
      << be_nl << be_nl;

  os_ << lname << "_Context (" << be_idt_nl
      << "::Components::CCMHome_ptr h," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << lname << "_Servant *sv);" << be_uidt_nl << be_nl;

  os_ << "virtual ~" << lname << "_Context (void);";

  os_ << be_nl << be_nl
      << "// CIAO-specific." << be_nl
      << "static " << lname << "_Context *" << be_nl
      << "_narrow ( ::Components::SessionContext_ptr p);";

  this->gen_context_r (node_);

  if (swapping)
    {
      os_ << be_nl << be_nl
          << "/// Operation defined in " << sname << "::CCM_"
          << lname << "_Context" << be_nl
          << "/// that enable component swapping in the container."
          << be_nl
          << "virtual ::Components::ConsumerDescriptions *" << be_nl
          << "get_registered_consumers (const char * publisher_name);";
    }

  os_ << be_uidt_nl
      << "};";
}

void
be_visitor_component_svh::gen_context_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }

  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // No '_cxx_' prefix.
  const char *lname =
    node->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "// Operations for " << lname
      << " receptacles and event sources,"
      << be_nl
      << "// defined in " << global << sname
      << "::CCM_" << lname << "_Context.";

  this->gen_ports (node, AST_Decl::NT_uses, false);
  this->gen_ports (node, AST_Decl::NT_publishes, false);
  this->gen_ports (node, AST_Decl::NT_emits, false);

  AST_Component *ancestor = node->base_component ();
  in_ancestor_ |= (ancestor != 0);

  this->gen_context_r (ancestor);
}

int
be_visitor_component_svh::gen_servant_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // No '_cxx_' prefix.
  const char *lname =
    node_->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "class " << export_macro_.c_str () << " " << lname
      << "_Servant" << be_idt_nl
      << ": public virtual" << be_idt << be_idt_nl
      << "::CIAO::Servant_Impl<" << be_idt_nl
      << "::" << node_->full_skel_name () << "," << be_nl
      << global << sname << "::CCM_" << lname << "," << be_nl
      << lname << "_Context>"
      << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  os_ << "typedef " << global << sname << "::CCM_" << lname
      << " _exec_type;" << be_nl;

  os_ << be_nl
      << lname << "_Servant (" << be_idt_nl
      << global << sname << "::CCM_"
      << lname << "_ptr executor," << be_nl
      << "::Components::CCMHome_ptr h," << be_nl
      << "const char * ins_name," << be_nl
      << "::CIAO::Home_Servant_Impl_Base *hs," << be_nl
      << "::CIAO::Container_ptr c);" << be_uidt_nl;

  os_ << be_nl
      << "virtual ~" << lname << "_Servant"
      << " (void);" << be_nl;

  os_ << be_nl
      << "virtual void" << be_nl
      << "set_attributes (const "
      << "::Components::ConfigValues & descr);"
      << be_nl;

  os_ << be_nl
      << "// Supported operations and attributes.";

  int status =
    node_->traverse_inheritance_graph (
      be_visitor_component_svh::op_attr_decl_helper,
      &os_,
      false,
      false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_svh::"
                         "gen_servant_class - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  /// Reset flag for servant class.
  in_ancestor_ = false;

  status = this->gen_servant_r (node_);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_svh::"
                         "gen_servant_class - "
                         "gen_servant_r() failed\n"),
                        -1);
    }

  this->gen_non_type_specific ();

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_component_svh::gen_servant_r (AST_Component *node)
{
  if (node == 0)
    {
      return 0;
    }

  os_ << be_nl << be_nl
      << "// " << (in_ancestor_ ? "Inherited p" : "P")
      << "orts code.";

  this->gen_ports (node, AST_Decl::NT_provides, true);
  this->gen_ports (node, AST_Decl::NT_uses, true);
  this->gen_ports (node, AST_Decl::NT_publishes, true);
  this->gen_ports (node, AST_Decl::NT_emits, true);
  this->gen_ports (node, AST_Decl::NT_consumes, true);

  os_ << be_nl << be_nl
      << "// " << (in_ancestor_ ? "Inherited c" : "C")
      << "omponent attributes.";

  int status = this->gen_component_attrs (node);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svh::")
                         ACE_TEXT ("gen_servant_r - ")
                         ACE_TEXT ("gen_component_attrs() failed\n")),
                        -1);
    }

  AST_Component *ancestor = node->base_component ();
  in_ancestor_ |= (ancestor != 0);

  return gen_servant_r (ancestor);
}

int
be_visitor_component_svh::gen_component_attrs (AST_Component *node)
{
  /// Traverse the scope and ignore everything but attributes.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      be_attribute *attr = be_attribute::narrow_from_decl (d);

      if (attr == 0)
        {
          continue;
        }

      if (attr->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_component_svh::")
                             ACE_TEXT ("gen_component_attrs - ")
                             ACE_TEXT ("be_attribute::accept() failed\n")),
                            -1);
        }
    }

 return 0;
}

void
be_visitor_component_svh::gen_ports (AST_Component *node,
                                     AST_Decl::NodeType nt,
                                     bool in_servant)
{
  os_ << be_nl << be_nl;

  switch (nt)
    {
      case AST_Decl::NT_provides:
        os_ << "// Facets.";
        break;
      case AST_Decl::NT_uses:
        os_ << "// Receptacles.";
        break;
      case AST_Decl::NT_publishes:
        os_ << "// Event publishers.";
        break;
      case AST_Decl::NT_consumes:
        os_ << "// Event consumers.";
        break;
      case AST_Decl::NT_emits:
        os_ << "// Event emitters.";
        break;
      default:
        break;
    }

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d->node_type () != nt)
        {
          continue;
        }

      switch (nt)
        {
          case AST_Decl::NT_provides:
            this->gen_provides (AST_Provides::narrow_from_decl (d));
            break;
          case AST_Decl::NT_uses:
            this->gen_uses (AST_Uses::narrow_from_decl (d),
                            in_servant);
            break;
          case AST_Decl::NT_publishes:
            this->gen_publishes (AST_Publishes::narrow_from_decl (d),
                                 in_servant);
            break;
          case AST_Decl::NT_emits:
            this->gen_emits (AST_Emits::narrow_from_decl (d),
                             in_servant);
            break;
          case AST_Decl::NT_consumes:
            this->gen_consumes (AST_Consumes::narrow_from_decl (d));
            break;
          default:
            break;
        }
    }
}

void
be_visitor_component_svh::gen_provides (AST_Provides *p)
{
  const char *obj_name = p->provides_type ()->full_name ();
  const char *port_name = p->local_name ()->get_string ();

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl
      << "virtual ::" << obj_name << "_ptr" << be_nl
      << "provide_" << port_name << " (void);"
      << be_uidt_nl << be_nl;

  os_ << "private:" << be_idt_nl
      << "::CORBA::Object_ptr" << be_nl
      << "provide_" << port_name << "_i (void);"
      << be_uidt_nl << be_nl;

  os_ << "private:" << be_idt_nl
      << "::" << obj_name << "_var" << be_nl
      << "provide_" << port_name << "_;";
}

void
be_visitor_component_svh::gen_uses (
  AST_Uses *u,
  bool in_servant)
{
  const char *obj_name = u->uses_type ()->full_name ();
  const char *port_name = u->local_name ()->get_string ();
  bool is_multiple = u->is_multiple ();

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl
      << "virtual ";

  if (is_multiple)
    {
      os_ << "::" << node_->full_name () << "::"
          << port_name << "Connections *" << be_nl
          << "get_connections_" << port_name << " (void);";
    }
  else
    {
      os_ << "::" << obj_name << "_ptr" << be_nl
          << "get_connection_" << port_name << " (void);";
    }

  os_ << be_nl;

  if (!in_servant)
    {
      os_ << be_uidt_nl
          << "protected:" << be_idt;
    }

  os_ << be_nl
      << "virtual "
      << (is_multiple ? "::Components::Cookie *" : "void")
      << be_nl
      << "connect_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << "_ptr);"
      << be_uidt_nl << be_nl;

  os_ << "virtual ::" << obj_name << "_ptr" << be_nl
      << "disconnect_" << port_name << " (";

     if (is_multiple)
      {
        os_ << be_idt_nl
            << "::Components::Cookie * ck);" << be_uidt;
      }
    else
      {
        os_ << "void);";
      }

  if (!in_servant)
    {
      os_ << be_uidt_nl << be_nl
          << "protected:" << be_idt_nl;

      if (is_multiple)
        {
          os_ << "// Multiplex " << port_name << " connection." << be_nl
              << "typedef ACE_Array_Map<ptrdiff_t," << be_nl
              << "                      ::"
              << obj_name << "_var>" << be_idt_nl
              << tao_cg->upcase (port_name)
              << "_TABLE;" << be_uidt_nl
              << tao_cg->upcase (port_name) << "_TABLE "
              << "ciao_uses_" << port_name << "_;" << be_nl
              << "TAO_SYNCH_MUTEX " << port_name << "_lock_;";
        }
      else
        {
          os_ << "// Simplex " << port_name << " connection." << be_nl
              << "::" << obj_name << "_var" << be_nl
              << "ciao_uses_" << port_name << "_;";
        }
    }
}

void
be_visitor_component_svh::gen_publishes (AST_Publishes *p,
                                         bool in_servant)
{
  const char *obj_name = p->publishes_type ()->full_name ();
  const char *port_name = p->local_name ()->get_string ();

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  if (!in_servant)
    {
      os_ << "virtual void" << be_nl
          << "push_" << port_name << " (" << be_idt_nl
          << "::" << obj_name << " * ev);" << be_uidt_nl;

      os_ << be_uidt_nl
          << "protected:" << be_idt_nl;
    }

  os_ << "virtual ::Components::Cookie *" << be_nl
      << "subscribe_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c);" << be_uidt_nl;

  os_ << be_nl
      << "// CIAO-specific." << be_nl
      << "::Components::Cookie *" << be_nl
      << "subscribe_" << port_name << "_generic (" << be_idt_nl
      << "::Components::EventConsumerBase_ptr c);" << be_uidt_nl;

  os_ << be_nl
      << "virtual ::" << obj_name << "Consumer_ptr" << be_nl
      << "unsubscribe_" << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck);" << be_uidt;

  if (!in_servant)
    {
      os_ << be_uidt_nl << be_nl
          << "protected:" << be_idt_nl;

      os_ << "typedef ACE_Array_Map<ptrdiff_t," << be_nl
          << "                      ::" << obj_name
          << "Consumer_var>" << be_idt_nl
          << tao_cg->upcase (port_name) << "_TABLE;" << be_uidt_nl
          << tao_cg->upcase (port_name) << "_TABLE ciao_publishes_"
          << port_name << "_;" << be_nl
          << "TAO_SYNCH_MUTEX " << port_name << "_lock_;"
          << be_nl << be_nl;

      os_ << "typedef ACE_Array_Map<ptrdiff_t," << be_nl
          << "                      ::Components::EventConsumerBase_var>"
          << be_idt_nl
          << tao_cg->upcase (port_name)
          << "_GENERIC_TABLE;" << be_uidt_nl
          << tao_cg->upcase (port_name)
          << "_GENERIC_TABLE ciao_publishes_"
          << port_name << "_generic_;";
    }
}

void
be_visitor_component_svh::gen_consumes (AST_Consumes *c)
{
  const char *obj_name = c->consumes_type ()->full_name ();
  const char *port_name = c->local_name ()->get_string ();

  ACE_CString holder (obj_name);
  ACE_CString::size_type pos = holder.rfind (':');
  const char *ev_lname = 0;

  if (pos == ACE_CString::npos)
    {
      ev_lname = obj_name;
    }
  else
    {
      holder = holder.substr (pos + 1);
      ev_lname = holder.c_str ();
    }

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  os_ << "// Servant class for the " << port_name
      << " consumer." << be_nl
      << "class " << export_macro_.c_str () << " " << ev_lname
      << "Consumer_" << port_name << "_Servant" << be_idt_nl
      << ": public virtual ::POA_" << obj_name << "Consumer"
      << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  ACE_CString sname_str (
    ScopeAsDecl (node_->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << ev_lname << "Consumer_" << port_name
      << "_Servant (" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_ptr executor," << be_nl
      << global << sname << "::CCM_" << lname
      << "_Context_ptr c);" << be_uidt_nl << be_nl;

  os_ << "virtual ~" << ev_lname << "Consumer_" << port_name
      << "_Servant (void);";

  os_ << be_nl << be_nl
      << "virtual void" << be_nl
      << "push_" << ev_lname << " (" << be_idt_nl
      << "::" << obj_name << " * evt);" << be_uidt;

  os_ << be_nl << be_nl
      << "// Inherited from ::Components::EventConsumerBase." << be_nl
      << "virtual void" << be_nl
      << "push_event ( ::Components::EventBase * ev);";

  os_ << be_nl << be_nl
      << "// CIAO-specific in ::Components::EventConsumerBase." << be_nl
      << "virtual void" << be_nl
      << "ciao_push_event ( ::Components::EventBase * ev," << be_nl
      << "                 const char * source_id," << be_nl
      << "                 ::CORBA::TypeCode_ptr tc);";

  os_ << be_nl << be_nl
      << "// CIAO-specific in ::Components::EventConsumerBase." << be_nl
      << "virtual ::CORBA::Boolean" << be_nl
      << "ciao_is_substitutable (const char * event_repo_id);";

  os_ << be_nl << be_nl
      << "// Get component implementation." << be_nl
      << "virtual ::CORBA::Object_ptr" << be_nl
      << "_get_component (void);";

  os_ << be_uidt_nl << be_nl
      << "protected:" << be_idt_nl;

  os_ << global << sname << "::CCM_" << lname << "_var" << be_nl
      << "executor_;";

  os_ << be_nl << be_nl
      << global << sname << "::CCM_"
      << lname << "_Context_var" << be_nl
      << "ctx_;";

  os_ << be_uidt_nl
      << "};";

  os_ << be_nl << be_nl
      << "virtual ::" << obj_name << "Consumer_ptr" << be_nl
      << "get_consumer_" << port_name << " (void);";

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl;

  os_ << "::Components::EventConsumerBase_ptr" << be_nl
      << "get_consumer_" << port_name << "_i (void);";

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl;

  os_ << "::" << obj_name << "Consumer_var" << be_nl
      << "consumes_" << port_name << "_;";
}

void
be_visitor_component_svh::gen_emits (AST_Emits *e,
                                     bool in_servant)
{
  const char *obj_name = e->emits_type ()->full_name ();
  const char *port_name = e->local_name ()->get_string ();

  if (in_servant)
    {
      os_ << be_nl << be_nl;
    }
  else
    {
      os_ << be_uidt_nl << be_nl
          << "public:" << be_idt_nl;

      os_ << "virtual void" << be_nl
          << "push_" << port_name << " (" << be_idt_nl
          << "::" << obj_name << " * ev);" << be_uidt;

      os_ << be_uidt_nl << be_nl
          << "protected:" << be_idt_nl;
    }

  os_ << "virtual void" << be_nl
      << "connect_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c);" << be_uidt;

  os_ << be_nl << be_nl
      << "virtual ::" << obj_name << "Consumer_ptr" << be_nl
      << "disconnect_" << port_name << " (void);";

  if (!in_servant)
    {
      os_ << be_uidt_nl << be_nl
          << "protected:" << be_idt_nl
          << "::" << obj_name << "Consumer_var" << be_nl
          << "ciao_emits_" << port_name << "_consumer_;";
    }
}

void
be_visitor_component_svh::gen_non_type_specific (void)
{
  os_ << be_nl << be_nl
      << "// Base class overrides.";

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl
      << "virtual ::Components::Cookie *" << be_nl
      << "connect (const char * name," << be_nl
      << "         ::CORBA::Object_ptr connection);";

  os_ << be_nl << be_nl
      << "virtual ::CORBA::Object_ptr" << be_nl
      << "disconnect (const char * name," << be_nl
      << "            ::Components::Cookie * ck);";

  os_ << be_nl << be_nl
      << "virtual ::Components::ReceptacleDescriptions *" << be_nl
      << "get_all_receptacles (void);";

  os_ << be_nl << be_nl
      << "virtual ::Components::Cookie *" << be_nl
      << "subscribe (const char * publisher_name," << be_nl
      << "           ::Components::EventConsumerBase_ptr subscriber);";

  os_ << be_nl << be_nl
      << "virtual ::Components::EventConsumerBase_ptr" << be_nl
      << "unsubscribe (const char * publisher_name," << be_nl
      << "             ::Components::Cookie * ck);";

  os_ << be_nl << be_nl
      << "virtual void" << be_nl
      << "connect_consumer (const char * emitter_name," << be_nl
      << "                  ::Components::EventConsumerBase_ptr consumer);";

  os_ << be_nl << be_nl
      << "virtual ::Components::EventConsumerBase_ptr" << be_nl
      << "disconnect_consumer (const char * source_name);";

  os_ << be_nl << be_nl
      << "virtual ::Components::PublisherDescriptions *" << be_nl
      << "get_all_publishers (void);";

  os_ << be_nl << be_nl
      << "virtual ::Components::EmitterDescriptions *" << be_nl
      << "get_all_emitters (void);";

  os_ << be_nl << be_nl
      << "// CIAO-specific." << be_nl
      << "::CORBA::Object_ptr" << be_nl
      << "get_facet_executor (const char * name);";

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "void" << be_nl
      << "populate_port_tables (void);";

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "const char * ins_name_;";
}

void
be_visitor_component_svh::gen_entrypoint (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::PortableServer::Servant" << be_nl
      << "create_" << node_->flat_name ()
      << "_Servant (" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr p," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << "const char * ins_name);" << be_uidt;
}

int
be_visitor_component_svh::op_attr_decl_helper (be_interface * /*derived */,
                                               be_interface *ancestor,
                                               TAO_OutStream *os)
{
  if (be_component::narrow_from_decl (ancestor) != 0)
    {
      return 0;
    }

  /// We're in a static method, so we have to instantiate a temporary
  /// visitor and context.
  be_visitor_context ctx;
  ctx.state (TAO_CodeGen::TAO_ROOT_SVH);
  ctx.stream (os);
  be_visitor_component_svh visitor (&ctx);

  /// Since this visitor overriddes only visit_operation() and
  /// visit_attribute(), we can get away with this for the declarations.
  return visitor.visit_scope (ancestor);
}

