// $Id$

#include "orbsvcs/Notify/Any/PushSupplier.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_PushSupplier::TAO_Notify_PushSupplier (TAO_Notify_ProxyConsumer* proxy)
  :TAO_Notify_Supplier (proxy)
{
}

TAO_Notify_PushSupplier::~TAO_Notify_PushSupplier ()
{
}

void
TAO_Notify_PushSupplier::init (CosEventComm::PushSupplier_ptr push_supplier)
{
  // TODO: verify single init call
  // push_supplier is optional
  this->push_supplier_ = CosEventComm::PushSupplier::_duplicate (push_supplier);

  try
    {
      this->subscribe_ = CosNotifyComm::NotifySubscribe::_narrow (push_supplier);
    }
  catch (const CORBA::Exception&)
    {
      // _narrow failed which probably means the interface is CosEventComm type.
    }
}

void
TAO_Notify_PushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}

ACE_CString
TAO_Notify_PushSupplier::get_ior (void) const
{
  ACE_CString result;
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
  try
  {
    CORBA::String_var ior = orb->object_to_string(this->push_supplier_.in());
    result = static_cast<const char*> (ior.in ());
  }
  catch (const CORBA::Exception&)
  {
    result.fast_clear();
  }
  return result;
}

CORBA::Object_ptr
TAO_Notify_PushSupplier::get_supplier (void)
{
  return CosEventComm::PushSupplier::_duplicate (this->push_supplier_.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
