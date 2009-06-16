//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_ex_idl.cpp
//
// = DESCRIPTION
//    Visitor generating code for Components in the CIAO executor IDL.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component,
           component_ex_idl,
           "$Id$")

// ******************************************************
// Component visitor for CIAO executor IDL
// ******************************************************

be_visitor_component_ex_idl::be_visitor_component_ex_idl (
    be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    os_ (*ctx->stream ())
{
}

be_visitor_component_ex_idl::~be_visitor_component_ex_idl (void)
{
}

int
be_visitor_component_ex_idl::visit_component (be_component *node)
{
  if (node->imported ())
    {
      return 0;
    }
    
  node_ = node;
  
  this->gen_facets ();
  
  this->gen_component ();
  
  this->gen_executor_derived ();
  
  return 0;
}

int
be_visitor_component_ex_idl::visit_attribute (be_attribute *node)
{
   bool rd_only = node->readonly ();

   // Keep output statements separate because of side effects.
   // No need to check for anonymous array - anonymous types not
   // accepted by parser for attributes.
   os_ << be_nl
       << (rd_only ? "readonly " : "") << "attribute ";
       
   be_type *ft = be_type::narrow_from_decl (node->field_type ());
   
   os_ << IdentifierHelper::type_name (ft, this);
   os_ << " "
       << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

   this->gen_exception_list (node->get_get_exceptions (),
                             rd_only ? "" : "get");

   this->gen_exception_list (node->get_set_exceptions (),
                             "set");

   os_ << ";";

  return 0;
}

int
be_visitor_component_ex_idl::visit_sequence (be_sequence *node)
{
  // Keep output statements separate because of side effects.
  os_ << "sequence<";
  
  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  
  os_ << IdentifierHelper::type_name (bt, this);

  if (!node->unbounded ())
    {
      os_ << ", " << node->max_size ()->ev ()->u.ulval;
    }

  os_ << "> ";

  return 0;
}

int
be_visitor_component_ex_idl::visit_string (be_string *node)
{
  os_ << (node->width () > sizeof (char) ? "w" : "") << "string";

  ACE_CDR::ULong bound = node->max_size ()->ev ()->u.ulval;

  if (bound > 0)
    {
      os_ << "<" << bound << ">";
    }

  return 0;
}

void
be_visitor_component_ex_idl::gen_nesting_open (AST_Decl *node)
{
  os_ << be_nl;
  
  for (UTL_IdListActiveIterator i (node->name ()); ! i.is_done () ;)
    {
      ACE_CString module_name (i.item ()->get_string ());
      
      if (module_name == "")
        {
          i.next ();
          continue;
        }
        
      i.next ();
      
      if (i.is_done ())
        {
          break;
        }
      
      os_ << be_nl
          << "module " << module_name.c_str () << be_nl
          << "{" << be_idt;
    }
}

void
be_visitor_component_ex_idl::gen_nesting_close (AST_Decl *node)
{
  for (UTL_IdListActiveIterator i (node->name ()); ! i.is_done () ;)
    {
      ACE_CString module_name (i.item ()->get_string ());
      
      if (module_name == "")
        {
          i.next ();
          continue;
        }
        
      i.next ();
      
      if (i.is_done ())
        {
          break;
        }
      
      os_ << be_uidt_nl
          << "};";
    }
}

void
be_visitor_component_ex_idl::gen_facets (void)
{
  AST_Component::port_description *pd = 0;

  for (AST_Component::PORTS::ITERATOR i =
         node_->provides ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      be_decl *intf =
        be_decl::narrow_from_decl (pd->impl);
        
      if (intf->ex_idl_facet_gen ())
        {
          continue;
        }
      
      this->gen_nesting_open (pd->impl);
      
      os_ << be_nl
          << "local interface CCM_"
          << pd->impl->local_name ()->get_string ()
          << " : ::" << pd->impl->full_name () << be_nl
          << "{" << be_idt;
          
      os_ << be_uidt_nl
          << "};";
      
      this->gen_nesting_close (pd->impl);
      
      intf->ex_idl_facet_gen (true);
    }
}

void
be_visitor_component_ex_idl::gen_component (void)
{
  this->gen_nesting_open (node_);
  
  this->gen_executor_base ();
      
  this->gen_context ();

  this->gen_nesting_close (node_);
}

void
be_visitor_component_ex_idl::gen_executor_base (void)
{
  AST_Component *base = node_->base_component ();
  
  os_ << be_nl
      << "local interface CCM_" << node_->local_name ()
      << be_idt_nl
      << ": ";
      
  if (base == 0)
    {
      os_ << "::Components::EnterpriseComponent";
      
      this->gen_supported ();
    }
  else
    {
      ACE_CString sname_str (
        ScopeAsDecl (base->defined_in ())->full_name ());
        
      const char *sname = sname_str.c_str ();
      const char *global = (sname_str == "" ? "" : "::");
        
      os_ << global << sname << "::CCM_"
          << base->local_name ()->get_string ();
    }
      
  os_ << be_uidt_nl
      << "{" << be_idt;
      
  this->gen_executor_contents ();
      
  os_ << be_uidt_nl
      << "};";
}

void
be_visitor_component_ex_idl::gen_supported (void)
{
  os_ << be_idt;
  
  AST_Interface **supported = node_->supports ();
  
  for (long i = 0; i < node_->n_supports (); ++i)
    {
      os_ << "," << be_nl
          << "::" << supported[i]->full_name ();
    }
    
  os_ << be_uidt;
}

void
be_visitor_component_ex_idl::gen_executor_contents (void)
{
  this->gen_facet_ops ();
  this->gen_consumer_ops ();
  
  /// This picks up the attributes.
  if (this->visit_scope (node_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_component_ex_idl::")
                  ACE_TEXT ("gen_executor_contents - ")
                  ACE_TEXT ("visit_scope() failed\n")));
    }
}

