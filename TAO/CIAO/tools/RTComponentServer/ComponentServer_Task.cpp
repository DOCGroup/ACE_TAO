// $Id$

#include "ComponentServer_Task.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "RTServer_Impl.h"
#include "ace/Null_Mutex.h"
#include "Server_init.h"
#include "CIAO_ServersC.h"
#include "../XML_Helpers/XML_Utils.h"
#include "RTPortableServer/RTPortableServer.h"

#if !defined (__ACE_INLINE__)
# include "ComponentServer_Task.inl"
#endif /* __ACE_INLINE__ */

void
add_rtcad_configs (const char *rtcadfile,
                   Components::ConfigValues &configs
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  CIAO::RTConfiguration::RTORB_Resource_Info resources;
  CIAO::RTConfiguration::Policy_Sets psets;

  if (CIAO::XML_Utils::parse_rtcad_extension (rtcadfile,
                                              resources,
                                              psets) == 0)
    {
      // Successfully parse the rtcad file.

      CORBA::ULong len = configs.length ();
      configs.length (len+2);

      Components::ConfigValue *newconfig
        = new OBV_Components::ConfigValue;

      newconfig->name ((const char *) "CIAO-RTResources");
      newconfig->value () <<= resources;
      configs[len] = newconfig;

      ++len;
      newconfig = new OBV_Components::ConfigValue;
      newconfig->name ((const char *) "CIAO-RTPolicySets");
      newconfig->value () <<= psets;
      configs[len] = newconfig;
    }

  ACE_DEBUG ((LM_DEBUG, "Done adding RTCAD config\n"));
}

int
CIAO::ComponentServer_Task::svc ()
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        this->orb_->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ...
      CIAO::RTServer::RTComponentServer_Impl *comserv_servant;

      ACE_NEW_RETURN (comserv_servant,
                      CIAO::RTServer::RTComponentServer_Impl (this->orb_.in (),
                                                              rt_orb.in (),
                                                              root_poa.in ()),
                      -1);

      PortableServer::ServantBase_var safe_servant (comserv_servant);

      // @@ We need to call ComponentServer servant's init method.
      // But it's not sure to me where exactly we can get the
      // ConfigValues needed by the init method at this moment.

      // @@ Manually add config values here.

      Components::ConfigValues configs;
      if (this->options_.rtcad_filename_.length () != 0)
        add_rtcad_configs (this->options_.rtcad_filename_.c_str (),
                           configs
                           ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      comserv_servant->init (configs
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Configuring ComponentServer.
      PortableServer::ObjectId_var cs_oid
        = root_poa->activate_object (comserv_servant
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object = root_poa->id_to_reference (cs_oid.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Components::Deployment::ComponentServer_var comserv_obj =
        Components::Deployment::ComponentServer::_narrow (object.in ()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (comserv_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to activate RTComponentServer object\n"),
                          -1);

      Components::Deployment::ServerActivator_var activator;

      // We are just storing the original configuration here.
      // Currently, we don't really use this ConfigValues direclty.
      Components::ConfigValues_var more_config;

      if (this->options_.use_callback_)
        {
          object = this->orb_->string_to_object (this->options_.callback_ior_.c_str ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CIAO::Activator_Callback_var act_callback
            = ::CIAO::Activator_Callback::_narrow (object.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Components::ConfigValues_out config_out (more_config.out ());

          activator
            = act_callback->register_component_server (comserv_obj.in (),
                                                       config_out
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      comserv_servant->set_objref (activator.in (),
                                   more_config.in (),
                                   comserv_obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str = this->orb_->object_to_string (comserv_obj.in ()
                                                            ACE_ENV_ARG_PARAMETER);

      if (this->options_.ior_output_filename_.length () != 0)
        CIAO::Utility::write_IOR (this->options_.ior_output_filename_.c_str (),
                                  str.in ());
      ACE_DEBUG ((LM_INFO, "RTComponentServer IOR: %s\n", str.in ()));

      // End Deployment part

      ACE_DEBUG ((LM_DEBUG,
                  "Running RTComponentServer...\n"));


      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
