// $Id$

#include "orbsvcs/Notify/MonitorControlExt/MonitorConsumerAdmin.h"

#include "ace/Monitor_Size.h"

#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"
#include "orbsvcs/Notify/MonitorControl/Control_Registry.h"
#include "orbsvcs/Notify/MonitorControl/Control.h"
#include "orbsvcs/Notify/Buffering_Strategy.h"
#include "orbsvcs/Notify/ThreadPool_Task.h"

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
    queue_size_ (0)
{
}

TAO_MonitorConsumerAdmin::~TAO_MonitorConsumerAdmin (void)
{
  // First, make sure we can get down to the real ec type.
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());
    
  if (ec != 0)
    {
      ec->unregister_statistic (this->stat_name_.c_str ());
      ec->remove_consumeradmin (this->id ());
      TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();
      cinstance->remove (this->control_name_);
    }

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
  this->stat_name_ += NotifyMonitoringExt::EventChannelQueueSize;
  
  ACE_NEW_THROW_EX (this->queue_size_,
                    Size_Monitor (this->stat_name_.c_str ()),
                    CORBA::NO_MEMORY ());
                    
  if (!mec->register_statistic (this->stat_name_, this->queue_size_))
    {
      // The constructor sets the refcount to 1 so this call will
      // delete the pointer.
      this->queue_size_->remove_ref ();
      this->queue_size_ = 0;
      throw NotifyMonitoringExt::NameAlreadyUsed ();
    }

  // If we've successfully registered the statistic, hook us into the
  // buffering strategy so it can let us know when the queue changes.
  TAO_Notify_ThreadPool_Task* tpt =
    dynamic_cast<TAO_Notify_ThreadPool_Task*> (this->get_worker_task ());
    
  if (tpt != 0)
    {
      TAO_Notify_Buffering_Strategy* bs = tpt->buffering_strategy ();
      bs->set_tracker (this);
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
  if (this->queue_size_ != 0)
    {
      // The message blocks stored in this queue are of size
      // zero.  However, each message block is a
      // TAO_Notify_Event which has an associated set of data
      // which can be used to estimate the amount of memory
      // allocated to the message queue
      this->queue_size_->receive (count * sizeof (TAO_Notify_Event));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

