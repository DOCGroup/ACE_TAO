// -*- C++ -*-
// $Id$

#include "tao/ORB_Core.h"
#include "tao/Codeset_Manager.h"
#include "tao/Codeset_Manager_Factory_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Codeset_Manager_Factory_Base::~TAO_Codeset_Manager_Factory_Base (void)
{
}

bool
TAO_Codeset_Manager_Factory_Base::is_default (void) const
{
  return true;
}

TAO_Codeset_Manager *
TAO_Codeset_Manager_Factory_Base::create (void)
{
  return 0;

}

int
TAO_Codeset_Manager_Factory_Base::initialize (void)
{
  return ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_Codeset_Manager_Factory_Base);
}

#if 0
static int
TAO_Requires_Codeset_Manager_Factory_Base =
TAO_Codeset_Manager_Factory_Base::initialize ();
#endif

ACE_FACTORY_DEFINE (TAO, TAO_Codeset_Manager_Factory_Base)
ACE_STATIC_SVC_DEFINE (TAO_Codeset_Manager_Factory_Base,
                       ACE_TEXT ("TAO_Codeset"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Codeset_Manager_Factory_Base),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

TAO_END_VERSIONED_NAMESPACE_DECL

