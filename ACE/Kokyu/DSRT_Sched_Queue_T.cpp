/* -*- C++ -*- */
/**
 *  @file  DSRT_Sched_Queue_T.cpp
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 */
#ifndef DSRT_SCHED_QUEUE_T_CPP
#define DSRT_SCHED_QUEUE_T_CPP

#include "DSRT_Sched_Queue_T.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
//#include "DSRT_Sched_Queue_T.i"
#endif /* __ACE_INLINE__ */

namespace Kokyu
{
template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                      More_Eligible_Comparator,
                      ACE_LOCK>::
current_size ()
{
  return ACE_Utils::truncate_cast<int> (
    this->dispatch_items_prio_queue_.current_size ());
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                      More_Eligible_Comparator,
                      ACE_LOCK>::
most_eligible (DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>& item)
{
  if (this->dispatch_items_prio_queue_.current_size () == 0)
    {
      return -1;
    }

  PRIO_QUEUE_ITERATOR const start = this->dispatch_items_prio_queue_.begin ();
  PRIO_QUEUE_ENTRY &ent = *start;
  item = ent.item ();
  return 0;
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                      More_Eligible_Comparator,
                      ACE_LOCK>::
find (Guid_t guid,
      DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>& found_item)
{
  ACE_GUARD_RETURN (ACE_LOCK, mon, lock_, -1);
  RB_Tree_Dispatch_Item_Node *rb_tree_node {};

  if (this->dispatch_items_hash_map_.find (guid, rb_tree_node) == -1)
    {
      return -1;
    }

  found_item = rb_tree_node->item ();
  return 0;
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                      More_Eligible_Comparator,
                      ACE_LOCK>::
insert (DSRT_Dispatch_Item<DSRT_Scheduler_Traits>* item)
{
  item->insertion_time (ACE_OS::gettimeofday ());
  DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> const item_var (item);

  ACE_GUARD_RETURN (ACE_LOCK, mon, lock_, -1);

  RB_Tree_Dispatch_Item_Node* rb_tree_node;
  Guid_t const guid = item->guid ();

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T) about to insert item in sched queue\n"));
#endif

  if (this->dispatch_items_hash_map_.find (guid, rb_tree_node) == -1)
    {
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T) item not found in hashmap\n"));
#endif
      if (this->dispatch_items_prio_queue_.bind (item_var,
                                                 item_var,
                                                 rb_tree_node) == 0)
        {
#ifdef KOKYU_DSRT_LOGGING
          ACE_DEBUG ((LM_DEBUG, "(%t|%T): item bound in rbtree\n"));
#endif
          if (this->dispatch_items_hash_map_.bind (guid, rb_tree_node) == 0)
            {
#ifdef KOKYU_DSRT_LOGGING
              ACE_DEBUG ((LM_DEBUG, "(%t|%T): item bound in hashmap\n"));
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents Begin===>\n"));
              this->dispatch_items_hash_map_.dump ();
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents End=====>\n"));
#endif
              return 0;
            }
        }
    }
  else
    {
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T) item found in hashmap\n"));
#endif
      this->dispatch_items_hash_map_.unbind (guid);
      this->dispatch_items_prio_queue_.unbind (rb_tree_node);

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T) item removed from hashmap and rbtree\n"));
#endif
      if (this->dispatch_items_prio_queue_.bind (item_var,
                                                 item_var,
                                                 rb_tree_node) == 0)
        {
#ifdef KOKYU_DSRT_LOGGING
          ACE_DEBUG ((LM_DEBUG,
                      "(%t|%T) replacement item bound to rbtree\n"));
#endif
          if (this->dispatch_items_hash_map_.bind (guid, rb_tree_node) == 0)
            {
#ifdef KOKYU_DSRT_LOGGING
              ACE_DEBUG ((LM_DEBUG,
                          "(%t|%T) replacement item bound to hashmap\n"));
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents Begin===>\n"));
              this->dispatch_items_hash_map_.dump ();
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents End===>\n"));
#endif
              return 0;
            }
        }
    }

  return -1;
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator, class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                      More_Eligible_Comparator, ACE_LOCK>::
remove (Guid_t guid)
{
  ACE_GUARD_RETURN (ACE_LOCK, mon, lock_, -1);
  RB_Tree_Dispatch_Item_Node *rb_tree_node {};

  if (this->dispatch_items_hash_map_.find (guid, rb_tree_node) == 0)
    {
      this->dispatch_items_hash_map_.unbind (guid);
      this->dispatch_items_prio_queue_.unbind (rb_tree_node);
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "<===Hash Table contents Begin===>\n"));
      this->dispatch_items_hash_map_.dump ();
      ACE_DEBUG ((LM_DEBUG,
                  "<===Hash Table contents End===>\n"));
#endif

      return 0;
    }

  return -1;
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
void Sched_Ready_Queue<DSRT_Scheduler_Traits,
                       More_Eligible_Comparator,
                       ACE_LOCK>::
dump ()
{
  ACE_GUARD (ACE_LOCK, mon, lock_);
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):##########################\n"));
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                       More_Eligible_Comparator,
                       ACE_LOCK>::
change_prio (int old_prio, int new_prio, int policy)
{
  if (this->dispatch_items_prio_queue_.current_size ())
    {
      PRIO_QUEUE_ITERATOR const end_iter = this->dispatch_items_prio_queue_.end ();
      PRIO_QUEUE_ITERATOR iter;
      int prio = 0;

      iter = this->dispatch_items_prio_queue_.begin ();
      while (iter != end_iter)
        {
          PRIO_QUEUE_ENTRY &ent = (*iter);
          DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> const item_var = ent.item ();
          ACE_OS::thr_getprio (item_var->thread_handle (), prio);
          if (prio == old_prio)
            {
              ACE_OS::thr_setprio (item_var->thread_handle (), new_prio, policy);
            }
          ++iter;
        }
    }
  return 0;
}

}

#endif /* DSRT_SCHED_QUEUE_T_CPP */
