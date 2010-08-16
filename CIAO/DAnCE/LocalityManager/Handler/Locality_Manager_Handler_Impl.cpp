// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from
// be/be_codegen.cpp:1560

#include "tao/ORB_Core.h"
#include "Locality_Manager_Handler_Impl.h"
#include "LocalityActivator_Impl.h"

#include "DAnCE/DAnCE_PropertiesC.h"

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

namespace DAnCE
{
  const char *
  Locality_Handler_i::instance_type_ =
    "edu.vanderbilt.dre.DAnCE.LocalityManager";

  // Implementation skeleton constructor
  Locality_Handler_i::
  Locality_Handler_i (void)
  {
  }

  // Implementation skeleton destructor
  Locality_Handler_i::~Locality_Handler_i (void)
  {
  }
  
  void
  Locality_Handler_i::close (void)
  {
    
  }
  
  char * Locality_Handler_i::instance_type (void)
  {
    DANCE_TRACE ("Locality_Handler_i::instance_type");
    return CORBA::string_dup (Locality_Handler_i::instance_type_);
  }

  void
  Locality_Handler_i::install_instance (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong instanceRef,
                                        ::CORBA::Any_out instance_reference)
  {
    CORBA::Any *outany (0);
    ACE_NEW_THROW_EX (outany,
                      CORBA::Any (),
                      CORBA::NO_MEMORY ());
    instance_reference = outany;
    
#ifdef GEN_OSTREAM_OPS
    {
      std::ostringstream plan_stream;
      plan_stream << plan << std::endl;

      DANCE_DEBUG (10, (LM_TRACE, DLINFO
                        ACE_TEXT ("Locality_Handler_i::install_instance - ")
                        ACE_TEXT ("Deploying instance %u of plan %C\n"),
                        instanceRef,
                        plan_stream.str ().c_str ()));
    }
#endif /* GEN_OSTREAM_OPS */
    
    if (plan.instance.length () <= instanceRef)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Locality_Handler_i::install_instance - ")
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
                         ACE_TEXT ("Locality_Handler_i::install_instance - ")
                         ACE_TEXT ("Invalid implementation reference %u provided ")
                         ACE_TEXT ("to install_instance\n"),
                         idd.implementationRef));
        throw ::Deployment::PlanError (plan.UUID.in (),
                                       "Invalid Implementation reference");
      }
    
    const ::Deployment::MonolithicDeploymentDescription &mdd =
      plan.implementation[idd.implementationRef];

    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Locality_Handler_i::install_instance - ")
                      ACE_TEXT ("Starting installation of instance <%C>\n"),
                      idd.name.in ()));

    CORBA::ULong allprops_len =
      idd.configProperty.length () + mdd.execParameter.length () + 1;
    ::Deployment::Properties allprops (allprops_len);

    Utility::append_properties (allprops,
                                mdd.execParameter);
    Utility::append_properties (allprops,
                                idd.configProperty);
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                     ACE_TEXT ("Locality_Handler_i::install_instance - ")
                     ACE_TEXT ("Passing %u properties to activator\n"),
                     allprops.length ()));

    ::DAnCE::LocalityManager_var lm_ref =
        this->activator_->create_locality_manager (allprops);

    if (CORBA::is_nil (lm_ref))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Locality_Handler_i::install_instance - ")
                         ACE_TEXT ("Received nil reference from LocalityActivator\n")));
        throw ::Deployment::StartError (idd.name.in (),
                                        "Received nil object reference for "
                                        "LocalityManager from Activator\n");
      }

    *instance_reference <<= lm_ref.in ();
  }

  void
  Locality_Handler_i::remove_instance (const ::Deployment::DeploymentPlan &,
                                       ::CORBA::ULong,
                                       const ::CORBA::Any &instance_reference)
  {
    LocalityManager_ptr lm_ref;

    if (!(instance_reference >>= lm_ref) ||
        CORBA::is_nil (lm_ref))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Locality_Handler_i::remove_instance - "),
                         ACE_TEXT ("Unable to extract valid LocalityManager ")
                         ACE_TEXT ("reference from parameter\n")));
      }
    this->activator_->remove_locality_manager (lm_ref);
  }

  void
  Locality_Handler_i::connect_instance (const ::Deployment::DeploymentPlan &,
                                        ::CORBA::ULong,
                                        const ::CORBA::Any &)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Locality_Handler_i::disconnect_instance (const ::Deployment::DeploymentPlan &,
                                           ::CORBA::ULong)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Locality_Handler_i::configure (const ::Deployment::Properties &prop )
  {
    ::DAnCE::Utility::PROPERTY_MAP pmap (prop.length ());
    
    ::DAnCE::Utility::build_property_map (pmap,
                                          prop);

    CORBA::ULong spawn = 0;
    const char *cs_path = 0;
    const char *cs_args = 0;
    CORBA::Boolean multithread = false;
    PortableServer::POA_var poa;

    Utility::get_property_value (DAnCE::LOCALITY_EXECUTABLE,
                                 pmap, cs_path);
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Component server path: %C\n"), cs_path));
    Utility::get_property_value (DAnCE::LOCALITY_ARGUMENTS,
                                 pmap, cs_args);
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Component server arguments: %C\n"), cs_args));
    Utility::get_property_value (DAnCE::LOCALITY_TIMEOUT,
                                 pmap, spawn);
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Spawn delay: %u\n"), spawn));
    Utility::get_property_value (DAnCE::LOCALITY_MULTITHREAD,
                                 pmap, multithread);
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Threading: %C\n"),
                     multithread ? "Multi" : "Single"));

    Utility::get_property_value (DAnCE::ENTITY_POA,
                                 pmap, poa);

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Threading: %C\n"),
                     multithread ? "Multi" : "Single"));

    CORBA::ORB_var orb = TAO_ORB_Core_instance ()->orb ();

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Spawning Locality handler\n")));
    
    ACE_NEW_THROW_EX (this->activator_,
                      DAnCE_LocalityActivator_i (spawn,
                                                 cs_path,
                                                 cs_args,
                                                 false,
                                                 orb,
                                                 poa),
                      CORBA::NO_MEMORY ());

    PortableServer::ServantBase_var safe_servant (this->activator_);

    poa->activate_object (this->activator_);
  }
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr create_Locality_Handler (void)
  {
    return new DAnCE::Locality_Handler_i ();
  }
}
