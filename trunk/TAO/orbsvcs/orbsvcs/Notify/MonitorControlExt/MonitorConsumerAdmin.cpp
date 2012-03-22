// $Id$

#include "orbsvcs/Notify/MonitorControlExt/MonitorConsumerAdmin.h"

#include "ace/Monitor_Size.h"
#include "ace/Monitor_Base.h"

#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"
#include "orbsvcs/Notify/MonitorControl/Control_Registry.h"
#include "orbsvcs/Notify/MonitorControl/Control.h"
#include "orbsvcs/Notify/Buffering_Strategy.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class ConsumerAdminControl : public TAO_NS_Control
{
public:
  ConsumerAdminControl (TAO_MonitorEventChannel* ec,
                        const ACE_CString& name,
                        CosNotifyChannelAdmin::AdminID id)
    : TAO_NS_Control (name.c_str ()),
      ec_ (ec),
      id_ (id)
  {
  }

  virtual bool execute (const char* command)
  {
    if (ACE_OS::strcmp (command, TAO_NS_CONTROL_REMOVE_CONSUMERADMIN) == 0)
      {
        CosNotifyChannelAdmin::ConsumerAdmin_var admin =
          this->ec_->get_consumeradmin (this->id_);

        if (!CORBA::is_nil (admin.in ()))
          {
            admin->destroy ();
          }
      }
    else
      {
        return false;
      }

    return true;
  }

private:
  TAO_MonitorEventChannel* ec_;
  CosNotifyChannelAdmin::AdminID id_;
};

TAO_MonitorConsumerAdmin::TAO_MonitorConsumerAdmin (void)
  : stat_name_ (),
    control_name_ (),
    queue_size_ (0),
    overflows_ (0)
{
}

TAO_MonitorConsumerAdmin::~TAO_MonitorConsumerAdmin (void)
{
  this->remove ();
  // The registry also manages this refcount. The pointer itself
  // should never be 0 since we throw an exception if anything
  // fails in allocation or registration.
  this->queue_size_->remove_ref ();
}

void
TAO_MonitorConsumerAdmin::register_stats_controls (
  TAO_MonitorEventChannel* mec,
  const ACE_CString& base)
{
  // Set up the statistic name, create it and register it
  this->stat_name_ = base + "/";
  this->queue_size_stat_name_ =  stat_name_ +
    NotifyMonitoringExt::EventChannelQueueSize;
  ACE_NEW_THROW_EX (this->queue_size_,
                    Monitor_Base (this->queue_size_stat_name_.c_str (),
                                  Monitor_Control_Types::MC_NUMBER),
                    CORBA::NO_MEMORY ());

  if (!mec->register_statistic (this->queue_size_stat_name_, this->queue_size_))
    {
      // The constructor sets the refcount to 1 so this call will
      // delete the pointer.
      this->queue_size_->remove_ref ();
      this->queue_size_ = 0;
      throw NotifyMonitoringExt::NameAlreadyUsed ();
    }

  this->overflow_stat_name_ = stat_name_ +
    NotifyMonitoringExt::EventChannelQueueOverflows;
  ACE_NEW_THROW_EX (this->overflows_,
                    Monitor_Base (this->overflow_stat_name_.c_str (),
                                  Monitor_Control_Types::MC_COUNTER),
                    CORBA::NO_MEMORY ());
  if (!mec->register_statistic (this->overflow_stat_name_, this->overflows_))
    {
      delete this->overflows_;
      this->overflows_ = 0;
      throw NotifyMonitoringExt::NameAlreadyUsed ();
    }

  this->control_name_ = base;
  ConsumerAdminControl* control = 0;
  ACE_NEW_THROW_EX (control,
                    ConsumerAdminControl (mec,
                                          this->control_name_.c_str (),
                                          this->id ()),
                    CORBA::NO_MEMORY ());
  TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();

  if (!cinstance->add (control))
    {
      delete control;
      ACE_ERROR ((LM_ERROR,
                  "Unable to add control: %s\n",
                  this->control_name_.c_str ()));
    }
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_MonitorConsumerAdmin::obtain_named_notification_push_supplier (
  CosNotifyChannelAdmin::ClientType ctype,
  CosNotifyChannelAdmin::ProxyID_out proxy_id,
  const char * name)
{
  // First, make sure we can get down to the real ec type.
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());

  if (ec == 0)
    {
      throw CORBA::INTERNAL ();
    }

  // Next, create the push consumer proxy.
  CosNotifyChannelAdmin::ProxySupplier_var proxy =
    this->TAO_Notify_ConsumerAdmin::obtain_notification_push_supplier (
      ctype, proxy_id);

  // Next, map the proxy id to the name in the ec.
  ec->map_supplier_proxy (proxy_id, name);

  return proxy._retn ();
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_MonitorConsumerAdmin::obtain_notification_push_supplier (
  CosNotifyChannelAdmin::ClientType ctype,
  CosNotifyChannelAdmin::ProxyID_out proxy_id)
{
  // First, make sure we can get down to the real ec type.
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());

  if (ec == 0)
    {
      throw CORBA::INTERNAL ();
    }

  // Next, create the push consumer proxy.
  CosNotifyChannelAdmin::ProxySupplier_var proxy =
    this->TAO_Notify_ConsumerAdmin::obtain_notification_push_supplier (
      ctype, proxy_id);

  // Next, map the proxy id to the name in the ec.
  char name[64];
  ACE_OS::sprintf (name, "%d", proxy_id);
  ec->map_supplier_proxy (proxy_id, name);

  return proxy._retn ();
}

void
TAO_MonitorConsumerAdmin::update_queue_count (size_t count)
{
  // NOTE: Formerly this code multiplied the count by an arbitrary and meaningless
  // constant, and only updated the statistic when the count was non-zero.
  this->queue_size_->receive (count);

  if (this->child_ != 0)
    {
      this->child_->update_queue_count (count);
    }
}


void
TAO_MonitorConsumerAdmin::count_queue_overflow (
  bool local_overflow,
  bool global_overflow)
{
  // note that someday we may wish to distinguish between
  // local and global overflows
  this->overflows_->receive ((size_t)1);

  if (this->child_ != 0)
    {
      this->child_->count_queue_overflow (local_overflow, global_overflow);
    }
}

TAO_MonitorEventChannel *
TAO_MonitorConsumerAdmin::get_ec (void) const
{
  TAO_MonitorEventChannel* ec = dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());
  if (ec == 0)
    throw CORBA::INTERNAL ();
  return ec;
}

const ACE_CString &
TAO_MonitorConsumerAdmin::stat_name (void)const
{
  return stat_name_;
}


void
TAO_MonitorConsumerAdmin::destroy (void)
{
  this->remove ();
  this->TAO_Notify_ConsumerAdmin::destroy ();
}

void
TAO_MonitorConsumerAdmin::remove (void)
{
  // First, make sure we can get down to the real ec type
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());
  if (ec != 0)
    {
      ec->unregister_statistic (this->queue_size_stat_name_);
      ec->unregister_statistic (this->overflow_stat_name_);
      ec->unregister_statistic (this->stat_name_);
      ec->remove_consumeradmin (this->id ());
      TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();
      cinstance->remove (this->control_name_);
    }

  // We don't own queue_size_, so we must not delete it
}



TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

