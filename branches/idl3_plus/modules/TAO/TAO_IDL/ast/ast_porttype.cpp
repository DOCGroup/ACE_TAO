// $Id$

#include "ast_porttype.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_err.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_porttype,
           "$Id$")

AST_PortType::AST_PortType (
      UTL_ScopedName *n,
      const FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_porttype,
              n),
    UTL_Scope (AST_Decl::NT_porttype),
    AST_Template_Common (template_params)
{
}

AST_PortType::~AST_PortType (void)
{
}

void
AST_PortType::destroy (void)
{
  this->AST_Decl::destroy ();
}

void
AST_PortType::dump (ACE_OSTREAM_TYPE & /* o */)
{
}

int
AST_PortType::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_porttype (this);
}

AST_Provides *
AST_PortType::fe_add_provides (AST_Provides *p)
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

AST_Uses *
AST_PortType::fe_add_uses (AST_Uses *u)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (u, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      u,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, u->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      u,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (u);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (u,
                           false,
                           u->local_name ());

  return u;
}

AST_Publishes *
AST_PortType::fe_add_publishes (AST_Publishes *p)
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

AST_Emits *
AST_PortType::fe_add_emits (AST_Emits *e)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (e, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      e,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, e->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      e,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (e);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (e,
                           false,
                           e->local_name ());

  return e;
}

AST_Consumes *
AST_PortType::fe_add_consumes (AST_Consumes *c)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (c, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      c,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, c->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      c,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (c);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (c,
                           false,
                           c->local_name ());

  return c;
}

IMPL_NARROW_FROM_DECL (AST_PortType)
IMPL_NARROW_FROM_SCOPE (AST_PortType)

