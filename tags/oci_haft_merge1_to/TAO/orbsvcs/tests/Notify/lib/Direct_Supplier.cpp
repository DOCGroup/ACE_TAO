// $Id$

#include "Direct_Supplier.h"

#if ! defined (__ACE_INLINE__)
#include "Direct_Supplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Tests_Direct_Supplier, "$id$")

#include "LookupManager.h"

TAO_Notify_Tests_Direct_Supplier::TAO_Notify_Tests_Direct_Supplier (ACE_CString& target)
  : target_ (target)
{
}

TAO_Notify_Tests_Direct_Supplier::~TAO_Notify_Tests_Direct_Supplier ()
{
}

void
TAO_Notify_Tests_Direct_Supplier::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa, this->poa_name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // set the POA
  this->set_poa (poa.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get hold of the reference.
  CosNotifyComm::StructuredPushSupplier_var supplier_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Register the activated object.
  LOOKUP_MANAGER->_register (supplier_ref.in (), this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Resolve the target object.
  LOOKUP_MANAGER->resolve (this->target_object_, this->target_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Tests_Direct_Supplier::send_event (const CosNotification::StructuredEvent& event
                                   ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->target_object_.in ()));

  this->target_object_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
