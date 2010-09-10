// $Id$

#include "Standard_Error.h"
#include "Deployment/Deployment_StartErrorC.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE_Utility.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  // Implementation skeleton constructor
  Standard_Error::Standard_Error (void)
  {
  }

  // Implementation skeleton destructor
  Standard_Error::~Standard_Error (void)
  {
  }

  void
  Standard_Error::configure (const ::Deployment::Properties & )
  {

  }

  void
  Standard_Error::post_install (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong index,
                                         const ::CORBA::Any &,
                                         const ::CORBA::Any &exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        DANCE_ERROR (2, (LM_ERROR, DLINFO
                         ACE_TEXT ("Standard_Error::post_install - ")
                         ACE_TEXT ("Propagating exception from instance <%C>\n"),
                         plan.instance[index].name.in ()));
        DAnCE::Utility::throw_exception_from_any (exception);
      }
  }

  void
  Standard_Error::post_connect (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong connection,
                                         const ::CORBA::Any &exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        DANCE_ERROR (2, (LM_ERROR, DLINFO
                         ACE_TEXT ("Standard_Error::post_connect - ")
                         ACE_TEXT ("Propagating exception from connection <%C>\n"),
                         plan.connection[connection].name.in ()));
        DAnCE::Utility::throw_exception_from_any (exception);
      }
  }
  
  void 
  Standard_Error::post_configured (const ::Deployment::DeploymentPlan & plan,
                                            ::CORBA::ULong index,
                                            const ::CORBA::Any &exception )
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        DANCE_ERROR (2, (LM_ERROR, DLINFO
                         ACE_TEXT ("Standard_Error::post_configured - ")
                         ACE_TEXT ("Propagating exception from configuring instance <%C>\n"),
                         plan.instance[index].name.in ()));
        DAnCE::Utility::throw_exception_from_any (exception);
      }
  }
  
  
  void
  Standard_Error::post_activate (const ::Deployment::DeploymentPlan & plan,
                                          ::CORBA::ULong index,
                                          const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        DANCE_ERROR (2, (LM_ERROR, DLINFO
                         ACE_TEXT ("Standard_Error::post_activate - ")
                         ACE_TEXT ("Propagating exception from activation of instance <%C>\n"),
                         plan.instance[index].name.in ()));
        DAnCE::Utility::throw_exception_from_any (exception);
      }
  }
  
  
  void
  Standard_Error::post_passivate (const ::Deployment::DeploymentPlan & plan,
                                           ::CORBA::ULong index,
                                           const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        DANCE_ERROR (2, (LM_ERROR, DLINFO
                         ACE_TEXT ("Standard_Error::post_passivate - ")
                         ACE_TEXT ("Propagating exception from passivation of instance <%C>\n"),
                         plan.instance[index].name.in ()));
        DAnCE::Utility::throw_exception_from_any (exception);
      }
  }
  
  
  void
  Standard_Error::post_remove (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong index,
                                        const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        DANCE_ERROR (2, (LM_ERROR, DLINFO
                         ACE_TEXT ("Standard_Error::post_remove - ")
                         ACE_TEXT ("Propagating exception from removal of instance <%C>\n"),
                         plan.instance[index].name.in ()));
        DAnCE::Utility::throw_exception_from_any (exception);
      }
  }
 }

extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr 
  create_DAnCE_Standard_Error (void)
  {
    return new DAnCE::Standard_Error ();
  }
}
