#include "ast_annotation_appl.h"
#include "ast_annotation_member.h"

AST_Annotation_Appl::Param::Param ()
  : used (false)
{
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
    params_ (params),
    annotation_decl_ (0)
{
}

AST_Annotation_Appl::~AST_Annotation_Appl ()
{
  delete [] original_name_;
}

void AST_Annotation_Appl::dump (ACE_OSTREAM_TYPE &o)
{
  dump_i (o, original_name_);
  if (params_)
    {
      dump_i (o, "(");
      Params::ITERATOR iter (*params_);
      Param **i = 0;
      while (!iter.done ())
        {
          iter.next (i);
          if ((*i)->id)
            {
              (*i)->id->dump (o);
              dump_i (o, " = ");
            }
          (*i)->expr->dump (o);
          iter.advance ();
          if (!iter.done ())
            {
              dump_i (o, ", ");
            }
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

const char *AST_Annotation_Appl::original_name () const
{
  return original_name_;
}

IMPL_NARROW_FROM_DECL (AST_Annotation_Appl)
IMPL_NARROW_FROM_SCOPE (AST_Annotation_Appl)

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
        AST_Annotation_Member::narrow_from_decl (si.item ());
      if (member)
        {
          AST_Annotation_Member *new_member = fe_add_annotation_member (
              new AST_Annotation_Member (member->name (), member));

          /*
           * Check to see if we have a parameter that matches this. If not,
           * make sure that the member has a default value.
           */
          Param *param = find_param (member->local_name ()->get_string ());
          if (param)
            {
              new_member->value (new AST_Expression (param->expr, member->expr_type()));
              if (new_member->invalid_value ())
              {
                idl_global->err ()->invalid_annotation_param_type (
                  this, member, param->expr);
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
  Params *parameters = params ();
  if (parameters)
    {
      Param **param = 0;
      for (Param::Iterator it (*parameters);
          !it.done (); it.advance ())
        {
          it.next (param);
          if ((*param) && !(*param)->used)
            {
              idl_global->err ()->invalid_annotation_param_error (
                this, decl, (*param)->id);
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
  return params_;
}

AST_Annotation_Decl *
AST_Annotation_Appl::annotation_decl ()
{
  return annotation_decl_;
}

AST_Annotation_Appl::Param *
AST_Annotation_Appl::find_param (const char *name)
{
  Params *parameters = params ();
  if (parameters)
    {
      // Check for single nameless parameter
      if (parameters->size () == 1)
        {
          Param *top;
          parameters->top (top);
          if (top && !top->id && top->expr)
            {
              // Don't reuse it if used
              return top->used ? 0 : top;
            }
        }
      Param **param;
      for (Param::Iterator it (*parameters);
          !it.done (); it.advance ())
        {
          it.next (param);
          if ((*param) && (*param)->id && !ACE_OS::strcmp ((*param)->id->get_string (), name))
            {
              return (*param);
            }
        }
    }

  return 0;
}
