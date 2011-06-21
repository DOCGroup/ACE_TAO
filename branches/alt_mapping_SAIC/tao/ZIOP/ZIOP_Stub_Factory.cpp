// $Id$
#include "tao/ZIOP/ZIOP_Stub_Factory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ZIOP/ZIOP_Stub.h"

#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ZIOP_Stub_Factory::~TAO_ZIOP_Stub_Factory (void)
{
}

TAO_Stub *
TAO_ZIOP_Stub_Factory::create_stub (const char *repository_id,
                                  const TAO_MProfile &profiles,
                                  TAO_ORB_Core *orb_core)
{
  TAO_Stub *retval = 0;

  ACE_NEW_THROW_EX (retval,
                    TAO_ZIOP_Stub (repository_id, profiles, orb_core),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_MAYBE));

  return retval;
}


// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_ZIOP_Stub_Factory,
                       ACE_TEXT ("ZIOP_Stub_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_ZIOP_Stub_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_ZIOP, TAO_ZIOP_Stub_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
