// $Id$

#ifndef TAO_EC_PROXY_COLLECTION_CPP
#define TAO_EC_PROXY_COLLECTION_CPP

#include "EC_Proxy_Collection.h"
#include "EC_Command.h"
#include "EC_Worker.h"
#include "EC_Defaults.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Proxy_Collection.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Proxy_Collection, "$Id$")

// ****************************************************************

template<class PROXY>
TAO_EC_Proxy_Collection<PROXY>::~TAO_EC_Proxy_Collection (void)
{
}

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
TAO_EC_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_EC_Immediate_Changes (void)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
TAO_EC_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_EC_Immediate_Changes (const COLLECTION &collection)
      :  collection_ (collection)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_EC_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    for_each (TAO_EC_Worker<PROXY> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  ITERATOR end = this->collection_.end ();
  for (ITERATOR i = this->collection_.begin (); i != end; ++i)
    {
      worker->work ((*i), ACE_TRY_ENV);
      ACE_CHECK;
    }
}

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    TAO_EC_Delayed_Changes (void)
      :  lock_ (this),
         busy_cond_ (busy_lock_),
         busy_count_ (0),
         write_delay_count_ (0),
         busy_hwm_ (TAO_EC_DEFAULT_BUSY_HWM),
         max_write_delay_ (TAO_EC_DEFAULT_MAX_WRITE_DELAY)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    TAO_EC_Delayed_Changes (const COLLECTION &collection)
      :  collection_ (collection),
         lock_ (this),
         busy_cond_ (busy_lock_),
         busy_count_ (0),
         write_delay_count_ (0),
         busy_hwm_ (TAO_EC_DEFAULT_BUSY_HWM),
         max_write_delay_ (TAO_EC_DEFAULT_MAX_WRITE_DELAY)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    for_each (TAO_EC_Worker<PROXY> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (Busy_Lock, ace_mon, this->lock_);

  ITERATOR end = this->collection_.end ();
  for (ITERATOR i = this->collection_.begin (); i != end; ++i)
    {
      worker->work (*i, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> int
TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    busy (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_, -1);

  while (this->busy_count_ >= this->busy_hwm_
         || this->write_delay_count_ >= this->max_write_delay_)
    this->busy_cond_.wait ();
  this->busy_count_++;

  return 0;
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> int
TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    idle (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_, -1);

  this->busy_count_--;
  if (this->busy_count_ == 0)
    {
      this->write_delay_count_ = 0;
      this->execute_delayed_operations ();
      this->busy_cond_.broadcast ();
    }
  return 0;
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> int
TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    execute_delayed_operations (void)
{
  while (!this->command_queue_.is_empty ())
    {
      ACE_Command_Base* command;
      this->command_queue_.dequeue_head (command);

      command->execute ();

      delete command;
    }
  return 0;
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    connected (PROXY *proxy,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  proxy->_incr_refcnt ();
  if (this->busy_count_ == 0)
    {
      // We can add the object immediately
      this->connected_i (proxy, ACE_TRY_ENV);
    }
  else
    {
      ACE_Command_Base* command;
      ACE_NEW (command,
               Connected_Command (this,
                                  proxy));
      this->command_queue_.enqueue_tail (command);
      this->write_delay_count_++;
    }
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    reconnected (PROXY *proxy,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  proxy->_incr_refcnt ();
  if (this->busy_count_ == 0)
    {
      // We can reconnect the object immediately
      this->reconnected_i (proxy, ACE_TRY_ENV);
    }
  else
    {
      ACE_Command_Base* command;
      ACE_NEW (command,
               Reconnected_Command (this,
                                    proxy));
      this->command_queue_.enqueue_tail (command);
      this->write_delay_count_++;
    }
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    disconnected (PROXY *proxy,
                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  if (this->busy_count_ == 0)
    {
      // We can remove the object immediately
      this->disconnected_i (proxy, ACE_TRY_ENV);
    }
  else
    {
      ACE_Command_Base* command;
      ACE_NEW (command,
               Disconnected_Command (this,
                                     proxy));
      this->command_queue_.enqueue_tail (command);
      this->write_delay_count_++;
    }
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  if (this->busy_count_ == 0)
    {
      // We can shutdown the object immediately
      this->shutdown_i (ACE_TRY_ENV);
    }
  else
    {
      ACE_Command_Base* command;
      ACE_NEW (command,
               Shutdown_Command (this));
      this->command_queue_.enqueue_tail (command);
      this->write_delay_count_++;
    }
}

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
TAO_EC_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_EC_Copy_On_Read (void)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
TAO_EC_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_EC_Copy_On_Read (const COLLECTION &collection)
      :  collection_ (collection)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_EC_Copy_On_Read<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    for_each (TAO_EC_Worker<PROXY> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Use an allocator for this memory...
  PROXY **proxies = 0;
  size_t size = 0;
  ACE_TRY
    {
      {
        ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

        size = this->collection_.size ();
        ACE_NEW (proxies, PROXY*[size]);
        PROXY **j = proxies;

        for (; j != proxies + size; ++j)
          *j = 0;

        j = proxies;
        ITERATOR end = this->collection_.end ();
        for (ITERATOR i = this->collection_.begin (); i != end; ++i)
          {
            *j = *i;
            (*j)->_incr_refcnt ();
            ++j;
          }
      }

      for (PROXY **j = proxies; j != proxies + size; ++j)
        {
          worker->work (*j, ACE_TRY_ENV);
          ACE_TRY_CHECK;
          (*j)->_decr_refcnt ();
        }
      delete[] proxies;
    }
  ACE_CATCHANY
    {
      for (PROXY **j = proxies; j != proxies + size; ++j)
        {
          if (*j != 0)
            (*j)->_decr_refcnt ();
        }
      delete[] proxies;

      ACE_RETHROW;
    }
  ACE_ENDTRY;
}

// ****************************************************************

#endif /* TAO_EC_PROXY_COLLECTION_CPP */
