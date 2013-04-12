// $Id$

#ifndef MONITORPROXYSUPPLIER_T_CPP
#define MONITORPROXYSUPPLIER_T_CPP

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorProxySupplier_T.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorConsumerAdmin.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"
#include "orbsvcs/Notify/ThreadPool_Task.h"
#include "ace/Monitor_Base.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename ProxyPushSupplier>
TAO_MonitorProxySupplier_T<ProxyPushSupplier>::TAO_MonitorProxySupplier_T (void)
  : queue_item_stat_name_ ()
  , overflow_stat_name_ ()
  , queue_item_count_ (0)
  , overflows_ (0)
  , event_channel_ (0)
  , admin_ (0)
{
}

template <typename ProxyPushSupplier>
TAO_MonitorProxySupplier_T<ProxyPushSupplier>::~TAO_MonitorProxySupplier_T (void)
{
  if (this->admin_ != 0)
    {
      this->admin_->unregister_child (this);
    }
  if (this->event_channel_ != 0)
    {
      this->event_channel_->unregister_statistic (this->queue_item_stat_name_);
    }
}

template <typename ProxyPushSupplier>
void
TAO_MonitorProxySupplier_T<ProxyPushSupplier>::update_queue_count (size_t count)
{
  if (this->queue_item_count_ != 0)
    {
      // get a count of events that are being held at the consumer level.
      size_t pending = 0;
      if (TAO_Notify_ProxySupplier::consumer() != 0)
        {
          pending = this->TAO_Notify_ProxySupplier::consumer()->pending_count();
        }
      this->queue_item_count_->receive (count + pending);
    }
  if (this->child_ != 0)
    {
      this->child_->update_queue_count (count);
    }
}

template <typename ProxyPushSupplier>
void
TAO_MonitorProxySupplier_T<ProxyPushSupplier>::count_queue_overflow (
  bool local_overflow,
  bool global_overflow)
{
  // note that someday we may wish to distinguish between
  // local and global overflows
  if (this->overflows_ != 0)
    {
      this->overflows_->receive ((size_t)1);
    }
  if (this->child_ != 0)
    {
      this->child_->count_queue_overflow (local_overflow, global_overflow);
    }
}

template <typename ProxyPushSupplier>
void
TAO_MonitorProxySupplier_T<ProxyPushSupplier>::configure(
  TAO_Notify_ConsumerAdmin & admin,
  CosNotifyChannelAdmin::ProxyID_out proxy_id)
{
  admin_ = dynamic_cast<TAO_MonitorConsumerAdmin *> (& admin);
  if (admin_ == 0)
    {
      throw CORBA::INTERNAL();
    }
  event_channel_ = admin_->get_ec ();

  TAO_Notify_Buffering_Strategy* bs = 0;
  TAO_Notify_Worker_Task * worker_task =  this->TAO_Notify_Object::get_worker_task ();
  TAO_Notify_ThreadPool_Task* tpt =
    dynamic_cast<TAO_Notify_ThreadPool_Task*> (worker_task);
  if (tpt != 0)
    {
      bs = tpt->buffering_strategy ();
    }
  if (bs != 0)
    {
      bs->set_tracker (admin_);
    }
  else
    {
      ORBSVCS_DEBUG ((LM_DEBUG, "Warning MC cannot enable statistics for buffering strategy.\n"));
    }

  this->base_stat_name_ = admin_->stat_name ();
  char name[64];
  ACE_OS::sprintf(name, "%d", proxy_id);
  this->base_stat_name_ += name;
  this->base_stat_name_ += "/";

  this->queue_item_stat_name_ = base_stat_name_;
  this->queue_item_stat_name_ += NotifyMonitoringExt::EventChannelQueueElementCount;
  ACE_NEW_THROW_EX (this->queue_item_count_,
                    Monitor_Base (this->queue_item_stat_name_.c_str (),
                                  Monitor_Control_Types::MC_NUMBER),
                    CORBA::NO_MEMORY ());
  if (!event_channel_->register_statistic (this->queue_item_stat_name_, this->queue_item_count_))
    {
      delete this->queue_item_count_;
      this->queue_item_count_ = 0;
      throw NotifyMonitoringExt::NameAlreadyUsed ();
    }

  this->overflow_stat_name_ = base_stat_name_;
  this->overflow_stat_name_ += NotifyMonitoringExt::EventChannelQueueOverflows;
  ACE_NEW_THROW_EX (this->overflows_,
                    Monitor_Base (this->overflow_stat_name_.c_str (),
                                  Monitor_Control_Types::MC_COUNTER),
                    CORBA::NO_MEMORY ());
  if (!event_channel_->register_statistic (this->overflow_stat_name_, this->overflows_))
    {
      delete this->overflows_;
      this->overflows_ = 0;
      throw NotifyMonitoringExt::NameAlreadyUsed ();
    }

  admin_->register_child (this);
}


template <typename ProxyPushSupplier>
ACE_CString &
TAO_MonitorProxySupplier_T<ProxyPushSupplier>::queue_item_stat_name(void)
{
  return this->queue_item_stat_name_;
}


template <typename ProxyPushSupplier>
ACE_CString &
TAO_MonitorProxySupplier_T<ProxyPushSupplier>::overflow_stat_name(void)
{
  return this->overflow_stat_name_;
}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_HAS_MONITOR_FRAMEWORK == 1

#endif // MONITORPROXYSUPPLIER_T_CPP
