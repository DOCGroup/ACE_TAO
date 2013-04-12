/* -*- c++ -*- */
// $Id$

#include "orbsvcs/Log_Macros.h"
#include "ifr_removing_visitor.h"
#include "utl_scope.h"
#include "ast_root.h"

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

      try
        {
          // Continue until each element is visited.
          while (!si.is_done ())
            {
              d = si.item ();

              if (d == 0)
                {
                  ORBSVCS_ERROR_RETURN ((
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

              CORBA::Contained_var top_level =
                be_global->repository ()->lookup_id (d->repoID ());

              if (!CORBA::is_nil (top_level.in ()))
                {
                  // All we have to do is call destroy() on each IR object
                  // in the global scope, because destroy() works on all
                  // the contents recursively.
                  top_level->destroy ();
                }

              si.next ();
            }
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            ACE_TEXT (
              "ifr_removing_visitor::visit_scope"));

          return -1;
        }
    }

  return 0;
}

int
ifr_removing_visitor::visit_root (AST_Root *node)
{
  try
    {
      CORBA::Container_var new_scope =
        CORBA::Container::_narrow (be_global->repository ());

      if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
        {
          ORBSVCS_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_removing_visitor::visit_root -")
              ACE_TEXT (" scope push failed\n")
            ),
            -1
          );
        }

      if (this->visit_scope (node) == -1)
        {
          ORBSVCS_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_removing_visitor::visit_root -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      CORBA::Container_ptr tmp = CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().pop (tmp) != 0)
        {
          ORBSVCS_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_removing_visitor::visit_root -")
              ACE_TEXT (" scope pop failed\n")
            ),
            -1
          );
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("visit_root"));

      return -1;
    }

  return 0;
}

