//$Id$

#ifndef CACHED_CONNECT_STRATEGY_T_C
#define CACHED_CONNECT_STRATEGY_T_C

#define ACE_BUILD_DLL

#include "ace/Cached_Connect_Strategy_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE.h"
#include "ace/Service_Repository.h"
#include "ace/Synch.h"
#include "ace/Service_Types.h"
#include "ace/Thread_Manager.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Pair_T.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Cached_Connect_Strategy_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, Cached_Connect_Strategy_T, "$Id$")

#define ACE_T1 class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class ATTRIBUTES, class MUTEX
#define ACE_T2 SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, ATTRIBUTES, MUTEX

template <ACE_T1>
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::ACE_Cached_Connect_Strategy_Ex
(CACHING_STRATEGY &caching_s,
 ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
 ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
 ACE_Recycling_Strategy<SVC_HANDLER> *rec_s,
 MUTEX *lock,
 int delete_lock)
  : CCSBASE (cre_s, con_s, rec_s, lock, delete_lock),
    connection_cache_ (caching_s)
{
  if (this->open (cre_s, con_s, rec_s) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Cached_Connect_Strategy_Ex<ACE_T2>\n")));
}

template <ACE_T1>
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::~ACE_Cached_Connect_Strategy_Ex (void)
{
#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
  // Close down all cached service handlers.
  for (ACE_TYPENAME CONNECTION_CACHE::ITERATOR iter = this->connection_cache_.begin ();
       iter != this->connection_cache_.end ();
       ++iter)
    {
      if ((*iter).second () != 0)
        {
          (*iter).second ()->recycler (0, 0);
          (*iter).second ()->close ();
        }
    }
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */
}

template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::check_hint_i
(SVC_HANDLER *&sh,
 const ACE_PEER_CONNECTOR_ADDR &remote_addr,
 ACE_Time_Value *timeout,
 const ACE_PEER_CONNECTOR_ADDR &local_addr,
 int reuse_addr,
 int flags,
 int perms,
 ACE_Hash_Map_Entry<ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>, ACE_Pair<SVC_HANDLER *, ATTRIBUTES> > *&entry,
 int &found)
{
  ACE_UNUSED_ARG (remote_addr);
  ACE_UNUSED_ARG (timeout);
  ACE_UNUSED_ARG (local_addr);
  ACE_UNUSED_ARG (reuse_addr);
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (perms);

  found = 0;

  // Get the recycling act for the svc_handler
  CONNECTION_CACHE_ENTRY *possible_entry = (CONNECTION_CACHE_ENTRY *) sh->recycling_act ();

  // Check to see if the hint svc_handler has been closed down
  if (possible_entry->ext_id_.recycle_state () == ACE_RECYCLABLE_CLOSED)
    {
      // If close, decrement refcount
      if (possible_entry->ext_id_.decrement () == 0)
        {
          // If refcount goes to zero, close down the svc_handler
          possible_entry->int_id_.first ()->recycler (0, 0);
          possible_entry->int_id_.first ()->close ();
          this->purge_i (possible_entry);
        }

      // Hint not successful
      found = 0;

      // Reset hint
      sh = 0;
    }

  // If hint is not closed, see if it is connected to the correct
  // address and is recyclable
  else if ((possible_entry->ext_id_.recycle_state () == ACE_RECYCLABLE_IDLE_AND_PURGABLE ||
            possible_entry->ext_id_.recycle_state () == ACE_RECYCLABLE_IDLE_BUT_NOT_PURGABLE) &&
           possible_entry->ext_id_.subject () == remote_addr)
    {
      // Hint successful
      found = 1;

      // Tell the <svc_handler> that it should prepare itself for
      // being recycled.
      this->prepare_for_recycling (sh);

      //
      // Update the caching attributes directly since we don't do a
      // find() on the cache map.
      //

      // Indicates successful find.
      int find_result = 0;

      int result = this->caching_strategy ().notify_find (find_result,
                                                          possible_entry->int_id_.second ());

      if (result == -1)
        return result;
    }
  else
    {
      // This hint will not be used.
      possible_entry->ext_id_.decrement ();

      // Hint not successful
      found = 0;

      // If <sh> is not connected to the correct address or is busy,
      // we will not use it.
      sh = 0;
    }

  if (found)
    entry = possible_entry;

  return 0;
}

template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::find_or_create_svc_handler_i
(SVC_HANDLER *&sh,
 const ACE_PEER_CONNECTOR_ADDR &remote_addr,
 ACE_Time_Value *timeout,
 const ACE_PEER_CONNECTOR_ADDR &local_addr,
 int reuse_addr,
 int flags,
 int perms,
 ACE_Hash_Map_Entry<ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>, ACE_Pair<SVC_HANDLER *, ATTRIBUTES> > *&entry,
 int &found)
{
  REFCOUNTED_HASH_RECYCLABLE_ADDRESS search_addr (remote_addr);

  // Try to find the address in the cache.  Only if we don't find it
  // do we create a new <SVC_HANDLER> and connect it with the server.
  if (this->find (search_addr, entry) == -1)
    {
      // Set the flag
      found = 0;

      // Create a new svc_handler
      if (this->make_svc_handler (sh) == -1)
        return -1;

      // Connect using the svc_handler.
      if (this->cached_connect (sh,
                                remote_addr,
                                timeout,
                                local_addr,
                                reuse_addr,
                                flags,
                                perms) == -1)
        {
          return -1;
        }
      else
        {
          // Insert the new SVC_HANDLER instance into the cache.
          if (this->connection_cache_.bind (search_addr,
                                            sh,
                                            entry) == -1)
            return -1;

          // Set the recycler and the recycling act
          this->assign_recycler (sh, this, entry);
        }
    }
  else // We found a cached svc_handler.
    {
      // Set the flag
      found = 1;

      // Get the cached <svc_handler>
      sh = entry->int_id_.first ();

      // Tell the <svc_handler> that it should prepare itself for
      // being recycled.
      this->prepare_for_recycling (sh);
    }

  return 0;

}

template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::cached_connect (SVC_HANDLER *&sh,
                                                        const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                                        ACE_Time_Value *timeout,
                                                        const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                                        int reuse_addr,
                                                        int flags,
                                                        int perms)
{
  // Actively establish the connection.  This is a timed blocking
  // connect.
  if (this->CONNECT_STRATEGY::connect_svc_handler (sh,
                                                   remote_addr,
                                                   timeout,
                                                   local_addr,
                                                   reuse_addr,
                                                   flags,
                                                   perms) == -1)
    {
      // If connect() failed because of timeouts, we have to reject
      // the connection entirely. This is necessary since currently
      // there is no way for the non-blocking connects to complete and
      // for the <Connector> to notify the cache of the completion of
      // connect().

      if (errno == EWOULDBLOCK)
        errno = ENOTSUP;
      else if (ACE::out_of_handles (errno))
        {
          // If the connect failed due to the process running out of
          // file descriptors then, auto_purging of some connections
          // are done from the CONNECTION_CACHE. This frees the
          // descriptors which get used in the connect process and
          // hence the same method is called again!
          if (this->purge_connections () == -1)
            return -1;

          // Try connecting again.
          if (this->CONNECT_STRATEGY::connect_svc_handler (sh,
                                                           remote_addr,
                                                           timeout,
                                                           local_addr,
                                                           reuse_addr,
                                                           flags,
                                                           perms) == -1)
            {
              if (errno == EWOULDBLOCK)
                errno = ENOTSUP;
              return -1;
            }
        }
      else
        {
          return -1;
        }
    }

  return 0;

}


template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::connect_svc_handler_i
(SVC_HANDLER *&sh,
 const ACE_PEER_CONNECTOR_ADDR &remote_addr,
 ACE_Time_Value *timeout,
 const ACE_PEER_CONNECTOR_ADDR &local_addr,
 int reuse_addr,
 int flags,
 int perms,
 int& found)
{
  CONNECTION_CACHE_ENTRY *entry = 0;

  // Check if the user passed a hint svc_handler
  if (sh != 0)
    {

      int result = this->check_hint_i (sh,
                                       remote_addr,
                                       timeout,
                                       local_addr,
                                       reuse_addr,
                                       flags,
                                       perms,
                                       entry,
                                       found);
      if (result != 0)
        return result;
    }

  // If not found
  if (!found)
    {
      int result = this->find_or_create_svc_handler_i (sh,
                                                       remote_addr,
                                                       timeout,
                                                       local_addr,
                                                       reuse_addr,
                                                       flags,
                                                       perms,
                                                       entry,
                                                       found);

      if (result != 0)
        return result;

    }

  // For all successful cases: mark the <svc_handler> in the cache
  // as being <in_use>.  Therefore recyclable is BUSY.
  entry->ext_id_.recycle_state (ACE_RECYCLABLE_BUSY);

  // And increment the refcount
  entry->ext_id_.increment ();

  return 0;
}


template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::cache_i (const void *recycling_act)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  // Mark the <svc_handler> in the cache as not being <in_use>.
  // Therefore recyclable is IDLE.
  entry->ext_id_.recycle_state (ACE_RECYCLABLE_IDLE_AND_PURGABLE);

  return 0;
}

