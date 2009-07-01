//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_svs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components in the servant source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component,
           component_svs,
           "$Id$")

// ******************************************************
// Component visitor for servant source
// ******************************************************

bool be_visitor_component_svs::in_facets_ = false;

be_visitor_component_svs::be_visitor_component_svs (be_visitor_context *ctx)
  : be_visitor_component (ctx),
    node_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->svnt_export_macro ()),
    swapping_ (be_global->gen_component_swapping ())
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

be_visitor_component_svs::~be_visitor_component_svs (void)
{
}

int
be_visitor_component_svs::visit_component (be_component *node)
{
  node_ = node;
  
  if (! be_global->gen_lem_force_all ())
    {
      if (this->gen_facets () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_component_svs::")
                             ACE_TEXT ("visit_component - ")
                             ACE_TEXT ("gen_facets() failed\n")),
                            -1);
        }
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
                         ACE_TEXT ("be_visitor_component_svs::")
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
be_visitor_component_svs::visit_operation (be_operation *node)
{
  be_visitor_operation_svs v (this->ctx_);
  v.for_facets (be_visitor_component_svs::in_facets_);
  v.scope (this->op_scope_);
  
  return v.visit_operation (node);
}

int
be_visitor_component_svs::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  
  v.for_facets (be_visitor_component_svs::in_facets_);
  v.op_scope (this->op_scope_);
  
  return v.visit_attribute (node);
}

int
be_visitor_component_svs::gen_facets (void)
{
  AST_Component::port_description *pd = 0;
  be_visitor_component_svs::in_facets_ = true;

  for (AST_Component::PORTS::ITERATOR i = node_->provides ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      be_interface *intf =
        be_interface::narrow_from_decl (pd->impl);
        
      if (intf->svnt_src_facet_gen ())
        {
          continue;
        }
      
      this->op_scope_ = intf;
      
      int status =
        intf->gen_facet_svnt_src (this, os_);
        
      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_component_svs")
                             ACE_TEXT ("::gen_facets - call to node")
                             ACE_TEXT ("svnt_src_facet_gen() failed\n")),
                            -1);
        }
      
      intf->svnt_src_facet_gen (true);
    }

  be_visitor_component_svs::in_facets_ = false;

  return 0;
}

void
be_visitor_component_svs::gen_context_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");
  
  os_ << be_nl
      << lname <<"_Context::" << lname << "_Context ("
      << be_idt << be_idt_nl
      << "::Components::CCMHome_ptr h," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << lname << "_Servant * sv)" << be_uidt_nl
      << ": ::CIAO::Context_Impl_Base (h, c)," << be_idt_nl
      << "::CIAO::Context_Impl<" << be_idt_nl
      << global << sname << "::CCM_"
      << lname << "_Context," << be_nl
      << lname << "_Servant," << be_nl
      << "::" << node_->full_name () << "> (h, c, sv)";
      
  if (swapping_)
    {
      os_ << "," << be_uidt_nl
          << "::CIAO::Upgradeable_Context_Impl<" << be_idt_nl
          << global << sname << "::CCM_" << lname
          << "_Context," << be_nl
          << lname << "_Servant," << be_nl
          << "::" << node_->full_name ()
          << "> (h, c, sv)";
    }
      
  os_ << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << lname << "_Context::~"
      << lname << "_Context (void)" << be_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "/// CIAO-specific." << be_nl
      << lname << "_Context *" << be_nl
      << lname
      <<  "_Context::_narrow ( ::Components::SessionContext_ptr p)"
      << be_nl
      << "{" << be_idt_nl
      << "return dynamic_cast<" << lname
      << "_Context *> (p);" << be_uidt_nl
      << "}";
      
  this->gen_uses_context_r (node_);
  this->gen_publishes_context_r (node_);
  this->gen_emits_context_r (node_);
  
  if (swapping_)
    {
      os_ << be_nl << be_nl
          << "/// Operations defined in " << sname
          << "::CCM_" << lname << "_Context" << be_nl
          << "/// that enable component swapping in the container"
          << be_nl << be_nl
          << "::Components::ConsumerDescriptions *" << be_nl
          << lname << "_Context::get_registered_consumers ("
          << be_idt_nl
          << "const char * publisher_name)" << be_uidt_nl
          << "{" << be_idt_nl
          << "if (publisher_name == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "throw ::Components::InvalidName ();" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "::Components::ConsumerDescriptions * tmp = 0;"
          << be_nl
          << "::Components::ConsumerDescriptions_var retval;"
          << be_nl
          << "::CORBA::ULong _ciao_index = 0UL;" << be_nl
          << "::CORBA::ULong _ciao_size = 0UL;" << be_nl
          << "ACE_UNUSED_ARG (tmp);" << be_nl
          << "ACE_UNUSED_ARG (retval);" << be_nl
          << "ACE_UNUSED_ARG (_ciao_index);" << be_nl
          << "ACE_UNUSED_ARG (_ciao_size);";
          
      this->gen_swapping_get_consumers_r (node_);
          
      os_ << be_nl << be_nl
          << "throw ::Components::InvalidName ();" << be_uidt_nl
          << "}";
    }
}

int
be_visitor_component_svs::gen_servant_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");
  
  os_ << be_nl << be_nl
      << lname << "_Servant::"
      << lname << "_Servant (" << be_idt << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_ptr exe," << be_nl
      << "::Components::CCMHome_ptr h," << be_nl
      << "const char * ins_name," << be_nl
      << "::CIAO::Home_Servant_Impl_Base * hs,"
      << be_nl
      << "::CIAO::Container_ptr c)" << be_uidt_nl
      << ": ::CIAO::Servant_Impl_Base (h, hs, c),"
      << be_idt_nl
      << "::CIAO::Servant_Impl<" << be_idt_nl
      << "::" << node_->full_skel_name ()
      << "," << be_nl
      << global << sname << "::CCM_"
      << lname << "," << be_nl
      << lname << "_Context> (exe, h, hs, c)," << be_uidt_nl
      << "ins_name_ (ins_name)" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "ACE_NEW (this->context_," << be_nl
      << "         " << lname << "_Context (h, c, this));"
      << be_nl << be_nl
      << "/// Set the instance id of the component on the context."
      << be_nl
      << "this->context_->_ciao_instance_id (this->ins_name_);";
      
  if (be_global->gen_ciao_valuefactory_reg ())
    {
      this->gen_all_factory_registration ();
    }

  os_ << be_nl << be_nl
      << "try" << be_idt_nl
      << "{" << be_idt_nl
      << "::Components::SessionComponent_var scom =" << be_idt_nl
      << "::Components::SessionComponent::_narrow (exe);"
      << be_uidt_nl << be_nl
      << "if (! ::CORBA::is_nil (scom.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "scom->set_session_context (this->context_);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->populate_port_tables ();" << be_uidt_nl
      << "}" << be_uidt_nl
      << "catch (const ::CORBA::Exception &)" << be_idt_nl
      << "{" << be_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << lname << "_Servant::~"
      << lname << "_Servant (void)" << be_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << lname << "_Servant::set_attributes (" << be_idt_nl
      << "const ::Components::ConfigValues & descr)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "for ( ::CORBA::ULong i = 0; i < descr.length (); ++i)"
      << be_idt_nl
      << "{" << be_idt_nl
      << "const char * descr_name = descr[i]->name ();" << be_nl
      << "::CORBA::Any & descr_value = descr[i]->value ();";
      
  this->gen_attr_set_r (node_);
      
  os_ << be_nl << be_nl
      << "ACE_UNUSED_ARG (descr_name);" << be_nl
      << "ACE_UNUSED_ARG (descr_value);" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";
     
  os_ << be_nl << be_nl
      << "/// Supported operations and all attributes.";
      
  this->op_scope_ = node_;
    
  /// The overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues. 
  node_->get_insert_queue ().reset ();
  node_->get_del_queue ().reset ();
  node_->get_insert_queue ().enqueue_tail (node_);
      
  Component_Op_Attr_Generator op_attr_gen (this);
    
  int status =
    node_->traverse_inheritance_graph (op_attr_gen,
                                       &os_,
                                       false,
                                       false);
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_component_svs::"
                         "gen_servant_class - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }
    
  this->gen_component_attrs_r (node_);
  
  os_ << be_nl << be_nl
      << "/// All port operations.";

  this->gen_provides_top ();
  this->gen_publishes_servant_top ();
  this->gen_consumes_r (node_);
  this->gen_uses_servant_top ();
  this->gen_emits_servant_top ();
  
  os_ << be_nl << be_nl
      << "/// Private method to trigger population of the port"
      << be_nl
      << "/// tables (facets and event consumers)." << be_nl
      << "void" << be_nl
      << node_->local_name ()
      << "_Servant::populate_port_tables (void)" << be_nl
      << "{" << be_idt_nl
      << "::CORBA::Object_var obj_var;" << be_nl
      << "::Components::EventConsumerBase_var ecb_var;"
      << be_nl;
      
  this->gen_populate_r (node_);
      
  os_ << be_uidt_nl
      << "}";
   
  return 0;
}

