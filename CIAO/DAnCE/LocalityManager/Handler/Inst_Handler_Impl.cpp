// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from
// be/be_codegen.cpp:1560

#include "tao/ORB_Core.h"
#include "Inst_Handler_Impl.h"
#include "LocalityActivator_Impl.h"
#include "LocalityManager/Scheduler/Plugin_Manager.h"

#include "DAnCE/DAnCE_PropertiesC.h"

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

namespace DAnCE
{
  const char *
  Inst_Handler_Impl::instance_type_ =
    "edu.vanderbilt.dre.DAnCE.InstanceHandler";

  // Implementation skeleton constructor
  Inst_Handler_Impl::
  Inst_Handler_Impl (void)
  {
  }

  // Implementation skeleton destructor
  Inst_Handler_Impl::~Inst_Handler_Impl (void)
  {
  }

  ::CORBA::StringSeq *
  Inst_Handler_Impl::dependencies (void)
  {
    ::CORBA::StringSeq *retval (0);
    ACE_NEW_THROW_EX (retval,
                      ::CORBA::StringSeq (0),
                      CORBA::NO_MEMORY ());
    return retval;
  }
  
  void
  Inst_Handler_Impl::close (void)
  {
    
  }
  
  char * Inst_Handler_Impl::instance_type (void)
  {
    DANCE_TRACE ("Inst_Handler_Impl::instance_type");
    return CORBA::string_dup (Inst_Handler_Impl::instance_type_);
  }

  void
  Inst_Handler_Impl::install_instance (const ::Deployment::DeploymentPlan & plan,
                                       ::CORBA::ULong instanceRef,
                                       ::CORBA::Any_out instance_reference)
  {
    CORBA::Any *outany (0);
    ACE_NEW_THROW_EX (outany,
                      CORBA::Any (),
                      CORBA::NO_MEMORY ());
    instance_reference = outany;
    
    if (plan.instance.length () <= instanceRef)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Inst_Handler_Impl::install_instance - ")
                         ACE_TEXT ("Invalid instance reference %u provided ")
                         ACE_TEXT ("to install_instance\n"),
                      instanceRef));
        throw ::Deployment::PlanError (plan.UUID.in (),
                                       "Invalid instance reference");
      }
    
    const ::Deployment::InstanceDeploymentDescription &idd =
      plan.instance[instanceRef];
    
    if (plan.implementation.length () <= idd.implementationRef)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Inst_Handler_Impl::install_instance - ")
                         ACE_TEXT ("Invalid implementation reference %u provided ")
                         ACE_TEXT ("to install_instance\n"),
                         idd.implementationRef));
        throw ::Deployment::PlanError (plan.UUID.in (),
                                       "Invalid Implementation reference");
      }
    
    const ::Deployment::MonolithicDeploymentDescription &mdd =
      plan.implementation[idd.implementationRef];

    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Inst_Handler_Impl::install_instance - ")
                      ACE_TEXT ("Starting installation of instance <%C>\n"),
                      idd.name.in ()));

    const char *entrypt (0);
    if (!DAnCE::Utility::get_property_value (DAnCE::DANCE_PLUGIN_ENTRYPT,
                                             mdd.execParameter,
                                             entrypt))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Inst_Handler_Impl::install_instance - ")
                         ACE_TEXT ("No entrypoint found for plug-in initialization\n")));
        throw ::Deployment::StartError (idd.name.in (),
                                        "No entrypoint found for plug-in initialization\n");
      }

    const char *artifact (0);
    if (!DAnCE::Utility::get_property_value (DAnCE::DANCE_PLUGIN_ARTIFACT,
                                             mdd.execParameter,
                                             artifact))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Inst_Handler_Impl::install_instance - ")
                         ACE_TEXT ("No artifact found for plug-in initialization\n")));
        throw ::Deployment::StartError (idd.name.in (),
                                        "No artifact found for plug-in initialization\n");
      }
    
    Plugin_Manager::IH_DEPS deps;
    
    for (CORBA::ULong i = 0; i < idd.configProperty.length (); ++i)
      {
        if (ACE_OS::strcmp (idd.configProperty[i].name.in (),
                            DAnCE::DANCE_IDH_DEPENDSON) == 0)
          {
            const char *id (0);
            
            if (idd.configProperty[i].value >>= CORBA::Any::to_string (id, 0))
              {
                deps.insert (id);
              }
          }
        else
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("Inst_Handler_Impl::install_instance - ")
                             ACE_TEXT ("Unable to extract dependency order from string")));
            throw ::Deployment::StartError (idd.name.in (),
                                            "Unable to extract dependency order from string");
          }
      }
    
    CORBA::String_var plugin_id = 
      PLUGIN_MANAGER::instance ()->register_installation_handler (ACE_TEXT_CHAR_TO_TCHAR (artifact),
                                                                  ACE_TEXT_CHAR_TO_TCHAR (entrypt),
                                                                  deps);
    
    (*outany) <<= CORBA::Any::from_string (plugin_id.in (), 0);
  }

  void
  Inst_Handler_Impl::remove_instance (const ::Deployment::DeploymentPlan &,
                                      ::CORBA::ULong,
                                      const ::CORBA::Any &instance_reference)
  {
  }

  void
  Inst_Handler_Impl::connect_instance (const ::Deployment::DeploymentPlan &,
                                       ::CORBA::ULong,
                                       const ::CORBA::Any &)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Inst_Handler_Impl::disconnect_instance (const ::Deployment::DeploymentPlan &,
                                          ::CORBA::ULong)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Inst_Handler_Impl::configure (const ::Deployment::Properties &prop )
  {
    ::DAnCE::Utility::PROPERTY_MAP pmap (prop.length ());
    
    ::DAnCE::Utility::build_property_map (pmap,
                                          prop);
  }
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr create_Inst_Handler (void)
  {
    return new DAnCE::Inst_Handler_Impl ();
  }
}
