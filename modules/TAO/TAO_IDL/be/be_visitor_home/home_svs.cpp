//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    home_svs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Homes in the servant source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_home,
           home_svs,
           "$Id$")

// ******************************************************
// Home visitor for server source
// ******************************************************

be_visitor_home_svs::be_visitor_home_svs (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    comp_ (0),
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
  TAO_OutStream &os_  = *this->ctx_->stream ();
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
      
  this->gen_ops_attrs ();
  
  os_ << be_nl << be_nl
      << "/// Factory operations.";
  
  this->gen_factories_r (node_);

  os_ << be_nl << be_nl
      << "/// Finder operations.";
  
  this->gen_finders_r (node_);
      
  return 0;
}

int
be_visitor_home_svs::gen_ops_attrs (void)
{
  os_ << be_nl << be_nl
      << "/// All home operations and attributes.";
      
  node_->get_insert_queue ().reset ();
  node_->get_del_queue ().reset ();
  node_->get_insert_queue ().enqueue_tail (node_);
  
  Home_Op_Attr_Generator op_attr_gen (this);
      
  int status =
    node_->traverse_inheritance_graph (op_attr_gen,
                                       &os_,
                                       false,
                                       false);
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_svs::")
                         ACE_TEXT ("gen_ops_attrs - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }
    
  return 0;
}

int
be_visitor_home_svs::gen_factories (void)
{
  return this->gen_factories_r (node_);
}

int
be_visitor_home_svs::gen_factories_r (AST_Home *node)
{
  if (node == 0)
    {
      return 0;
    }
    
  if (this->gen_init_ops (node->factories (), false) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_svs::")
                         ACE_TEXT ("gen_factories_r - ")
                         ACE_TEXT ("gen_init_ops_i() failed\n")),
                        -1);
    }
    
  AST_Home *base = node->base_home ();
  
  return this->gen_factories_r (base);
}

int
be_visitor_home_svs::gen_finders (void)
{
  os_ << be_nl << be_nl
      << "// Finder operations.";
  
  return this->gen_finders_r (node_);
}

int
be_visitor_home_svs::gen_finders_r (AST_Home *node)
{
  if (node == 0)
    {
      return 0;
    }
    
  if (this->gen_init_ops (node->finders (), true) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_svs::")
                         ACE_TEXT ("gen_finders_r - ")
                         ACE_TEXT ("gen_init_ops_i() failed\n")),
                        -1);
    }
    
  AST_Home *base = node->base_home ();
  
  return this->gen_finders_r (base);
}

int
be_visitor_home_svs::gen_init_ops (AST_Home::INIT_LIST & list,
                                   bool finder_list)
{
  AST_Operation **op = 0;
  ACE_CString comp_sname_str (
    ScopeAsDecl (comp_->defined_in ())->full_name ());
  const char *comp_sname = comp_sname_str.c_str ();
  const char *comp_lname = comp_->local_name ()->get_string ();
  const char *global = (comp_sname_str == "" ? "" : "::");
  
  for (AST_Home::INIT_LIST::ITERATOR i = list.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (op);
      be_operation *bop = be_operation::narrow_from_decl (*op);
      
      // Retrieve the operation return type.
      be_type *bt = be_type::narrow_from_decl (bop->return_type ());

      if (bt == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_home_svs::"
                             "gen_init_ops - "
                             "Bad return type\n"),
                            -1);
        }
        
      os_ << be_nl << be_nl;

      be_visitor_operation_rettype rt_visitor (this->ctx_);

      if (bt->accept (&rt_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_home_svs::"
                             "gen_init_ops - "
                             "codegen for return type failed\n"),
                            -1);
        }
        
      os_ << be_nl
          << node_->original_local_name ()->get_string ()
          << "_Servant::" << bop->local_name ();

      be_visitor_operation_arglist al_visitor (this->ctx_);
      
      // Finder operations are as yet unimplemented in CIAO, so
      // any args will be unused and should be commented out.
      al_visitor.unused (finder_list);

      if (bop->accept (&al_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_home_svs::"
                             "gen_init_ops - "
                             "codegen for argument list failed\n"),
                            -1);
        }
        
      os_ << be_nl
          << "{" << be_idt_nl;
      
      if (finder_list)
        {
          os_ << "throw ::CORBA::NO_IMPLEMENT ();";
        }
      else
        {    
          os_ << "::Components::EnterpriseComponent_var _ciao_ec ="
              << be_idt_nl
              << "this->executor_->" << bop->local_name () << " (";
              
          if (bop->argument_count () != 0)
            {
              os_ << be_idt_nl;
              
              if (this->visit_scope (bop) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("be_visitor_home_svs")
                                     ACE_TEXT ("::gen_init_ops - ")
                                     ACE_TEXT ("visit_scope() failed\n")),
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
      << "                ins_name," << be_nl
      << "                c)," << be_uidt_nl
      << "                0);" << be_nl << be_nl
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

