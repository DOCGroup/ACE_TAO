/* -*- C++ -*- */
/**
 *  @file  DSRT_Sched_Queue_T.cpp
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
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

ACE_RCSID(Kokyu,
          DSRT_Sched_Queue_T,
          "$Id$")

namespace Kokyu
{

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
u_long
Sched_Ready_Queue<DSRT_Scheduler_Traits,
                  More_Eligible_Comparator,
                  ACE_LOCK>::
Guid_Hash::operator () (const Guid_t& id)
{
  typename DSRT_Scheduler_Traits::Guid_Hash guid_hash;
  return guid_hash(id);
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                      More_Eligible_Comparator,
                      ACE_LOCK>::
current_size ()
{
  return dispatch_items_prio_queue_.current_size ();
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                      More_Eligible_Comparator,
                      ACE_LOCK>::
most_eligible (DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>& item)
{
  if (dispatch_items_prio_queue_.current_size () == 0)
      return -1;

  PRIO_QUEUE_ITERATOR start = dispatch_items_prio_queue_.begin ();
  PRIO_QUEUE_ENTRY &ent = (*start);
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
  RB_Tree_Dispatch_Item_Node* rb_tree_node;

  if (dispatch_items_hash_map_.find(guid, rb_tree_node) == -1)
    {
      return -1;
    }
  else
    {
      found_item = rb_tree_node->item ();
      return 0;
    }

  return 0;
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                      More_Eligible_Comparator,
                      ACE_LOCK>::
insert(Guid_t guid, const DSRT_QoSDescriptor_t& qos)
{
  DSRT_Dispatch_Item<DSRT_Scheduler_Traits>* item;
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

  ACE_NEW_RETURN (item,
                  DSRT_Dispatch_Item<DSRT_Scheduler_Traits> (guid, qos),
                  -1);
  item->thread_handle (thr_handle);
  item->insertion_time (ACE_OS::gettimeofday ());
  DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> item_var(item);

  ACE_GUARD_RETURN (ACE_LOCK, mon, lock_, -1);

  RB_Tree_Dispatch_Item_Node* rb_tree_node;
  if (dispatch_items_hash_map_.find(guid, rb_tree_node) == -1)
    {
      if (dispatch_items_prio_queue_.bind (item_var,
                                           item_var,
                                           rb_tree_node) == 0)
        {
          if (dispatch_items_hash_map_.bind (guid, rb_tree_node) == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "(%t) insert item done\n"));
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents Begin===>\n"));
              dispatch_items_hash_map_.dump ();
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents End=====>\n"));
              return 0;
            }
        }
    }
  else
    {
      dispatch_items_hash_map_.unbind (guid);
      dispatch_items_prio_queue_.unbind (rb_tree_node);
      if (dispatch_items_prio_queue_.bind (item_var,
                                           item_var,
                                           rb_tree_node) == 0)
        {
          if (dispatch_items_hash_map_.bind (guid, rb_tree_node) == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "(%t) insert item done\n"));
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents Begin===>\n"));
              dispatch_items_hash_map_.dump ();
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents End===>\n"));
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
remove(Guid_t guid)
{
  ACE_GUARD_RETURN (ACE_LOCK, mon, lock_, -1);
  RB_Tree_Dispatch_Item_Node* rb_tree_node;

  if (dispatch_items_hash_map_.find(guid, rb_tree_node) == 0)
    {
      dispatch_items_hash_map_.unbind (guid);
      dispatch_items_prio_queue_.unbind (rb_tree_node);
      ACE_DEBUG ((LM_DEBUG,
                  "<===Hash Table contents Begin===>\n"));
      dispatch_items_hash_map_.dump ();
      ACE_DEBUG ((LM_DEBUG,
                  "<===Hash Table contents End===>\n"));
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
dump()
{
  ACE_GUARD (ACE_LOCK, mon, lock_);
  if (dispatch_items_prio_queue_.current_size ())
    {
      PRIO_QUEUE_ITERATOR end_iter = dispatch_items_prio_queue_.end ();
      PRIO_QUEUE_ITERATOR start;

      start = dispatch_items_prio_queue_.begin ();
      while( start != end_iter )
        {
          PRIO_QUEUE_ENTRY &ent = (*start);
          DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>
            item_var = ent.item ();
          /*
          int guid;
          ACE_OS::memcpy (&guid,
                  item_var->guid ()->get_buffer (),
                  item_var->guid ()->length ());

          ACE_DEBUG ((LM_DEBUG, "guid %d\n", guid));
          */
          ++start;
        }
    }
}

}

#endif /* DSRT_SCHED_QUEUE_T_CPP */
