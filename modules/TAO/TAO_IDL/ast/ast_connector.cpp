// $Id$

#include "ast_connector.h"
#include "ast_tmpl_mirror_port.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_err.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_connector,
           "$Id$")

AST_Connector::AST_Connector (
      UTL_ScopedName *n,
      AST_Connector *base_connector,
      FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_connector,
              n),
    AST_Type (AST_Decl::NT_connector,
              n),
    UTL_Scope (AST_Decl::NT_connector),
    AST_Interface (n,
                   0,
                   0,
                   0,
                   0,
                   false,
                   false),
    AST_Component (n,
                   base_connector,
                   0,
                   0,
                   0,
                   0),
    template_params_ (template_params)
{
}

AST_Connector::~AST_Connector (void)
{
}

AST_Connector *
AST_Connector::base_connector (void) const
{
  return AST_Connector::narrow_from_decl (this->pd_base_component);
}

void
AST_Connector::destroy (void)
{
  this->AST_Component::destroy ();
  delete this->template_params_;
  this->template_params_ = 0;
}

void
AST_Connector::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "connector ");

  this->local_name ()->dump (o);

  this->dump_i (o, " ");

  if (this->pd_base_component != 0)
    {
      this->dump_i (o, ": ");
      this->pd_base_component->local_name ()->dump (o);
    }

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}");
}

int
AST_Connector::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_connector (this);
}

AST_Tmpl_Port *
AST_Connector::fe_add_tmpl_port (AST_Tmpl_Port *p)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (p, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      p,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, p->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      p,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (p);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (p,
                           false,
                           p->local_name ());

  return p;
}

AST_Tmpl_Mirror_Port *
AST_Connector::fe_add_tmpl_mirror_port (AST_Tmpl_Mirror_Port *p)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (p, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      p,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, p->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      p,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (p);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (p,
                           false,
                           p->local_name ());

  return p;
}

IMPL_NARROW_FROM_DECL (AST_Connector)
IMPL_NARROW_FROM_SCOPE (AST_Connector)
