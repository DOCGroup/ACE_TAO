 /**
  * @file PIP_DA_Strategy_Adapter.h
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  * This file contains the specification for a class
  * that adapts a deadlock avoidance strategy to additionally
  * support priority inheritance protocol annotations
*/


#ifndef PIP_DA_STRATEGY_ADAPTER
#define PIP_DA_STRATEGY_ADAPTER

#include "ace/DA_Strategy_Base.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Unbounded_Set.h"
#include "ace/Mutex.h"
#include "ace/Null_Mutex.h"

/**
 * @class ACE_PIP_DA_Strategy_Adapter
 * @brief Extends deadlock avoidance strategies
 *        to support priority inheritance annotations
 *
 * Deadlock avoidance strategies associate a resource cost annotation
 * with each handle. This class extends the strategies to support
 * the association of annotations with each priority at which the 
 * handle can be dispatched, i.e. the priority at which the corresponding
 * thread resource can dispatch the handle
*/
template <typename Handle_Id, typename Lock>
class ACE_PIP_DA_Strategy_Adapter
{
  public:

   /// Constructor that takes the deadlock avoidance strategy that
   /// the Strategy Adapter adapts.
   ACE_PIP_DA_Strategy_Adapter(DA_Strategy_Base<ACE_UINT64>* DA_strategy);
   ~ACE_PIP_DA_Strategy_Adapter();

   /// Indicates whether allocating a thread to the handle
   /// at the specified priority could potentially result in deadlock.
   int is_deadlock_potential(Handle_Id handle, ACE_UINT32 priority);

   /// Grant the handle a thread at the specified priority.
   void grant(Handle_Id handle, ACE_UINT32 priority);

   /// Release the thread
   void release(Handle_Id handle, ACE_UINT32 priority);

   /// Determine the number of threads being managed by 
   /// the DA_Strategy adapter.
   int get_max_threads();

   /// Add an annotation value for the handle / priority pair.
   int add_annotation (Handle_Id handle, ACE_UINT32 priority, int annotation);

   /// Remove every annotation associated with this handle.
   int remove_annotation (Handle_Id handle);
   int remove_annotation (Handle_Id handle, ACE_UINT32 priority);

private:

   /// Associates each message handler with an internally generated id
   /// which can be used, along with a priority, to lookup an annotation.
   typedef ACE_Hash_Map_Manager_Ex<Handle_Id, 
     ACE_UINT32,
     ACE_Hash<Handle_Id>,
     ACE_Equal_To<Handle_Id>,
     ACE_Null_Mutex> HANDLE_ID_MAP;

   /// Associates each message handler with a set of potential priorities.
   /// Message handler represented by internally generated id.
   typedef ACE_Hash_Map_Manager_Ex<ACE_UINT32,
     ACE_Unbounded_Set<ACE_UINT32>*,
     ACE_Hash<ACE_UINT32>,
     ACE_Equal_To<ACE_UINT32>,
     ACE_Null_Mutex> HANDLE_ID_PRIORITY_MAP;

   /// Determines an id that uniquely identifies a handler/priority pair.
   ACE_UINT64 hash_handle_id_and_priority(ACE_UINT32 handle_id,
                                          ACE_UINT32 priority) const;

   /// Generates an annotation ID given the actual handle and priority.
   ACE_UINT64 get_annotation_id(Handle_Id handle, ACE_UINT32 priority);

   DA_Strategy_Base<ACE_UINT64>* DA_strategy_;
   HANDLE_ID_MAP                 handle_ids_;
   HANDLE_ID_PRIORITY_MAP        id_to_priority_map_;
   Lock                          lock_;
   ACE_UINT32                    next_id_;
};

template <typename Handle_Id, typename Lock>
ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   ACE_PIP_DA_Strategy_Adapter(DA_Strategy_Base<ACE_UINT64>* DA_strategy)
: DA_strategy_(DA_strategy)
, next_id_(0)
{   
}

template <typename Handle_Id, typename Lock>
ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::~ACE_PIP_DA_Strategy_Adapter()
{
   HANDLE_ID_PRIORITY_MAP::iterator it = this->id_to_priority_map_.begin();
   for (; it != this->id_to_priority_map_.end(); ++it)
   {
      delete it->item();
   }
}

template <typename Handle_Id, typename Lock>
ACE_INLINE int ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::get_max_threads()
{
   return this->DA_strategy_->get_max_threads();
}

