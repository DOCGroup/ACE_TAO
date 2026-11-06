#ifndef TAO_ESF_DELAYED_CHANGES_CPP
#define TAO_ESF_DELAYED_CHANGES_CPP

#include "orbsvcs/ESF/ESF_Delayed_Changes.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Delayed_Changes.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/ESF/ESF_Defaults.h"
#include "orbsvcs/ESF/ESF_Worker.h"
#include "orbsvcs/ESF/ESF_Delayed_Command.h"
#include "tao/SystemException.h"

#include "ace/Functor.h"
#include "ace/Guard_T.h"
#include <new>
#include <utility>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  template <typename COMMAND, typename... ARGS>
  std::unique_ptr<ACE_Command_Base> make_esf_command (ARGS &&... args)
  {
    try
      {
        return std::make_unique<COMMAND> (std::forward<ARGS> (args)...);
      }
    catch (std::bad_alloc const &)
      {
        throw CORBA::NO_MEMORY ();
      }
  }
}

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    TAO_ESF_Delayed_Changes ()
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
    TAO_ESF_Delayed_Changes (COLLECTION const &collection)
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
    for_each (TAO_ESF_Worker<PROXY> *worker)
{
  ACE_GUARD (Busy_Lock, ace_mon, this->lock_);

  worker->set_size (this->collection_.size ());
  ITERATOR end = this->collection_.end ();
  for (ITERATOR i = this->collection_.begin (); i != end; ++i)
    {
      worker->work (*i);
  }
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> int
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    busy ()
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_, -1);

  while (this->busy_count_ >= this->busy_hwm_
         || this->write_delay_count_ >= this->max_write_delay_)
    this->busy_cond_.wait ();
  ++this->busy_count_;

  return 0;
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> int
TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    idle ()
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_, -1);

  --this->busy_count_;
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
    execute_delayed_operations ()
{
  while (!this->command_queue_.is_empty ())
    {
      ACE_Command_Base *command {};
      this->command_queue_.dequeue_head (command);
      std::unique_ptr<ACE_Command_Base> command_guard (command);
      command_guard->execute ();
    }
  return 0;
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    connected (PROXY *proxy)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      CORBA::INTERNAL ());

  if (this->busy_count_ == 0)
    {
      // We can add the object immediately
      proxy->_incr_refcnt ();
      this->connected_i (proxy);
    }
  else
    {
      std::unique_ptr<ACE_Command_Base> command =
        make_esf_command<Connected_Command> (this, proxy);
      proxy->_incr_refcnt ();
      if (this->command_queue_.enqueue_tail (command.get ()) == -1)
        {
          proxy->_decr_refcnt ();
          throw CORBA::NO_MEMORY ();
        }
      command.release ();
      this->write_delay_count_++;
    }
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    reconnected (PROXY *proxy)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      CORBA::INTERNAL ());

  if (this->busy_count_ == 0)
    {
      // We can reconnect the object immediately
      proxy->_incr_refcnt ();
      this->reconnected_i (proxy);
    }
  else
    {
      std::unique_ptr<ACE_Command_Base> command =
        make_esf_command<Reconnected_Command> (this, proxy);
      proxy->_incr_refcnt ();
      if (this->command_queue_.enqueue_tail (command.get ()) == -1)
        {
          proxy->_decr_refcnt ();
          throw CORBA::NO_MEMORY ();
        }
      command.release ();
      this->write_delay_count_++;
    }
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    disconnected (PROXY *proxy)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      CORBA::INTERNAL ());

  if (this->busy_count_ == 0)
    {
      // We can remove the object immediately
      this->disconnected_i (proxy);
    }
  else
    {
      std::unique_ptr<ACE_Command_Base> command =
        make_esf_command<Disconnected_Command> (this, proxy);
      if (this->command_queue_.enqueue_tail (command.get ()) == -1)
        {
          throw CORBA::NO_MEMORY ();
        }
      command.release ();
      this->write_delay_count_++;
    }
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::shutdown ()
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->busy_lock_,
      CORBA::INTERNAL ());

  if (this->busy_count_ == 0)
    {
      // We can shutdown the object immediately
      this->shutdown_i ();
    }
  else
    {
      std::unique_ptr<ACE_Command_Base> command =
        make_esf_command<Shutdown_Command> (this);
      if (this->command_queue_.enqueue_tail (command.get ()) == -1)
        {
          throw CORBA::NO_MEMORY ();
        }
      command.release ();
      this->write_delay_count_++;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_DELAYED_CHANGES_CPP */
