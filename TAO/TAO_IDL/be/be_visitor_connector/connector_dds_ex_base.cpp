
//=============================================================================
/**
 *  @file    connector_dds_ex_base.cpp
 *
 *  $Id$
 *
 *  Base class for visitors generating code for DDS Connectors in
 *  the exec impl.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_connector_dds_ex_base::be_visitor_connector_dds_ex_base (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    t_inst_ (0),
    t_args_ (0),
    base_tname_ (0)
{
}

be_visitor_connector_dds_ex_base::~be_visitor_connector_dds_ex_base (void)
{
}

bool
be_visitor_connector_dds_ex_base::begin (be_connector *node)
{
  node_ = node;

  AST_Connector *base = node->base_connector ();

  this->process_template_args (base);

  /// If the previous call hasn't initialized our arg
  /// list, it could be because we were processing
  /// DDS_State or DDS_Event, whose base class,
  /// DDS_TopicBase, is not in a template module.
  /// To check this, we call again with the node itself,
  /// and if we get an arg list this time, proceed.
  if (this->t_args_ == 0)
    {
      this->process_template_args (node);
      
      if (this->t_args_ == 0)
        {
          return false;
        }
    }

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  ACE_CString lname = node->local_name ();  
  const char *base_tname = 0;
  
  if (lname == "DDS_Event" || lname == "DDS_State")
    {
      this->base_tname_ = node->local_name ();  
    }
  else
    {
      this->base_tname_ =
        node->base_connector ()->local_name ()->get_string ();
    }
    
  return true;
}

void
be_visitor_connector_dds_ex_base::process_template_args (
  AST_Connector *node)
{
  AST_Decl *d =
    this->node_->defined_in ()->lookup_by_name (
      ScopeAsDecl (node->defined_in ())->name (),
      true);

  // The template module instantiation and the corresponding
  // implied IDL module have the same name, but the instantiation
  // comes first in the scope list, so that is what the scope
  // iteration will find first.
  this->t_inst_ =
    AST_Template_Module_Inst::narrow_from_decl (d);

  if (this->t_inst_ == 0)
    {
      // More shaky logic, but it's the best we have for now.
      // Depends on the application connector have a base
      // connector declared in the instantiation of a template
      // module.
      return;
    }

  this->t_args_ = this->t_inst_->template_args ();

  // We depend on the DDS datatype being the first template
  // argument for now, this may change.
  AST_Decl **datatype = 0;
  int status = this->t_args_->get (datatype, 0UL);

  if (status != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_dds_ex_base::")
                  ACE_TEXT ("process_template_args - ")
                  ACE_TEXT ("template arg not found\n ")));

      return;
    }

  this->dds_traits_name_ = (*datatype)->flat_name ();
  this->dds_traits_name_ += "_DDS_Traits";
}

