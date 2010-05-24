// $Id$

#include "Deployment_Interceptors.h"
#include "Logger/Log_Macros.h"
#include "Name_Utilities.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE_Utility.h"

namespace DAnCE
{
  // Implementation skeleton constructor
  DAnCE_StoreReferences_i::DAnCE_StoreReferences_i (CORBA::ORB_ptr orb,
                                                    const ::Deployment::Properties *prop_ptr)
    : orb_ (CORBA::ORB::_duplicate (orb))
      
  {
    if (prop_ptr)
      {
        const ::Deployment::Properties &props = *prop_ptr;

        for (CORBA::ULong i = 0; i < props.length (); ++i)
          {
            if (ACE_OS::strcmp (props[i].name.in (),
                                DAnCE::LOCALITY_NAMINGCONTEXT) == 0)
              {
                CORBA::Object_var obj;
                props[i].value >>= CORBA::Any::to_object (obj);
                
                ctx_ = CosNaming::NamingContext::_narrow (obj.in ());
              }
          }
      }
  }

  // Implementation skeleton destructor
  DAnCE_StoreReferences_i::~DAnCE_StoreReferences_i (void)
  {
  }

  void DAnCE_StoreReferences_i::instance_pre_install (::Deployment::DeploymentPlan &,
                                                      ::CORBA::ULong)
  {
    // no-op
  }

  void DAnCE_StoreReferences_i::instance_post_install (const ::Deployment::DeploymentPlan &plan,
                                                       ::CORBA::ULong instance_index,
                                                       const ::CORBA::Any &instance_reference,
                                                       const ::CORBA::Any &)
  {
    const ::Deployment::InstanceDeploymentDescription &inst =
      plan.instance[instance_index];
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO 
                     ACE_TEXT ("DAnCE_StoreReferences_i::instance_post_install - ")
                     ACE_TEXT ("Interceptor post install for instance %C\n"),
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
                                 ACE_TEXT ("DAnCE_StoreReferences_i::instance_post_install - ")
                                 ACE_TEXT ("Unable to extract instance reference from Any\n")));
              }
            
            const char * name;
            inst.configProperty[i].value >>= CORBA::Any::to_string (name, 0);
            
            DANCE_DEBUG (9, (LM_TRACE, DLINFO 
                             ACE_TEXT ("DAnCE_StoreReferences_i::instance_post_install - ")
                             ACE_TEXT ("Registering name %C for instance %C\n"),
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

            DAnCE::Utility::write_IOR (ACE_TEXT_CHAR_TO_TCHAR (name), ior.c_str ());
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
  DAnCE_ReferenceLookup_i::instance_pre_connect (::Deployment::DeploymentPlan &,
                                                 ::CORBA::ULong,
                                                 ::CORBA::Any &)
  {
    // Add your implementation here
  }

  void DAnCE_ReferenceLookup_i::instance_post_connect (const ::Deployment::DeploymentPlan &,
                                                       ::CORBA::ULong,
                                                       const ::CORBA::Any &)
  {
    // Add your implementation here
  }

 
}