template <typename Handle_Id, typename Lock>
ACE_INLINE ACE_UINT64 ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   hash_handle_id_and_priority(ACE_UINT32 handle_id, ACE_UINT32 priority) const
{
   ACE_UINT64 result = handle_id;
   result = (result << 32) | priority;
   return result;
}

template <typename Handle_Id, typename Lock>
int ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   is_deadlock_potential(Handle_Id handle, ACE_UINT32 priority)
{
   ACE_Guard<Lock> guard(this->lock_);
   ACE_UINT64 annotation_id = get_annotation_id(handle, priority);
   return this->DA_strategy_->is_deadlock_potential(annotation_id);
}

template <typename Handle_Id, typename Lock>
void ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   grant(Handle_Id handle, ACE_UINT32 priority)
{
   ACE_Guard<Lock> guard(this->lock_);
   ACE_UINT64 annotation_id = get_annotation_id(handle, priority);
   return this->DA_strategy_->grant(annotation_id);
}

template <typename Handle_Id, typename Lock>
void ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   release(Handle_Id handle, ACE_UINT32 priority)
{
   ACE_Guard<Lock> guard(this->lock_);
   ACE_UINT64 annotation_id = get_annotation_id(handle, priority);
   this->DA_strategy_->release(annotation_id);  
}

template <typename Handle_Id, typename Lock>
int ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   add_annotation (Handle_Id handle, ACE_UINT32 priority, int annotation)
{
   ACE_UINT32                     internal_handle_id(0);
   ACE_Unbounded_Set<ACE_UINT32>* priorities(0);

   ACE_Guard<Lock> guard(this->lock_);
   if (this->handle_ids_.find(handle, internal_handle_id) == -1)
   {
      // This is the first time handle has been encountered, so generate an 
      // internal handle id.
      internal_handle_id = this->next_id_++;
      this->handle_ids_.bind(handle, internal_handle_id);
      priorities = new ACE_Unbounded_Set<ACE_UINT32>;
      this->id_to_priority_map_.bind(internal_handle_id, priorities);
   }
   else 
   {
      this->id_to_priority_map_.find(internal_handle_id, priorities);
   }

   priorities->insert(priority);

   return this->DA_strategy_->add_annotation(
      hash_handle_id_and_priority(internal_handle_id, priority), annotation);
}

template <typename Handle_Id, typename Lock>
int ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   remove_annotation (Handle_Id handle)
{
   ACE_Guard<Lock> guard(this->lock_);
   ACE_UINT32 internal_handle_id(0);
   if (this->handle_ids_.unbind(handle, internal_handle_id) != -1)
   {
      ACE_Unbounded_Set<ACE_UINT32>* priorities(0);
      if (this->id_to_priority_map_.unbind(internal_handle_id, priorities) != -1)
      {
         for (ACE_Unbounded_Set<ACE_UINT32>::ITERATOR it = priorities->begin();
              it != priorities->end();
              ++it)
         {
            this->DA_strategy_->remove_annotation(
               get_annotation_id(internal_handle_id, *it));
         }

         delete priorities;
      }
   }

   return 0;
}

template <typename Handle_Id, typename Lock>
int ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   remove_annotation (Handle_Id handle, ACE_UINT32 priority)
{
   ACE_Guard<Lock> guard(this->lock_);
   ACE_UINT32 internal_handle_id(0);
   int result(0);
   if (this->handle_ids_.find(handle, internal_handle_id) != -1)
   {
      ACE_Unbounded_Set<ACE_UINT32>* priorities(0);
      if (this->id_to_priority_map_.find(internal_handle_id, priorities) != -1)
      {
         if (priorities->remove(priority) != -1)
         {         
            result = this->DA_strategy_->remove_annotation(
                        get_annotation_id(internal_handle_id, priority));
         }
         if (priorities->is_empty())
         {
            // This was the last annotation for this handle,
            // so remove the handle information
            this->id_to_priority_map_.unbind(internal_handle_id, priorities);
            delete priorities;
            this->handle_ids_.unbind(handle, internal_handle_id);
         }
      }
   }

   return result;
}

template <typename Handle_Id, typename Lock>
ACE_UINT64 ACE_PIP_DA_Strategy_Adapter<Handle_Id, Lock>::
   get_annotation_id(Handle_Id handle, ACE_UINT32 priority)
{
   ACE_UINT64 annotation_id(0);
   ACE_UINT32 handle_id(0);

   if (this->handle_ids_.find(handle, handle_id) != -1)
   {
      annotation_id = hash_handle_id_and_priority(handle_id, priority);
   }

   return annotation_id;
}

#endif

