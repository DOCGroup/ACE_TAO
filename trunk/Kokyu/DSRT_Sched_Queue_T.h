/* -*- C++ -*- */
/**
 *  @file  DSRT_Sched_Queue_T.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 */

#ifndef DSRT_SCHED_QUEUE_T_H
#define DSRT_SCHED_QUEUE_T_H
#include "ace/pre.h"

#include "DSRT_Dispatch_Item_T.h"
#include "ace/RB_Tree.h"
#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Kokyu_dsrt.h"

namespace Kokyu
{

  /**
   * @class Sched_Ready_Queue
   *
   * @brief RB_Tree based template class for implementation of
   * reordering queue.
   *
   * This queue is used as a priority queue to store schedulable
   * entities. The item at the top of the RB_Tree is the most eligible
   * item. The comparator used to determine the most eligible item is
   * passed as a template parameter <code> More_Eligible_Comparator
   * </code>. This is expected to be a functor which compares two
   * schedulable items. The mutex type template parameter for RB_Tree
   * is chosen to be a null mutex since all the methods in the
   * enclosing <code> Sched_Ready_Queue </code> class are thread
   * safe. Since QoS is used for comparison between two schedulable
   * items, QoSDescriptor is the ideal candidate to be used as the key
   * or the EXT_ID for RB_Tree instantiation. But two qos descriptors
   * could be the same. The existing implementation of RB_Tree does
   * not allow duplicate keys. In order to facilitate insertion of
   * duplicate qos descriptors, the qos descriptors are contained in a
   * <code> DSRT_Dispatch_Item </code> and this is used as the basis
   * of comparison. To resolve tie between equal qos values, an
   * insertion time stamp is maintained in each item and an item with
   * an earlier time stamp is more eligible than an item with an
   * identical qos value. Another requirement is that it should be
   * possible to remove an item from the RB_Tree based on guid. Since
   * we have already used up the qos descriptor for the key, we need a
   * separate index into the list of schedulable items. The second
   * index should be based on guid. This is achieved by using a hash
   * map to store <guid, RB_Tree_Node*> pairs. This makes the deletion
   * of nodes from RB_Tree more efficient.
   *
   */
  template <class DSRT_Scheduler_Traits,
            class More_Eligible_Comparator,
            class ACE_LOCK>
  class Sched_Ready_Queue
  {
    /// Extract the necessary types from the traits class
    typedef typename DSRT_Scheduler_Traits::Guid_t Guid_t;

    typedef typename
    DSRT_Scheduler_Traits::QoSDescriptor_t DSRT_QoSDescriptor_t;

  public:

    /**
     * Given a guid, find an item in the priority queue.
     *
     * @param guid Guid of item
     *
     * @param found_item Reference to DSRT_Dispatch_Item_var
     *                   to hold the found item.
     * @return  -1 if no item found and 0 otherwise.
     */
    int find(Guid_t guid,
             DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>&
             found_item);


    /**
     * Insert an item in the priority queue. If item with same guid is
     * already in the queue, the existing one is deleted and the new
     * one inserted. A deletion and insertion has to happen instead of
     * update since the rebalancing of the RB_Tree should take place.
     *
     * @param guid Guid of item.
     *
     * @param qos QoS associated with item.
     *
     * @return -1 if insertion failed and 0 otherwise.
     */
    int insert(Guid_t guid, const DSRT_QoSDescriptor_t& qos);

    /**
     * Remove an item from the priority queue.
     *
     * @param guid Guid of item.
     *
     * @param qos QoS associated with item.
     *
     * @return -1 if removal failed and 0 otherwise.
     */
    int remove(Guid_t guid);

    /**
     * Returns current size of the priority queue.
     */
    int current_size ();

    /**
     * Get the most eligible item from the priority queue.
     *
     * @param item Item which is most eligible, i.e. one at the
     *             "top" of the priority queue.
     *
     * @return -1 if there are no items in the priority queue.
     */
    int most_eligible (DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>&
                       item);

    void dump();

  private:

    /**
     * @class Guid_Hash
     *
     * @brief Internal class to generate hash for guid.
     *
     * This acts just as a wrapper functor to the Hash functor passed
     * as part of the traits class <code> DSRT_Scheduler_Traits
     * </code>.
     *
     */
    class Guid_Hash
    {
    public:
      /// Returns hash value.
      u_long operator () (const Guid_t &id);
    };

    // RB_Tree related typedefs
    typedef ACE_RB_Tree <DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>,
                         DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>,
                         More_Eligible_Comparator,
                         ACE_SYNCH_NULL_MUTEX> Dispatch_Items_Priority_Queue;


    typedef
    ACE_RB_Tree_Node<DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>,
                     DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits> >
    RB_Tree_Dispatch_Item_Node;

    typedef typename
    Dispatch_Items_Priority_Queue::ITERATOR PRIO_QUEUE_ITERATOR;

    typedef typename
    Dispatch_Items_Priority_Queue::ENTRY PRIO_QUEUE_ENTRY;

    // Hash map related typedefs
    typedef ACE_Hash_Map_Manager_Ex<Guid_t,
                                    RB_Tree_Dispatch_Item_Node*,
                                    Guid_Hash,
                                    ACE_Equal_To<Guid_t>,
                                    ACE_SYNCH_NULL_MUTEX>
    Dispatch_Items_Hash_Map;

    typedef ACE_Hash_Map_Iterator_Ex<Guid_t,
                                     RB_Tree_Dispatch_Item_Node*,
                                     Guid_Hash,
                                     ACE_Equal_To<Guid_t>,
                                     ACE_SYNCH_NULL_MUTEX>
    Dispatch_Items_Hash_Map_Iterator;

    typedef ACE_Hash_Map_Entry <Guid_t,
                                RB_Tree_Dispatch_Item_Node*>
    Dispatch_Items_Hash_Map_Entry;

    /**
     * Lock used to protect the state of the scheduler queue. A
     * separate lock is not used for the internal RB_Tree and hashmap.
     */
    ACE_LOCK lock_;

    /**
     * Hash table to maintain a second index into the list of
     * schedulable items. This is for efficient removal of items from
     * the RB_Tree based on guid. The guid is used as the key for the
     * hash map, whereas the qos value is used as the key for the
     * RB_Tree.
     */
    Dispatch_Items_Hash_Map dispatch_items_hash_map_;

    /**
     * RB_Tree implementation of priority queue of schedulable items.
     */
    Dispatch_Items_Priority_Queue dispatch_items_prio_queue_;
  };
}

#if !defined (__ACE_INLINE__)
//#include "DSRT_Sched_Queue_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DSRT_Sched_Queue_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DSRT_Sched_Queue_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* DSRT_SCHED_QUEUE_T_H */
