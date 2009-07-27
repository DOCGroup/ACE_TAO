// $Id$

#include "ast_porttype.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_constant.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_namelist.h"
#include "utl_err.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_porttype,
           "$Id$")

AST_PortType::AST_PortType (
      UTL_ScopedName *n,
      FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_porttype,
              n),
    AST_Type (AST_Decl::NT_porttype,
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
  this->AST_Type::destroy ();
  this->AST_Template_Common::destroy ();
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

AST_PortType::T_ARGLIST *
AST_PortType::match_arg_names (UTL_NameList *arg_names)
{
  size_t names_len = static_cast<size_t> (arg_names->length ());
  
  if (names_len != this->template_params_->size ())
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_T_ARG_LENGTH,
                                  this);
      return 0;
    }
    
  size_t slot = 0UL;
  bool good = true;
  T_ARGLIST *retval = 0;
  ACE_NEW_RETURN (retval,
                  T_ARGLIST,
                  0);
  
  for (UTL_NamelistActiveIterator iter (arg_names);
       !iter.is_done ();
       iter.next (), ++slot)
    {
      UTL_ScopedName *name = iter.item ();
      
      AST_Decl *d = this->lookup_by_name (name, true);
      
      if (d == 0)
        {
          idl_global->err ()->lookup_error (name);
          good = false;
          break;
        }
      else if (d->node_type () == AST_Decl::NT_typedef)
        {
          AST_Typedef *td =
            AST_Typedef::narrow_from_decl (d);
            
          d = td->primitive_base_type ();
        }
      
      FE_Utils::T_Param_Info *param = 0;
      (void) this->template_params_->get (param, slot);
        
      if (! this->match_param_type (param->type_, d))
        {
          idl_global->err ()->mismatched_template_param (d->name ());
          good = false;
          break;
        }
        
      retval->enqueue_tail (d);
    }
    
  if (! good)
    {
      delete retval;
      retval = 0;
    }
    
  return retval;
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

bool
AST_PortType::match_param_type (AST_Decl::NodeType my_type,
                                AST_Decl *d)
{
  if (my_type == AST_Decl::NT_type)
    {
      return true;
    }
    
  AST_Decl::NodeType other_type = d->node_type ();
    
  if (other_type == AST_Decl::NT_field)
    {
      other_type =
        AST_Field::narrow_from_decl (d)->field_type ()->node_type ();
    }
    
  if (other_type == AST_Decl::NT_const)
    {
      AST_Constant *c =
        AST_Constant::narrow_from_decl (d);
        
      switch (c->et ())
        {
          case AST_Expression::EV_enum:
            other_type = AST_Decl::NT_enum;
            break;
          case AST_Expression::EV_string:
            other_type = AST_Decl::NT_string;
            break;
          case AST_Expression::EV_wstring:
            other_type = AST_Decl::NT_wstring;
            break;
          case AST_Expression::EV_any:
          case AST_Expression::EV_object:
            return false;
          default:
            other_type = AST_Decl::NT_pre_defined;
            break;
        }
    }
    
  // Mapping 'primitive' keyword to NT_pre_defined isn't
  // clearly correct - need to raise an issue about what the
  // keyword means.  
  switch (my_type)
    {
      case AST_Decl::NT_pre_defined:
        switch (other_type)
          {
            case AST_Decl::NT_string:
            case AST_Decl::NT_wstring:
            case AST_Decl::NT_pre_defined:
              return true;
            default:
              return false;
          }
      default:
        return my_type == other_type;
    }
}

IMPL_NARROW_FROM_DECL (AST_PortType)
IMPL_NARROW_FROM_SCOPE (AST_PortType)

