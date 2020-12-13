#include "MC_Default_Factory.h"
#include "MonitorEventChannel.h"
#include "MonitorEventChannelFactory.h"
#include "MonitorSupplierAdmin.h"
#include "MonitorConsumerAdmin.h"
#include "MonitorProxyPushSupplier.h"
#include "MonitorStructuredProxyPushSupplier.h"
#include "MonitorSequenceProxyPushSupplier.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_MC_Default_Factory::create (TAO_Notify_EventChannelFactory*& factory,
                                const char* name)
{
  ACE_NEW_THROW_EX (factory,
                    TAO_MonitorEventChannelFactory (name),
                    CORBA::NO_MEMORY ());
}

void
TAO_MC_Default_Factory::create (TAO_Notify_EventChannel*& ec,
                                const char* name)
{
  ACE_NEW_THROW_EX (ec,
                    TAO_MonitorEventChannel (name),
                    CORBA::NO_MEMORY ());
}

void
TAO_MC_Default_Factory::create (TAO_Notify_SupplierAdmin*& admin)
{
  ACE_NEW_THROW_EX (admin,
                    TAO_MonitorSupplierAdmin,
                    CORBA::NO_MEMORY ());
}

void
TAO_MC_Default_Factory::create (TAO_Notify_ConsumerAdmin*& admin)
{
  ACE_NEW_THROW_EX (admin,
                    TAO_MonitorConsumerAdmin,
                    CORBA::NO_MEMORY ());
}

void
TAO_MC_Default_Factory::create (TAO_Notify_ProxyPushSupplier*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_MonitorProxyPushSupplier,
                    CORBA::NO_MEMORY ());
}

void
TAO_MC_Default_Factory::create (TAO_Notify_StructuredProxyPushSupplier*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_MonitorStructuredProxyPushSupplier,
                    CORBA::NO_MEMORY ());
}

void
TAO_MC_Default_Factory::create (TAO_Notify_SequenceProxyPushSupplier*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_MonitorSequenceProxyPushSupplier,
                    CORBA::NO_MEMORY ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

