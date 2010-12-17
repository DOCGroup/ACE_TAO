// $Id$

#include "orbsvcs/Notify/Default_Factory.h"
#include "orbsvcs/Notify/EventChannelFactory.h"
#include "orbsvcs/Notify/EventChannel.h"
#include "orbsvcs/Notify/Admin.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/SupplierAdmin.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"
#include "orbsvcs/Notify/Structured/StructuredProxyPushConsumer.h"
#include "orbsvcs/Notify/Structured/StructuredProxyPushSupplier.h"
#include "orbsvcs/Notify/Any/ProxyPushConsumer.h"
#include "orbsvcs/Notify/Any/ProxyPushSupplier.h"
#include "orbsvcs/Notify/Any/CosEC_ProxyPushConsumer.h"
#include "orbsvcs/Notify/Any/CosEC_ProxyPushSupplier.h"
#include "orbsvcs/Notify/Sequence/SequenceProxyPushConsumer.h"
#include "orbsvcs/Notify/Sequence/SequenceProxyPushSupplier.h"
#include "orbsvcs/Notify/Supplier.h"

#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Copy_On_Write.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class PROXY>
class COW_Collection_Default_Factory
{
public:
  typedef typename TAO_ESF_Proxy_List<PROXY>::Iterator PROXY_ITER;
  typedef TAO_ESF_Copy_On_Write<PROXY, TAO_ESF_Proxy_List<PROXY>,PROXY_ITER, ACE_SYNCH> COLLECTION;
  typedef TAO_ESF_Proxy_Collection<PROXY> BASE_COLLECTION;

  void create (BASE_COLLECTION* &collection)
  {
    ACE_NEW_THROW_EX (collection,
                      COLLECTION (),
                      CORBA::INTERNAL ());
  }
};

TAO_Notify_Default_Factory::TAO_Notify_Default_Factory (void)
{
}

TAO_Notify_Default_Factory::~TAO_Notify_Default_Factory ()
{
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_ProxySupplier_Collection* &collection)
{
  COW_Collection_Default_Factory<TAO_Notify_ProxySupplier> f;
  f.create (collection);
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_ProxyConsumer_Collection* &collection)
{
  COW_Collection_Default_Factory<TAO_Notify_ProxyConsumer> f;
  f.create (collection);
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_EventChannel_Collection* &collection)
{
  COW_Collection_Default_Factory<TAO_Notify_EventChannel> f;
  f.create (collection);
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_ConsumerAdmin_Collection* &collection)
{
  COW_Collection_Default_Factory<TAO_Notify_ConsumerAdmin> f;
  f.create (collection);
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_SupplierAdmin_Collection* &collection)
{
  COW_Collection_Default_Factory<TAO_Notify_SupplierAdmin> f;
  f.create (collection);
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_Proxy_Collection* &collection)
{
  COW_Collection_Default_Factory<TAO_Notify_Proxy> f;
  f.create (collection);
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_EventChannelFactory*& factory,
                                    const char*)
{
  ACE_NEW_THROW_EX (factory,
                    TAO_Notify_EventChannelFactory (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_EventChannel*& channel,
                                    const char*)
{
  ACE_NEW_THROW_EX (channel,
                    TAO_Notify_EventChannel (),
                    CORBA::NO_MEMORY ());
}


void
TAO_Notify_Default_Factory::create (TAO_Notify_SupplierAdmin*& admin)
{
  ACE_NEW_THROW_EX (admin,
                    TAO_Notify_SupplierAdmin (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_ConsumerAdmin*& admin)
{
  ACE_NEW_THROW_EX (admin,
                    TAO_Notify_ConsumerAdmin (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_StructuredProxyPushConsumer*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_StructuredProxyPushConsumer (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_StructuredProxyPushSupplier*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_StructuredProxyPushSupplier (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_ProxyPushConsumer*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_ProxyPushConsumer (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_ProxyPushSupplier*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_ProxyPushSupplier (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_CosEC_ProxyPushConsumer*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_CosEC_ProxyPushConsumer (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_CosEC_ProxyPushSupplier*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_CosEC_ProxyPushSupplier (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_SequenceProxyPushConsumer*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_SequenceProxyPushConsumer (),
                    CORBA::NO_MEMORY ());
}

void
TAO_Notify_Default_Factory::create (TAO_Notify_SequenceProxyPushSupplier*& proxy)
{
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_SequenceProxyPushSupplier (),
                    CORBA::NO_MEMORY ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_Notify_Serv, TAO_Notify_Default_Factory)

