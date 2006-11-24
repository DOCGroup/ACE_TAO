// $Id$

#include "orbsvcs/Notify/Any/PushSupplier.h"

ACE_RCSID (Notify, TAO_Notify_PushSupplier, "$Id$")
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
TAO_Notify_PushSupplier::init (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
{
  // TODO: verify single init call
  // push_supplier is optional
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

ACE_CString
TAO_Notify_PushSupplier::get_ior (void) const
{
  ACE_CString result;
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    CORBA::String_var ior = orb->object_to_string(this->push_supplier_.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    result = static_cast<const char*> (ior.in ());
  }
  ACE_CATCHANY
  {
    result.fast_clear();
  }
  ACE_ENDTRY;
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
