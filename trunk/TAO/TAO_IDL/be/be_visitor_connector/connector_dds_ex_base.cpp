
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
    t_args_ (0)
{
}

be_visitor_connector_dds_ex_base::~be_visitor_connector_dds_ex_base (void)
{
}

void
be_visitor_connector_dds_ex_base::process_template_args (
  AST_Connector *base)
{
  AST_Decl *d =
    node_->defined_in ()->lookup_by_name (
      ScopeAsDecl (base->defined_in ())->name (),
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

