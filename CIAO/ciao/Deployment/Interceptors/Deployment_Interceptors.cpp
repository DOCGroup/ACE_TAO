// $Id$

#include "Deployment_Interceptors.h"
#include "tao/ORB_Core.h"
#include "Name_Utilities.h"
#include "Deployment/Deployment_StartErrorC.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE_Utility.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  // Implementation skeleton constructor
  CIAO_StoreReferences_i::CIAO_StoreReferences_i (void)
  {
    this->orb_ = TAO_ORB_Core_instance ()->orb ();
    
    if (CORBA::is_nil (this->orb_))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Container_Handler_i::configure -"
                        "Unable to locate ORB.\n"));
        throw ::Deployment::StartError ("CIAO Container Handler",
                                        "Unable to locate ORB");
      }
  }

  // Implementation skeleton destructor
  CIAO_StoreReferences_i::~CIAO_StoreReferences_i (void)
  {
  }

  void CIAO_StoreReferences_i::instance_pre_install (::Deployment::DeploymentPlan &,
                                                      ::CORBA::ULong)
  {
    // no-op
  }

  void CIAO_StoreReferences_i::instance_post_install (const ::Deployment::DeploymentPlan &plan,
                                                       ::CORBA::ULong instance_index,
                                                       const ::CORBA::Any &instance_reference,
                                                       const ::CORBA::Any &)
  {
    const ::Deployment::InstanceDeploymentDescription &inst =
      plan.instance[instance_index];
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO 
                     ACE_TEXT ("CIAO_StoreReferences_i::instance_post_install - ")
                     ACE_TEXT ("Interceptor post install for instance %C\n"),
                     plan.instance[instance_index].name.in ()));
    
    if (&instance_reference == 0)
      {
        DANCE_ERROR (3, (LM_WARNING, DLINFO
                         ACE_TEXT ("CIAO_StoreReferences_i::instance_post_install - ")
                         ACE_TEXT ("Got a nil instance_reference, unable to store reference ")
                         ACE_TEXT ("for instance <%C>\n"),
                         inst.name.in ()));
        return;
      }
    
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
                                 ACE_TEXT ("CIAO_StoreReferences_i::instance_post_install - ")
                                 ACE_TEXT ("Unable to extract instance reference from Any\n")));
              }
            
            const char * name = 0;
            inst.configProperty[i].value >>= CORBA::Any::to_string (name, 0);
            
            DANCE_DEBUG (9, (LM_TRACE, DLINFO 
                             ACE_TEXT ("CIAO_StoreReferences_i::instance_post_install - ")
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

            const char * name = 0;
            inst.configProperty[i].value >>= CORBA::Any::to_string (name, 0);
            
            ACE_CString ior = this->orb_->object_to_string (obj.in ());

            DAnCE::Utility::write_IOR (ACE_TEXT_CHAR_TO_TCHAR (name), ior.c_str ());
          }
      }
  }

  void
  CIAO_StoreReferences_i::configure (const ::Deployment::Properties &props )
  {
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

  // Implementation skeleton constructor
  CIAO_ReferenceLookup_i::CIAO_ReferenceLookup_i (void)
  {
  }

  // Implementation skeleton destructor
  CIAO_ReferenceLookup_i::~CIAO_ReferenceLookup_i (void)
  {
  }

  void 
  CIAO_ReferenceLookup_i::instance_pre_connect (::Deployment::DeploymentPlan &,
                                                 ::CORBA::ULong,
                                                 ::CORBA::Any &)
  {
    // Add your implementation here
  }

  void CIAO_ReferenceLookup_i::instance_post_connect (const ::Deployment::DeploymentPlan &,
                                                       ::CORBA::ULong,
                                                       const ::CORBA::Any &)
  {
    // Add your implementation here
  }

  void
  CIAO_ReferenceLookup_i::configure (const ::Deployment::Properties & )
  {
  }
 }

extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr 
  CIAO_Deployment_Interceptors_Export create_CIAO_StoreReferences (void)
  {
    return new CIAO::CIAO_StoreReferences_i ();
  }
}
