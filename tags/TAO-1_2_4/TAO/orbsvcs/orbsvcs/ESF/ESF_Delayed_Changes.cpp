// $Id$

#ifndef TAO_ESF_DELAYED_CHANGES_CPP
#define TAO_ESF_DELAYED_CHANGES_CPP

#include "ESF_Delayed_Changes.h"

#if ! defined (__ACE_INLINE__)
#include "ESF_Delayed_Changes.i"
#endif /* __ACE_INLINE__ */

#include "ESF_Defaults.h"
#include "ESF_Worker.h"
#include "ESF_Delayed_Command.h"
#include "ace/Functor.h"

ACE_RCSID(ESF, ESF_Delayed_Changes, "$Id$")

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    TAO_ESF_Delayed_Changes (void)
      :  lock_ (this),
         busy_cond_ (busy_lock_),
         busy_count_ (0),
         write_delay_count_ (0),
         busy_hwm_ (TAO_ESF_DEFAULT_BUSY_HWM),
         max_write_delay_ (TAO_ESF_DEFAULT_MAX_WRITE_DELAY)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    TAO_ESF_Delayed_Changes (const COLLECTION &collection)
      :  collection_ (collection),
         lock_ (this),
         busy_cond_ (busy_lock_),
         busy_count_ (0),
         write_delay_count_ (0),
         busy_hwm_ (TAO_ESF_DEFAULT_BUSY_HWM),
         max_write_delay_ (TAO_ESF_DEFAULT_MAX_WRITE_DELAY)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    for_each (TAO_ESF_Worker<PROXY> *worker
              ACE_ENV_ARG_DECL)
{
  ACE_GUARD (Busy_Lock, ace_mon, this->lock_);

  ITERATOR end = this->collection_.end ();
  for (ITERATOR i = this->collection_.begin (); i != end; ++i)
    {
      worker->work (*i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

  }
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> int
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
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
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
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
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
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
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    connected (PROXY *proxy
               ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      CORBA::INTERNAL ());
  ACE_CHECK;

  proxy->_incr_refcnt ();
  if (this->busy_count_ == 0)
    {
      // We can add the object immediately
      this->connected_i (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    reconnected (PROXY *proxy
                 ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      CORBA::INTERNAL ());
  ACE_CHECK;

  proxy->_incr_refcnt ();
  if (this->busy_count_ == 0)
    {
      // We can reconnect the object immediately
      this->reconnected_i (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    disconnected (PROXY *proxy
                  ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->busy_count_ == 0)
    {
      // We can remove the object immediately
      this->disconnected_i (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->busy_count_ == 0)
    {
      // We can shutdown the object immediately
      this->shutdown_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
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

#endif /* TAO_ESF_DELAYED_CHANGES_CPP */
