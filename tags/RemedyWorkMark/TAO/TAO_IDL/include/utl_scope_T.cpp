// $Id$

#ifndef TAO_IDL_UTL_SCOPE_T_CPP
#define TAO_IDL_UTL_SCOPE_T_CPP

#include "utl_scope.h"

#include "nr_extern.h"
#include "global_extern.h"

#include "utl_err.h"

#include "ast_interface_fwd.h"

template<typename DECL>
DECL *
UTL_Scope::fe_add_full_intf_decl (DECL *t)
{
  if (t->redef_clash ())
    {
      return 0;
    }

  AST_Decl *predef = 0;
  DECL *fwd = 0;

  // Already defined?
  if ((predef = this->lookup_for_add (t)) != 0)
    {
      // Treat fwd declared interfaces specially
      if (predef->node_type () == DECL::NT)
        {
          fwd = DECL::narrow_from_decl (predef);

          if (fwd == 0)
            {
              return 0;
            }

          // Forward declared and not defined yet.
          if (!fwd->is_defined ())
            {
              if (fwd->defined_in () != this)
                {
                  idl_global->err ()->error3 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                              fwd,
                                              t,
                                              ScopeAsDecl (this));

                  return 0;
                }
            }
          // OK, not illegal redef of forward declaration. Now check whether.
          // it has been referenced already.
          else if (this->referenced (predef, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          ScopeAsDecl (this),
                                          predef);

              return 0;
            }

        }
      else if (!FE_Utils::can_be_redefined (predef, t))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      ScopeAsDecl (this),
                                      predef);

          return 0;
        }
      else if (referenced (predef, t->local_name ()) && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      ScopeAsDecl (this),
                                      predef);

          return 0;
        }
      else if (t->has_ancestor (predef))
        {
          idl_global->err ()->redefinition_in_scope (t, predef);

          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // We do this for interfaces, valuetypes and components in
  // a different place than we do for structs and unions,
  // since fwd declared structs and unions must be defined in
  // the same translation unit.
  AST_InterfaceFwd *fd = t->fwd_decl ();

  if (0 != fd)
    {
      fd->set_as_defined ();
    }

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           false,
                           t->local_name ());
  return t;
}

template<typename FULL_DECL>
typename FULL_DECL::FWD_TYPE *
UTL_Scope::fe_add_fwd_intf_decl (typename FULL_DECL::FWD_TYPE *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add() calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_prev_mods() for modules. If look_in_prev_mods()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (nt == FULL_DECL::NT)
        {
          FULL_DECL *itf = FULL_DECL::narrow_from_decl (d);

          if (itf == 0)
            {
              return 0;
            }

          // If the lookup found the full_definition member of another
          // interface_fwd, don't reset this full_definition. Otherwise
          // reset the member and set is_defined_ on i so it itf won't
          // get destroyed twice.
          if (itf->is_defined ())
            {
              if (!t->is_defined ())
                {
                  FULL_DECL *prev_fd =
                    FULL_DECL::narrow_from_decl (t->full_definition ());

                  prev_fd->destroy ();
                  // No need to delete prev_fd, the call to
                  // set_full_definition() below will do it.
                }

              t->set_full_definition (itf);
              t->set_as_defined ();
            }
        }

      if (!FE_Utils::can_be_redefined (d, t)) {

          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      ScopeAsDecl (this),
                                      d);
          return 0;
        }

      // No need to call referenced() for forward declared interafces,
      // they can be redeclared after referencing.

      if (t->has_ancestor (d))
        {
          idl_global->err ()->redefinition_in_scope  (t, d);
          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           false,
                           t->local_name ());

  return t;
}

#endif // TAO_IDL_UTL_SCOPE_T_CPP

