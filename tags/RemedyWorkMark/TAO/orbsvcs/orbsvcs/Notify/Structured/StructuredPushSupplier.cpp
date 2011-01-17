// $Id$

#include "orbsvcs/Notify/Structured/StructuredPushSupplier.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_StructuredPushSupplier::TAO_Notify_StructuredPushSupplier (TAO_Notify_ProxyConsumer* proxy)
  :TAO_Notify_Supplier (proxy)
{
}

TAO_Notify_StructuredPushSupplier::~TAO_Notify_StructuredPushSupplier ()
{
}

void
TAO_Notify_StructuredPushSupplier::init (CosNotifyComm::StructuredPushSupplier_ptr push_supplier)
{
  // TODO: verify single init call
  // push_supplier is optional
  this->push_supplier_ = CosNotifyComm::StructuredPushSupplier::_duplicate (push_supplier);
  this->subscribe_ = CosNotifyComm::NotifySubscribe::_duplicate (push_supplier);
}

void
TAO_Notify_StructuredPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}

ACE_CString
TAO_Notify_StructuredPushSupplier::get_ior (void) const
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
TAO_Notify_StructuredPushSupplier::get_supplier (void)
{
  return CosNotifyComm::StructuredPushSupplier::_duplicate (this->push_supplier_.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
