// $Id$

#include "IdAssignmentStrategyFactoryImpl.h"
#include "IdAssignmentStrategySystem.h"
#include "IdAssignmentStrategyUser.h"

ACE_RCSID (PortableServer,
           IdAssignmentStrategyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentStrategyFactoryImpl::~IdAssignmentStrategyFactoryImpl (void)
    {
    }
    IdAssignmentStrategy*
    IdAssignmentStrategyFactoryImpl::create (
      ::PortableServer::IdAssignmentPolicyValue value)
    {
      IdAssignmentStrategy* strategy = 0;

      switch (value)
      {
        case ::PortableServer::SYSTEM_ID :
        {
          ACE_NEW_RETURN (strategy, System_Id_Assignment_Strategy, 0);
          break;
        }
        case ::PortableServer::USER_ID :
        {
          ACE_NEW_RETURN (strategy, User_Id_Assignment_Strategy, 0);
          break;
        }
      }

      return strategy;
    }

    ACE_STATIC_SVC_DEFINE (
        IdAssignmentStrategyFactoryImpl,
        ACE_TEXT ("IdAssignmentStrategyFactoryImpl"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdAssignmentStrategyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdAssignmentStrategyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<IdAssignmentStrategyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdAssignmentStrategyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

