// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorSupplierAdmin.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"
#include "orbsvcs/Notify/MonitorControl/Control_Registry.h"
#include "orbsvcs/Notify/MonitorControl/Control.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class SupplierAdminControl: public TAO_NS_Control
{
public:
  SupplierAdminControl (TAO_MonitorEventChannel* ec,
                        const ACE_CString& name,
                        CosNotifyChannelAdmin::AdminID id)
  : TAO_NS_Control (name.c_str ()),
    ec_ (ec),
    id_ (id) {
  }

  virtual bool execute (const char* command) {
    if (ACE_OS::strcmp (command, TAO_NS_CONTROL_REMOVE_SUPPLIERADMIN) == 0)
      {
        CosNotifyChannelAdmin::SupplierAdmin_var admin =
          this->ec_->get_supplieradmin (this->id_);
        if (!CORBA::is_nil (admin.in ()))
          admin->destroy ();
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

TAO_MonitorSupplierAdmin::TAO_MonitorSupplierAdmin (void)
{
}

TAO_MonitorSupplierAdmin::~TAO_MonitorSupplierAdmin (void)
{
  // First, make sure we can get down to the real ec type
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());
  if (ec != 0)
    {
      ec->remove_supplieradmin (this->id ());
      TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();
      cinstance->remove (this->control_name_);
    }
}

void
TAO_MonitorSupplierAdmin::register_stats_controls (
                                          TAO_MonitorEventChannel* mec,
                                          const ACE_CString& base)
{
  this->control_name_ = base;
  SupplierAdminControl* control = 0;
  ACE_NEW_THROW_EX (control,
                    SupplierAdminControl (mec,
                                          this->control_name_.c_str (),
                                          this->id ()),
                    CORBA::NO_MEMORY ());
  TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();
  if (!cinstance->add (control))
    {
      delete control;
      ORBSVCS_ERROR ((LM_ERROR, "Unable to add control: %s\n",
                  this->control_name_.c_str ()));
    }
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_MonitorSupplierAdmin::obtain_named_notification_push_consumer (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id,
        const char * name)
{
  // First, make sure we can get down to the real ec type
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());
  if (ec == 0)
    throw CORBA::INTERNAL ();

  // Next, create the push consumer proxy
  CosNotifyChannelAdmin::ProxyConsumer_var proxy =
    this->TAO_Notify_SupplierAdmin::obtain_notification_push_consumer (
      ctype, proxy_id);

  // Next, map the proxy id to the name in the ec
  ec->map_consumer_proxy (proxy_id, name);

  return proxy._retn ();
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_MonitorSupplierAdmin::obtain_notification_push_consumer (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id)
{
  // First, make sure we can get down to the real ec type
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());
  if (ec == 0)
    throw CORBA::INTERNAL ();

  // Next, create the push consumer proxy
  CosNotifyChannelAdmin::ProxyConsumer_var proxy =
    this->TAO_Notify_SupplierAdmin::obtain_notification_push_consumer (
      ctype, proxy_id);

  // Next, map the proxy id to the name in the ec
  char name[64];
  ACE_OS::sprintf(name, "%d", proxy_id);
  ec->map_consumer_proxy (proxy_id, name);

  return proxy._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

