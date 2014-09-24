// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/CSD_Framework/CSD_Object_Adapter_Factory.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Framework_Loader::TAO_CSD_Framework_Loader (void)
  : initialized_ (false)
{
}

TAO_CSD_Framework_Loader::~TAO_CSD_Framework_Loader (void)
{
}


int
TAO_CSD_Framework_Loader::static_init (void)
{
  ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_CSD_Object_Adapter_Factory);
  ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_CSD_Strategy_Repository);
  ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_CSD_Framework_Loader);

  return 0;
}

int
TAO_CSD_Framework_Loader::init (int, ACE_TCHAR* [])
{
  ACE_TRACE ("TAO_CSD_Framework_Loader::init");

  // Only allow initialization once.
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

  return 0;
}


/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_CSD_FW, TAO_CSD_Framework_Loader)
ACE_STATIC_SVC_DEFINE (TAO_CSD_Framework_Loader,
                       ACE_TEXT ("CSD_Framework_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CSD_Framework_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
