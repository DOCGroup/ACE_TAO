//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_scope.cpp
//
// = DESCRIPTION
//    Visitor for the base be_scope node. This serves to maintain the current
//    state (context) of code generation for the derived visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_scope.h"

ACE_RCSID(be, be_visitor_scope, "$Id$")


// ******************************************************
//  Generic visitor for a scope.
//  All elements that give rise to a scope inherit from
//  this class
// ******************************************************

be_visitor_scope::be_visitor_scope (be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    elem_number_ (0)
{
}

be_visitor_scope::~be_visitor_scope (void)
{
}

// visit the scope and its elements
int
be_visitor_scope::visit_scope (be_scope *node)
{
  // proceed if the number of members in our scope is greater than 0
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);
      this->elem_number_ = 0;
      // continue until each element is visited
      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();
          if (!d)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "bad node in this scope\n"), -1);

            }

          be_decl *bd = be_decl::narrow_from_decl (d);

          // set the scope node as "node" in which the code is being
          // generated so that elements in the node's scope can use it
          // for code generation
          this->ctx_->scope (node->decl ());

          // set the node to be visited
          this->ctx_->node (bd);
          this->elem_number_++;

          // Do any pre processing using the next item info. 
          if (this->pre_process (bd) == -1)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "pre processing failed\n"), -1);
            }

          // Send the visitor.
          if (bd == 0 ||  bd->accept (this) == -1)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "codegen for scope failed\n"), -1);

            }

          // Do any post processing using this item info.
          if (this->post_process (bd) == -1)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "post processing failed\n"), -1);
            }
          si->next ();
        } // end of while loop
      delete si;
    } // end of if


#if 0
  // For debugging the prefix...
  be_decl* decl = node->decl ();
  ACE_DEBUG ((LM_DEBUG, "Enter scope <%s> : [%s]\n",
              decl->full_name (), decl->prefix ()));
  if (decl->pragmas () != 0)
    {
      for (UTL_StrlistActiveIterator i(decl->pragmas ());
           !i.is_done ();
           i.next ())
        {
          String* s = i.item ();

          ACE_DEBUG ((LM_DEBUG, "  pragma = <%s>\n", s->get_string ()));
        }
    }
#endif

  return 0;
}

int
be_visitor_scope::post_process (be_decl *)
{
  return 0;
}

int
be_visitor_scope::pre_process (be_decl *)
{
  return 0;
}

int
be_visitor_scope::elem_number (void)
{
  // return the current element that we are working on
  return this->elem_number_;
}

// find the element that succeeds "elem" in the list
int
be_visitor_scope::next_elem (be_decl *elem,
                             be_decl *&successor)
{
  be_decl *ctx_scope = this->ctx_->scope ();
  be_scope *node = 0;

  if (ctx_scope)
    {
      node = ctx_scope->scope ();
    }

  if (!ctx_scope || !node)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_scope::next_elem - "
                         "bad scope\n"), -1);

    }

  successor = 0;
  // proceed if the number of members in our scope is greater than 0
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);
      // continue until each element is visited
      while (!si->is_done ())
        {
          be_decl *bd = be_decl::narrow_from_decl (si->item ());
          if (!bd)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::next_elem - "
                                 "bad node in this scope\n"), -1);

            }

          if (bd == elem)
            {
              // find who is next to me
              si->next ();
              if (si->is_done ())
                {
                  // nobody left in the list
                  delete si;
                  return 0;
                }
              successor = be_decl::narrow_from_decl (si->item ());
              if (!successor)
                {
                  delete si;
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_visitor_scope::next_elem - "
                                     "bad node in this scope\n"), -1);

                }
              // nothing else to do
              delete si;
              return 0;
            }
          else
            // proceed to the next element
            si->next ();
        } // end of while loop
      delete si;
    } // end of if
  return 0;
}

idl_bool
be_visitor_scope::last_node (be_decl *bd)
{
  be_decl *next = 0;
  (void) this->next_elem (bd, next);
  if (next)
    // not the last
    return 0;

  // I am the last one
  return 1;
}

idl_bool
be_visitor_scope::last_inout_or_out_node (be_decl *)
{
  // Return true if we are the last inout or out argument
  be_decl *next = 0;
  this->next_elem (this->ctx_->node (),
                   next);
  while (next)
    {
      be_argument *arg = be_argument::narrow_from_decl (next);

      if (arg->direction () == AST_Argument::dir_INOUT
          || arg->direction () == AST_Argument::dir_OUT)
          // not the last
          return 0;

      be_decl *next_next = 0;
      this->next_elem (next, next_next);

      next = next_next;
    }
  // I am the last one
  return 1;
}

