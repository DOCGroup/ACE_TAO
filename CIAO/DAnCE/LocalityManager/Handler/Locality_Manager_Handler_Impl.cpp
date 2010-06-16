// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from
// be/be_codegen.cpp:1560

#include "Locality_Manager_Handler_Impl.h"
#include "LocalityActivator_Impl.h"

#include "DAnCE/DAnCE_PropertiesC.h"

namespace DAnCE
{
  const char *
  Locality_Handler_i::instance_type_ =
    "edu.vanderbilt.dre.DAnCE.LocalityManager";

  // Implementation skeleton constructor
  Locality_Handler_i::
  Locality_Handler_i (const Utility::PROPERTY_MAP &prop,
                      CORBA::ORB_ptr orb,
                      PortableServer::POA_ptr poa)
    : activator_ (0),
      properties_ (prop)
  {
    CORBA::ULong spawn = 0;
    const char *cs_path = 0;
    const char *cs_args = 0;
    CORBA::Boolean multithread = false;

    Utility::get_property_value (DAnCE::LOCALITY_EXECUTABLE,
                                 this->properties_, cs_path);
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Component server path: %C\n"), cs_path));
    Utility::get_property_value (DAnCE::LOCALITY_ARGUMENTS,
                                 this->properties_, cs_args);
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Component server arguments: %C\n"), cs_args));
    Utility::get_property_value (DAnCE::LOCALITY_TIMEOUT,
                                 this->properties_, spawn);
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Spawn delay: %u\n"), spawn));
    Utility::get_property_value (DAnCE::LOCALITY_MULTITHREAD,
                                 this->properties_, multithread);
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Threading: %C\n"),
                     multithread ? "Multi" : "Single"));

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Locality_Handler_i - ")
                     ACE_TEXT("Spawning Locality handler\n")));

    ACE_NEW_THROW_EX (this->activator_,
                      DAnCE_LocalityActivator_i (spawn,
                                                 cs_path,
                                                 cs_args,
                                                 multithread,
                                                 orb,
                                                 poa),
                      CORBA::NO_MEMORY ());

    PortableServer::ServantBase_var safe_servant (this->activator_);

    poa->activate_object (this->activator_);
  }

  // Implementation skeleton destructor
  Locality_Handler_i::~Locality_Handler_i (void)
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
    const ::Deployment::MonolithicDeploymentDescription &mdd =
      plan.implementation[idd.implementationRef];

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

    CORBA::Any *outany (0);
    ACE_NEW_THROW_EX (outany,
                      CORBA::Any (),
                      CORBA::NO_MEMORY ());
    (*outany) <<= lm_ref.in ();

    instance_reference = outany;
  }

  void
  Locality_Handler_i::remove_instance (const ::Deployment::DeploymentPlan &,
                                       ::CORBA::ULong,
                                       const ::CORBA::Any &instance_reference)
  {
    LocalityManager_var lm_ref;

    if (!(instance_reference >>= lm_ref) ||
        CORBA::is_nil (lm_ref.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Locality_Handler_i::remove_instance - "),
                         ACE_TEXT ("Unable to extract valid LocalityManager ")
                         ACE_TEXT ("reference from parameter\n")));
      }
    this->activator_->remove_locality_manager (lm_ref.in ());
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
  }
}
