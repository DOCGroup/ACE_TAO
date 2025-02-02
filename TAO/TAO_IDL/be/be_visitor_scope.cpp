
//=============================================================================
/**
 *  @file    be_visitor_scope.cpp
 *
 *  Visitor for the base be_scope node. This serves to maintain the current
 *  state (context) of code generation for the derived visitor.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#include "be_argument.h"
#include "be_scope.h"
#include "be_visitor_scope.h"
#include "be_visitor_context.h"
#include "ace/Log_Msg.h"

// ******************************************************
//  Generic visitor for a scope.
//  All elements that give rise to a scope inherit from
//  this class.
// ******************************************************

be_visitor_scope::be_visitor_scope (be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    elem_number_ (0)
{
}

be_visitor_scope::~be_visitor_scope ()
{
}

// Visit the scope and its elements.
int
be_visitor_scope::visit_scope (be_scope *node)
{
  if (node == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_visitor_scope::visit_scope - "
                         "nill node passed\n"),
                        -1);
    }

  // Proceed if the number of members in our scope is greater than 0.
  this->elem_number_ = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d == nullptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"),
                            -1);
        }

      if (d->node_type () == AST_Decl::NT_annotation_decl)
        {
          continue;
        }

      be_decl *bd = dynamic_cast<be_decl*> (d);

      // Set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation.
      this->ctx_->scope (node);

      // Set the node to be visited.
      this->ctx_->node (bd);
      ++this->elem_number_;

      // Do any pre processing using the next item info.
      if (this->pre_process (bd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "pre processing failed\n"),
                            -1);
        }

      // Send the visitor.
      if (bd == nullptr || bd->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "codegen for scope failed\n"),
                            -1);
        }

      // Do any post processing using this item info.
      if (this->post_process (bd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "post processing failed\n"),
                            -1);
        }
    }

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
be_visitor_scope::elem_number ()
{
  // Return the current element that we are working on.
  return this->elem_number_;
}

// Find the element that succeeds "elem" in the list.
int
be_visitor_scope::next_elem (be_decl *elem,
                             be_decl *&successor)
{
  be_decl *ctx_scope = this->ctx_->scope ()->decl ();
  be_scope *node = nullptr;

  if (ctx_scope != nullptr)
    {
      node = ctx_scope->scope ();
    }

  if (ctx_scope == nullptr || node == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_scope::next_elem - "
                         "bad scope\n"),
                        -1);
    }

  successor = nullptr;

  // Initialize an iterator to iterate thru our scope.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      be_decl *bd = dynamic_cast<be_decl*> (si.item ());

      if (bd == nullptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::next_elem - "
                             "bad node in this scope\n"),
                            -1);
        }

      if (bd != elem)
        {
          continue;
        }

      // Find who is next to me.
      si.next ();

      if (si.is_done ())
        {
          // Nobody left in the list.
          return 0;
        }

      successor = dynamic_cast<be_decl*> (si.item ());

      if (successor == nullptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::next_elem - "
                             "bad node in this scope\n"),
                            -1);
        }

      // Nothing else to do.
      return 0;
    }

  return 0;
}

bool
be_visitor_scope::last_node (be_decl *bd)
{
  be_decl *next = nullptr;
  (void) this->next_elem (bd,
                          next);

  return (next == nullptr);
}

bool
be_visitor_scope::last_inout_or_out_node (be_decl *)
{
  // Return true if we are the last inout or out argument.
  be_decl *next = nullptr;
  (void) this->next_elem (this->ctx_->node (),
                          next);

  while (next != nullptr)
    {
      be_argument *arg = dynamic_cast<be_argument*> (next);

      if (arg->direction () == AST_Argument::dir_INOUT
          || arg->direction () == AST_Argument::dir_OUT)
        {
          // Not the last.
          return false;
        }

      be_decl *next_next = nullptr;
      this->next_elem (next,
                       next_next);

      next = next_next;
    }

  // I am the last one.
  return true;
}
