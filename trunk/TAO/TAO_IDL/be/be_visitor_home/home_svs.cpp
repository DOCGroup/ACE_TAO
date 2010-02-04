
//=============================================================================
/**
 *  @file    home_svs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Homes in the servant source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ******************************************************
// Home visitor for server source
// ******************************************************

be_visitor_home_svs::be_visitor_home_svs (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    comp_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->svnt_export_macro ()),
    swapping_ (be_global->gen_component_swapping ()),
    for_finder_ (false)
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

be_visitor_home_svs::~be_visitor_home_svs (void)
{
}

int
be_visitor_home_svs::visit_home (be_home *node)
{
  if (node->imported ())
    {
      return 0;
    }

  node_ = node;
  comp_ = node_->managed_component ();

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << comp_->flat_name () << "_Impl" << be_nl
      << "{" << be_idt;

  if (this->gen_servant_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_svs::")
                         ACE_TEXT ("visit_home - ")
                         ACE_TEXT ("gen_servant_class() failed\n")),
                        -1);
    }

  this->gen_entrypoint ();

  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_home_svs::visit_operation (be_operation *node)
{
  be_visitor_operation_svs v (this->ctx_);
  v.scope (node_);
  return v.visit_operation (node);
}

int
be_visitor_home_svs::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  v.op_scope (node_);
  return v.visit_attribute (node);
}

int
be_visitor_home_svs::visit_argument (be_argument *node)
{
  os_ << node->local_name ();

  if (! this->last_node (node))
    {
      os_ << "," << be_nl;
    }

  return 0;
}

int
be_visitor_home_svs::visit_factory (be_factory *node)
{
  os_ << be_nl << be_nl
      << "::" << comp_->name () << "_ptr" << be_nl
      << node_->original_local_name ()->get_string ()
      << "_Servant::" << node->local_name ();
      
  be_visitor_operation_arglist al_visitor (this->ctx_);
  al_visitor.unused (this->for_finder_);

  if (al_visitor.visit_factory (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_factory_svs::"
                         "visit_factory - "
                         "codegen for argument list failed\n"),
                        -1);
    }
                            
  os_ << be_nl
      << "{" << be_idt_nl;
      
  if (this->for_finder_)
    {
      os_ << "throw ::CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,"
          << be_nl
          << "                             CORBA::COMPLETED_NO);";
    }
  else
    {
      ACE_CString comp_sname_str (
        ScopeAsDecl (comp_->defined_in ())->full_name ());
      const char *comp_sname = comp_sname_str.c_str ();
      const char *comp_lname = comp_->local_name ()->get_string ();
      const char *global = (comp_sname_str == "" ? "" : "::");

      os_ << "::Components::EnterpriseComponent_var _ciao_ec ="
          << be_idt_nl
          << "this->executor_->" << node->local_name () << " (";
     
      if (node->argument_count () > 0)
        {
          os_ << be_idt_nl;
        
          if (this->visit_scope (node) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_factory_svs::"
                                 "visit_factory - "
                                 "codegen for scope failed\n"),
                                -1);
            }
            
          os_ << be_uidt;
        }
        
      os_ << ");" << be_uidt_nl << be_nl
          << global << comp_sname << "::CCM_" << comp_lname
          << "_var _ciao_comp =" << be_idt_nl
          << global << comp_sname << "::CCM_" << comp_lname
          << "::_narrow (_ciao_ec.in ());" << be_uidt_nl << be_nl
          << "return this->_ciao_activate_component "
          << "(_ciao_comp.in ());";
    }
    
  os_ << be_uidt_nl
      << "}";
    
  // In case it was set for the call above.  
  this->for_finder_ = false;
  
  return 0;
}

int
be_visitor_home_svs::visit_finder (be_finder *node)
{
  this->for_finder_ = true;
  return this->visit_factory (node);
}

int
be_visitor_home_svs::gen_servant_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();

  // Avoid '_cxx_' prefix.
  const char *lname =
    node_->original_local_name ()->get_string ();

  const char *clname = comp_->local_name ()->get_string ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl
      << lname << "_Servant::"
      << lname << "_Servant (" << be_idt << be_idt_nl
      << global << sname << "::CCM_" << lname << "_ptr exe," << be_nl
      << "const char * ins_name," << be_nl
      << "::CIAO::Container_ptr c)" << be_uidt_nl
      << ": ::CIAO::Home_Servant_Impl_Base (c)," << be_idt_nl
      << "::CIAO::" << (swapping_ ? "Swapping_" : "" )
      << "Home_Servant_Impl<" << be_idt_nl
      << "::" << node_->full_skel_name () << "," << be_nl
      << global << sname << "::CCM_" << lname << "," << be_nl
      << clname << "_Servant> (exe, c, ins_name)"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl << be_nl
      << lname << "_Servant::~" << lname << "_Servant (void)"
      << be_nl
      << "{" << be_nl
      << "}";
      
  AST_Type *pk = node_->primary_key ();
  
  if (pk != 0)
    {
      os_ << be_nl << be_nl
          << "::" << comp_->name () << "_ptr" << be_nl
          << lname << "_Servant::create (" << be_idt_nl
          << "::" << pk->name () << " * /* key */)" << be_uidt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,"
          << be_nl
          << "                             CORBA::COMPLETED_NO);"
          << be_uidt_nl
          << "}";

      os_ << be_nl << be_nl
          << "::" << comp_->name () << "_ptr" << be_nl
          << lname << "_Servant::find_by_primary_key (" << be_idt_nl
          << "::" << pk->name () << " * /* key */)" << be_uidt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,"
          << be_nl
          << "                             CORBA::COMPLETED_NO);"
          << be_uidt_nl
          << "}";

      os_ << be_nl << be_nl
          << "void" << be_nl
          << lname << "_Servant::remove (" << be_idt_nl
          << "::" << pk->name () << " * /* key */)" << be_uidt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,"
          << be_nl
          << "                             CORBA::COMPLETED_NO);"
          << be_uidt_nl
          << "}";

      os_ << be_nl << be_nl
          << "::" << pk->name () << " *" << be_nl
          << lname << "_Servant::get_primary_key (" << be_idt_nl
          << "::" << comp_->name () << "_ptr /* comp */)" << be_uidt_nl
          << "{" << be_idt_nl
          << "throw ::CORBA::NO_IMPLEMENT (CORBA::OMGVMCID | 8,"
          << be_nl
          << "                             CORBA::COMPLETED_NO);"
          << be_uidt_nl
          << "}";
    }
    
  be_home *h = node_;
  
  while (h != 0)
    {
      if (this->visit_scope (h) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_home_svs::")
                             ACE_TEXT ("gen_servant_class - ")
                             ACE_TEXT ("codegen for scope failed\n")),
                            -1);
        }
        
      for (long i = 0; i < h->n_inherits (); ++i)
        {
          // A closure of all the supported interfaces is stored
          // in the base class 'pd_inherits_flat' member.
          be_interface *bi =
            be_interface::narrow_from_decl (h->inherits ()[i]);
   
          bi->get_insert_queue ().reset ();
          bi->get_del_queue ().reset ();
          bi->get_insert_queue ().enqueue_tail (bi);
          
          Home_Op_Attr_Generator op_attr_gen (this);

          int status =
            bi->traverse_inheritance_graph (op_attr_gen,
                                            &os_,
                                            false,
                                            false);
          
          if (status == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_home_svs::")
                                 ACE_TEXT ("gen_servant_class - ")
                                 ACE_TEXT ("traverse_inheritance_graph() ")
                                 ACE_TEXT ("failed for %s\n"),
                                 bi->full_name ()),
                                -1);
            }

        }  
        
      h = be_home::narrow_from_decl (h->base_home ());
    }

  return 0;
}

void
be_visitor_home_svs::gen_entrypoint (void)
{
  ACE_CString sname_str (
    ScopeAsDecl (node_->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();

  // Avoid _cxx_ prefix.
  const char *lname =
    node_->original_local_name ()->get_string ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::PortableServer::Servant" << be_nl
      << "create_" << node_->flat_name ()
      << "_Servant (" << be_idt_nl
      << "::Components::HomeExecutorBase_ptr p," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << "const char * ins_name)" << be_uidt_nl
      << "{" << be_idt_nl
      << "::PortableServer::Servant retval = 0;" << be_nl
      << global << sname << "::CCM_" << lname
      << "_var x =" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "::_narrow (p);" << be_uidt_nl << be_nl
      << "if (! ::CORBA::is_nil (x.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_NEW_NORETURN (retval," << be_nl
      << "                  " << lname << "_Servant (" << be_idt_nl
      << "                  x.in ()," << be_nl
      << "                  ins_name," << be_nl
      << "                  c));" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retval;" << be_uidt_nl
      << "}";
}

Home_Op_Attr_Generator::Home_Op_Attr_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Home_Op_Attr_Generator::emit (be_interface * /* derived_interface */,
                              TAO_OutStream * /* os */,
                              be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}

