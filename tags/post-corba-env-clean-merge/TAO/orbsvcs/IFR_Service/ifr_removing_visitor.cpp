/* -*- c++ -*- */
// $Id$

#include "ifr_removing_visitor.h"
#include "utl_scope.h"

ACE_RCSID(IFR_Service, ifr_removing_visitor, "$Id$")

ifr_removing_visitor::ifr_removing_visitor (void)
{
}

ifr_removing_visitor::~ifr_removing_visitor (void)
{
}

int
ifr_removing_visitor::visit_scope (UTL_Scope *node)
{
  // Proceed if the number of members in our scope is greater than 0.
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate over our scope.
      UTL_ScopeActiveIterator si (node,
                                  UTL_Scope::IK_decls);

      AST_Decl *d = 0;

      TAO_ENV_DECLARE_NEW_ENV;
      ACE_TRY
        {
          // Continue until each element is visited.
          while (!si.is_done ())
            {
              d = si.item ();

              if (d == 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_removing_visitor::visit_scope -")
                      ACE_TEXT (" bad node in this scope\n")
                    ),
                    -1
                  );
                }

              if (d->node_type () == AST_Decl::NT_pre_defined)
                {
                  // We can skip these - they don't get destroyed in the IfR.
                  si.next ();
                  continue;
                }

              CORBA_Contained_var top_level =
                be_global->repository ()->lookup_id (d->repoID ()
                                                     TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (!CORBA::is_nil (top_level.in ()))
                {
                  // All we have to do is call destroy() on each IR object
                  // in the global scope, because destroy() works on all
                  // the contents recursively.
                  top_level->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }

              si.next ();
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (
              ACE_ANY_EXCEPTION,
              ACE_TEXT ("ifr_removing_visitor::visit_scope")
            );

          return -1;
        }
      ACE_ENDTRY;
    }

  return 0;
}

