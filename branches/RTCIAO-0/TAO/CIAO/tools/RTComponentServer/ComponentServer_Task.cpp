// $Id$

#include "ComponentServer_Task.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ComponentServer_Impl.h"
#include "Server_init.h"
#include "CIAO_ServersC.h"

#if !defined (__ACE_INLINE__)
# include "ComponentServer_Task.inl"
#endif /* __ACE_INLINE__ */

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
      CIAO::ComponentServer_Impl *comserv_servant;

      ACE_NEW_RETURN (comserv_servant,
                      CIAO::ComponentServer_Impl (this->orb_.in (),
                                                  root_poa.in ()),
                      -1);

      PortableServer::ServantBase_var safe_servant (comserv_servant);

      // @@ We need to call ComponentServer servant's init method.
      // But it's not sure to me where exactly we can get the
      // ConfigValues needed by the init method at this moment.

      // comserv_servant->init (config ACE_ENV_ARG_PARAMETER);

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
      Components::ConfigValues_var config;

      if (this->options_.use_callback_)
        {
          object = this->orb_->string_to_object (this->options_.callback_ior_.c_str ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CIAO::Activator_Callback_var act_callback
            = ::CIAO::Activator_Callback::_narrow (object.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Components::ConfigValues_out config_out (config.out ());

          activator
            = act_callback->register_component_server (comserv_obj.in (),
                                                       config_out
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      comserv_servant->set_objref (activator.in (),
                                   config.in (),
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
