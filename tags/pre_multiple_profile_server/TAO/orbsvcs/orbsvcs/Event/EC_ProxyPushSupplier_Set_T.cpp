// $Id$

#ifndef TAO_EC_PROXYPUSHSUPPLIER_SET_T_CPP
#define TAO_EC_PROXYPUSHSUPPLIER_SET_T_CPP

#include "EC_ProxyPushSupplier_Set_T.h"
#include "EC_Command.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ProxyPushSupplier_Set_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxyPushSupplier_Set_T, "$Id$")

template<class ACE_LOCK>int
TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_LOCK>::busy (void)
{
  return this->lock_.acquire_read ();
}

template<class ACE_LOCK> int
TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_LOCK>::idle (void)
{
  return this->lock_.release ();
}

template<class ACE_LOCK> void
TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_LOCK>::connected (
      TAO_EC_ProxyPushSupplier* supplier,
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  supplier->_incr_refcnt ();
  this->connected_i (supplier, ACE_TRY_ENV);
}

template<class ACE_LOCK> void
TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_LOCK>::disconnected (
      TAO_EC_ProxyPushSupplier* supplier,
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  this->disconnected_i (supplier, ACE_TRY_ENV);
}

template<class ACE_LOCK> void
TAO_EC_ProxyPushSupplier_Set_Immediate<ACE_LOCK>::shutdown (
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;
  this->shutdown_i (ACE_TRY_ENV);
}

// ****************************************************************

template<ACE_SYNCH_DECL>int
TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH_USE>::busy (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  while (this->busy_count_ >= this->busy_hwm ()
         || this->write_delay_ >= this->max_write_delay ())
    this->busy_cond_.wait ();
  this->busy_count_++;

  return 0;
}

template<ACE_SYNCH_DECL> int
TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH_USE>::idle (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  this->busy_count_--;
  if (this->busy_count_ == 0)
    {
      this->write_delay_ = 0;
      this->execute_delayed_operations ();
      this->busy_cond_.broadcast ();
    }
  return 0;
}

template<ACE_SYNCH_DECL> void
TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH_USE>::connected (
      TAO_EC_ProxyPushSupplier* supplier,
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  if (this->busy_count_ == 0)
    {
      // We can add the object immediately
      supplier->_incr_refcnt ();
      this->connected_i (supplier, ACE_TRY_ENV);
    }
  else
    {
      supplier->_incr_refcnt ();
      ACE_Command_Base* command;
      ACE_NEW (command,
               TAO_EC_ProxyPushSupplier_Set::Connected_Command (this,
                                                        supplier));
      this->command_queue_.enqueue_tail (command);
      this->write_delay_++;
    }
}

template<ACE_SYNCH_DECL> void
TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH_USE>::disconnected (
      TAO_EC_ProxyPushSupplier* supplier,
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  if (this->busy_count_ == 0)
    {
      // We can remove the object immediately
      this->disconnected_i (supplier, ACE_TRY_ENV);
    }
  else
    {
      ACE_Command_Base* command;
      ACE_NEW (command,
               TAO_EC_ProxyPushSupplier_Set::Disconnected_Command (this,
                                                           supplier));

      this->command_queue_.enqueue_tail (command);
      this->write_delay_++;
    }
}

template<ACE_SYNCH_DECL> void
TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH_USE>::shutdown (
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  if (this->busy_count_ == 0)
    {
      // We can remove the object immediately
      this->shutdown_i (ACE_TRY_ENV);
    }
  else
    {
      ACE_Command_Base* command;
      ACE_NEW (command,
               TAO_EC_ProxyPushSupplier_Set::Shutdown_Command (this));
      ACE_DEBUG ((LM_DEBUG,
                  "EC (%P|%t) Delayed shutdown command = %x\n",
                  command));

      this->command_queue_.enqueue_tail (command);
      this->write_delay_++;
    }
}

template<ACE_SYNCH_DECL> void
TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH_USE>::execute_delayed_operations (void)
{
  // LOCKING: the lock is taken by the idle() function
  while (!this->command_queue_.is_empty ())
    {
      ACE_Command_Base* command;
      this->command_queue_.dequeue_head (command);

      command->execute ();

      delete command;
    }
}

#endif /* TAO_EC_PROXYPUSHSUPPLIER_SET_T_CPP */
