//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    home_svh.cpp
//
// = DESCRIPTION
//    Visitor generating code for HOmes in the servant header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

// ******************************************************
// Home visitor for server header
// ******************************************************

be_visitor_home_svh::be_visitor_home_svh (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    comp_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->svnt_export_macro ())
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

be_visitor_home_svh::~be_visitor_home_svh (void)
{
}

int
be_visitor_home_svh::visit_home (be_home *node)
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
                         ACE_TEXT ("be_visitor_home_svh::")
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
be_visitor_home_svh::visit_operation (be_operation *node)
{
  be_visitor_operation_ch v (this->ctx_);
  return v.visit_operation (node);
}

int
be_visitor_home_svh::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}

int
be_visitor_home_svh::visit_factory (be_factory *node)
{
  os_ << be_nl << be_nl
      << "virtual ::" << comp_->name () << "_ptr" << be_nl
      << node->local_name ();
      
  // We can reuse this visitor.
  be_visitor_valuetype_init_arglist_ch v (this->ctx_);
  
  if (v.visit_factory (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_svh::")
                         ACE_TEXT ("visit_factory - ")
                         ACE_TEXT ("codegen for argument ")
                         ACE_TEXT ("list failed\n")),
                        -1);
    }
    
  os_ << ";";
    
  return 0;
}

int
be_visitor_home_svh::visit_finder (be_finder *node)
{
  return this->visit_factory (node);
}

int
be_visitor_home_svh::gen_servant_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  
  // No '_cxx_' prefix.
  const char *lname =
    node_->original_local_name ()->get_string ();
    
  const char *clname = comp_->local_name ()->get_string ();
  const char *global = (sname_str == "" ? "" : "::");
  bool swapping = be_global->gen_component_swapping ();
  
  os_ << be_nl
      << "class " << export_macro_.c_str () << " " << lname
      << "_Servant" << be_idt_nl
      << ": public virtual" << be_idt << be_idt_nl
      << "::CIAO::"
      << (swapping ? "Swapping_" : "")
      << "Home_Servant_Impl<" << be_idt_nl
      << "::" << node_->full_skel_name () << "," << be_nl
      << global << sname << "::CCM_" << lname << "," << be_nl
      << clname << "_Servant>"
      << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;
      
  os_ << lname << "_Servant (" << be_idt_nl
      << global << sname << "::CCM_" << lname << "_ptr exe," << be_nl
      << "const char * ins_name," << be_nl
      << "::CIAO::Container_ptr c);" << be_uidt;
      
  os_ << be_nl << be_nl
      << "virtual ~" << lname << "_Servant (void);";
      
  AST_Type *pk = node_->primary_key ();
  
  if (pk != 0)
    {
      os_ << be_nl << be_nl
          << "// Implicit home primary key operations - not supported.";
          
      os_ << be_nl << be_nl
          << "virtual ::" << comp_->name () << "_ptr" << be_nl
          << "create (" << be_idt_nl
          << "::" << pk->name () << " * key);" << be_uidt;
          
      os_ << be_nl << be_nl
          << "virtual ::" << comp_->name () << "_ptr" << be_nl
          << "find_by_primary_key (" << be_idt_nl
          << "::" << pk->name () << " * key);" << be_uidt;
          
      os_ << be_nl << be_nl
          << "virtual void" << be_nl
          << "remove (" << be_idt_nl
          << "::" << pk->name () << " * key);" << be_uidt;
          
      os_ << be_nl << be_nl
          << "virtual ::" << pk->name () << " *" << be_nl
          << "get_primary_key (" << be_idt_nl
          << "::" << comp_->name () << "_ptr comp);" << be_uidt;
    }
    
  be_home *h = node_;
  
  while (h != 0)
    {
      if (this->visit_scope (h) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_home_svh::")
                             ACE_TEXT ("gen_servant_class - ")
                             ACE_TEXT ("visit_scope() failed\n")),
                            -1);
        }
        
      h = be_home::narrow_from_decl (h->base_home ());
    }

  os_ << be_uidt_nl
      << "};";
     
  return 0;
}

void
be_visitor_home_svh::gen_entrypoint (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::PortableServer::Servant" << be_nl
      << "create_" << node_->flat_name ()
      << "_Servant (" << be_idt_nl
      << "::Components::HomeExecutorBase_ptr p," << be_nl
      << "::CIAO::Container_ptr c," << be_nl
      << "const char * ins_name);" << be_uidt; 
}


