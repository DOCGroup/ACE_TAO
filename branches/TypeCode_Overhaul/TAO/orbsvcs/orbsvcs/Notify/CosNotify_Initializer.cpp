// $Id$

#include "CosNotify_Initializer.h"

ACE_RCSID(Notify, TAO_Notify_CosNotify_Initializer, "$Id$")

#include "CosNotify_Service.h"

TAO_Notify_CosNotify_Initializer::TAO_Notify_CosNotify_Initializer (void)
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_CosNotify_Service);
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_Notify_Default_EMO_Factory_OLD);
}