int
be_visitor_component_svs::gen_component_attrs_r (AST_Component *node)
{
  if (node == 0)
    {
      return 0;
    }
         
  int status = this->gen_component_attrs (node);
  
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svs::")
                         ACE_TEXT ("gen_servant_r - ")
                         ACE_TEXT ("gen_component_attrs() failed\n")),
                        -1);
    }
  
  AST_Component *ancestor = node->base_component ();
  return gen_component_attrs_r (ancestor);
}

int
be_visitor_component_svs::gen_component_attrs (AST_Component *node)
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
                             ACE_TEXT ("be_visitor_component_svs::")
                             ACE_TEXT ("gen_component_attrs - ")
                             ACE_TEXT ("be_attribute::accept() failed\n")),
                            -1);
        }
    }

 return 0;
}

void
be_visitor_component_svs::gen_provides_top (void)
{
  os_ << be_nl << be_nl
      << "/// CIAO-specific." << be_nl
      << "::CORBA::Object_ptr" << be_nl
      << node_->local_name ()
      << "_Servant::get_facet_executor (" << be_idt_nl
      << "const char * name)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "if (name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt;
      
  AST_Component *node = node_;  
  AST_Component::port_description *pd = 0;
      
  while (node != 0)
    {  
      for (AST_Component::PORTS::ITERATOR i =
             node->provides ().begin ();
           !i.done ();
           i.advance ())
        {
          i.next (pd);
          
          this->gen_facet_executor_block (pd->id->get_string ());
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "return ::CORBA::Object::_nil ();" << be_uidt_nl
      << "}";
      
  this->gen_provides_r (node_);
}

void
be_visitor_component_svs::gen_facet_executor_block (
  const char *port_name)
{
  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (name, \"" << port_name
      << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->executor_->get_" << port_name
      << " ();" << be_uidt_nl
      << "}" << be_uidt;
}

void
be_visitor_component_svs::gen_provides_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }
    
  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node->provides ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      this->gen_provides (pd->impl,
                          pd->id->get_string ());
    }
    
  node = node->base_component ();
  this->gen_provides_r (node);
}

void
be_visitor_component_svs::gen_provides (AST_Type *obj,
                                        const char *port_name)
{
  const char *obj_name = obj->full_name ();
  AST_Decl *scope = ScopeAsDecl (obj->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  
  // Avoid '_cxx_' prefix.
  const char *lname =
    obj->original_local_name ()->get_string ();
    
  const char *global = (sname_str == "" ? "" : "::");
  const char *prefix_connector = (sname_str == "" ? "" : "_");
  
  os_ << be_nl << be_nl
      << "::" << obj_name << "_ptr" << be_nl
      << node_->local_name () << "_Servant::provide_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "if (! ::CORBA::is_nil (this->provide_"
      << port_name << "_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      <<  "::" << obj_name << "::_duplicate (this->provide_"
      << port_name << "_.in ());" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::CORBA::Object_var obj =" << be_idt_nl
      << "this->provide_" << port_name << "_i ();"
      << be_uidt_nl << be_nl
      << "::" << obj_name << "_var fo =" << be_idt_nl
      << "::" << obj_name << "::_narrow (obj.in ());"
      << be_uidt_nl << be_nl
      << "this->provide_" << port_name << "_ = fo;" << be_nl
      << "return" << be_idt_nl
      <<  "::" << obj_name << "::_duplicate (this->provide_"
      << port_name << "_.in ());" << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::CORBA::Object_ptr" << be_nl
      << node_->local_name () << "_Servant::provide_"
      << port_name << "_i (void)" << be_nl
      << "{" << be_idt_nl
      << "::CORBA::Object_ptr ret =" << be_idt_nl
      << "this->lookup_facet (\"" << port_name << "\");"
      << be_uidt_nl << be_nl
      << "if (! ::CORBA::is_nil (ret))" << be_idt_nl
      << "{" << be_idt_nl
      << "return ret;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "typedef" << be_idt_nl
      << "::CIAO::Port_Activator_T<" << be_idt_nl
      << "::CIAO_FACET" << prefix_connector
      << scope->flat_name () << "::" << lname
      << "_Servant," << be_nl
      << global << sname << "::CCM_" << lname << "," << be_nl
      << "::Components::CCMContext," << be_nl
      << node_->local_name () << "_Servant>" << be_uidt_nl
      << "MACRO_MADNESS_TYPEDEF;" << be_uidt_nl << be_nl
      << "MACRO_MADNESS_TYPEDEF * tmp = 0;" << be_nl
      << "ACE_CString obj_id (this->ins_name_);" << be_nl
      << "obj_id += \"_" << port_name << "\";" << be_nl << be_nl
      << "ACE_NEW_THROW_EX (" << be_idt_nl
      << "tmp," << be_nl
      << "MACRO_MADNESS_TYPEDEF (" << be_idt_nl
      << "obj_id.c_str ()," << be_nl
      << "\"" << port_name << "\"," << be_nl
      << "::CIAO::Port_Activator_Types::FACET," << be_nl
      << "0," << be_nl
      << "this->context_," << be_nl
      << "this)," << be_uidt_nl
      << "::CORBA::NO_MEMORY ());" << be_uidt_nl << be_nl
      << "::CIAO::Port_Activator_var pa = tmp;" << be_nl << be_nl
      << "::CIAO::Servant_Activator_var sa =" << be_idt_nl
      << "this->container_->ports_servant_activator ();"
      << be_uidt_nl << be_nl
      << "if (! sa->register_port_activator (pa._retn ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "return ::" << obj_name << "::_nil ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::CORBA::Object_var obj =" << be_idt_nl
      << "this->container_->generate_reference (" << be_idt_nl
      << "obj_id.c_str ()," << be_nl
      << "\"" << obj->repoID () << "\"," << be_nl
      << "::CIAO::Container_Types::FACET_CONSUMER_t);"
      << be_uidt_nl << be_uidt_nl
      << "this->add_facet (\"" << port_name << "\", obj.in ());"
      << be_nl << be_nl
      << "return obj._retn ();" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_uses_context_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }

  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node->uses ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      this->gen_uses_context (pd->impl,
                              pd->id->get_string (),
                              pd->is_multiple);
    }
    
  node = node->base_component ();
  this->gen_uses_context_r (node);
}

void
be_visitor_component_svs::gen_uses_context (
  AST_Type *obj,
  const char *port_name,
  bool is_multiple)
{
  if (is_multiple)
    {
      this->gen_uses_context_multiplex (obj, port_name);
    }
  else
    {
      this->gen_uses_context_simplex (obj, port_name);
    }
}

void
be_visitor_component_svs::gen_uses_context_simplex (
  AST_Type *obj,
  const char *port_name)
{
  const char *fname = obj->full_name ();
  
  os_ << be_nl << be_nl
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name () << "_Context::get_connection_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return ::" << fname << "::_duplicate (" << be_idt_nl
      << "this->ciao_uses_" << port_name << "_.in ());"
      << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Context::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (this->ciao_uses_"
      << port_name << "_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::AlreadyConnected ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->ciao_uses_" << port_name << "_ =" << be_idt_nl
      << "::" << fname << "::_duplicate (c);"
      << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name () << "_Context::disconnect_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (this->ciao_uses_"
      << port_name << "_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::NoConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return this->ciao_uses_" << port_name
      << "_._retn ();" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_uses_context_multiplex (
  AST_Type *obj,
  const char *port_name)
{
  const char *fname = obj->full_name ();
  bool static_config = be_global->gen_ciao_static_config ();
  
  os_ << be_nl << be_nl
      << "::" << node_->full_name () << "::" << port_name
      << "Connections *" << be_nl
      << node_->local_name () << "_Context::get_connections_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl;
      
  if (! static_config)
    {
      os_ << "ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                       mon," << be_nl
          << "                       this->" << port_name
          << "_lock_," << be_nl
          << "                       0);" << be_nl << be_nl; 
    }
    
  os_ << "::" << node_->full_name () << "::" << port_name
      << "Connections * tmp_retv = 0;" << be_nl
      << "ACE_NEW_THROW_EX (tmp_retv," << be_nl
      << "                  ::" << node_->full_name ()
      << "::" << port_name << "Connections (" << be_nl
      << "                    this->ciao_uses_"
      << port_name << "_.size ())," << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "::" << node_->full_name () << "::" << port_name
      << "Connections_var retv = tmp_retv;" << be_nl
      << "retv->length (this->ciao_uses_" << port_name
      << "_.size ());" << be_nl
      << "::CORBA::ULong i = 0UL;" << be_nl << be_nl
      << "for (" << tao_cg->upcase (port_name)
      << "_TABLE::const_iterator iter =" << be_nl
      << "       this->ciao_uses_" << port_name << "_.begin ();"
      << be_nl
      << "     iter != this->ciao_uses_"
      << port_name << "_.end ();" << be_nl
      << "     ++iter, ++i)" << be_idt_nl
      << "{" << be_idt_nl
      << "retv[i].objref = iter->second;" << be_nl << be_nl
      << "ACE_NEW_THROW_EX (retv[i].ck.inout ()," << be_nl
      << "                  ::CIAO::Cookie_Impl (iter->first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retv._retn (); " << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Context::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "std::pair<" << tao_cg->upcase (port_name)
      << "_TABLE::iterator, bool> result;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::value_type entry;" << be_nl
      << "entry.first = reinterpret_cast<ptrdiff_t> (c);" << be_nl
      << "entry.second = ::" << fname
      << "::_duplicate (c);";
  
  if (! static_config)
    {
      os_ << be_nl << be_nl
          << "{" << be_idt_nl
          << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                        mon," << be_nl
          << "                        this->" << port_name
          << "_lock_," << be_nl
          << "                        0);";
    }
    
  os_ << be_nl << be_nl
      << "result = this->ciao_uses_" << port_name
      << "_.insert (entry);";
      
  if (! static_config)
    {
      os_ << be_uidt_nl
          << "}";
    }
    
  os_ << be_nl << be_nl
      << "if (! result.second)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();"
      << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::Cookie * ck = 0;" << be_nl
      << "ACE_NEW_THROW_EX (ck," << be_nl
      << "                  ::CIAO::Cookie_Impl (entry.first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "return ck;" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::" << fname << "_ptr" << be_nl
      << node_->local_name  () << "_Context::disconnect_"
      << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl
      << "::" << fname << "_var retv;" << be_nl
      << "ptrdiff_t key = 0UL;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::size_type n = 0UL;" << be_nl << be_nl
      << "if (ck == 0 || ! ::CIAO::Cookie_Impl::extract (ck, key))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt;
      
  if (! static_config)
    {
      os_ << be_nl << be_nl
          << "{" << be_idt_nl
          << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                        mon," << be_nl
          << "                        this->" << port_name
          << "_lock_," << be_nl
          << "                        ::" << fname
          << "::_nil ());";
    }
    
  os_ << be_nl << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::iterator iter =" << be_idt_nl
      << "this->ciao_uses_" << port_name
      << "_.find (key);" << be_uidt_nl << be_nl
      << "if (iter == this->ciao_uses_" << port_name
      << "_.end ())" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "retv = iter->second;" << be_nl
      << "n = this->ciao_uses_" << port_name
      << "_.erase (key);";
      
  if (! static_config)
    {
      os_ << be_uidt_nl
          << "}";
    }
    
  os_ << be_nl << be_nl
      << "if (n != 1UL)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retv._retn ();" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_uses_servant_top (void)
{
  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Servant::connect (" << be_idt_nl
      << "const char * name," << be_nl
      << "::CORBA::Object_ptr connection)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "/// If the component has no receptacles, "
      << "arg will be unused." << be_nl
      << "ACE_UNUSED_ARG (connection);" << be_nl << be_nl
      << "if (name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}" << be_uidt;
      
  AST_Component *node = node_;  
  AST_Component::port_description *pd = 0;
      
  while (node != 0)
    {  
      for (AST_Component::PORTS::ITERATOR i =
             node->uses ().begin ();
           !i.done ();
           i.advance ())
        {
          i.next (pd);
          
          this->gen_connect_block (pd->impl->full_name (),
                                   pd->id->get_string (),
                                   pd->is_multiple);
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::CORBA::Object_ptr" << be_nl
      << node_->local_name () << "_Servant::disconnect ("
      << be_idt_nl
      << "const char * name," << be_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "ACE_UNUSED_ARG (ck);" << be_nl << be_nl
      << "if (name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt;
      
  node = node_;  
      
  while (node != 0)
    {  
      for (AST_Component::PORTS::ITERATOR i =
             node->uses ().begin ();
           !i.done ();
           i.advance ())
        {
          i.next (pd);
          
          this->gen_disconnect_block (pd->id->get_string (),
                                      pd->is_multiple);
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::ReceptacleDescriptions *" << be_nl
      << node_->local_name ()
      << "_Servant::get_all_receptacles (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::ReceptacleDescriptions * retval = 0;"
      << be_nl
      << "ACE_NEW_RETURN (retval," << be_nl
      << "                ::Components::ReceptacleDescriptions,"
      << be_nl
      << "                0);" << be_nl
      << "::Components::ReceptacleDescriptions_var "
      << "safe_retval = retval;" << be_nl
      << "safe_retval->length (" << node_->uses ().size ()
      << "UL);";
      
  node = node_;
  ACE_CDR::ULong slot = 0UL;
      
  while (node != 0)
    {  
      for (AST_Component::PORTS::ITERATOR i =
             node->uses ().begin ();
           !i.done ();
           i.advance (), ++slot)
        {
          i.next (pd);
          
          this->gen_receptacle_description (pd->impl,
                                            pd->id->get_string (),
                                            pd->is_multiple,
                                            slot);
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "return safe_retval._retn ();" << be_uidt_nl
      << "}";
    
  this->gen_uses_servant_r (node_);  
}

void
be_visitor_component_svs::gen_connect_block (
  const char *obj_name,
  const char *port_name,
  bool is_multiple)
{
  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (name, \"" << port_name
      << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << obj_name << "_var _ciao_conn =" << be_idt_nl
      << "::" << obj_name << "::_narrow (connection);"
      << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (_ciao_conn.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;
      
  if (! is_multiple)
    {
      os_ << "ACE_CString receptacle_name (\"" << port_name
          << "\");" << be_nl
          << "receptacle_name += \'_\';" << be_nl
          << "receptacle_name += "
          << "this->context_->_ciao_instance_id ();" << be_nl
          << "::CORBA::PolicyList_var policy_list =" << be_idt_nl
          << "this->container_->get_receptacle_policy "
          << "(receptacle_name.c_str ());" << be_uidt_nl << be_nl
          << "if (policy_list->length () != 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "::CORBA::Object_var over_ridden_object ="
          << be_idt_nl
          << "_ciao_conn->_set_policy_overrides (policy_list.in (),"
          << be_nl
          << "                                   CORBA::SET_OVERRIDE);"
          << be_uidt_nl
          << "_ciao_conn =" << be_idt_nl
          << "::" << obj_name << "::_narrow (over_ridden_object.in ());"
          << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;
    }
      
  os_ << "/// " << (is_multiple ? "Multiplex" : "Simplex")
      << " connect." << be_nl
      << (is_multiple ? "return " : "") << "this->connect_"
      << port_name << " (_ciao_conn.in ());";
  
  if (! is_multiple)
    {   
      os_ << be_nl << be_nl
          << "return 0;";
    }
      
  os_ << be_uidt_nl
      << "}" << be_uidt;  
}

void
be_visitor_component_svs::gen_disconnect_block (
  const char *port_name,
  bool is_multiple)
{
  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (name, \"" << port_name
      << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "/// " << (is_multiple ? "Multiplex" : "Simplex")
      << " disconnect." << be_nl;
      
 if (is_multiple)
  {
    os_ << "if (ck == 0)" << be_idt_nl
        << "{" << be_idt_nl
        << "throw ::Components::CookieRequired ();" << be_uidt_nl
        << "}" << be_uidt_nl << be_nl;
  }
      
 os_ << "return this->disconnect_" << port_name
      << " (" << (is_multiple ? "ck" : "") << ");" << be_uidt_nl
      << "}" << be_uidt;
}

void
be_visitor_component_svs::gen_uses_servant_r (
  AST_Component *node)
{
  if (node == 0)
    {
      return;
    }

  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node->uses ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      this->gen_uses_servant (pd->impl->full_name (),
                              pd->id->get_string (),
                              pd->is_multiple);
    }
    
  node = node->base_component ();
  this->gen_uses_servant_r (node);
}

void
be_visitor_component_svs::gen_uses_servant (const char *obj_name,
                                            const char *port_name,
                                            bool is_multiple)
{
  os_ << be_nl << be_nl
      << (is_multiple ? "::Components::Cookie *" : "void")
      << be_nl
      << node_->local_name () << "_Servant::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << obj_name << "_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (is_multiple)
    {
      os_ << "::Components::Cookie * cookie =" << be_idt_nl;
    }
      
  os_ << "this->context_->connect_" << port_name
      << " (c);" << be_nl;
      
  if (is_multiple)
    {
      os_ << be_uidt_nl;
    }
      
  os_ << "this->add_receptacle (\"" << port_name
      << "\", c, " << (is_multiple ? "cookie" : "0")
      << ");";
      
  if (is_multiple)
    {
      os_ << be_nl << be_nl
          << "return cookie;";
    }
      
  os_ << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::" << obj_name << "_ptr" << be_nl
      << node_->local_name () << "_Servant::disconnect_"
      << port_name << " (";
      
  if (is_multiple)
    {
      os_ << be_idt_nl
          << "::Components::Cookie * ck" << be_uidt;
    }
  else
    {
      os_ << "void";
    }
      
  os_ << ")" << be_nl
      << "{" << be_idt_nl
      << "return this->context_->disconnect_"
      << port_name << " (" << (is_multiple ? "ck" : "")
      << ");" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::";
      
  if (is_multiple)
    {
      os_ << node_->full_name () << "::" << port_name
          << "Connections *";
    }
  else
    {
      os_ << obj_name << "_ptr";
    }
       
  os_ << be_nl
      << node_->local_name () << "_Servant::get_connection"
      << (is_multiple ? "s" : "") << "_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->context_->get_connection"
      << (is_multiple ? "s" : "") << "_"
      << port_name << " ();" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_receptacle_description (
  AST_Type *obj,
  const char *port_name,
  bool is_multiple,
  ACE_CDR::ULong slot)
{
  os_ << be_nl << be_nl;
  
  bool gen_guard =
    is_multiple && ! be_global->gen_ciao_static_config ();
  
  if (gen_guard)
    {
      os_ << "{" << be_idt_nl
          << "ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                       mon," << be_nl
          << "                       this->context_->"
          << port_name << "_lock_," << be_nl
          << "                       0);" << be_nl << be_nl;
    }
  
  os_ << "::CIAO::Servant::describe_"
      << (is_multiple ? "multiplex" : "simplex")
      << "_receptacle<" << be_idt_nl
      << "::" << obj->full_name () << "_var> (" << be_idt_nl
      << "\"" << port_name << "\"," << be_nl
      << "\"" << obj->repoID () << "\"," << be_nl
      << "this->context_->ciao_uses_"
      << port_name << "_," << be_nl
      << "safe_retval," << be_nl
      << slot << "UL);" << be_uidt << be_uidt;
      
  if (gen_guard)
    {
      os_ << be_uidt_nl
          << "}";
    }
}

void
be_visitor_component_svs::gen_publishes_context_r (
  AST_Component *node)
{
  if (node == 0)
    {
      return;
    }

  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node->publishes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      this->gen_publishes_context (pd->impl,
                                   pd->id->get_string ());
    }
    
  node = node->base_component ();
  this->gen_publishes_context_r (node);
}

void
be_visitor_component_svs::gen_publishes_context (
  AST_Type *obj,
  const char *port_name)
{
  const char *fname = obj->full_name ();
  const char *lname = obj->local_name ()->get_string ();
  ACE_CString sname_str (
    ScopeAsDecl (obj->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();
  const char *global = (sname_str == "" ? "" : "::");
  bool static_config =
    be_global->gen_ciao_static_config ();

  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Context::push_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << " * ev)" << be_uidt_nl
      << "{" << be_idt_nl;
  
  if (! static_config)
    {
      os_ << "ACE_READ_GUARD (TAO_SYNCH_MUTEX," << be_nl
          << "                mon," << be_nl
          << "                this->" << port_name
          << "_lock_);" << be_nl << be_nl;
    }
      
  os_ << "for (" << tao_cg->upcase (port_name)
      << "_TABLE::const_iterator iter =" << be_nl
      << "       this->ciao_publishes_" << port_name
      << "_.begin ();" << be_nl
      << "     iter != this->ciao_publishes_" << port_name
      << "_.end ();" << be_nl
      << "     ++iter)" << be_idt_nl
      << "{" << be_idt_nl
      << "iter->second->push_" << lname << " (ev);"
      << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "ACE_CString source_id (this->_ciao_instance_id ());"
      << be_nl
      << "source_id += \"_" << port_name << "\";"
      << be_nl << be_nl
      << "for (" << tao_cg->upcase (port_name)
      << "_GENERIC_TABLE::const_iterator giter =" << be_nl
      << "       this->ciao_publishes_" << port_name
      << "_generic_.begin ();" << be_nl
      << "     giter != this->ciao_publishes_"
      << port_name << "_generic_.end ();" << be_nl
      << "     ++giter)" << be_idt_nl
      << "{" << be_idt_nl
      << "giter->second->ciao_push_event (ev," << be_nl
      << "                                source_id.c_str (),"
      << be_nl
      << "                                " << global
      << sname << "::_tc_" << lname << ");" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Context::subscribe_"
      << port_name << " (" << be_idt_nl
      << "::" << fname << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "std::pair<" << tao_cg->upcase (port_name)
      << "_TABLE::iterator, bool> result;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::value_type entry;" << be_nl
      << "entry.first = reinterpret_cast<ptrdiff_t> (c);"
      << be_nl
      << "entry.second = ::" << fname
      << "Consumer::_duplicate (c);" << be_nl << be_nl;
  
  if (! static_config)
    {    
    os_ << "{" << be_idt_nl
        << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
        << "                        mon," << be_nl
        << "                        this->" << port_name
        << "_lock_," << be_nl
        << "                        0);" << be_nl << be_nl;
    }
      
  os_ << "result = this->ciao_publishes_" << port_name
      << "_.insert (entry);";
  
  if (! static_config)
    {    
      os_ << be_uidt_nl
          << "}";
    }
      
  os_ << be_nl << be_nl
      << "if (! result.second)" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_ERROR_RETURN ((LM_ERROR," << be_nl
      << "                   ACE_TEXT (\"subscribe on %s failed\\n\"),"
      << be_nl
      << "                   ACE_TEXT (\"" << port_name
      << "\"))," << be_nl
      << "                  0);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::Cookie * retv = 0;" << be_nl
      << "ACE_NEW_THROW_EX (retv," << be_nl
      << "                  ::CIAO::Cookie_Impl (entry.first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "return retv;" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Context::subscribe_"
      << port_name << "_generic (" << be_idt_nl
      << "::Components::EventConsumerBase_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "std::pair<" << tao_cg->upcase (port_name)
      << "_GENERIC_TABLE::iterator, bool> result;" << be_nl
      << tao_cg->upcase (port_name)
      << "_GENERIC_TABLE::value_type entry;" << be_nl
      << "entry.first = reinterpret_cast<ptrdiff_t> (c);"
      << be_nl
      << "entry.second =" << be_idt_nl
      << "::Components::EventConsumerBase::_duplicate (c);"
      << be_uidt_nl << be_nl;
  
  if (! static_config)
    {    
      os_ << "{" << be_idt_nl
          << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                        mon," << be_nl
          << "                        this->" << port_name
          << "_lock_," << be_nl
          << "                        0);" << be_nl << be_nl;
    }
      
  os_ << "result =" << be_idt_nl
      << "this->ciao_publishes_" << port_name
      << "_generic_.insert (entry);" << be_uidt;
  
  if (! static_config)
    {    
      os_ << be_uidt_nl
          << "}";
    }
      
  os_ << be_nl << be_nl
      << "if (! result.second)" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_ERROR_RETURN ((LM_ERROR," << be_nl
      << "                   ACE_TEXT (\"generic subscribe \")"
      << be_nl
      << "                   ACE_TEXT (\"on %s failed\\n\"),"
      << be_nl
      << "                   ACE_TEXT (\"" << port_name
      << "\"))," << be_nl
      << "                  0);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::Cookie * retv = 0;" << be_nl
      << "ACE_NEW_THROW_EX (retv," << be_nl
      << "                  ::CIAO::Cookie_Impl (entry.first),"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "return retv;" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::" << fname << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Context::unsubscribe_"
      << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl
      << "ptrdiff_t key = 0UL;" << be_nl
      << tao_cg->upcase (port_name)
      << "_TABLE::size_type n = 0UL;" << be_nl << be_nl
      << "if (ck == 0 || ! ::CIAO::Cookie_Impl::extract (ck, key))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;
  
  if (! static_config)
    {    
      os_ << "{" << be_idt_nl
          << "ACE_WRITE_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                        mon," << be_nl
          << "                        this->" << port_name
          << "_lock_," << be_nl
          << "                        ::" << fname
          << "Consumer::_nil ());" << be_nl << be_nl;
    }
      
  os_ << tao_cg->upcase (port_name) << "_TABLE::iterator iter ="
      << be_idt_nl
      << "this->ciao_publishes_" << port_name << "_.find (key);"
      << be_uidt_nl << be_nl
      << "if (iter != this->ciao_publishes_" << port_name
      << "_.end ())" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << fname
      << "Consumer_var retv = iter->second;" << be_nl
      << "n = this->ciao_publishes_" << port_name
      << "_.erase (key);" << be_nl << be_nl
      << "if (n == 1UL)" << be_idt_nl
      << "{" << be_idt_nl
      << "return retv._retn ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << tao_cg->upcase (port_name)
      << "_GENERIC_TABLE::iterator giter =" << be_idt_nl
      << "this->ciao_publishes_" << port_name
      << "_generic_.find (key);" << be_uidt_nl << be_nl
      << "if (giter == this->ciao_publishes_" << port_name
      << "_generic_.end ())" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "n = this->ciao_publishes_" << port_name
      << "_generic_.erase (key);";
  
  if (! static_config)
    {    
      os_ << be_uidt_nl
          << "}";
    }
      
  os_ << be_nl << be_nl
      << "if (n != 1UL)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return ::" << fname << "Consumer::_nil ();"
      << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_publishes_servant_top (void)
{
  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Servant::subscribe ("
      << be_idt_nl
      << "const char * publisher_name," << be_nl
      << "::Components::EventConsumerBase_ptr subscribe)"
      << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "ACE_UNUSED_ARG (subscribe);" << be_nl << be_nl
      << "if (publisher_name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}" << be_uidt;
      
  AST_Component *node = node_;
  AST_Component::port_description *pd = 0;
  
  while (node != 0)
    {
      for (AST_Component::PORTS::ITERATOR i =
             node->publishes ().begin ();
           !i.done ();
           i.advance ())
        {
          i.next (pd);
          
          this->gen_subscribe_block (pd->impl->full_name (),
                                     pd->id->get_string ());
        }
        
      node = node->base_component ();
    }
      
  os_ << be_nl << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::EventConsumerBase_ptr" << be_nl
      << node_->local_name () << "_Servant::unsubscribe ("
      << be_idt_nl
      << "const char * publisher_name," << be_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "ACE_UNUSED_ARG (ck);" << be_nl << be_nl
      << "if (publisher_name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}" << be_uidt;
     
  node = node_;
  
  while (node != 0)
    {
      for (AST_Component::PORTS::ITERATOR i =
             node->publishes ().begin ();
           !i.done ();
           i.advance ())
        {
          i.next (pd);
          
          this->gen_unsubscribe_block (pd->id->get_string ());
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::PublisherDescriptions *" << be_nl
      << node_->local_name ()
      << "_Servant::get_all_publishers (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::PublisherDescriptions *retval = 0;"
      << be_nl
      << "ACE_NEW_RETURN (retval," << be_nl
      << "                ::Components::PublisherDescriptions,"
      << be_nl
      << "                0);" << be_nl << be_nl
      << "::Components::PublisherDescriptions_var "
      << "safe_retval = retval;" << be_nl
      << "safe_retval->length (" << node_->publishes ().size ()
      << "UL);";
    
  node = node_;
  ACE_CDR::ULong slot = 0UL;
  
  while (node != 0)
    {
      for (AST_Component::PORTS::ITERATOR i =
             node->publishes ().begin ();
           !i.done ();
           i.advance (), ++slot)
        {
          i.next (pd);
          
          this->gen_event_source_description (pd->impl,
                                              pd->id->get_string (),
                                              slot);
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "return safe_retval._retn ();" << be_uidt_nl
      << "}";
    
  this->gen_publishes_servant_r (node_);
}

void
be_visitor_component_svs::gen_subscribe_block (
  const char *obj_name,
  const char *port_name)
{
  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (publisher_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << obj_name << "Consumer_var sub =" << be_idt_nl
      << "::" << obj_name << "Consumer::_narrow (subscribe);"
      << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (sub.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "::CORBA::Boolean const substitutable =" << be_idt_nl
      << "subscribe->ciao_is_substitutable (" << be_idt_nl
      << "::" << obj_name
      << "::_tao_obv_static_repository_id ());"
      << be_uidt << be_uidt_nl << be_nl
      << "if (substitutable)" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->subscribe_" << port_name
      << "_generic (subscribe);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->subscribe_" << port_name
      << " (sub.in ());" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt;
}

void
be_visitor_component_svs::gen_unsubscribe_block (
  const char *port_name)
{
  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (publisher_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->unsubscribe_" << port_name
      << " (ck);" << be_uidt_nl
      << "}" << be_uidt;
}

void
be_visitor_component_svs::gen_event_source_description (
  AST_Type *obj,
  const char *port_name,
  ACE_CDR::ULong slot)
{
  os_ << be_nl << be_nl;
  
  if (! be_global->gen_ciao_static_config ())
    {
      os_ << "{" << be_idt_nl
          << "ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                       mon," << be_nl
          << "                       this->context_->"
          << port_name << "_lock_," << be_nl
          << "                       0);" << be_nl << be_nl;
    }
    
  os_ << "::CIAO::Servant::describe_pub_event_source<"
      << be_idt_nl
      << "::" << obj->full_name () << "Consumer_var> ("
      << be_idt_nl
      << "\"" << port_name << "\"," << be_nl
      << "\"" << obj->repoID () << "\"," << be_nl
      << "this->context_->ciao_publishes_"
      << port_name << "_," << be_nl
      << "safe_retval," << be_nl
      << slot << "UL);" << be_uidt << be_uidt;

  if (! be_global->gen_ciao_static_config ())
    {
      os_ << be_uidt_nl
          << "}";
    }
}
                                     
void
be_visitor_component_svs::gen_publishes_servant_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }

  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node->publishes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      this->gen_publishes_servant (pd->impl->full_name (),
                                   pd->id->get_string ());
    }
    
  node = node->base_component ();
  this->gen_publishes_servant_r (node);
}

void
be_visitor_component_svs::gen_publishes_servant (
  const char *obj_name,
  const char *port_name)
{
  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Servant::subscribe_"
      << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl;
      
   if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl;
    }
      
 os_ << "return this->context_->subscribe_" << port_name
      << " (c);" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::Cookie *" << be_nl
      << node_->local_name () << "_Servant::subscribe_"
      << port_name << "_generic (" << be_idt_nl
      << "::Components::EventConsumerBase_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl;
    }
      
  os_ << "return this->context_->subscribe_" << port_name
      << "_generic (c);" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::" << obj_name << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Servant::unsubscribe_"
      << port_name << " (" << be_idt_nl
      << "::Components::Cookie * ck)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl;
    }
      
  os_ << "return this->context_->unsubscribe_" << port_name
      << " (ck);" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_consumes_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }
    
  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node->consumes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      this->gen_consumes (pd->impl,
                          pd->id->get_string ());
    }
    
  node = node->base_component ();
  this->gen_consumes_r (node);
}

void
be_visitor_component_svs::gen_consumes (AST_Type *obj,
                                        const char *port_name)
{
  const char *comp_lname = node_->local_name ();
  ACE_CString comp_sname_str (
    ScopeAsDecl (node_->defined_in ())->full_name ());   
  const char *comp_sname = comp_sname_str.c_str ();
  const char *global = (comp_sname_str == "" ? "" : "::");
    
  const char *lname = obj->local_name ()->get_string ();
  const char *fname = obj->full_name ();  
    
  os_ << be_nl << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant (" << be_idt << be_idt_nl
      << global << comp_sname << "::CCM_" << comp_lname
      << "_ptr executor," << be_nl
      << global << comp_sname << "::CCM_" << comp_lname
      << "_Context_ptr c)" << be_uidt_nl
      << ": executor_ ( " << global << comp_sname << "::CCM_"
      << comp_lname << "::_duplicate (executor))," << be_idt_nl
      << "ctx_ ( " << global << comp_sname
      << "::CCM_" << comp_lname
      << "_Context::_duplicate (c))" << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";
    
  os_ << be_nl << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::~" << lname << "Consumer_"
      << port_name << "_Servant (void)" << be_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::CORBA::Object_ptr" << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::_get_component (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->ctx_->get_CCM_object ();" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::push_" << lname
      << " (" << be_idt_nl
      << "::" << fname << " * evt)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->executor_->push_" << port_name
      << " (evt);" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "/// Inherited from ::Components::EventConsumerBase."
      << be_nl
      << "void" << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::push_event (" << be_idt_nl
      << "::Components::EventBase * ev)" << be_uidt_nl
      << "{" << be_idt_nl
      << "::" << fname << " * ev_type =" << be_idt_nl
      << "::" << fname << "::_downcast (ev);"
      << be_uidt_nl << be_nl
      << "if (ev_type != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "this->push_" << lname << " (ev_type);" << be_nl
      << "return;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "throw ::Components::BadEventType ();" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "/// CIAO-specific."
      << be_nl
      << "void" << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::ciao_push_event (" << be_idt_nl
      << "::Components::EventBase * ev," << be_nl
      << "const char * /* source_id */," << be_nl
      << "::CORBA::TypeCode_ptr /* tc */)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->push_event (ev);" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "/// CIAO-specific."
      << be_nl
      << "::CORBA::Boolean" << be_nl
      << comp_lname << "_Servant::" << lname << "Consumer_"
      << port_name << "_Servant::ciao_is_substitutable ("
      << be_idt_nl
      << "const char * event_repo_id)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (event_repo_id == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()->orb ();"
      << be_nl << be_nl
      << "CORBA::ValueFactory f =" << be_idt_nl
      << "orb->lookup_value_factory (event_repo_id);"
      << be_uidt_nl << be_nl
      << "if (f == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "CORBA::ValueBase_var v = f->create_for_unmarshal ();"
      << be_nl
      << "f->_remove_ref ();" << be_nl << be_nl
      << "if (v.in () == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return dynamic_cast< ::" << fname
      << " *> (v.in ()) != 0;" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::" << fname << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Servant::get_consumer_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (this->consumes_" << port_name
      << "_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "return ::" << fname
      << "Consumer::_duplicate (this->consumes_"
      << port_name << "_.in ());" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::EventConsumerBase_var obj =" << be_idt_nl
      << "this->get_consumer_" << port_name << "_i ();"
      << be_uidt_nl << be_nl
      << "::" << fname << "Consumer_var eco =" << be_idt_nl
      << "::" << fname << "Consumer::_narrow (obj.in ());"
      << be_uidt_nl << be_nl
      << "this->consumes_" << port_name << "_ = eco;" << be_nl
      << "return" << be_idt_nl
      << "::" << fname << "Consumer::_duplicate (" << be_idt_nl
      << "this->consumes_" << port_name << "_.in ());"
      << be_uidt << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::EventConsumerBase_ptr" << be_nl
      << node_->local_name () << "_Servant::get_consumer_"
      << port_name << "_i (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::EventConsumerBase_ptr ret =" << be_idt_nl
      << "this->lookup_consumer (\"" << port_name << "\");"
      << be_uidt_nl << be_nl
      << "if (! ::CORBA::is_nil (ret))" << be_idt_nl
      << "{" << be_idt_nl
      << "return ret;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "typedef" << be_idt_nl
      << "::CIAO::Port_Activator_T<" << be_idt_nl
      << node_->local_name () << "_Servant::" << lname
      << "Consumer_" << port_name << "_Servant," << be_nl
      << global << comp_sname << "::CCM_" << comp_lname
      << "," << be_nl
      << global << comp_sname << "::CCM_" << comp_lname
      << "_Context," << be_nl
      << node_->local_name () << "_Servant>" << be_uidt_nl
      << "MACRO_MADNESS_TYPEDEF;" << be_uidt_nl << be_nl
      << "MACRO_MADNESS_TYPEDEF * tmp = 0;" << be_nl
      << "ACE_CString obj_id (this->ins_name_);" << be_nl
      << "obj_id += \"_" << port_name << "\";" << be_nl << be_nl
      << "ACE_NEW_THROW_EX (" << be_idt_nl
      << "tmp," << be_nl
      << "MACRO_MADNESS_TYPEDEF (" << be_idt_nl
      << "obj_id.c_str ()," << be_nl
      << "\"" << port_name << "\"," << be_nl
      << "::CIAO::Port_Activator_Types::SINK," << be_nl
      << "this->executor_.in ()," << be_nl
      << "this->context_," << be_nl
      << "this)," << be_uidt_nl
      << "::CORBA::NO_MEMORY ());" << be_uidt_nl << be_nl
      << "::CIAO::Port_Activator_var pa = tmp;" << be_nl << be_nl
      << "::CIAO::Servant_Activator_var sa =" << be_idt_nl
      << "this->container_->ports_servant_activator ();"
      << be_uidt_nl << be_nl
      << "if (! sa->register_port_activator (tmp))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "return ::" << fname << "Consumer::_nil ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::CORBA::Object_var obj =" << be_idt_nl
      << "this->container_->generate_reference (" << be_idt_nl
      << "obj_id.c_str ()," << be_nl
      << "\"";
      
  ACE_CString work (obj->repoID ());
  ACE_CString result (work.substr (0, work.rfind (':')));
  result += "Consumer:1.0";
  
  os_ << result.c_str ();
      
  os_ << "\"," << be_nl
      << "::CIAO::Container_Types::FACET_CONSUMER_t);"
      << be_uidt_nl << be_uidt_nl
      << "::Components::EventConsumerBase_var ecb =" << be_idt_nl
      << "::Components::EventConsumerBase::_narrow (obj.in ());"
      << be_uidt_nl << be_nl
      << "this->add_consumer (\"" << port_name << "\", ecb.in ());"
      << be_nl << be_nl
      << "return ecb._retn ();" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_emits_context_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }

  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node->emits ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      this->gen_emits_context (pd->impl,
                               pd->id->get_string ());
    }
    
  node = node->base_component ();
  this->gen_emits_context_r (node);
}

void
be_visitor_component_svs::gen_emits_context (
  AST_Type *obj,
  const char *port_name)
{
  const char *fname = obj->full_name ();
  const char *lname = obj->local_name ()->get_string ();
  
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Context::push_" << port_name
      << " (" << be_idt_nl
      << "::" << fname << " * ev)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (this->ciao_emits_"
      << port_name << "_consumer_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "this->ciao_emits_" << port_name << "_consumer_->push_"
      << lname << " (ev);" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Context::connect_" << port_name
      << " (" << be_idt_nl
      << "::" << fname << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (c))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if (! ::CORBA::is_nil (this->ciao_emits_"
      << port_name << "_consumer_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::AlreadyConnected ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->ciao_emits_" << port_name
      << "_consumer_ =" << be_idt_nl
      << "::" << fname << "Consumer::_duplicate (c);"
      << be_uidt << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::" << fname << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Context::disconnect_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (this->ciao_emits_"
      << port_name << "_consumer_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::NoConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return this->ciao_emits_" << port_name
      << "_consumer_._retn ();" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_emits_servant_top (void)
{
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Servant::connect_consumer ("
      << be_idt_nl
      << "const char * emitter_name," << be_nl
      << "::Components::EventConsumerBase_ptr consumer)"
      << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "if (emitter_name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt;
      
  AST_Component *node = node_;
  AST_Component::port_description *pd = 0;
  
  while (node != 0)
    {
      for (AST_Component::PORTS::ITERATOR i =
             node->emits ().begin ();
           !i.done ();
           i.advance ())
        {
          i.next (pd);
          
          this->gen_connect_consumer_block (pd->impl->full_name (),
                                            pd->id->get_string ());
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "ACE_UNUSED_ARG (consumer);" << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::EventConsumerBase_ptr" << be_nl
      << node_->local_name ()
      << "_Servant::disconnect_consumer (" << be_idt_nl
      << "const char * source_name)" << be_uidt_nl
      << "{" << be_idt_nl;
      
  if (swapping_)
    {
      os_ << "this->activate_component ();" << be_nl << be_nl;
    }
      
  os_ << "if (source_name == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
      << "}" << be_uidt;
      
  node = node_;
  
  while (node != 0)
    {
      for (AST_Component::PORTS::ITERATOR i =
             node->emits ().begin ();
           !i.done ();
           i.advance ())
        {
          i.next (pd);
          
          this->gen_disconnect_consumer_block (pd->id->get_string ());
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "throw ::Components::InvalidName ();" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::Components::EmitterDescriptions *" << be_nl
      << node_->local_name ()
      << "_Servant::get_all_emitters (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::EmitterDescriptions *retval = 0;"
      << be_nl
      << "ACE_NEW_RETURN (retval," << be_nl
      << "                ::Components::EmitterDescriptions,"
      << be_nl
      << "                0);" << be_nl << be_nl
      << "::Components::EmitterDescriptions_var "
      << "safe_retval = retval;" << be_nl
      << "safe_retval->length (" << node_->emits ().size ()
      << "UL);";
      
  node = node_;
  ACE_CDR::ULong slot = 0UL;
  
  while (node != 0)
    {
      for (AST_Component::PORTS::ITERATOR i =
             node->emits ().begin ();
           !i.done ();
           i.advance (), ++slot)
        {
          i.next (pd);
          
          this->gen_emitter_description (pd->impl,
                                         pd->id->get_string (),
                                         slot);
        }
        
      node = node->base_component ();
    }
    
  os_ << be_nl << be_nl
      << "return safe_retval._retn ();" << be_uidt_nl
      << "}";
      
  this->gen_emits_servant_r (node_);
}

void
be_visitor_component_svs::gen_connect_consumer_block (
  const char *obj_name,
  const char *port_name)
{
  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (emitter_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "::" << obj_name
      << "Consumer_var _ciao_consumer =" << be_idt_nl
      << "::" << obj_name
      << "Consumer::_narrow (consumer);" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (_ciao_consumer.in ()))"
      << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->connect_" << port_name
      << " (_ciao_consumer.in ());" << be_nl << be_nl
      << "return;" << be_uidt_nl
      << "}" << be_uidt;
}

void
be_visitor_component_svs::gen_disconnect_consumer_block (
  const char *port_name)
{
  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (source_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return this->disconnect_" << port_name
      << " ();" << be_uidt_nl
      << "}" << be_uidt;
}

void
be_visitor_component_svs::gen_emitter_description (
  AST_Type *obj,
  const char *port_name,
  ACE_CDR::ULong slot)
{
  os_ << be_nl << be_nl
      << "::CIAO::Servant::describe_emit_event_source<"
      << be_idt_nl
      << "::" << obj->full_name () << "Consumer_var> ("
      << be_idt_nl
      << "\"" << port_name << "\"," << be_nl
      << "\"" << obj->repoID () << "\"," << be_nl
      << "this->context_->ciao_emits_" << port_name
      << "_consumer_," << be_nl
      << "safe_retval," << be_nl
      << slot << "UL);" << be_uidt << be_uidt;
}

void
be_visitor_component_svs::gen_emits_servant_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }

  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i = node->emits ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      this->gen_emits_servant (pd->impl->full_name (),
                               pd->id->get_string ());
    }
    
  node = node->base_component ();
  this->gen_emits_context_r (node);
}

void
be_visitor_component_svs::gen_emits_servant (const char *obj_name,
                                             const char *port_name)
{
  os_ << be_nl << be_nl
      << "void" << be_nl
      << node_->local_name () << "_Servant::connect_"
      << port_name << " (" << be_idt_nl
      << "::" << obj_name << "Consumer_ptr c)" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->context_->connect_" << port_name
      << " (c);" << be_uidt_nl
      << "}";
      
  os_ << be_nl << be_nl
      << "::" << obj_name << "Consumer_ptr" << be_nl
      << node_->local_name () << "_Servant::disconnect_"
      << port_name << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->context_->disconnect_"
      << port_name << " ();" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_attr_set_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }
    
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      // Get the next AST decl node
      AST_Decl *d = si.item ();

      if (d->node_type () == AST_Decl::NT_attr)
        {
          be_attribute *attr =
            be_attribute::narrow_from_decl (d);
            
          if (attr->readonly ())
            {
              continue;
            }
        
          be_visitor_attribute_component_init v (this->ctx_);
          
          if (v.visit_attribute (attr) == -1)
            {
            }
        }
    }
    
  node = node->base_component ();
  this->gen_attr_set_r (node);
}

void
be_visitor_component_svs::gen_populate_r (AST_Component *node)
{
  if (node == 0)
    {
      return;
    }

  AST_Component::port_description *pd = 0;
  
  for (AST_Component::PORTS::ITERATOR i =
         node->provides ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      os_ << be_nl
          << "obj_var = this->provide_"
          << pd->id->get_string () << "_i ();";
    }
    
  for (AST_Component::PORTS::ITERATOR i =
         node->consumes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      os_ << be_nl
          << "ecb_var = this->get_consumer_"
          << pd->id->get_string () << "_i ();";
    }
    
  node = node->base_component ();
  this->gen_populate_r (node);
}

void
be_visitor_component_svs::gen_entrypoint (void)
{
  ACE_CString sname_str (
    ScopeAsDecl (node_->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::PortableServer::Servant" << be_nl
      << "create_" << node_->flat_name ()
      << "_Servant (" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr p," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << "const char * ins_name)" << be_uidt_nl
      << "{" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_var x =" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "::_narrow (p);" << be_uidt_nl << be_nl
      << "if ( ::CORBA::is_nil (x.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::PortableServer::Servant retval = 0;" << be_nl
      << "ACE_NEW_RETURN (retval," << be_nl
      << "                " << lname << "_Servant (" << be_idt_nl
      << "                x.in ()," << be_nl
      << "                ::Components::CCMHome::_nil ()," << be_nl
      << "                ins_name," << be_nl
      << "                0," << be_nl
      << "                c)," << be_uidt_nl
      << "                0);" << be_nl << be_nl
      << "return retval;" << be_uidt_nl
      << "}";
}

void
be_visitor_component_svs::gen_all_factory_registration (void)
{
  AST_Component::port_description *pd = 0;

  for (AST_Component::PORTS::ITERATOR i = node_->publishes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      this->gen_one_factory_registration (pd->impl);
    }
    
  for (AST_Component::PORTS::ITERATOR i = node_->emits ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      this->gen_one_factory_registration (pd->impl);
    }
    
  for (AST_Component::PORTS::ITERATOR i = node_->consumes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      this->gen_one_factory_registration (pd->impl);
    }
}

void
be_visitor_component_svs::gen_one_factory_registration (AST_Type *t)
{
  be_valuetype *v = be_valuetype::narrow_from_decl (t);
  be_valuetype::FactoryStyle fs = v->determine_factory_style ();
  
  if (fs == be_valuetype::FS_CONCRETE_FACTORY)
    {
      os_ << be_nl << be_nl
          << "TAO_OBV_REGISTER_FACTORY (" << be_idt_nl
          << "::" << t->full_name () << "_init," << be_nl
          << "::" << t->full_name () << ");" << be_uidt;
    }
}

void
be_visitor_component_svs::gen_swapping_get_consumers_r (
  AST_Component *node)
{
  if (node == 0)
    {
      return;
    }
    
  AST_Component::port_description *pd = 0;

  for (AST_Component::PORTS::ITERATOR i =
         node->publishes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      this->gen_swapping_get_comsumer_block (pd->id->get_string ());
    }
    
  node = node->base_component ();
  this->gen_swapping_get_consumers_r (node);
}

void
be_visitor_component_svs::gen_swapping_get_comsumer_block (
  const char * port_name)
{
  bool static_config =
    be_global->gen_ciao_static_config ();

  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (publisher_name, \""
      << port_name << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "_ciao_size = this->_ciao_publishes_"
      << port_name << "_.size ();" << be_nl << be_nl
      << "ACE_NEW_THROW_EX (tmp," << be_nl
      << "                  ::Components::"
      << "ConsumerDescriptions (_ciao_size)," << be_nl
      << "                  ::CORBA::NO_MEMORY ());";
      
  if (! static_config)
    {
      os_ << be_nl << be_nl
          << "{" << be_idt_nl
          << "ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX," << be_nl
          << "                       mon," << be_nl
          << "                       this->" << port_name
          << "_lock_," << be_nl
          << "                       0);";
    }
    
  os_ << be_nl << be_nl
      << "for (" << tao_cg->upcase (port_name)
      << "_TABLE::const_iterator iter =" << be_idt_nl
      << "     this->ciao_publishes_" << port_name
      << "_.begin ();" << be_uidt_nl
      << "     iter != this->ciao_publishes_" << port_name
      << ".end ();" << be_nl
      << "     ++iter, ++_ciao_index)" << be_idt_nl
      << "{" << be_idt_nl
      << "if ( ::CORBA::is_nil (iter->second.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "throw ::Components::InvalidConnection ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::Components::ConsumerDescription * cd = 0;" << be_nl
      << "ACE_NEW_THROW_EX (cd," << be_nl
      << "                  OBV_Components::ConsumerDescription,"
      << be_nl
      << "                  ::CORBA::NO_MEMORY ());"
      << be_nl << be_nl
      << "::Components::ConsumerDescription_var safe = cd;"
      << be_nl
      << "safe->name (\"\");" << be_nl
      << "safe->type_id (\"\");" << be_nl
      << "safe->consumer (iter->second.in ());"
      << be_nl << be_nl
      << "retval[_ciao_index] = safe;" << be_uidt_nl
      << "}" << be_uidt;
      
  if (! static_config)
    {
      os_ << be_uidt_nl
          << "}";
    }
    
  os_ << be_uidt_nl << be_nl
      << "return retval._retn ();" << be_uidt_nl
      << "}";
}

Component_Op_Attr_Generator::Component_Op_Attr_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Component_Op_Attr_Generator::emit (be_interface * /*derived_interface */,
                                   TAO_OutStream * /* os */,
                                   be_interface * base_interface)
{
  if (base_interface->node_type () == AST_Decl::NT_component)
    {
      return 0;
    }

  return visitor_->visit_scope (base_interface);
}

