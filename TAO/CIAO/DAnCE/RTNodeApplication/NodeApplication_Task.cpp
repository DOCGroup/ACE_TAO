// $Id$

#include "ciao/NodeApplication_CallBackC.h"
#include "ciao/CIAO_common.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/Null_Mutex.h"
#include "NodeApplication_Task.h"
#include "RTServer_Impl.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplication_Task.inl"
#endif /* __ACE_INLINE__ */

int
CIAO::NodeApplication_Task::svc ()
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
      CIAO::RTServer::RTNodeApplication_Impl *nodeapp_servant;

      ACE_NEW_RETURN (nodeapp_servant,
                      CIAO::RTServer::RTNodeApplication_Impl (this->orb_.in (),
                                                              rt_orb.in (),
                                                              root_poa.in ()),
                      -1);

      PortableServer::ServantBase_var safe_servant (nodeapp_servant);

      // Configuring NodeApplication.
      PortableServer::ObjectId_var cs_oid
        = root_poa->activate_object (nodeapp_servant
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object = root_poa->id_to_reference (cs_oid.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Deployment::NodeApplication_var nodeapp_obj =
        Deployment::NodeApplication::_narrow (object.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (nodeapp_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to activate RTNodeApplication object\n"),
                          -1);

      /** @@ We need to call NodeApplication servant's init method.
       * But it's not sure to me where exactly we can get the
       * Properties needed by the init method at this moment. There are two
       * ways to set the property of the NodeApplication:
       * 1. call init remotely from NodeApplicationManager
       * 2. call init locally on the servant of NodeApplication.
       */
      bool retval = nodeapp_servant->init (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval)
        {
          ACE_DEBUG ((LM_DEBUG, "NodeApplication Failed on creating and\
                               initializing the session container!"));
	return 1;
      }

      CORBA::String_var str = this->orb_->object_to_string (nodeapp_obj.in ()
                                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->options_.ior_output_filename_.length () != 0)
        CIAO::Utility::write_IOR (this->options_.ior_output_filename_.c_str (),
                                  str.in ());

      // End Deployment part
      if (CIAO::debug_level () > 10)
        {
          ACE_DEBUG ((LM_INFO, "RTNodeApplication IOR: %s\n", str.in ()));

        }

      Deployment::NodeApplicationManager_var nodeapp_man;
      Deployment::Properties_var prop = new Deployment::Properties;

      if (this->options_.use_callback_)
        {
          object = this->orb_->string_to_object (this->options_.callback_ior_.c_str ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CIAO::NodeApplication_Callback_var nam_callback
            = CIAO::NodeApplication_Callback::_narrow (object.in ()
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Deployment::Properties_out properties_out (prop.out ());

          nodeapp_man
            = nam_callback->register_node_application (nodeapp_obj.in (),
                                                       properties_out
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Running RTNodeApplication...\n"));

      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
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

  ACE_DEBUG ((LM_DEBUG,
              "Exiting RTNodeApplication...\n"));
  return 0;
}
