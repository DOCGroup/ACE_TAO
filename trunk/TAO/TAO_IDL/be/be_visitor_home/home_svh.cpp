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

ACE_RCSID (be_visitor_home,
           home_svh,
           "$Id$")

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
      
  this->gen_ops_attrs ();
  
  this->gen_factories ();
  
  this->gen_finders ();
      
  os_ << be_uidt_nl
      << "};";
     
  return 0;
}

int
be_visitor_home_svh::gen_ops_attrs (void)
{
  os_ << be_nl << be_nl
      << "// All operations and attributes.";
      
  int status =
    node_->traverse_inheritance_graph (
      be_visitor_home_svh::op_attr_decl_helper,
      &os_);
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_svh::")
                         ACE_TEXT ("gen_ops_attrs - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }
    
  return 0;
}

int
be_visitor_home_svh::gen_factories (void)
{
  os_ << be_nl << be_nl
      << "// Factory operations.";
  
  return this->gen_factories_r (node_);
}

int
be_visitor_home_svh::gen_factories_r (AST_Home *node)
{
  if (node == 0)
    {
      return 0;
    }
    
  if (this->gen_init_ops_i (node->factories ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_svh::")
                         ACE_TEXT ("gen_factories_r - ")
                         ACE_TEXT ("gen_init_ops_i() failed\n")),
                        -1);
    }
    
  AST_Home *base = node->base_home ();
  
  return this->gen_factories_r (base);
}

int
be_visitor_home_svh::gen_finders (void)
{
  os_ << be_nl << be_nl
      << "// Finder operations.";
  
  return this->gen_finders_r (node_);
}

int
be_visitor_home_svh::gen_finders_r (AST_Home *node)
{
  if (node == 0)
    {
      return 0;
    }
    
  if (this->gen_init_ops_i (node->finders ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_svh::")
                         ACE_TEXT ("gen_finders_r - ")
                         ACE_TEXT ("gen_init_ops_i() failed\n")),
                        -1);
    }
    
  AST_Home *base = node->base_home ();
  
  return this->gen_finders_r (base);
}

int
be_visitor_home_svh::gen_init_ops_i (AST_Home::INIT_LIST & list)
{
  AST_Operation **op = 0;
  
  for (AST_Home::INIT_LIST::ITERATOR i = list.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (op);
      be_operation *bop = be_operation::narrow_from_decl (*op);
      
      if (this->visit_operation (bop) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_home_svh::")
                             ACE_TEXT ("gen_init_ops_i - ")
                             ACE_TEXT ("visit_operation() failed\n")),
                            -1);
        }
    }
    
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

int
be_visitor_home_svh::op_attr_decl_helper (be_interface * /* derived */,
                                          be_interface *ancestor,
                                          TAO_OutStream *os)
{
  /// We're in a static method, so we have to instantiate a temporary
  /// visitor and context.
  be_visitor_context ctx;
  ctx.state (TAO_CodeGen::TAO_ROOT_SVH);
  ctx.stream (os);
  be_visitor_home_svh visitor (&ctx);
  
  /// Since this visitor overriddes only visit_operation() and 
  /// visit_attribute(), we can get away with this for the declarations.
  return visitor.visit_scope (ancestor);
}


