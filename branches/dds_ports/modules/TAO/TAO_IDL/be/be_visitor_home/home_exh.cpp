//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    home_exh.cpp
//
// = DESCRIPTION
//    Visitor generating code for homes in the exec impl header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_home,
           home_exh,
           "$Id$")

// ******************************************************
// Home visitor for exec impl header
// ******************************************************

be_visitor_home_exh::be_visitor_home_exh (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    comp_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->exec_export_macro ())
{
}

be_visitor_home_exh::~be_visitor_home_exh (void)
{
}

int
be_visitor_home_exh::visit_home (be_home *node)
{
  node_ = node;
  TAO_OutStream &os_  = *this->ctx_->stream ();
  comp_ = node_->managed_component ();
  
  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << comp_->flat_name () << "_Impl" << be_nl
      << "{" << be_idt;
    
  if (this->gen_exec_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exh::")
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
be_visitor_home_exh::visit_operation (be_operation *node)
{
  be_visitor_operation_ch v (this->ctx_);
  return v.visit_operation (node);
}

int
be_visitor_home_exh::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}

int
be_visitor_home_exh::gen_exec_class (void)
{
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());
  ACE_CString sname_str (scope->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *clname = comp_->local_name ()->get_string ();
  const char *global = (sname_str == "" ? "" : "::");
  
  os_ << be_nl
      << "class " << export_macro_.c_str () << " " << lname
      << "_exec_i" << be_idt_nl
      << ": public virtual " << lname << "_Exec," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt;
      
  os_ << be_nl
      << lname << "_exec_i (void);";
      
  os_ << be_nl << be_nl
      << "virtual ~" << lname << "_exec_i (void);";
      
  this->gen_ops_attrs ();
 
  this->gen_factories ();
  
  this->gen_finders ();

  os_ << be_uidt_nl
      << "};";
     
  return 0;
}

int
be_visitor_home_exh::gen_ops_attrs (void)
{
  os_ << be_nl << be_nl
      << "// All operations and attributes.";
      
  int status =
    node_->traverse_inheritance_graph (
      be_visitor_home_exh::op_attr_decl_helper,
      &os_);
      
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exh::")
                         ACE_TEXT ("gen_ops_attrs - ")
                         ACE_TEXT ("traverse_inheritance_graph() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }
    
  return 0;
}

int
be_visitor_home_exh::gen_factories (void)
{
  os_ << be_nl << be_nl
      << "// Factory operations.";
  
  return this->gen_factories_r (node_);
}

int
be_visitor_home_exh::gen_factories_r (AST_Home *node)
{
  if (node == 0)
    {
      return 0;
    }
    
  if (this->gen_init_ops_i (node->factories ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exh::")
                         ACE_TEXT ("gen_factories_r - ")
                         ACE_TEXT ("gen_init_ops_i() failed\n")),
                        -1);
    }
    
  AST_Home *base = node->base_home ();
  
  return this->gen_factories_r (base);
}

int
be_visitor_home_exh::gen_finders (void)
{
  os_ << be_nl << be_nl
      << "// Finder operations.";
  
  return this->gen_finders_r (node_);
}

int
be_visitor_home_exh::gen_finders_r (AST_Home *node)
{
  if (node == 0)
    {
      return 0;
    }
    
  if (this->gen_init_ops_i (node->finders ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exh::")
                         ACE_TEXT ("gen_finders_r - ")
                         ACE_TEXT ("gen_init_ops_i() failed\n")),
                        -1);
    }
    
  AST_Home *base = node->base_home ();
  
  return this->gen_finders_r (base);
}

int
be_visitor_home_exh::gen_init_ops_i (AST_Home::INIT_LIST & list)
{
  AST_Operation **op = 0;
  
  for (AST_Home::INIT_LIST::ITERATOR i = list.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (op);
      be_operation *bop = be_operation::narrow_from_decl (*op);
      
      /// Return type for home exec factories and finders is not
      /// the same as for the corresponding home servant, so we
      /// generate the return type and op name by hand, then finish
      /// the operation traversal with an arglist visitor.
      os_ << be_nl << be_nl
          << "::Components::EnterpriseComponent_ptr" << be_nl
          << bop->local_name ();
          
      be_visitor_operation_arglist visitor (this->ctx_);
      
      if (visitor.visit_operation (bop) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_home_exh::")
                             ACE_TEXT ("gen_init_ops_i - ")
                             ACE_TEXT ("visit_operation() failed\n")),
                            -1);
        }
        
      os_ << ";";
    }
    
  return 0;
}

void
be_visitor_home_exh::gen_entrypoint (void)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::Components::HomeExecutorBase_ptr" << be_nl
      << "create_" << node_->flat_name ()
      << "_Impl (void);"; 
}

int
be_visitor_home_exh::op_attr_decl_helper (be_interface *derived,
                                          be_interface *ancestor,
                                          TAO_OutStream *os)
{
  /// We're in a static method, so we have to instantiate a temporary
  /// visitor and context.
  be_visitor_context ctx;
  ctx.state (TAO_CodeGen::TAO_ROOT_EXH);
  ctx.stream (os);
  be_visitor_home_exh visitor (&ctx);
  
  /// Since this visitor overriddes only visit_operation() and 
  /// visit_attribute(), we can get away with this for the declarations.
  return visitor.visit_scope (ancestor);
}


