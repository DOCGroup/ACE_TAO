// $Id$

#include "StructuredPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_Notify_StructuredPushSupplier, "$Id$")
#include "../Properties.h"

TAO_Notify_StructuredPushSupplier::TAO_Notify_StructuredPushSupplier (TAO_Notify_ProxyConsumer* proxy)
  :TAO_Notify_Supplier (proxy)
{
}

TAO_Notify_StructuredPushSupplier::~TAO_Notify_StructuredPushSupplier ()
{
}

void
TAO_Notify_StructuredPushSupplier::init (CosNotifyComm::StructuredPushSupplier_ptr push_supplier ACE_ENV_ARG_DECL_NOT_USED)
{
  this->push_supplier_ = CosNotifyComm::StructuredPushSupplier::_duplicate (push_supplier);

  this->subscribe_ = CosNotifyComm::NotifySubscribe::_duplicate (push_supplier);
}

void
TAO_Notify_StructuredPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}

bool
TAO_Notify_StructuredPushSupplier::get_ior (ACE_CString & iorstr) const
{
  bool result = false;
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    CORBA::String_var ior = orb->object_to_string(this->push_supplier_.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    iorstr = ACE_static_cast (const char *, ior.in ());
    result = true;
  }
  ACE_CATCHANY
  {
    ACE_ASSERT(0);
  }
  ACE_ENDTRY;
  return result;
}
