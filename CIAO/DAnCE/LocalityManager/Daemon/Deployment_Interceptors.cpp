// $Id$

#include "Deployment_Interceptors.h"
#include "Logger/Log_Macros.h"
#include "Name_Utilities.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE_Utility.h"

namespace DAnCE
{
  // Implementation skeleton constructor
  DAnCE_StoreReferences_i::DAnCE_StoreReferences_i (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
      
  {
    try
      {
        CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
        ctx_ = CosNaming::NamingContext::_narrow (obj.in ());
      }
    catch (...)
      {
        // swallow
      }
  }

  // Implementation skeleton destructor
  DAnCE_StoreReferences_i::~DAnCE_StoreReferences_i (void)
  {
  }

  void DAnCE_StoreReferences_i::instance_pre_install (::Deployment::DeploymentPlan & plan,
                                                      ::CORBA::ULong instanceRef)
  {
    // no-op
  }

  void DAnCE_StoreReferences_i::instance_post_install (const ::Deployment::DeploymentPlan & plan,
                                                       ::CORBA::ULong instance_index,
                                                       const ::CORBA::Any & instance_reference,
                                                       const ::CORBA::Any & exception_thrown)
  {
    const ::Deployment::InstanceDeploymentDescription &inst =
      plan.instance[instance_index];
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO "DAnCE_StoreReferences_i::instance_post_install - "
                     "Interceptor post install for instance %C\n",
                     plan.instance[instance_index].name.in ()));
    
    for (CORBA::ULong i = 0;
         i < inst.configProperty.length ();
         ++i)
      {
        if (ACE_OS::strcmp (inst.configProperty[i].name.in (),
                            DAnCE::REGISTER_NAMING) == 0)
          {
            CORBA::Object_var obj;
            
            if (!(instance_reference >>= CORBA::Any::to_object (obj)))
              {
                DANCE_ERROR (1, (LM_WARNING, DLINFO
                                 "DAnCE_StoreReferences_i::instance_post_install - "
                                 "Unable to extract instance reference from Any\n"));
              }
            
            const char * name;
            inst.configProperty[i].value >>= CORBA::Any::to_string (name, 0);
            
            DANCE_DEBUG (9, (LM_TRACE, DLINFO "DAnCE_StoreReferences_i::instance_post_install - "
                             "Registering name %C for instance %C\n",
                             name,
                             plan.instance[instance_index].name.in ()));

            Name_Utilities::bind_object (name,
                                         obj.in (),
                                         this->ctx_.in ());
          }
        else if (ACE_OS::strcmp (inst.configProperty[i].name.in (),
                                 DAnCE::INSTANCE_IOR_FILE) == 0)
          {
            CORBA::Object_var obj;
            instance_reference >>= CORBA::Any::to_object (obj);

            const char * name;
            inst.configProperty[i].value >>= CORBA::Any::to_string (name, 0);
            
            ACE_CString ior = this->orb_->object_to_string (obj.in ());

            DAnCE::Utility::write_IOR (name, ior.c_str ());
          }
      }
  }

  // Implementation skeleton constructor
  DAnCE_ReferenceLookup_i::DAnCE_ReferenceLookup_i (void)
  {
  }

  // Implementation skeleton destructor
  DAnCE_ReferenceLookup_i::~DAnCE_ReferenceLookup_i (void)
  {
  }

  void 
  DAnCE_ReferenceLookup_i::instance_pre_connect (::Deployment::DeploymentPlan & plan,
                                                 ::CORBA::ULong connection_index,
                                                 ::CORBA::Any & provided_reference)
  {
    // Add your implementation here
  }

  void DAnCE_ReferenceLookup_i::instance_post_connect (const ::Deployment::DeploymentPlan & plan,
                                                       ::CORBA::ULong connectionRef,
                                                       const ::CORBA::Any & exceptionThrown)
  {
    // Add your implementation here
  }

 
}
