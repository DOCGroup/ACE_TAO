// $Id$
#include "orbsvcs/Notify/MonitorControlExt/MC_Default_Factory.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannelFactory.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorSupplierAdmin.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorConsumerAdmin.h"

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


TAO_END_VERSIONED_NAMESPACE_DECL
