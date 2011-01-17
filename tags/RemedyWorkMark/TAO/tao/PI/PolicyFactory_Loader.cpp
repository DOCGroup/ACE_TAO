/* -*- C++ -*- */

// =================================================================
/**
 * @file PolicyFactory_Loader.cpp
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 */
// =================================================================

#include "tao/PI/PolicyFactory_Loader.h"
#include "tao/PI/PolicyFactory_Registry.h"

#include "tao/ORB.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::PolicyFactory_Registry_Adapter*
TAO_PolicyFactory_Loader::create (void)
{
  TAO::PolicyFactory_Registry_Adapter* obj = 0;
  ACE_NEW_RETURN (obj,
                  TAO_PolicyFactory_Registry,
                  0);
  return obj;
}

int
TAO_PolicyFactory_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_PolicyFactory_Loader);
}


ACE_STATIC_SVC_DEFINE (TAO_PolicyFactory_Loader,
                       ACE_TEXT ("PolicyFactory_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PolicyFactory_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PI, TAO_PolicyFactory_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL
