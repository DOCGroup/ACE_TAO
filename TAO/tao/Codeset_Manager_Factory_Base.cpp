// -*- C++ -*-
#include "ORB_Core.h"
#include "Codeset_Manager.h"
#include "Codeset_Manager_Factory_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Codeset_Manager_Factory_Base::~TAO_Codeset_Manager_Factory_Base ()
{
}

bool
TAO_Codeset_Manager_Factory_Base::is_default () const
{
  return true;
}

TAO_Codeset_Manager *
TAO_Codeset_Manager_Factory_Base::create ()
{
  return nullptr;

}

int
TAO_Codeset_Manager_Factory_Base::initialize ()
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

