//$Id:

#ifndef CACHED_CONNECT_STRATEGY_T_C
#define CACHED_CONNECT_STRATEGY_T_C

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE.h"
#include "Cached_Connect_Strategy_T.h"
#include "ace/Service_Repository.h"
#include "ace/Synch.h"
#include "ace/Service_Types.h"
#include "ace/Thread_Manager.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Pair_T.h"
#include "Options.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "Cached_Connect_Strategy_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(Web_Crawler, Cached_Connect_Strategy_T, "$Id$")

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX>
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::ACE_Cached_Connect_Strategy_Ex
  (ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
   ACE_Recycling_Strategy<SVC_HANDLER> *rec_s)
    : creation_strategy_ (0),
      delete_creation_strategy_ (0),
      concurrency_strategy_ (0),
      delete_concurrency_strategy_ (0),
      recycling_strategy_ (0),
      delete_recycling_strategy_ (0)
{

  ACE_DEBUG ((LM_DEBUG, "ACE_Cached_Connect_Strategy_Ex::ACE_Cached_Connect_Strategy_Ex\n"));

  if (this->open (cre_s, con_s, rec_s) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Cached_Connect_Strategy_Ex::ACE_Cached_Connect_Strategy_Ex")));
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX>
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::~ACE_Cached_Connect_Strategy_Ex (void)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Cached_Connect_Strategy_Ex::~ACE_Cached_Connect_Strategy_Ex\n"));
  if (this->delete_creation_strategy_)
    delete this->creation_strategy_;
  this->delete_creation_strategy_ = 0;
  this->creation_strategy_ = 0;

  if (this->delete_concurrency_strategy_)
    delete this->concurrency_strategy_;
  this->delete_concurrency_strategy_ = 0;
  this->concurrency_strategy_ = 0;

  if (this->delete_recycling_strategy_)
    delete this->recycling_strategy_;
  this->delete_recycling_strategy_ = 0;
  this->recycling_strategy_ = 0;

  // Close down all cached service handlers.
  for (CONNECTION_CACHE_ITERATOR iter = this->connection_cache_.begin ();
       iter != this->connection_cache_.end ();
       ++iter)
    {
      if ((*iter).second () != 0)
       {
         (*iter).second ()->recycler (0, 0);
         (*iter).second ()->close ();
       }
    }
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::open
  (ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
   ACE_Recycling_Strategy<SVC_HANDLER> *rec_s)
{
  // Initialize the creation strategy.
  // First we decide if we need to clean up.
  if (this->creation_strategy_ != 0 &&
      this->delete_creation_strategy_ != 0 &&
      cre_s != 0)
    {
      delete this->creation_strategy_;
      this->creation_strategy_ = 0;
      this->delete_creation_strategy_ = 0;
    }

  if (cre_s != 0)
    this->creation_strategy_ = cre_s;
  else if (this->creation_strategy_ == 0)
    {
      ACE_NEW_RETURN (this->creation_strategy_,
                      CREATION_STRATEGY, -1);
      this->delete_creation_strategy_ = 1;
    }

  // Initialize the concurrency strategy.

  if (this->concurrency_strategy_ != 0 &&
      this->delete_concurrency_strategy_ != 0 &&
      con_s != 0)
    {
      delete this->concurrency_strategy_;
      this->concurrency_strategy_ = 0;
      this->delete_concurrency_strategy_ = 0;
    }

  if (con_s != 0)
    this->concurrency_strategy_ = con_s;
  else if (this->concurrency_strategy_ == 0)
    {
      ACE_NEW_RETURN (this->concurrency_strategy_,
                      CONCURRENCY_STRATEGY, -1);
      this->delete_concurrency_strategy_ = 1;
    }

  // Initialize the recycling strategy.

  if (this->recycling_strategy_ != 0 &&
      this->delete_recycling_strategy_ != 0 &&
      rec_s != 0)
    {
      delete this->recycling_strategy_;
      this->recycling_strategy_ = 0;
      this->delete_recycling_strategy_ = 0;
    }

  if (rec_s != 0)
    this->recycling_strategy_ = rec_s;
  else if (this->recycling_strategy_ == 0)
    {
      ACE_NEW_RETURN (this->recycling_strategy_,
                      RECYCLING_STRATEGY, -1);
      this->delete_recycling_strategy_ = 1;
    }

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::check_hint_i
  (SVC_HANDLER *&sh,
   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
   ACE_Time_Value *timeout,
   const ACE_PEER_CONNECTOR_ADDR &local_addr,
   int reuse_addr,
   int flags,
   int perms,
   ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::CONNECTION_CACHE_ENTRY *&entry,
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
  if (possible_entry->ext_id_.state () == ACE_Recyclable::CLOSED)
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
  else if (possible_entry->ext_id_ == remote_addr)
    {
      // Hint successful
      found = 1;

      // Tell the <svc_handler> that it should prepare itself for
      // being recycled.
      this->prepare_for_recycling (sh);
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

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::find_or_create_svc_handler_i
  (SVC_HANDLER *&sh,
   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
   ACE_Time_Value *timeout,
   const ACE_PEER_CONNECTOR_ADDR &local_addr,
   int reuse_addr,
   int flags,
   int perms,
   ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::CONNECTION_CACHE_ENTRY *&entry,
   int &found)
{

  ACE::set_handle_limit (OPTIONS::instance ()->handle_limit ());
  //ACE_DEBUG ((LM_DEBUG, "MAX handles %d \n", ACE::max_handles ()));
  // Explicit type conversion
  REFCOUNTED_HASH_RECYCLABLE_ADDRESS search_addr (remote_addr);

  // Try to find the address in the cache.  Only if we don't find it
  // do we create a new <SVC_HANDLER> and connect it with the server.
  if (this->connection_cache_.find (search_addr, entry) == -1)
    {
      // Set the flag
      found = 0;

      // Create a new svc_handler
      if (this->make_svc_handler (sh) == -1)
        return -1;

      if (this->connect (...) == -1)
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

connect ()
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
      else if (errno == EMFILE)
        {
          // @@purge here...

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
  else
    {
      return 0;
    }
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::connect_svc_handler
  (SVC_HANDLER *&sh,
   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
   ACE_Time_Value *timeout,
   const ACE_PEER_CONNECTOR_ADDR &local_addr,
   int reuse_addr,
   int flags,
   int perms)
{

  ACE_DEBUG ((LM_DEBUG, "ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::connect_svc_handler\n"));
  int found = 0;

  // This artificial scope is required since we need to let go of the
  // lock *before* registering the newly created handler with the
  // Reactor.
  {
    // Synchronization is required here as the setting of the
    // recyclable state must be done atomically with the finding and
    // binding of the service handler in the cache.
    ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

    int result = this->connect_svc_handler_i (sh,
                                              remote_addr,
                                              timeout,
                                              local_addr,
                                              reuse_addr,
                                              flags,
                                              perms,
                                              found);
    if (result != 0)
      return result;

  }

  // If it is a new connection, activate it.
  //
  // Note: This activation is outside the scope of the lock of the
  // cached connector.  This is necessary to avoid subtle deadlock
  // conditions with this lock and the Reactor lock.
  //
  // @@ If an error occurs on activation, we should try to remove this
  // entry from the internal table.

  if (!found)
    if (this->activate_svc_handler (sh))
      return -1;

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::connect_svc_handler
  (SVC_HANDLER *&sh,
   SVC_HANDLER *&sh_copy,
   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
   ACE_Time_Value *timeout,
   const ACE_PEER_CONNECTOR_ADDR &local_addr,
   int reuse_addr,
   int flags,
   int perms)
{
  int found = 0;

  // This artificial scope is required since we need to let go of the
  // lock *before* registering the newly created handler with the
  // Reactor.
  {
    // Synchronization is required here as the setting of the
    // recyclable state must be done atomically with the finding and
    // binding of the service handler in the cache.
    ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

    int result = this->connect_svc_handler_i (sh,
                                              remote_addr,
                                              timeout,
                                              local_addr,
                                              reuse_addr,
                                              flags,
                                              perms,
                                              found);
    sh_copy = sh;

    ACE_DEBUG ((LM_DEBUG,"after connect_svc_handler_i ACE_Cached_Connect_Strategy_Ex::connect_svc_handler sh_Copy result =%d\n", result));
    if (result != 0)
      return result;

  }

  // If it is a new connection, activate it.
  //
  // Note: This activation is outside the scope of the lock of the
  // cached connector.  This is necessary to avoid subtle deadlock
  // conditions with this lock and the Reactor lock.
  //
  // @@ If an error occurs on activation, we should try to remove this
  // entry from the internal table.

  if (!found)
    if (this->activate_svc_handler (sh))
      return -1;

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::connect_svc_handler_i
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
      // @@ Remove debugging
      ACE_DEBUG ((LM_DEBUG, "ACE_Cached_Connect_Strategy_Ex::connect_svc_handler_i CHECK_HINT_I result = %d\n", result));

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

      // @@ Purging here is not correct...

      if (result != 0)
        {
          // If the connect failed due to the process running out of
          // file descriptors then, auto_purging of some connections
          // are done from the CONNECTION_CACHE. This frees the
          // descriptors which get used in the connect process and
          // hence the same method is called again!
          if (errno == EMFILE)
            {
              int result = this->connection_cache_.purge ();
              if (result == -1)
                return result;

              if (this->find_or_create_svc_handler_i (sh,
                                                      remote_addr,
                                                      timeout,
                                                      local_addr,
                                                      reuse_addr,
                                                      flags,
                                                      perms,
                                                      entry,
                                                      found) == -1)
                return -1;
            }
          else
            return result;
        }
    }

  // For all successful cases: mark the <svc_handler> in the cache
  // as being <in_use>.  Therefore recyclable is BUSY.
  entry->ext_id_.state (ACE_Recyclable::BUSY);

  // And increment the refcount
  entry->ext_id_.increment ();

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::cache (const void *recycling_act)
{
  // Synchronization is required here as the setting of the recyclable
  // state must be done atomically with respect to other threads that
  // are querying the cache.
  ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

  ACE_DEBUG ((LM_DEBUG, "ACE_Cached_Connect_Strategy_Ex::cache\n"));
  return this->cache_i (recycling_act);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::cache_i (const void *recycling_act)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  // Mark the <svc_handler> in the cache as not being <in_use>.
  // Therefore recyclable is IDLE.

  entry->ext_id_.state (ACE_Recyclable::IDLE);

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::purge (const void *recycling_act)
{
  // Excluded other threads from changing cache while we take this
  // entry out.
  ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

  return this->purge_i (recycling_act);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::purge_i (const void *recycling_act)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  return this->connection_cache_.unbind (entry);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::mark_as_closed (const void *recycling_act)
{
  // Excluded other threads from changing cache while we take this
  // entry out.
  ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

  return this->mark_as_closed_i (recycling_act);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::mark_as_closed_i (const void *recycling_act)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  // Mark the <svc_handler> in the cache as CLOSED.
  entry->ext_id_.state (ACE_Recyclable::CLOSED);

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::cleanup_hint (const void *recycling_act)
{
  // Excluded other threads from changing cache while we take this
  // entry out.
  ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

  return this->cleanup_hint_i (recycling_act);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> int
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::cleanup_hint_i (const void *recycling_act)
{
  // The wonders and perils of ACT
  CONNECTION_CACHE_ENTRY *entry = (CONNECTION_CACHE_ENTRY *) recycling_act;

  // Decrement the refcount on the <svc_handler>.
  int refcount = entry->ext_id_.decrement ();

  // If the svc_handler state is closed and the refcount == 0, call
  // close() on svc_handler.
  if (entry->ext_id_.state () == ACE_Recyclable::CLOSED &&
      refcount == 0)
    {
      entry->int_id_.first ()->recycler (0, 0);
      entry->int_id_.first ()->close ();
      this->purge_i (entry);
    }

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> ACE_Creation_Strategy<SVC_HANDLER> *
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::creation_strategy (void) const
{
  return this->creation_strategy_;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> ACE_Recycling_Strategy<SVC_HANDLER> *
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::recycling_strategy (void) const
{
  return this->recycling_strategy_;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX> ACE_Concurrency_Strategy<SVC_HANDLER> *
ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::concurrency_strategy (void) const
{
  return this->concurrency_strategy_;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Cached_Connect_Strategy_Ex)

#endif /* CACHED_CONNECT_STRATEGY_T_C */
