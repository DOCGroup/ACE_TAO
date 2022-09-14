#include "be_connector.h"
#include "be_visitor.h"
#include "ast_module.h"
#include "ast_template_module_inst.h"

#include "utl_identifier.h"

be_connector::be_connector (
      UTL_ScopedName *n,
      AST_Connector *base_connector)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_connector,
              n),
    AST_Type (AST_Decl::NT_connector,
              n),
    UTL_Scope (AST_Decl::NT_connector),
    AST_Interface (n,
                   nullptr,
                   0,
                   nullptr,
                   0,
                   false,
                   false),
    AST_Component (n,
                   base_connector,
                   nullptr,
                   0,
                   nullptr,
                   0),
    AST_Connector (n,
                   base_connector),
    be_scope (AST_Decl::NT_connector),
    be_decl (AST_Decl::NT_connector,
              n),
    be_type (AST_Decl::NT_connector,
             n),
    be_interface (n,
                  nullptr,
                  0,
                  nullptr,
                  0,
                  false,
                  false),
    be_component (n,
                  base_connector,
                  nullptr,
                  0,
                  nullptr,
                  0),
    dds_connector_ (false),
    ami_connector_ (false)
{
  if (! this->imported ())
    {
      this->check_ancestors ();
    }
}

be_connector::~be_connector ()
{
}

void
be_connector::destroy ()
{
  this->AST_Connector::destroy ();
  this->be_component::destroy ();
}

int
be_connector::accept (be_visitor *visitor)
{
  return visitor->visit_connector (this);
}

bool
be_connector::dds_connector ()
{
  return this->dds_connector_;
}

bool
be_connector::ami_connector ()
{
  return this->ami_connector_;
}

void
be_connector::check_ancestors ()
{
  AST_Connector *base = this;

  while (base != nullptr)
    {
      const char *base_fname = base->full_name ();

      if (ACE_OS::strcmp (base_fname, "CCM_DDS::DDS_Base") == 0)
        {
          this->dds_connector_ = true;
          idl_global->dds_connector_seen_ = true;
          break;
        }
      else if (ACE_OS::strcmp (base_fname, "CCM_AMI::AMI4CCM_Base") == 0)
        {
          this->ami_connector_ = true;
          idl_global->ami_connector_seen_ = true;
          break;
        }
      else
        {
          // If we have a templated module, enable it as dds_connector_
          // for the moment
          AST_Module *m =
            dynamic_cast<AST_Module*> (this->defined_in ());
          AST_Template_Module_Inst *t_inst = nullptr;
          while (t_inst == nullptr && m != nullptr)
            {
              t_inst = m->from_inst ();
              m = dynamic_cast<AST_Module*> (m->defined_in ());
            }
          if (t_inst != nullptr && t_inst->template_args () != nullptr)
            {
              if (t_inst->template_args ()->size () > 0)
                {
                  this->dds_connector_ = true;
                  idl_global->dds_connector_seen_ = true;
                }
            }
        }

      base = base->base_connector ();
    }
}
