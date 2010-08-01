// $Id$

#include "Best_Effort.h"
#include "Deployment/Deployment_StartErrorC.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE_Utility.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  // Implementation skeleton constructor
  Best_Effort::Best_Effort (void)
  {
  }

  // Implementation skeleton destructor
  Best_Effort::~Best_Effort (void)
  {
  }

  void
  Best_Effort::configure (const ::Deployment::Properties & )
  {

  }

  void
  Best_Effort::instance_post_install (const ::Deployment::DeploymentPlan &plan,
                                      ::CORBA::ULong instance_index,
                                      const ::CORBA::Any &,
                                      const ::CORBA::Any &exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Best_Effort::instance_post_install - ")
                         ACE_TEXT ("Received exception while installing instance ")
                         ACE_TEXT ("<%C>:<%C>\n"),
                         plan.instance[instance_index].name.in (),
                         result.c_str ()));
      }
  }

  void
  Best_Effort::instance_post_connect (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong connection,
                                         const ::CORBA::Any &exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Best_Effort::instance_post_install - ")
                         ACE_TEXT ("Received exception while establishing connection ")
                         ACE_TEXT ("<%C>:<%C>\n"),
                         plan.connection[connection].name.in (),
                         result.c_str ()));
      }
  }
  
  void 
  Best_Effort::instance_post_configured (const ::Deployment::DeploymentPlan & plan,
                                            ::CORBA::ULong instance_index,
                                            const ::CORBA::Any &exception )
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Best_Effort::instance_post_install - ")
                         ACE_TEXT ("Received exception while completing configuration of ")
                         ACE_TEXT ("instance <%C>:<%C>\n"),
                         plan.instance[instance_index].name.in (),
                         result.c_str ()));
      }
  }
  
  
  void
  Best_Effort::instance_post_activate (const ::Deployment::DeploymentPlan & plan,
                                       ::CORBA::ULong instance_index,
                                       const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Best_Effort::instance_post_install - ")
                         ACE_TEXT ("Received exception while activating instance ")
                         ACE_TEXT ("<%C>:<%C>\n"),
                         plan.instance[instance_index].name.in (),
                         result.c_str ()));
      }
  }
  
  
  void
  Best_Effort::instance_post_passivate (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong instance_index,
                                        const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Best_Effort::instance_post_install - ")
                         ACE_TEXT ("Received exception while passivating instance ")
                         ACE_TEXT ("<%C>:<%C>\n"),
                         plan.instance[instance_index].name.in (),
                         result.c_str ()));
      }
  }
  
  
  void
  Best_Effort::instance_post_remove (const ::Deployment::DeploymentPlan & plan,
                                     ::CORBA::ULong instance_index,
                                     const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Best_Effort::instance_post_install - ")
                         ACE_TEXT ("Received exception while removing instance ")
                         ACE_TEXT ("<%C>:<%C>\n"),
                         plan.instance[instance_index].name.in (),
                         result.c_str ()));

      }
  }
 }

extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr 
  create_DAnCE_Best_Effort (void)
  {
    return new DAnCE::Best_Effort ();
  }
}