template<ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::recycle_state_i (const void *recycling_act,
                                                         ACE_Recyclable_State new_state)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  // Mark the <svc_handler> in the cache as not being <in_use>.
  // Therefore recyclable is IDLE.
  entry->ext_id_.recycle_state (new_state);

  return 0;
}

template<ACE_T1> ACE_Recyclable_State
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::recycle_state_i (const void *recycling_act) const
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  // Mark the <svc_handler> in the cache as not being <in_use>.
  // Therefore recyclable is IDLE.
  return entry->ext_id_.recycle_state ();
}

template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::purge_i (const void *recycling_act)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  return this->connection_cache_.unbind (entry);
}


template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::mark_as_closed_i (const void *recycling_act)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  // Mark the <svc_handler> in the cache as CLOSED.
  entry->ext_id_.recycle_state (ACE_RECYCLABLE_CLOSED);

  return 0;
}

template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::cleanup_hint_i (const void *recycling_act)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  // Decrement the refcount on the <svc_handler>.
  int refcount = entry->ext_id_.decrement ();

  // If the svc_handler state is closed and the refcount == 0, call
  // close() on svc_handler.
  if (entry->ext_id_.recycle_state () == ACE_RECYCLABLE_CLOSED &&
      refcount == 0)
    {
      entry->int_id_.first ()->recycler (0, 0);
      entry->int_id_.first ()->close ();
      this->purge_i (entry);
    }

  return 0;
}

