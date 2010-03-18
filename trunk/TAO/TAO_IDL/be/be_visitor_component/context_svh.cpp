
//=============================================================================
/**
 *  @file    context_svh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for a context class in the
 *  servant header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_context_svh::be_visitor_context_svh (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_context_svh::~be_visitor_context_svh (void)
{
}

int
be_visitor_context_svh::visit_component (be_component *node)
{
  // This visitor is spawned by be_visitor_component_svh,
  // which already does a check for imported node, so none
  // is needed here.

  node_ = node;

  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // No '_cxx_' prefix.
  const char *lname =
    node->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl
      << "class " << lname << "_Servant;"
      << be_nl << be_nl;

  os_ << "class " << export_macro_.c_str () << " " << lname
      << "_Context" << be_idt_nl
      << ": public virtual ::CIAO::"
      << "Context_Impl<" << be_idt << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_Context," << be_nl
      << "::" << node->name () << ">" << be_uidt << be_uidt << be_uidt_nl
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
      << "::" << node->name () << ">" << be_uidt_nl
      << "base_type;" << be_uidt_nl << be_nl;

  os_ << "typedef base_type::context_type context_type;" << be_nl
      << "typedef base_type::component_type component_type;"
      << be_nl << be_nl;

  os_ << lname << "_Context (" << be_idt_nl
      << "::Components::CCMHome_ptr h," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << "PortableServer::Servant sv," << be_nl
      << "const char *id);" << be_uidt_nl << be_nl;

  os_ << "virtual ~" << lname << "_Context (void);";

  os_ << be_nl << be_nl
      << "// CIAO-specific." << be_nl
      << "static " << lname << "_Context *" << be_nl
      << "_narrow ( ::Components::SessionContext_ptr p);";

  os_ << be_nl << be_nl
      << "// Operations for " << lname
      << " receptacles and event sources,"
      << be_nl
      << "// defined in " << global << sname
      << "::CCM_" << lname << "_Context.";

  if (this->visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_context_svh")
                         ACE_TEXT ("::visit_component - ")
                         ACE_TEXT ("visit_component_scope() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_context_svh::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

int
be_visitor_context_svh::visit_uses (be_uses *node)
{
  ACE_CString prefix (this->port_prefix_);
  prefix += node->local_name ()->get_string ();
  const char *port_name = prefix.c_str ();

  const char *obj_name = node->uses_type ()->full_name ();
  bool const is_multiple = node->is_multiple ();

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

  if (be_global->ami4ccm_call_back ())
    {
      os_ << be_nl << be_nl
          << "virtual ";

      if (is_multiple)
        {
          os_ << "::" << node_->full_name () << "::"
              << port_name << "Connections *" << be_nl
              << "get_connections_" << port_name << " (void);";
        }
      else
        {
          ACE_CString original_op_name (
            node->uses_type ()->name ()->last_component ()->get_string ());
          ACE_CString new_op_name = ACE_CString ("AMI_") + original_op_name;
          UTL_ScopedName *op_name =
            static_cast<UTL_ScopedName *> (node->uses_type ()->name ()->copy ());
          op_name->last_component ()->replace_string (new_op_name.c_str ());

          os_ << "::" << op_name << "_ptr" << be_nl
              << "get_connection_sendc_" << port_name << " (void);";

          os_ << be_nl << be_uidt_nl
              << "protected:" << be_idt_nl
              << "virtual "
              << (is_multiple ? "::Components::Cookie *" : "void")
              << be_nl
              << "connect_sendc_" << port_name << " (" << be_idt_nl
              << "::" << op_name << "_ptr);"
              << be_uidt_nl << be_nl;

          os_ << "virtual ::" << op_name << "_ptr" << be_nl
              << "disconnect_sendc_" << port_name << " (";

          if (is_multiple)
            {
              os_ << be_idt_nl
                  << "::Components::Cookie * ck);" << be_uidt;
            }
          else
            {
              os_ << "void);";
            }

          op_name->destroy ();
          delete op_name;
          op_name = 0;
        }
    }

  os_ << be_nl << be_uidt_nl
      << "protected:" << be_idt_nl
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

      if (be_global->ami4ccm_call_back ())
        {
          ACE_CString original_op_name (
            node->uses_type ()->name ()->last_component ()->get_string ());
          ACE_CString new_op_name =
            ACE_CString ("AMI_") + original_op_name;
          UTL_ScopedName *op_name =
            static_cast<UTL_ScopedName *> (
              node->uses_type ()->name ()->copy ());
          op_name->last_component ()->replace_string (new_op_name.c_str ());
          
          os_ << be_nl << be_nl
              << "// Simplex sendc_" << port_name << " connection." << be_nl
              << "::" << op_name << "_var" << be_nl
              << "ciao_uses_sendc_" << port_name << "_;";
              
          op_name->destroy ();
          delete op_name;
          op_name = 0;
        }
    }

  return 0;
}

int
be_visitor_context_svh::visit_publishes (be_publishes *node)
{
  const char *obj_name = node->publishes_type ()->full_name ();
  const char *port_name = node->local_name ()->get_string ();

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl
      << "virtual void" << be_nl
      << "push_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << " * ev);" << be_uidt_nl;

  os_ << be_uidt_nl
      << "protected:" << be_idt_nl;

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

  return 0;
}

int
be_visitor_context_svh::visit_emits (be_emits *node)
{
  const char *obj_name = node->emits_type ()->full_name ();
  const char *port_name = node->local_name ()->get_string ();

  os_ << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  os_ << "virtual void" << be_nl
      << "push_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << " * ev);" << be_uidt;

  os_ << be_uidt_nl << be_nl
          << "protected:" << be_idt_nl;

  os_ << "virtual void" << be_nl
      << "connect_" << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c);" << be_uidt;

  os_ << be_nl << be_nl
      << "virtual ::" << obj_name << "Consumer_ptr" << be_nl
      << "disconnect_" << port_name << " (void);";

  os_ << be_uidt_nl << be_nl
      << "protected:" << be_idt_nl
      << "::" << obj_name << "Consumer_var" << be_nl
      << "ciao_emits_" << port_name << "_consumer_;";

  return 0;
}