void
be_visitor_component_ex_idl::gen_facet_ops (void)
{
  AST_Component::port_description *pd = 0;

  for (AST_Component::PORTS::ITERATOR i =
         node_->provides ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      AST_Decl *scope = ScopeAsDecl (pd->impl->defined_in ());
      ACE_CString sname_str (scope->full_name ());
      const char *sname = sname_str.c_str ();
      const char *lname = pd->impl->local_name ()->get_string ();
      const char *global = (sname_str == "" ? "" : "::");
      
      os_ << be_nl
          << global << sname << "::CCM_" << lname << " get_"
          << pd->id->get_string () << " ();";
    }
}

void
be_visitor_component_ex_idl::gen_consumer_ops (void)
{
  AST_Component::port_description *pd = 0;

  for (AST_Component::PORTS::ITERATOR i =
         node_->consumes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      os_ << be_nl
          << "void push_" << pd->id->get_string () << " (in ::"
          << pd->impl->full_name () << " e);";
    }
}

void
be_visitor_component_ex_idl::gen_exception_list (
  UTL_ExceptList *exceptions,
  const char *prefix,
  bool closed)
{
  if (exceptions != 0 && exceptions->length () > 0)
    {
      os_ << be_idt_nl
          << prefix << "raises ( ";

      for (UTL_ExceptlistActiveIterator ei (exceptions);
           !ei.is_done ();)
        {
          os_ << "::"
              << IdentifierHelper::orig_sn (ei.item ()->name ()).c_str ();

          ei.next ();

          if (!ei.is_done () || !closed)
            {
              os_ << ", ";
            }
        }

      if (closed)
        {
          os_ << ")" << be_uidt;
        }
    }
}

void
be_visitor_component_ex_idl::gen_context (void)
{
  os_ << be_nl << be_nl
      << "local interface CCM_" << node_->local_name ()
      << "_Context" << be_idt_nl
      << ": ";
      
  AST_Component *base = node_->base_component ();
  
  if (base == 0)
    {
      os_ << "::Components::SessionContext";
    }
  else
    {
      ACE_CString sname_str (
        ScopeAsDecl (base->defined_in ())->full_name ());
      const char *sname = sname_str.c_str ();
      const char *lname = base->local_name ()->get_string ();
      const char *global = (sname_str == "" ? "" : "::");
      
      os_ << global << sname << "::CCM_" << lname << "_Context";
    }
    
  os_ << be_uidt_nl
      << "{" << be_idt;
      
  this->gen_publisher_ops ();
  this->gen_emitter_ops ();
  this->gen_receptacle_ops ();
      
  os_ << be_uidt_nl
      << "};";
}

void
be_visitor_component_ex_idl::gen_publisher_ops (void)
{
  AST_Component::port_description *pd = 0;

  for (AST_Component::PORTS::ITERATOR i =
         node_->publishes ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      os_ << be_nl
          << "void push_" << pd->id->get_string () << " (in ::"
          << pd->impl->full_name () << " e);";
    }
}

void
be_visitor_component_ex_idl::gen_emitter_ops (void)
{
  AST_Component::port_description *pd = 0;

  for (AST_Component::PORTS::ITERATOR i =
         node_->emits ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      os_ << be_nl
          << "void push_" << pd->id->get_string () << " (in ::"
          << pd->impl->full_name () << " e);";
    }
}

void
be_visitor_component_ex_idl::gen_receptacle_ops (void)
{
  AST_Component::port_description *pd = 0;

  for (AST_Component::PORTS::ITERATOR i =
         node_->uses ().begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      
      os_ << be_nl
          << "::";
      
      if (pd->is_multiple)
        {
          os_ << node_->full_name () << "::"
              << pd->id->get_string ()
              << "Connections get_connections_"
              << pd->id->get_string () << " ();";
        }
      else
        {
          os_ << pd->impl->full_name ()
              << " get_connection_" << pd->id->get_string ()
              << " ();";
        }
    }
}

void
be_visitor_component_ex_idl::gen_executor_derived (void)
{
  ACE_CString sname_str (
    ScopeAsDecl (node_->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node_->local_name ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "module CIAO_" << node_->flat_name () << "_Impl" << be_nl
      << "{" << be_idt_nl
      << "typedef " << global << sname << "::CCM_" << lname
      << "_Context " << lname << "_Exec_Context;";
      
  os_ << be_nl << be_nl
      << "local interface " << lname << "_Exec" << be_idt_nl
      << ": " << global << sname << "::CCM_" << lname
      << "," << be_idt_nl
      << "::Components::SessionComponent" << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";
      
  os_ << be_uidt_nl
      << "};";
}