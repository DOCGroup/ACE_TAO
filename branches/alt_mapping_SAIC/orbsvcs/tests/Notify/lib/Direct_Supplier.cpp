// $Id$

#include "Direct_Supplier.h"

#if ! defined (__ACE_INLINE__)
#include "Direct_Supplier.inl"
#endif /* __ACE_INLINE__ */



#include "LookupManager.h"

TAO_Notify_Tests_Direct_Supplier::TAO_Notify_Tests_Direct_Supplier (ACE_CString& target)
  : target_ (target)
{
}

TAO_Notify_Tests_Direct_Supplier::~TAO_Notify_Tests_Direct_Supplier ()
{
}

void
TAO_Notify_Tests_Direct_Supplier::connect (void)
{
  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa, this->poa_name_.c_str ());

  // set the POA
  this->set_poa (poa.in ());

  // Get hold of the reference.
  CosNotifyComm::StructuredPushSupplier_var supplier_ref =
    this->_this ();

  // Register the activated object.
  LOOKUP_MANAGER->_register (supplier_ref.in (), this->name_.c_str ());

  // Resolve the target object.
  LOOKUP_MANAGER->resolve (this->target_object_, this->target_.c_str ());
}

void
TAO_Notify_Tests_Direct_Supplier::send_event (const CosNotification::StructuredEvent& event)
{
  ACE_ASSERT (!CORBA::is_nil (this->target_object_.in ()));

  this->target_object_->push_structured_event (event);
}
