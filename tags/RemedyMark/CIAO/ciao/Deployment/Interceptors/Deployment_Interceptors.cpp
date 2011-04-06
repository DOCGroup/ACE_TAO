// $Id$

#include "Deployment_Interceptors.h"
#include "tao/ORB_Core.h"
#include "Name_Utilities.h"
#include "dance/Deployment/Deployment_StartErrorC.h"
#include "dance/DAnCE_PropertiesC.h"
#include "dance/DAnCE_Utility.h"
#include "dance/LocalityManager/Scheduler/Plugin_Manager.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  /// Implementation skeleton constructor
  CIAO_StoreReferences_i::CIAO_StoreReferences_i (void)
  {
    this->orb_ = DAnCE::PLUGIN_MANAGER::instance ()->get_orb ();

    if (CORBA::is_nil (this->orb_))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Container_Handler_i::configure -"
                        "Unable to locate ORB.\n"));
        throw ::Deployment::StartError ("CIAO Container Handler",
                                        "Unable to locate ORB");
      }
  }

  /// Implementation skeleton destructor
  CIAO_StoreReferences_i::~CIAO_StoreReferences_i (void)
  {
  }

  void CIAO_StoreReferences_i::pre_install (::Deployment::DeploymentPlan &,
                                                      ::CORBA::ULong)
  {
    // no-op
  }

  void CIAO_StoreReferences_i::post_install (const ::Deployment::DeploymentPlan &plan,
                                             ::CORBA::ULong index,
                                             const ::CORBA::Any &reference,
                                             const ::CORBA::Any &)
  {
    const ::Deployment::InstanceDeploymentDescription &inst =
      plan.instance[index];

    CIAO_DEBUG (9, (LM_TRACE, CLINFO
                    "CIAO_StoreReferences_i::post_install - "
                    "Interceptor post install for instance %C\n",
                     plan.instance[index].name.in ()));

    if (reference.type() == ::CORBA::_tc_null)
      {
        CIAO_ERROR (3, (LM_WARNING, CLINFO
                        "CIAO_StoreReferences_i::post_install - "
                        "Got a nil reference, unable to store reference "
                        "for instance <%C>\n",
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

            if (!(reference >>= CORBA::Any::to_object (obj)))
              {
                CIAO_ERROR (1, (LM_WARNING, CLINFO
                                "CIAO_StoreReferences_i::post_install - "
                                "Unable to extract instance reference from Any\n"));
              }

            const char * name = 0;
            inst.configProperty[i].value >>= CORBA::Any::to_string (name, 0);

            CIAO_DEBUG (9, (LM_TRACE, CLINFO
                            "CIAO_StoreReferences_i::post_install - "
                            "Registering name %C for instance %C\n",
                             name,
                             plan.instance[index].name.in ()));

            Name_Utilities::bind_object (name,
                                         obj.in (),
                                         this->ctx_.in ());
          }
        else if (ACE_OS::strcmp (inst.configProperty[i].name.in (),
                                 DAnCE::INSTANCE_IOR_FILE) == 0)
          {
            CORBA::Object_var obj;
            reference >>= CORBA::Any::to_object (obj);

            const char * name = 0;
            inst.configProperty[i].value >>= CORBA::Any::to_string (name, 0);

            CORBA::String_var ior = this->orb_->object_to_string (obj.in ());

            CIAO::Name_Utilities::write_ior (ACE_TEXT_CHAR_TO_TCHAR (name), ior.in ());
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

  /// Implementation skeleton constructor
  CIAO_ReferenceLookup_i::CIAO_ReferenceLookup_i (void)
  {
  }

  /// Implementation skeleton destructor
  CIAO_ReferenceLookup_i::~CIAO_ReferenceLookup_i (void)
  {
  }

  void
  CIAO_ReferenceLookup_i::pre_connect (::Deployment::DeploymentPlan &,
                                       ::CORBA::ULong,
                                       ::CORBA::Any &)
  {
  }

  void CIAO_ReferenceLookup_i::post_connect (const ::Deployment::DeploymentPlan &,
                                             ::CORBA::ULong,
                                             const ::CORBA::Any &)
  {
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
