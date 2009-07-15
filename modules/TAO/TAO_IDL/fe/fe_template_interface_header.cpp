// $Id$

#include "fe_template_interface_header.h"
#include "fe_extern.h"
#include "ast_template_interface.h"
#include "utl_err.h"
#include "global_extern.h"

FE_Template_InterfaceHeader::FE_Template_InterfaceHeader (
      UTL_ScopedName *n,
      FE_Utils::T_PARAMLIST_INFO *params,
      FE_Utils::T_REFLIST_INFO *inherited)
  : FE_InterfaceHeader (n,
                        0,
                        false,
                        false,
                        false),
    FE_TemplateHeader_Common (params),
    parent_info_ (inherited)
{
  this->compile_template_inheritance ();

  if (this->duplicate_param_id ())
    {
      idl_global->err ()->duplicate_param_id (this->interface_name_);
    }
}

FE_Template_InterfaceHeader::~FE_Template_InterfaceHeader (void)
{
}

void
FE_Template_InterfaceHeader::destroy (void)
{
  // Must destroy the queue element members (but not the elements
  // themselves) before we destroy the queue.
  if (this->parent_info_ != 0)
    {
      for (FE_Utils::T_REFLIST_INFO::CONST_ITERATOR i (*this->parent_info_);
           !i.done ();
           i.advance ())
        {
          FE_Utils::T_Ref_Info *info = 0;
          i.next (info);
          info->destroy ();
        }
    }

  delete this->parent_info_;
  this->parent_info_ = 0;

  this->FE_InterfaceHeader::destroy ();
  this->FE_TemplateHeader_Common::destroy ();
}

// Data accessors.

void
FE_Template_InterfaceHeader::compile_template_inheritance (void)
{
  if (this->parent_info_ == 0)
    {
      return;
    }

  for (FE_Utils::T_REFLIST_INFO::CONST_ITERATOR iter (*this->parent_info_);
       !iter.done ();
       iter.advance ())
    {
      FE_Utils::T_Ref_Info *item = 0;
      iter.next (item);

      UTL_Scope *s = idl_global->scopes ().top_non_null ();
      AST_Decl *d = s->lookup_by_name (item->name_,
                                       true,
                                       true,
                                       true);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (item->name_);

          // Bad IDL, best to avoid a possible crash.
          throw Bailout ();
        }

      if (d->node_type () == AST_Decl::NT_typedef)
        {
          d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
        }

      AST_Template_Interface *i =
        AST_Template_Interface::narrow_from_decl (d);

      if (i == 0)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_TMPL_IFACE_EXPECTED,
                                      i);

          // Keep going but don't process the bad parent further
          continue;
        }

      // Each parent template parameter must match one in the child.  
      if (! this->match_params (i))
        {
          // Keep going but don't process the bad parent further 
          continue;
        }

      // This part is the same as for a non-template interface.  
      this->compile_one_inheritance (i);
    }

  // Same as for non-template interface.  
  this->install_in_header ();
}

bool
FE_Template_InterfaceHeader::match_params (AST_Template_Interface *node)
{
  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (node->template_params ());
       !i.done ();
       i.advance ())
    {
      FE_Utils::T_Param_Info *item = 0;
      i.next (item);

      bool one_matched = false;

      for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR j (*this->param_info_);
           !j.done ();
           j.advance ())
        {
          FE_Utils::T_Param_Info *my_item = 0;
          j.next (my_item);

          if (item->type_ == my_item->type_
              && item->name_ == my_item->name_)
            {
              one_matched = true;
              break;
            }
        }

      if (! one_matched)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_MISMATCHED_T_PARAM,
                                      node);

          return false;
        }
    }

  return true;
}

