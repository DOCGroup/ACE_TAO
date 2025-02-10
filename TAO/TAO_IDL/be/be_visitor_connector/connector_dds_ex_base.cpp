
//=============================================================================
/**
 *  @file    connector_dds_ex_base.cpp
 *
 *  Base class for visitors generating code for DDS Connectors in
 *  the exec impl.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "connector.h"

#include "ast_structure.h"
#include "ast_typedef.h"
#include "ast_template_module.h"
#include "ast_template_module_ref.h"
#include "ast_native.h"

#include "utl_strlist.h"
#include "utl_string.h"

#include "fe_extern.h"

be_visitor_connector_dds_ex_base::be_visitor_connector_dds_ex_base (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    t_inst_ (nullptr),
    t_ref_ (nullptr),
    t_params_ (nullptr),
    base_tname_ (nullptr)
{
}

be_visitor_connector_dds_ex_base::~be_visitor_connector_dds_ex_base (
  )
{
}

bool
be_visitor_connector_dds_ex_base::begin (be_connector *node)
{
  node_ = node;

  this->process_template_args (node);

  AST_Connector *base = node;

  while (this->t_args_.is_empty () && base != nullptr)
    {
      this->process_template_args (base);
      base = base->base_connector ();
    }

  if (this->t_args_.is_empty ())
    {
      return false;
    }

  /// Use 'CIAO_' + component's flat name.
  os_ << be_nl_2
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  this->base_tname_ = node->local_name ();

  return true;
}

bool
be_visitor_connector_dds_ex_base::is_dds_type (
  be_connector *node, AST_Decl *d)
{
  bool result = false;
  AST_Connector* base = node->base_connector ();

  if (base)
    {
      while (base->base_connector () != nullptr)
        {
          base = base->base_connector ();
        }

      const char* lname = base->local_name ()->get_string ();

      if (ACE_OS::strcmp (lname, "DDS_Base") == 0)
        {
          AST_Structure *s = dynamic_cast<AST_Structure*> (d);

          if (s == nullptr)
            {
              AST_Typedef *td = dynamic_cast<AST_Typedef*> (d);

              if (td != nullptr)
                {
                  s = dynamic_cast<AST_Structure*> (td->primitive_base_type ());
                }
            }

          if (s)
            {
              result = true;
            }
        }
    }

  return result;
}

void
be_visitor_connector_dds_ex_base::process_template_args (
  AST_Connector *node)
{
  AST_Module *m =
    dynamic_cast<AST_Module*> (node->defined_in ());

  /// If this is non-zero, we use this to limit our template
  /// args list to those used in the alias. If it is zero, we
  /// just use the complete template arg list in the
  /// instantiation.
  this->t_ref_ = m->from_ref ();
  this->t_inst_ = m->from_inst ();

  /// We assume the connector comes from the instantiation
  /// of a template module, and the regular module it's
  /// defined in (at some level) will have a reference to it.
  while (this->t_inst_ == nullptr && m != nullptr)
    {
      this->t_inst_ = m->from_inst ();
      m = dynamic_cast<AST_Module*> (m->defined_in ());
    }

  if (this->t_inst_ == nullptr)
    {
      /// Probably means we're trying the base connector
      /// of DDS_State or DDS_Event, in which case we
      /// return so the caller can try again with the
      /// derived connector, which will succeed in
      /// finding the template module instantiation and
      /// its associated template arguments.
      return;
    }

  /// We need to fetch and store these so we can match them
  /// to the corresponding args and determine whether a
  /// corresponding boolean template arg (indicating FIXED
  /// size type) should be inserted into the
  /// connector base class template arg list.
  this->t_params_ = this->t_inst_->ref ()->template_params ();

  this->match_template_args ();

  /// We depend on the DDS datatype being the first template
  /// argument for now, this may change.
  AST_Decl **datatype = nullptr;
  int const status = this->t_args_.get (datatype, 0UL);

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

void
be_visitor_connector_dds_ex_base::match_template_args ()
{
  /// If the connector was declared in an aliased templated
  /// module, we want to populate our template args list with
  /// only the args referenced in the alias (see FOR loop
  /// below). Otherwise, we just copy the containing template
  /// module instantiation's arg list.
  if (this->t_ref_ == nullptr)
    {
      this->t_args_ = *this->t_inst_->template_args ();
      return;
    }

  for (UTL_StrlistActiveIterator i (this->t_ref_->param_refs ());
       ! i.is_done ();
       i.next ())
    {
      const char *ref_name = i.item ()->get_string ();
      size_t slot = 0;

      for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR ci (
             *this->t_inst_->ref ()->template_params ());
           ! ci.done ();
           ci.advance (), ++slot)
        {
          FE_Utils::T_Param_Info *t_param = nullptr;
          ci.next (t_param);

          if (t_param->name_ == ref_name)
            {
              AST_Decl **d = nullptr;
              this->t_inst_->template_args ()->get (d, slot);
              this->t_args_.enqueue_tail (*d);
              break;
            }
        }
    }
}

