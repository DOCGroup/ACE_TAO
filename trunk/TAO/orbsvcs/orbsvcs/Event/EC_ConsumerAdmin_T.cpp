// $Id$

#ifndef TAO_EC_CONSUMERADMIN_T_CPP
#define TAO_EC_CONSUMERADMIN_T_CPP

#include "EC_ConsumerAdmin_T.h"
#include "EC_Command.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ConsumerAdmin_T, "$Id$")

template<ACE_SYNCH_DECL>int
TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE>::busy (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->busy_i ();
}

template<ACE_SYNCH_DECL> int
TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE>::idle (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->idle_i ();
}

template<ACE_SYNCH_DECL> int
TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE>::busy_i (void)
{
  if (this->busy_count_ >= this->busy_hwm ())
    {
      this->reached_hwm_ = 1;
      while (this->reached_hwm_ != 0)
        this->busy_cond_.wait ();
    }
  this->busy_count_++;
  return 0;
}

template<ACE_SYNCH_DECL> int
TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE>::idle_i (void)
{
  this->busy_count_--;
  if (this->busy_count_ == 0)
    {
      this->reached_hwm_ = 0;
      this->execute_delayed_operations ();
      this->busy_cond_.broadcast ();
    }
  return 0;
}

// ****************************************************************

template<ACE_SYNCH_DECL> void
TAO_EC_ConsumerAdmin_Immediate<ACE_SYNCH_USE>::connected (
      TAO_EC_ProxyPushSupplier* supplier,
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  supplier->_incr_refcnt ();
  this->connected_i (supplier, ACE_TRY_ENV);
}

template<ACE_SYNCH_DECL> void
TAO_EC_ConsumerAdmin_Immediate<ACE_SYNCH_USE>::disconnected (
      TAO_EC_ProxyPushSupplier* supplier,
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  this->disconnected_i (supplier, ACE_TRY_ENV);
}

// ****************************************************************

template<ACE_SYNCH_DECL> void
TAO_EC_ConsumerAdmin_Delayed<ACE_SYNCH_USE>::connected (
      TAO_EC_ProxyPushSupplier* supplier,
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  if (this->busy_count () == 0)
    {
      // We can add the object immediately
      this->connected_i (supplier, ACE_TRY_ENV);
    }
  else
    {
      supplier->_incr_refcnt ();
      ACE_Command_Base* command;
      ACE_NEW (command,
               TAO_EC_ConsumerAdmin::Connected_Command (this,
                                                        supplier));
               
      ACE_DEBUG ((LM_DEBUG,
                  "EC (%P|%t) Delayed connection command = %x\n",
                  command));

      this->command_queue_.enqueue_tail (command);
    }
}

template<ACE_SYNCH_DECL> void
TAO_EC_ConsumerAdmin_Delayed<ACE_SYNCH_USE>::disconnected (
      TAO_EC_ProxyPushSupplier* supplier,
      CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  if (this->busy_count () == 0)
    {
      // We can remove the object immediately
      this->disconnected_i (supplier, ACE_TRY_ENV);
    }
  else
    {
      ACE_Command_Base* command;
      ACE_NEW (command,
               TAO_EC_ConsumerAdmin::Disconnected_Command (this,
                                                           supplier));
      ACE_DEBUG ((LM_DEBUG,
                  "EC (%P|%t) Delayed disconnection command = %x\n",
                  command));
               
      this->command_queue_.enqueue_tail (command);
    }
}

template<ACE_SYNCH_DECL> void
TAO_EC_ConsumerAdmin_Delayed<ACE_SYNCH_USE>::execute_delayed_operations (void)
{
  // LOCKING: the lock is taken by the idle() function
  while (!this->command_queue_.is_empty ())
    {
      ACE_Command_Base* command;
      this->command_queue_.dequeue_head (command);

      command->execute ();

      ACE_DEBUG ((LM_DEBUG,
                  "EC (%P|%t) Executed delayed command = %x\n",
                  command));

      delete command;
    }
}

#endif /* TAO_EC_CONSUMERADMIN_T_CPP */
