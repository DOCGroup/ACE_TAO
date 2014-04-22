// $Id$

#include "Selective_Error.h"
#include "dance/Deployment/Deployment_StartErrorC.h"
#include "dance/DAnCE_PropertiesC.h"
#include "dance/DAnCE_Utility.h"
#include "dance/Logger/Log_Macros.h"

namespace DAnCE
{
  // Implementation skeleton constructor
  Selective_Error::Selective_Error (void)
  {
  }

  // Implementation skeleton destructor
  Selective_Error::~Selective_Error (void)
  {
  }

  void
  Selective_Error::configure (const ::Deployment::Properties & )
  {

  }

  void
  Selective_Error::post_install (const ::Deployment::DeploymentPlan &plan,
                                 ::CORBA::ULong index,
                                 const ::CORBA::Any &,
                                 const ::CORBA::Any &exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("Selective_Error::post_install - ")
                      ACE_TEXT ("Received exception while installing instance ")
                      ACE_TEXT ("<%C>:<%C>\n"),
                      plan.instance[index].name.in (),
                      result.c_str ()));

      }
  }

  void
  Selective_Error::post_connect (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong connection,
                                         const ::CORBA::Any &exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("Selective_Error::post_install - ")
                      ACE_TEXT ("Received exception while establishing connection ")
                      ACE_TEXT ("<%C>:<%C>\n"),
                      plan.connection[connection].name.in (),
                      result.c_str ()));
        DAnCE::Utility::throw_exception_from_any (exception);
      }
  }

  void
  Selective_Error::post_configured (const ::Deployment::DeploymentPlan & plan,
                                            ::CORBA::ULong index,
                                            const ::CORBA::Any &exception )
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("Selective_Error::post_install - ")
                      ACE_TEXT ("Received exception while completing configuration of ")
                      ACE_TEXT ("instance <%C>:<%C>\n"),
                      plan.instance[index].name.in (),
                      result.c_str ()));
      }
  }


  void
  Selective_Error::post_activate (const ::Deployment::DeploymentPlan & plan,
                                       ::CORBA::ULong index,
                                       const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("Selective_Error::post_install - ")
                      ACE_TEXT ("Received exception while activating instance ")
                      ACE_TEXT ("<%C>:<%C>\n"),
                      plan.instance[index].name.in (),
                      result.c_str ()));
      }
  }


  void
  Selective_Error::post_passivate (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong index,
                                        const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("Selective_Error::post_install - ")
                      ACE_TEXT ("Received exception while passivating instance ")
                      ACE_TEXT ("<%C>:<%C>\n"),
                      plan.instance[index].name.in (),
                      result.c_str ()));
      }
  }


  void
  Selective_Error::post_remove (const ::Deployment::DeploymentPlan & plan,
                                     ::CORBA::ULong index,
                                     const ::CORBA::Any & exception)
  {
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("Selective_Error::post_install - ")
                      ACE_TEXT ("Received exception while removing instance ")
                      ACE_TEXT ("<%C>:<%C>\n"),
                      plan.instance[index].name.in (),
                      result.c_str ()));
      }
  }
 }

extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr
  create_DAnCE_Selective_Error (void)
  {
    return new DAnCE::Selective_Error ();
  }
}
