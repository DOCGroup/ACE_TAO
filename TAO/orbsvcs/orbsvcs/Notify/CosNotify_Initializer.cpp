// $Id$

#include "orbsvcs/Notify/CosNotify_Initializer.h"

ACE_RCSID(Notify, TAO_Notify_CosNotify_Initializer, "$Id$")

#include "orbsvcs/Notify/CosNotify_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_CosNotify_Initializer::TAO_Notify_CosNotify_Initializer (void)
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_CosNotify_Service);
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_Notify_Default_EMO_Factory_OLD);
}

TAO_END_VERSIONED_NAMESPACE_DECL
