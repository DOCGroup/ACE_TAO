// $Id$

#include "PushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "PushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_Notify_PushSupplier, "$Id$")
#include "../Properties.h"

TAO_Notify_PushSupplier::TAO_Notify_PushSupplier (TAO_Notify_ProxyConsumer* proxy)
  :TAO_Notify_Supplier (proxy)
{
}

TAO_Notify_PushSupplier::~TAO_Notify_PushSupplier ()
{
}

void
TAO_Notify_PushSupplier::init (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
{
  this->push_supplier_ = CosEventComm::PushSupplier::_duplicate (push_supplier);

  ACE_TRY
    {
      this->subscribe_ = CosNotifyComm::NotifySubscribe::_narrow (push_supplier ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // _narrow failed which probably means the interface is CosEventComm type.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_PushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}
bool
TAO_Notify_PushSupplier::get_ior (ACE_CString & iorstr) const
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