template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::purge_connections (void)
{
  return this->connection_cache_.purge ();
}

template <ACE_T1> CACHING_STRATEGY &
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::caching_strategy (void)
{
  return this->connection_cache_.caching_strategy ();
}

template <ACE_T1> int
ACE_Cached_Connect_Strategy_Ex<ACE_T2>::find (ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR> &search_addr,
                                              ACE_Hash_Map_Entry<ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>, ACE_Pair<SVC_HANDLER *, ATTRIBUTES> > *&entry)
{
  typedef ACE_Hash_Map_Bucket_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDRESS,
                                       ACE_Pair<SVC_HANDLER *, ATTRIBUTES>,
                                       ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>,
                                       ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>,
                                       ACE_Null_Mutex>
    CONNECTION_CACHE_BUCKET_ITERATOR;

  CONNECTION_CACHE_BUCKET_ITERATOR iterator (this->connection_cache_.map (),
                                             search_addr);

  CONNECTION_CACHE_BUCKET_ITERATOR end (this->connection_cache_.map (),
                                        search_addr,
                                        1);

  for (;
       iterator != end;
       ++iterator)
    {
      REFCOUNTED_HASH_RECYCLABLE_ADDRESS &addr = (*iterator).ext_id_;

      if (addr.recycle_state () != ACE_RECYCLABLE_IDLE_AND_PURGABLE &&
          addr.recycle_state () != ACE_RECYCLABLE_IDLE_BUT_NOT_PURGABLE)
        continue;

      if (addr.subject () != search_addr.subject ())
        continue;

      entry = &(*iterator);

      //
      // Update the caching attributes directly since we don't do a
      // find() on the cache map.
      //

      // Indicates successful find.
      int find_result = 0;

      int result = this->caching_strategy ().notify_find (find_result,
                                                          entry->int_id_.second ());

      if (result == -1)
        return result;

      return 0;
    }

  return -1;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Cached_Connect_Strategy_Ex)

#undef ACE_T1
#undef ACE_T2

#endif /* CACHED_CONNECT_STRATEGY_T_C */
