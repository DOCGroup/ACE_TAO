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
/*
//@@VS: This is somehow not being recognized by MSVC, which results
//in a link error. For now, the definition has been moved to the .h
//file. Needs further investigation.

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
*/
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

  if (this->find_i(guid, rb_tree_node) == -1)
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
insert (DSRT_Dispatch_Item<DSRT_Scheduler_Traits>* item)
{
  item->insertion_time (ACE_OS::gettimeofday ());
  DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> item_var(item);

  ACE_GUARD_RETURN (ACE_LOCK, mon, lock_, -1);

  RB_Tree_Dispatch_Item_Node* rb_tree_node;
  Guid_t guid = item->guid ();

#ifdef KOKYU_DSRT_LOGGING
  ACE_hthread_t thr_handle = item->thread_handle ();

  int new_guid;
  ACE_OS::memcpy (&new_guid,
                  guid.get_buffer (),
                  guid.length ());
 
  ACE_DEBUG ((LM_DEBUG, 
              "(%t|%T) about to insert %d in sched queue and GUID is %d\n", 
              thr_handle,new_guid));
#endif
/*
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "<===Hash Table contents Begin===>\n"));
      dispatch_items_hash_map_.dump ();
      ACE_DEBUG ((LM_DEBUG,
                  "<===Hash Table contents End===>\n"));
#endif
*/
//  int return_val = dispatch_items_hash_map_.find (guid, rb_tree_node);
  if( this->find_i (guid, rb_tree_node) == -1)
    {
      if (dispatch_items_prio_queue_.bind (item_var,
                                           item_var,
                                           rb_tree_node) == 0)
        {
#ifdef KOKYU_DSRT_LOGGING
          ACE_DEBUG ((LM_DEBUG, "(%t|%T): item bound in rbtree\n"));
#endif
/*          if (dispatch_items_hash_map_.bind (guid, rb_tree_node) == 0)
            {
#ifdef KOKYU_DSRT_LOGGING
              ACE_DEBUG ((LM_DEBUG, "(%t|%T): item bound in hashmap\n"));
              void * point = (void *) rb_tree_node;
	      ACE_DEBUG((LM_DEBUG,"INSERT POINT IS %x\n",point));
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents Begin===>\n"));
              dispatch_items_hash_map_.dump ();
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents End=====>\n"));
#endif              
              return 0;
            }
*/
	return 0;
        }
    }
  else
    {
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t|%T) %d found in hashmap\n", thr_handle));
#endif

/*      int i=dispatch_items_hash_map_.unbind (guid);
      ACE_DEBUG((LM_DEBUG,"UNBIND SUCCESS?%d\n",i));
         DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>
            tmp_var = rb_tree_node->item ();

          int int_guid;
          ACE_OS::memcpy (&int_guid,
                  tmp_var->guid ().get_buffer (),
                  tmp_var->guid ().length ());
      void * point1 = (void *) rb_tree_node;
      ACE_DEBUG((LM_DEBUG,"RB_TREE_POINT AFTER HASH_MAP UNBIND IS %x\n",point1));
          ACE_DEBUG ((LM_DEBUG, "(%t|%T):guid %d, thr_handle = %d\n",
                      int_guid, tmp_var->thread_handle ()));

  if (dispatch_items_prio_queue_.current_size ())
    {
      PRIO_QUEUE_ITERATOR end_iter = dispatch_items_prio_queue_.end ();
      PRIO_QUEUE_ITERATOR iter;

      iter = dispatch_items_prio_queue_.begin ();
      while( iter != end_iter )
        {
          PRIO_QUEUE_ENTRY &ent = (*iter);
          DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>
            item_var = ent.item ();
         
          DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>
            key_var = ent.key ();
 
          int guid;
          ACE_OS::memcpy (&guid,
                  item_var->guid ().get_buffer (),
                  item_var->guid ().length ());
          void * point = (void *) (&(*iter));
          ACE_DEBUG ((LM_DEBUG, "(%t|%T): ITEM guid %d, thr_handle = %d, ADDRESS IS %x\n",
                      guid, item_var->thread_handle (), point));
        
	  ACE_OS::memcpy (&guid,
                  item_var->guid ().get_buffer (),
                  item_var->guid ().length ());
          ACE_DEBUG ((LM_DEBUG, "(%t|%T): KEY guid %d, thr_handle = %d\n",
                      guid, key_var->thread_handle ()));
 
          ++iter;
        }
    }
*/
      dispatch_items_prio_queue_.unbind (rb_tree_node);

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t|%T) %d removed from hashmap and rbtree\n", thr_handle));      
#endif

      if (dispatch_items_prio_queue_.bind (item_var,
                                           item_var,
                                           rb_tree_node) == 0)
        {
#ifdef KOKYU_DSRT_LOGGING
          void * point = (void *) rb_tree_node;
          ACE_DEBUG ((LM_DEBUG, 
                      "(%t|%T) %d bound to rbtree and NEW ADDRESS IS %x\n", thr_handle, point));      
#endif
/*          if (dispatch_items_hash_map_.bind (guid, rb_tree_node) == 0)
            {
#ifdef KOKYU_DSRT_LOGGING
              ACE_DEBUG ((LM_DEBUG, 
                          "(%t|%T) %d bound to hashmap\n", thr_handle));      
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents Begin===>\n"));
              dispatch_items_hash_map_.dump ();
              ACE_DEBUG ((LM_DEBUG,
                          "<===Hash Table contents End===>\n"));
#endif              
              return 0;
            }
*/
	return 0;
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
  RB_Tree_Dispatch_Item_Node* rb_tree_node;

//  if (dispatch_items_hash_map_.find(guid, rb_tree_node) == 0)
  if(this->find_i (guid,rb_tree_node) == 0)
    {
//      dispatch_items_hash_map_.unbind (guid);
      dispatch_items_prio_queue_.unbind (rb_tree_node);
/*
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "<===Hash Table contents Begin===>\n"));
      dispatch_items_hash_map_.dump ();
      ACE_DEBUG ((LM_DEBUG,
                  "<===Hash Table contents End===>\n"));
#endif
*/      
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
  if (dispatch_items_prio_queue_.current_size ())
    {
      PRIO_QUEUE_ITERATOR end_iter = dispatch_items_prio_queue_.end ();
      PRIO_QUEUE_ITERATOR iter;

      iter = dispatch_items_prio_queue_.begin ();
      while( iter != end_iter )
        {
          PRIO_QUEUE_ENTRY &ent = (*iter);
          DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>
            item_var = ent.item ();
          /*          
          int guid;
          ACE_OS::memcpy (&guid,
                  item_var->guid ().get_buffer (),
                  item_var->guid ().length ());

          ACE_DEBUG ((LM_DEBUG, "(%t|%T):guid %d, thr_handle = %d\n", 
                      guid, item_var->thread_handle ()));
          */
          ++iter;
        }
    }
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):##########################\n"));
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                       More_Eligible_Comparator,
                       ACE_LOCK>::
find_i (Guid_t guid, RB_Tree_Dispatch_Item_Node* &RB_node )
{
  if (dispatch_items_prio_queue_.current_size ())
    {
      PRIO_QUEUE_ITERATOR end_iter = dispatch_items_prio_queue_.end ();
      PRIO_QUEUE_ITERATOR iter;

      iter = dispatch_items_prio_queue_.begin ();
      while( iter != end_iter )
        {
          PRIO_QUEUE_ENTRY &ent = (*iter);
          DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>
            item_var = ent.item ();
          if (item_var->guid ()==guid)
          {
		RB_node = &(*iter);
		return(0);
	  }
          ++iter;
        }
    }
    return(-1);
}

template <class DSRT_Scheduler_Traits,
          class More_Eligible_Comparator,
          class ACE_LOCK>
int Sched_Ready_Queue<DSRT_Scheduler_Traits,
                       More_Eligible_Comparator,
                       ACE_LOCK>::
change_prio(int old_prio, int new_prio, int policy)
{
  if (dispatch_items_prio_queue_.current_size ())
    {
      PRIO_QUEUE_ITERATOR end_iter = dispatch_items_prio_queue_.end ();
      PRIO_QUEUE_ITERATOR iter;
      int prio;

      iter = dispatch_items_prio_queue_.begin ();
      while( iter != end_iter )
        {
          PRIO_QUEUE_ENTRY &ent = (*iter);
          DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>
            item_var = ent.item ();
          ACE_OS::thr_getprio (item_var->thread_handle (), prio);
          if (prio==old_prio) {
	    ACE_OS::thr_setprio(item_var->thread_handle (), new_prio, policy);  
          }
          ++iter;
        }
    }
    return(0);
}

}

#endif /* DSRT_SCHED_QUEUE_T_CPP */
