// $Id$

#include "ORB_Core.h"
#include "Codeset_Manager.h"
#include "Codeset_Manager_Factory_Base.h"

ACE_RCSID (tao,
           Codeset_Manager_Factory_Base,
           "$Id$")

TAO_Codeset_Manager_Factory_Base::~TAO_Codeset_Manager_Factory_Base ()
{
}

int
TAO_Codeset_Manager_Factory_Base::is_default() const
{
  return 1;
}

TAO_Codeset_Manager *
TAO_Codeset_Manager_Factory_Base::create ()
{
  return 0;

}

int
TAO_Codeset_Manager_Factory_Base::initialize (void)
{
  return ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_Codeset_Manager_Factory_Base);
}

static int
TAO_Requires_Codeset_Manager_Factory_Base =
TAO_Codeset_Manager_Factory_Base::initialize ();

ACE_FACTORY_DEFINE (TAO, TAO_Codeset_Manager_Factory_Base)
ACE_STATIC_SVC_DEFINE (TAO_Codeset_Manager_Factory_Base,
                       ACE_TEXT ("TAO_Codeset"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Codeset_Manager_Factory_Base),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
