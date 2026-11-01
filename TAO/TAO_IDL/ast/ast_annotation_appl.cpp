#include "ast_annotation_appl.h"
#include "ast_annotation_member.h"

#include <memory>

AST_Annotation_Appl::Param::~Param ()
{
  if (id)
    {
      id->destroy ();
    }
  if (expr)
    {
      expr->destroy ();
    }
}

void
AST_Annotation_Appl::delete_params (AST_Annotation_Appl::Params* params)
{
  delete params;
}

AST_Decl::NodeType const AST_Annotation_Appl::NT = AST_Decl::NT_annotation_appl;

AST_Annotation_Appl::AST_Annotation_Appl (
  UTL_ScopedName *name, AST_Annotation_Appl::Params *params)
  : AST_Decl (NT, name),
    AST_Type (NT, name),
    AST_ConcreteType (NT, name),
    UTL_Scope (NT),
    AST_Structure (name, false, false),
    AST_Annotation_Decl (name),
    original_name_ (name->get_string_copy ()),
    params_ (params)
{
}

AST_Annotation_Appl::~AST_Annotation_Appl ()
{
  params_.reset ();
  AST_Structure::destroy ();
}

void AST_Annotation_Appl::dump (ACE_OSTREAM_TYPE &o)
{
  dump_i (o, this->original_name_.c_str ());
  if (params_)
    {
      dump_i (o, "(");
      bool first = true;
      for (std::unique_ptr<Param> const& param : *params_)
        {
          if (!first)
            {
              dump_i (o, ", ");
            }
          first = false;

          if (param->id)
            {
              param->id->dump (o);
              dump_i (o, " = ");
            }
          param->expr->dump (o);
        }
      dump_i (o, ")");
    }
}

int AST_Annotation_Appl::ast_accept (ast_visitor *)
{
  return 0;
}

void AST_Annotation_Appl::destroy ()
{
}

std::string const& AST_Annotation_Appl::original_name () const
{
  return original_name_;
}

bool
AST_Annotation_Appl::apply_from (AST_Annotation_Decl *decl)
{
  // Apply Each Member From the Annotation Declaration
  for (UTL_ScopeActiveIterator si (
      decl,
      UTL_Scope::IK_decls);
    !si.is_done ();
    si.next ())
    {
      AST_Annotation_Member *member =
        dynamic_cast<AST_Annotation_Member*> (si.item ());
      if (member)
        {
          AST_Annotation_Member *new_member = fe_add_annotation_member (
              new AST_Annotation_Member (member->name (), member));

          /*
           * Check to see if we have a parameter that matches this. If not,
           * make sure that the member has a default value.
           */
          Param *param = find_param (
            member->local_name ()->get_string ());
          if (param)
            {
              new_member->value (
                std::make_unique<AST_Expression> (param->expr.get (), member->expr_type ()));
              if (new_member->invalid_value ())
              {
                idl_global->err ()->invalid_annotation_param_type (
                  this, member, param->expr.get ());
                return false;
              }
              param->used = true;
            }
          else if (!new_member->value ())
            {
              idl_global->err ()->annotation_param_missing_error (
                this, member);
              return false;
            }
        }
    }

  // Make sure all the parameters were used
  if (params_)
    {
      for (std::unique_ptr<Param> const& param : *params_)
        {
          if (param && !param->used)
            {
              idl_global->err ()->invalid_annotation_param_error (
                this, decl, param->id.get ());
              return false;
            }
        }
    }

  annotation_decl_ = decl;

  return true;
}

AST_Annotation_Appl::Params *
AST_Annotation_Appl::params ()
{
  return params_.get ();
}

AST_Annotation_Decl *
AST_Annotation_Appl::annotation_decl ()
{
  return annotation_decl_;
}

AST_Annotation_Appl::Param *
AST_Annotation_Appl::find_param (std::string const& name) const
{
  if (params_)
    {
      // Check for single nameless parameter
      if (params_->size () == 1)
        {
          Param *top = params_->front ().get ();
          if (top && !top->id && top->expr)
            {
              // Don't reuse it if used
              return top->used ? nullptr : top;
            }
        }
      for (std::unique_ptr<Param> const& param : *params_)
        {
          if (param && param->id
              && param->id->get_string () == name)
            {
              return param.get ();
            }
        }
    }

  return nullptr;
}
