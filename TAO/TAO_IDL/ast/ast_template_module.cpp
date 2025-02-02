#include "ast_template_module.h"
#include "ast_template_module_ref.h"
#include "ast_constant.h"
#include "ast_enum.h"
#include "ast_typedef.h"
#include "ast_visitor.h"

#include "utl_err.h"
#include "utl_string.h"
#include "utl_strlist.h"
#include "global_extern.h"
#include "nr_extern.h"

AST_Decl::NodeType const
AST_Template_Module::NT = AST_Decl::NT_module;

AST_Template_Module::AST_Template_Module (
      UTL_ScopedName *n,
      FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_module,
              n),
    UTL_Scope (AST_Decl::NT_module),
    AST_Module (n),
    AST_Type (AST_Decl::NT_module,
              n),
    template_params_ (template_params)
{
}

AST_Template_Module::~AST_Template_Module ()
{
}

FE_Utils::T_PARAMLIST_INFO *
AST_Template_Module::template_params () const
{
  return this->template_params_;
}

bool
AST_Template_Module::match_arg_names (FE_Utils::T_ARGLIST *args)
{
  if (args->size () != this->template_params_->size ())
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_T_ARG_LENGTH,
                                  this);
      return false;
    }

  size_t slot = 0UL;

  for (FE_Utils::T_ARGLIST::CONST_ITERATOR i (*args);
       !i.done ();
       i.advance (), ++slot)
    {
      AST_Decl **item = nullptr;
      i.next (item);
      AST_Decl *d = *item;

      if (d->node_type () == AST_Decl::NT_typedef)
        {
          AST_Typedef *td = dynamic_cast<AST_Typedef*> (d);

          d = td->primitive_base_type ();
        }

      FE_Utils::T_Param_Info *param = nullptr;
      (void) this->template_params_->get (param, slot);

      if (!this->match_one_param (param, d))
        {
          UTL_ScopedName *n = d->name ();
          const char *s = nullptr;

          if (!n)
            {
              AST_Constant *c = dynamic_cast<AST_Constant *> (d);
              s = AST_Expression::exprtype_to_string (c->et ());
            }
          else
            {
              s = d->full_name ();
            }

          idl_global->err ()->mismatched_template_param (s);

          return false;
        }
    }

  return true;
}

bool
AST_Template_Module::match_param_refs (UTL_StrList *refs,
                                       UTL_Scope *decl_scope)
{
  UTL_Scope *s = decl_scope;
  AST_Template_Module *enclosing = nullptr;

  while (enclosing == nullptr && s != nullptr)
    {
      enclosing = dynamic_cast<AST_Template_Module*> (s);
      s = ScopeAsDecl (s)->defined_in ();
    }

  if (enclosing == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("AST_Template_Module::")
                         ACE_TEXT ("match_param_refs - ")
                         ACE_TEXT ("enclosing template ")
                         ACE_TEXT ("module scope not found\n")),
                        false);
    }

  for (UTL_StrlistActiveIterator i (refs);
       !i.is_done ();
       i.next ())
    {
      FE_Utils::T_Param_Info *enclosing_param =
        enclosing->find_param (i.item ());

      if (enclosing_param == nullptr)
        {
          // Enclosing param not found
          return false;
        }

      if (!this->match_param_by_type (enclosing_param))
        {
          // Referenced param type not matched to enclosiong param.
          return false;
        }
    }

  return true;
}

void
AST_Template_Module::destroy ()
{
  delete this->template_params_;
  this->template_params_ = nullptr;

  this->AST_Module::destroy ();
}

int
AST_Template_Module::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_template_module (this);
}

AST_Template_Module_Ref *
AST_Template_Module::fe_add_template_module_ref (
  AST_Template_Module_Ref *m)
{
  return dynamic_cast<AST_Template_Module_Ref*> (this->fe_add_ref_decl (m));
}

void
AST_Template_Module::dump (ACE_OSTREAM_TYPE & /* o */)
{
}

bool
AST_Template_Module::match_one_param (FE_Utils::T_Param_Info *param,
                                      AST_Decl *d)
{
  if (param->type_ == AST_Decl::NT_type)
    {
      return true;
    }

  if (d->node_type () == AST_Decl::NT_typedef)
    {
      AST_Typedef *td = dynamic_cast<AST_Typedef*> (d);
      d = td->primitive_base_type ();
    }

  AST_Decl::NodeType other_type = d->node_type ();

  if (other_type == AST_Decl::NT_const)
    {
      AST_Constant *c = dynamic_cast<AST_Constant*> (d);

      AST_Expression *ex = c->constant_value ();

      AST_Expression::AST_ExprValue *ev =
        ex->check_and_coerce (param->const_type_,
                              param->enum_const_type_decl_);

      if (ev == nullptr)
        {
          idl_global->err ()->coercion_error (ex,
                                              param->const_type_);
        }

      bool retval = (ev != nullptr);
      delete ev;
      ev = nullptr;
      return retval;
    }

  return (param->type_ == other_type);
}

FE_Utils::T_Param_Info *
AST_Template_Module::find_param (UTL_String *name)
{
  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (
         *this->template_params_);
       !i.done ();
       i.advance ())
    {
      FE_Utils::T_Param_Info *param = nullptr;
      i.next (param);

      if (param->name_ == name->get_string ())
        {
          return param;
        }
    }

  return nullptr;
}

bool
AST_Template_Module::match_param_by_type (
  FE_Utils::T_Param_Info *param)
{
  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (
         *this->template_params_);
       !i.done ();
       i.advance ())
    {
      FE_Utils::T_Param_Info *my_param = nullptr;
      i.next (my_param);

      if (param->type_ == my_param->type_)
        {
          if (param->type_ == AST_Decl::NT_const)
            {
              if (param->const_type_ == my_param->const_type_)
                {
                  if (param->const_type_ == AST_Expression::EV_enum)
                    {
                      if (param->enum_const_type_decl_
                            == my_param->enum_const_type_decl_)
                        {
                          return true;
                        }
                    }
                  else
                    {
                      return true;
                    }
                }
            }
          else
            {
              return true;
            }
        }
    }

  idl_global->err ()->mismatched_template_param (
    param->name_.c_str ());

  return false;
}
