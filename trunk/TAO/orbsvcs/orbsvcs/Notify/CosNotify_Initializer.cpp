// $Id$

#include "CosNotify_Initializer.h"

#if ! defined (__ACE_INLINE__)
#include "CosNotify_Initializer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_CosNotify_Initializer, "$id$")

#include "Notify_Service.h"

TAO_NS_CosNotify_Initializer::TAO_NS_CosNotify_Initializer (void)
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_NS_Notify_Service);
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_Notify_Default_EMO_Factory_OLD);
}
